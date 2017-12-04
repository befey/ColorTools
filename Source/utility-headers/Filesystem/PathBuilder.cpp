//
//  PathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#include "PathBuilder.h"
#include "PreferenceWriter.hpp"
#include "SgPathConstants.hpp"
#include "SafeguardFileConstants.h"

namespace fs = boost::filesystem;

using SafeguardFile::PlateNumber;
using PrintToPdf::PathBuilder;
using PrintToPdf::ManufacturingPathBuilder;
using PrintToPdf::ProofPathBuilder;
using PrintToPdf::MicrProofPathBuilder;
using PrintToPdf::TestingPathBuilder;
using PrintToPdf::UserPathBuilder;

std::unique_ptr<PathBuilder> PathBuilder::GetPathBuilder(PdfPreset preset, bool userOutputFolder, std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer)
{
    if (userOutputFolder)
    {
        return std::unique_ptr<PathBuilder> { std::make_unique<UserPathBuilder>(chooser, writer) };
    }
    
    if (preset == PdfPreset::Manufacturing)
    {
         return std::unique_ptr<PathBuilder> { std::make_unique<ManufacturingPathBuilder>(chooser, writer) };
    }
    else if (preset == PdfPreset::MicrProof)
    {
        return std::unique_ptr<PathBuilder> { std::make_unique<MicrProofPathBuilder>(chooser, writer) };
    }
    else if (preset == PdfPreset::Proof)
    {
        return std::unique_ptr<PathBuilder> { std::make_unique<ProofPathBuilder>(chooser, writer) };
    }
    else
    {
        return std::unique_ptr<PathBuilder> { std::make_unique<UserPathBuilder>(chooser, writer) };
    }
}

fs::path ManufacturingPathBuilder::GetPath(const PlateNumber pn) const
{
    std::string prodCode;
    
    if (pn.GetProductType() == SafeguardFile::BusinessStat)
    {
        prodCode = "BS";
    }
    else if (pn.GetProductIndicator() == "RC" || pn.GetProductIndicator() == "VS")
    {
        prodCode = "RC-VS";
    }
    else
    {
        prodCode = pn.GetProductIndicator();
    }
    
    fs::path saveasFilePath;
    if (!prefWriter->GetFilePathFromIdentifier(MANUFACTURING_PDF_PRESET, saveasFilePath))
    {
        saveasFilePath = PATH_TO_PLANT_MANUFACTURING;
    }
    
    saveasFilePath /= (prodCode + " to be Plated");
    saveasFilePath /= (pn.GetPlantIndicator() + " " + prodCode + " to be Plated");
    
    if (!fs::is_directory(saveasFilePath) || prodCode == "")
    {
        UserPathBuilder upb(dirChooser, prefWriter);
        saveasFilePath = upb.GetPath(pn);
    }
    
    return saveasFilePath;
}

fs::path ProofPathBuilder::GetPath(const PlateNumber pn) const
{
    fs::path saveasFilePath;
    if (!prefWriter->GetFilePathFromIdentifier(REG_PROOF_PDF_PRESET, saveasFilePath))
    {
        saveasFilePath = PATH_TO_PDFPROOFS;
    }
    
    std::string prodCode = pn.GetProductIndicator();
    if (!fs::is_directory(saveasFilePath) || prodCode == "")
    {
        UserPathBuilder upb(dirChooser, prefWriter);
        saveasFilePath = upb.GetPath(pn);
    }
    
    return saveasFilePath;
}

fs::path MicrProofPathBuilder::GetPath(const PlateNumber pn) const
{
    fs::path saveasFilePath;
    if (!prefWriter->GetFilePathFromIdentifier(MICR_PROOF_PDF_PRESET, saveasFilePath))
    {
        saveasFilePath = PATH_TO_MICR_PDF;
    }
    
    std::string prodCode = pn.GetProductIndicator();
    if (!fs::is_directory(saveasFilePath) || prodCode == "")
    {
        UserPathBuilder upb(dirChooser, prefWriter);
        saveasFilePath = upb.GetPath(pn);
    }
    
    return saveasFilePath;
}

fs::path UserPathBuilder::GetPath(const PlateNumber pn) const
{      
    return dirChooser->GetDirectory();;
}


fs::path TestingPathBuilder::GetPath(const PlateNumber pn) const
{
    std::string prodCode;
    fs::path saveasFilePath;
    
    if (pn.GetProductType() == SafeguardFile::BusinessStat)
    {
        prodCode = "BS";
    }
    else if (pn.GetProductIndicator() == "RC" || pn.GetProductIndicator() == "VS")
    {
        prodCode = "RC-VS";
    }
    else
    {
        prodCode = pn.GetProductIndicator();
    }
    
    if (prodCode == "")
    {
        UserPathBuilder upb(dirChooser, prefWriter);
        saveasFilePath = upb.GetPath(pn);
    }
    else
    {
        saveasFilePath = TESTING_OUTPUTPATH;
        saveasFilePath /= (prodCode + " to be Plated");
        saveasFilePath /= (pn.GetPlantIndicator() + " " + prodCode + " to be Plated");
    }
    
    return saveasFilePath;
}
