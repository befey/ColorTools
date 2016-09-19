
// Tell Xcode to export the following symbols
#if defined(__GNUC__)
#pragma GCC visibility push(default)
#endif

#include "ColorFuncs.h"
#include "stdio.h"
#include "FixBlack.h"
#include "SafeguardToolsSuites.h"
#include "BtSwatchList.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardFileConstants.h"

bool FixBlack() {

	//Set the undo/redo text
	sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Fix Black"), ai::UnicodeString("Redo Fix Black"));
	
	AIBoolean converted = FALSE;
	sAIDocument->SetDocumentSpotColorMode(NULL, kAILegacySpotColorMode, TRUE, &converted);
    
    BtSwatchList swatchList;
    swatchList.CreateOrConvertToCustomColor(SafeguardFile::BLACK_COLOR_NAME);
    swatchList.CreateOrConvertToCustomColor(SafeguardFile::WHITE_COLOR_NAME);

    swatchList.AdjustAllColors();
	
	// 4. Name all the colors
    VisitAIColorFlags controlFlags = kVisitColorsSolidOnly | kVisitGlobalObjectsOnceOnly;
	sAIPathStyle->AdjustObjectAIColors( NULL , NameAllColors , NULL , controlFlags , NULL );
	
	// 5. Remove unused colors
	swatchList.RemoveUnusedColors();
	
	return TRUE;
}