//
//  main.cpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/28/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#include <iostream>
#include <vector>
#include <set>
#include "PathBuilder.h"
#include "CommandLineDirectoryChooser.hpp"
#include "CommandLinePreferenceWriter.hpp"
#include "ExistingFileDeleter.h"
#include "TargetPlate.hpp"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <thread>


static constexpr auto MANUFACTURING_VOID_CLEANUP_FOLDERPREFS_EXTENSION =  "com.gosafeguard.SafeguardTools.ManufacturingVoidCleanup.FolderPrefs";

bool DO_NOT_DELETE = false;

int GetFoundSize(std::vector<FilesystemResults> resultList)
{
    int count = 0;
    
    for (auto r : resultList)
    {
        if (r.IsTransactionFound())
            count++;
    }
    
    return count;
}

void DisplayReport(std::set<TargetPlate> plates, std::vector<FilesystemResults> resultList)
{
    std::cout << "-----------------------------------------\n";
    std::cout << "Total Plates: " << plates.size() << "\n";
    std::cout << "Deleted (found): " << GetFoundSize(resultList) << "\n";
}

bool PauseIfLessThanFor(time_t& lastTime, int lessThanSecs, int pauseForMill)
{
    bool returnVal = false;
    time_t currTime =  time(NULL);
    std::chrono::milliseconds timespan(pauseForMill - ((currTime - lastTime)*1000));
    std::cout << "Time Elapsed:\t" << currTime - lastTime << "\tseconds\n";
    std::cout << "Pausing for:\t" << pauseForMill / 1000 << "\tseconds\t";
    if (currTime - lastTime < lessThanSecs)
    {
        returnVal = true;
        std::this_thread::sleep_for(timespan);
    }
    lastTime = currTime;

    return returnVal;
}

void GetDirectoryListingInSet(std::set<boost::filesystem::directory_entry>& directoryListing, boost::filesystem::path fp)
{
    namespace fs = boost::filesystem;
    
    typedef std::function<bool(const fs::directory_entry&)> filterfunc;
    filterfunc pred = [](const fs::directory_entry& e)
    {
        return true;
    };
    
    try {
        std::copy(
                  boost::make_filter_iterator(pred, fs::directory_iterator(fp), fs::directory_iterator()),
                  boost::make_filter_iterator(pred, fs::directory_iterator(), fs::directory_iterator()),
                  std::inserter(directoryListing, directoryListing.end())
                  );
    }
    catch(const fs::filesystem_error& e)
    {
        e.what();
    }
}

int main(int argc, const char * argv[]) {
    time_t storedTimeOfDirList = time(NULL);
    
    std::vector<std::string> filesFailedToDelete;
    std::vector<std::string> filesNotFound;
    std::vector<std::string> filesSuccessfullyDeleted;
    
    // insert code here...
    std::cout << "Reading file -------------------------------------\n";
    
    //Read in csv file
    std::string data("data.csv");
    
    std::ifstream in(data.c_str());
    if (!in.is_open())
    {
        return 1;
    }
    
    //Setup the tokenizer
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    std::string line;
    std::set<TargetPlate> plates;
    std::vector<std::string> invalidPlates;
    std::vector<FilesystemResults> resultList;

    //Read in each plate number
    while (std::getline(in,line))
    {
        Tokenizer tok(line);
        std::string plateNumber = *tok.begin();
        boost::trim(plateNumber);
        
        if (SafeguardFile::PlateNumber(plateNumber).IsValid())
        {
            plates.insert(TargetPlate(plateNumber));
        }
        else
        {
            invalidPlates.push_back(plateNumber);
        }
    }
    
    //Setup Manufacturing server path builder
    std::shared_ptr<DirectoryChooser> chooser = std::make_shared<CommandLineDirectoryChooser>();
    std::shared_ptr<PreferenceWriter> writer = std::make_shared<CommandLinePreferenceWriter>(MANUFACTURING_VOID_CLEANUP_FOLDERPREFS_EXTENSION);
    std::unique_ptr<PrintToPdf::PathBuilder> pbuilder = PrintToPdf::PathBuilder::GetPathBuilder(PrintToPdf::PdfPreset::Manufacturing, false, chooser, writer);
    
    //Pass each TargetPlate in and try to delete all the files
    int count = 0;
    
    boost::filesystem::path storedPath;
    std::string storedPlateNumber;
    std::set<boost::filesystem::directory_entry> storedDirectoryListing;
    
    for ( auto p : plates )
    {
        if (storedPath != pbuilder->GetPath(p.PlateNumber()))
        {
            storedPlateNumber = p.PlateNumber();
            storedPath = pbuilder->GetPath(storedPlateNumber);
            if ( PauseIfLessThanFor(storedTimeOfDirList, 5, 5000) )
            {
                std::cout << "  DIR LIST\n";
            }
            GetDirectoryListingInSet(storedDirectoryListing, storedPath);
        }
        
        std::cout << "\nPLATE: " << p.PlateNumber() << "\t#\t" << ++count << " of\t" << plates.size() << "\n";
        
        FilesystemResults deletedFiles;
        
        bool success = false;
        for ( auto file : storedDirectoryListing )
        {
            std::string fileStr = file.path().string();
            boost::algorithm::to_upper(fileStr);
            
            if (fileStr.find(p.PlateNumber()) != std::string::npos)
            {
                deletedFiles.AddResult({.action = FilesystemResults::Transaction::Found, .path = file });
                boost::system::error_code ec;
                
                
                    //PauseIfLessThanFor(storedTimeOfDelete, 1, 1000);

                    boost::filesystem::exists(file, ec);
                    if (ec == boost::system::errc::success)
                    {
                        if (!DO_NOT_DELETE)
                        {
                            boost::filesystem::remove(file, ec);
                            if (ec == boost::system::errc::success)
                            {
                                deletedFiles.AddResult({.action = FilesystemResults::Transaction::Deleted, .path = file });
                                filesSuccessfullyDeleted.push_back(file.path().filename().string());
                            }
                            else
                            {
                                filesFailedToDelete.push_back(p.PlateNumber());
                            }
                        }
                        success = true;
                        std::cout << "  DELETE ATTEMPT \t" << file.path().filename().string() << "\tSUCCESS";
                    }
                    std::cout << "\n";
            }
        }
        if (success != true)
        {
            filesNotFound.push_back(p.PlateNumber());
        }
        resultList.push_back(deletedFiles);
    }
    
    
    std::cout << "Deleting.....\n";
    std::cout << "Finished -----------------------------------------\n";
    std::cout << "Total Plates:\t\t\t" << plates.size() << "\n";
    std::cout << "Found:\t\t\t\t" << GetFoundSize(resultList) << "\n";
    std::cout << "Could not find:\t\t" << filesNotFound.size() << "\n";
    std::cout << "Total files deleted:\t" << filesSuccessfullyDeleted.size() << "\n";
    std::cout << "Could not delete:\t\t" << filesFailedToDelete.size() << "\n";
    
    std::ofstream deletedPlates("deleted.csv");
    std::ofstream notFoundPlates("notfound.csv");
    
    for (auto p : resultList)
    {
        for ( auto pn : p.GetAsVectorOfPlateNumbers() )
        {
            deletedPlates << pn << std::endl;
        }
    }
    
    for (auto p : filesNotFound)
    {
        notFoundPlates << p << std::endl;
    }
    return 0;
}
