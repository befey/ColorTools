//
//  PrintToPdfConstants.h
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#ifndef SafeguardTools_PrintToPdfConstants_h
#define SafeguardTools_PrintToPdfConstants_h

namespace PrintToPdf
{
    static constexpr auto PRINTTOPDF_FOLDERPREFS_EXTENSION =  "com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs";
    
    static constexpr auto MANUFACTURING_PDF_PRESET =        "Manufacturing";
    static constexpr auto REG_PROOF_PDF_PRESET =            "Proof";
    static constexpr auto MICR_PROOF_PDF_PRESET =           "MICR";
    
    static constexpr auto PATH_TO_PLANT_MANUFACTURING =     "/Volumes/Plant_Manufacturing/Safeguard_Mfg_Files";
    static constexpr auto PATH_TO_PDFPROOFS =               "/Volumes/PDFProofs/BAM Proofs";
    static constexpr auto PATH_TO_MICR_PDF =                "/Volumes/PDFProofs/BAM Proofs";
    static constexpr auto TESTING_OUTPUTPATH =              "/Users/t431962/Desktop/WORKING";

    
    enum class PdfPreset
    {
        Manufacturing =                                     0,
        Proof =                                             1,
        MicrProof =                                         2
    };
}

#endif
