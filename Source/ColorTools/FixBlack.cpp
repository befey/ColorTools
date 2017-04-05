
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
	sAIDocument->SetDocumentSpotColorMode(nullptr, kAILegacySpotColorMode, TRUE, &converted);
    
    BtSwatchList swatchList;
    swatchList.FixStdColors();

    swatchList.AdjustAllColors();
	
	// 4. Name all the colors
    VisitAIColorFlags controlFlags = kVisitColorsSolidOnly | kVisitGlobalObjectsOnceOnly;
	sAIPathStyle->AdjustObjectAIColors( nullptr , NameAllColors , nullptr , controlFlags , nullptr );
	
	// 5. Remove unused colors
	swatchList.RemoveUnusedColors();
	
	return TRUE;
}
