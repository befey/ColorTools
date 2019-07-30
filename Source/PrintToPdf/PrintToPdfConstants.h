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
    static constexpr auto PLATE_REQUEST_PDF_PRESET =        "Plate Request";
    
    enum class PdfPreset
    {
        Manufacturing =                                     0,
        Proof =                                             1,
        MicrProof =                                         2,
        PlateRequest =                                      3
    };
}

#endif
