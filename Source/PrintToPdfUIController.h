//
//  PrintToPdfUIController.h
//  SafeguardTools
//
//  Created by T431962 on 4/15/16.
//
//

#ifndef __SafeguardTools__PrintToPdfUIController__
#define __SafeguardTools__PrintToPdfUIController__

//=================================
// included dependencies
#include <string>
#include "SDKPlugPlug.h"
#include "FlashUIController.h"

//=================================
// forward declared dependencies
class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;


//=================================
// PrintToPdfUIController - handles communication between extension and plugin
class PrintToPdfUIController : public FlashUIController
{
public:
    PrintToPdfUIController(void);
    
    /** Registers the events this plug-in will listen for with PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RegisterCSXSEventListeners();
    
    /** Removes the previously added event listeners from PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RemoveEventListeners();
    
    ASErr SendData();
    
    void ParseData(const char* eventData);
    
    static void MakePdfButtonClickedFunc (const csxs::event::Event* const event, void* const context);
    static void CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        
    //=================================
    // Constant definitions
    static constexpr auto PRINTTOPDF_UI_EXTENSION =     "com.gosafeguard.SafeguardTools.PrintToPdf";
    static constexpr auto EVENT_TYPE_MAKEPDF_CLICKED =  "com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton";
    static constexpr auto EVENT_TYPE_CANCEL_CLICKED =   "com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton";
    static constexpr auto ILST_APP =                    "ILST";

    static constexpr auto MANUFACTURING_PDF_PRESET =    "Manufacturing";
    static constexpr auto MICR_PROOF_PDF_PRESET =       "MICR";
    static constexpr auto REG_PROOF_PDF_PRESET =        "Proof";
    
    static constexpr auto PATH_TO_PLANT_MANUFACTURING = "/Volumes/Plant_Manufacturing";
    static constexpr auto PATH_TO_PDFPROOFS =           "/Volumes/PDFProofs";
    static constexpr auto PATH_TO_MICR_PDF =            "/Volumes/MICR_PDF";
    static constexpr auto DEFAULT_OUTPUTPATH =          "/Users/t431962/Desktop/WORKING";
    
    static constexpr auto NO_TOKEN_DESIG =              "F";
    
    static constexpr auto PRESET_SELECT =               "preset-select";
    enum class PdfPreset
    {
        Manufacturing =                                 0,
        Proof =                                         1,
        MicrProof =                                     2
    };
    
    static constexpr auto RANGE_TEXT =                  "range-text";
    static constexpr auto ALLPAGES_CHECK =              "allpages-check";
    static constexpr auto SEPARATEFILES_CHECK =         "separatefiles-check";

    
private:

};

#endif /* defined(__SafeguardTools__PrintToPdfUIController__) */
