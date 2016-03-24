#include "IllustratorSDK.h"
#include "SafeguardToolsSuites.h"

// Suite externs
extern "C"
{
	SPBlocksSuite*			sSPBlocks = NULL;
	AIUnicodeStringSuite*	sAIUnicodeString = NULL;
    AIMenuSuite*			sAIMenu = NULL;
    
    AIArtSuite*             sAIArt = NULL;
    
    AIUndoSuite*            sAIUndo = NULL;
    AISwatchLibrariesSuite* sAISwatchLibrary = NULL;
	AISwatchListSuite*      sAISwatchList = NULL;
	AISwatchGroupSuite*     sAISwatchGroup = NULL;
	AICustomColorSuite*     sAICustomColor = NULL;
	AIColorConversionSuite* sAIColorConversion = NULL;
	AIDocumentSuite*        sAIDocument = NULL;
	AIDocumentListSuite*    sAIDocumentList = NULL;
	AIArtSetSuite*          sAIArtSet = NULL;
	AIArtStyleSuite*        sAIArtStyle = NULL;
	AIPathStyleSuite*       sAIPathStyle = NULL;
	AIRealMathSuite*        sAIRealMath = NULL;
	AIMatchingArtSuite*     sAIMatchingArt = NULL;
	AIMdMemorySuite*        sAIMdMemory = NULL;
	AIPatternSuite*         sAIPattern = NULL;
	
	AITextFrameSuite*       sAITextFrame = NULL;
	AIATEPaintSuite*        sAIATEPaint = NULL;
	AIATETextUtilSuite*     sAIATETextUtil = NULL;
    AICSXSExtensionSuite*   sAICSXSExtension = NULL;
    
	EXTERN_TEXT_SUITES
}

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
    
    kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
    
    kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
    
    kAIUndoSuite, kAIUndoSuiteVersion, &sAIUndo,
	kAISwatchLibrariesSuite, kAISwatchLibrariesSuiteVersion, &sAISwatchLibrary,
	kAISwatchListSuite, kAISwatchListSuiteVersion, &sAISwatchList,
	kAISwatchGroupSuite, kAISwatchGroupSuiteVersion, &sAISwatchGroup,
	kAICustomColorSuite, kAICustomColorSuiteVersion, &sAICustomColor,
	kAIColorConversionSuite, kAIColorConversionSuiteVersion, &sAIColorConversion,
	kAIDocumentSuite, kAIDocumentSuiteVersion, &sAIDocument,
	kAIDocumentListSuite, kAIDocumentListVersion, &sAIDocumentList,
	kAIArtSetSuite, kAIArtSetSuiteVersion, &sAIArtSet,
	kAIArtStyleSuite, kAIArtStyleSuiteVersion, &sAIArtStyle,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
	kAIRealMathSuite, kAIRealMathSuiteVersion, &sAIRealMath,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
    
    kAIPatternSuite, kAIPatternSuiteVersion, &sAIPattern,
    kAITextFrameSuite, kAITextFrameSuiteVersion, &sAITextFrame,
    kAIATEPaintSuite, kAIATEPaintSuiteVersion, &sAIATEPaint,
    kAIATETextUtilSuite, kAIATETextUtilSuiteVersion, &sAIATETextUtil,
    kAICSXSExtensionSuite, kAICSXSExtensionSuiteVersion, &sAICSXSExtension,
    
    IMPORT_TEXT_SUITES
    
	nil, 0, nil
};
