//
//  main.cpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/28/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#include <iostream>
#include "PathBuilder.h"
#include "CommandLineDirectoryChooser.hpp"
#include "CommandLinePreferenceWriter.hpp"
#include "ExistingFileDeleter.h"

static constexpr auto MANUFACTURING_VOID_CLEANUP_FOLDERPREFS_EXTENSION =  "com.gosafeguard.SafeguardTools.ManufacturingVoidCleanup.FolderPrefs";

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Reading file -------------------------------------\n";
    
    std::shared_ptr<DirectoryChooser> chooser = std::make_shared<CommandLineDirectoryChooser>();
    std::shared_ptr<PreferenceWriter> writer = std::make_shared<CommandLinePreferenceWriter>(MANUFACTURING_VOID_CLEANUP_FOLDERPREFS_EXTENSION);
    std::unique_ptr<PrintToPdf::PathBuilder> pbuilder = PrintToPdf::PathBuilder::GetPathBuilder(PrintToPdf::PdfPreset::Manufacturing, false, chooser, writer);
    
    boost::filesystem::path path = pbuilder->GetPath(SafeguardFile::PlateNumber("Y16SF000123"));
    std::cout << path.string() << "\n";
    
    std::cout << "Deleting.....\n";
    std::cout << "Finished -----------------------------------------\n";
    std::cout << "Deleted:\n";
    std::cout << "Plates:\n";
    std::cout << "Total files:\n";
    std::cout << "Could not delete:\n";
    return 0;
}
