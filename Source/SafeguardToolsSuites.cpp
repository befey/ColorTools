#include "IllustratorSDK.h"
#include "SafeguardToolsSuites.h"

// Suite externs
	SPBlocksSuite*			sSPBlocks = NULL;
	AIUnicodeStringSuite*	sAIUnicodeString = NULL;
    AIMenuSuite*			sAIMenu = NULL;
    
    AIArtSuite*             sAIArt = NULL;
    AIGroupSuite*           sAIGroup = NULL;
    AILayerSuite*           sAILayer = NULL;
    AIPluginGroupSuite*     sAIPluginGroup = NULL;
    AIPlacedSuite*          sAIPlaced = NULL;
    AIRasterSuite*          sAIRaster = NULL;
    AIFOConversionSuite*    sAIFOConversion = NULL;
    AISymbolSuite*          sAISymbol = NULL;

    AIUndoSuite*            sAIUndo = NULL;
    AISwatchLibrariesSuite* sAISwatchLibrary = NULL;
	AISwatchListSuite*      sAISwatchList = NULL;
	AISwatchGroupSuite*     sAISwatchGroup = NULL;
	AICustomColorSuite*     sAICustomColor = NULL;
	AIColorConversionSuite* sAIColorConversion = NULL;

    AIDocumentSuite*        sAIDocument = NULL;
	AIDocumentListSuite*    sAIDocumentList = NULL;
    AIDocumentViewSuite*    sAIDocumentView = NULL;
    AIArtboardSuite*        sAIArtboard = NULL;
    AIArtboardRangeSuite*   sAIArtboardRange = NULL;
    AIHardSoftSuite*        sAIHardSoft = NULL;
    AIIsolationModeSuite*   sAIIsolationMode = NULL;

	AIArtSetSuite*          sAIArtSet = NULL;
	AIArtStyleSuite*        sAIArtStyle = NULL;
    
    AIPathSuite*            sAIPath = NULL;
	AIPathStyleSuite*       sAIPathStyle = NULL;

    AITransformArtSuite*    sAITransformArt = NULL;
    AIRealMathSuite*        sAIRealMath = NULL;
	AIMatchingArtSuite*     sAIMatchingArt = NULL;
	AIMdMemorySuite*        sAIMdMemory = NULL;
	AIPatternSuite*         sAIPattern = NULL;
	
	AITextFrameSuite*       sAITextFrame = NULL;
	AIATEPaintSuite*        sAIATEPaint = NULL;
	AIATETextUtilSuite*     sAIATETextUtil = NULL;
    AICSXSExtensionSuite*   sAICSXSExtension = NULL;
    AIFontSuite*            sAIFont = NULL;
    
    AIUIDSuite*             sAIUID = NULL;
    AIUIDREFSuite*          sAIUIDREF = NULL;
    AIUIDUtilsSuite*        sAIUIDUtils = NULL;
    AIUIDPoolSuite*         sAIUIDPool = NULL;
    
    AIDictionarySuite*      sAIDictionary = NULL;
    AIEntrySuite*           sAIEntry = NULL;
    AIArraySuite*           sAIArray = NULL;

    AIActionManagerSuite*   sAIActionManager = NULL;

    
	EXTERN_TEXT_SUITES

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
    
    kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
    
    kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
    kAIGroupSuite, kAIGroupSuiteVersion, &sAIGroup,
    kAILayerSuite, kAILayerSuiteVersion, &sAILayer,
    kAIPluginGroupSuite, kAIPluginGroupSuiteVersion, &sAIPluginGroup,
    kAIPlacedSuite, kAIPlacedSuiteVersion, &sAIPlaced,
    kAIRasterSuite, kAIRasterSuiteVersion, &sAIRaster,
    kAIFOConversionSuite, kAIFOConversionSuiteVersion, &sAIFOConversion,
    kAISymbolSuite, kAISymbolSuiteVersion, &sAISymbol,
    
    kAIUndoSuite, kAIUndoSuiteVersion, &sAIUndo,
	kAISwatchLibrariesSuite, kAISwatchLibrariesSuiteVersion, &sAISwatchLibrary,
	kAISwatchListSuite, kAISwatchListSuiteVersion, &sAISwatchList,
	kAISwatchGroupSuite, kAISwatchGroupSuiteVersion, &sAISwatchGroup,
	kAICustomColorSuite, kAICustomColorSuiteVersion, &sAICustomColor,
	kAIColorConversionSuite, kAIColorConversionSuiteVersion, &sAIColorConversion,

    kAIDocumentSuite, kAIDocumentSuiteVersion, &sAIDocument,
	kAIDocumentListSuite, kAIDocumentListVersion, &sAIDocumentList,
    kAIDocumentViewSuite, kAIDocumentViewSuiteVersion, &sAIDocumentView,
    kAIArtboardSuite, kAIArtboardSuiteVersion, &sAIArtboard,
    kAIArtboardRangeSuite, kAIArtboardRangeSuiteVersion, &sAIArtboardRange,
    kAIHardSoftSuite, kAIHardSoftSuiteVersion, &sAIHardSoft,
    kAIIsolationModeSuite, kAIIsolationModeSuiteVersion, &sAIIsolationMode,

    kAIArtSetSuite, kAIArtSetSuiteVersion, &sAIArtSet,
	kAIArtStyleSuite, kAIArtStyleSuiteVersion, &sAIArtStyle,

    kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,

    kAITransformArtSuite, kAITransformArtSuiteVersion, &sAITransformArt,
    kAIRealMathSuite, kAIRealMathSuiteVersion, &sAIRealMath,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
    
    kAIPatternSuite, kAIPatternSuiteVersion, &sAIPattern,
    kAITextFrameSuite, kAITextFrameSuiteVersion, &sAITextFrame,
    kAIFontSuite, kAIFontSuiteVersion, &sAIFont,
    kAIATEPaintSuite, kAIATEPaintSuiteVersion, &sAIATEPaint,
    kAIATETextUtilSuite, kAIATETextUtilSuiteVersion, &sAIATETextUtil,
    kAICSXSExtensionSuite, kAICSXSExtensionSuiteVersion, &sAICSXSExtension,
    
    kAIUIDSuite, kAIUIDSuiteVersion, &sAIUID,
    kAIUIDREFSuite, kAIUIDREFSuiteVersion, &sAIUIDREF,
    kAIUIDUtilsSuite, kAIUIDUtilsSuiteVersion, &sAIUIDUtils,
    kAIUIDPoolSuite, kAIUIDPoolSuiteVersion, &sAIUIDPool,
    
    kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
    kAIEntrySuite, kAIEntrySuiteVersion, &sAIEntry,
    kAIArraySuite, kAIArraySuiteVersion, &sAIArray,
    
    kAIActionManagerSuite, kAIActionManagerSuiteVersion, &sAIActionManager,

    
    IMPORT_TEXT_SUITES
    
	nil, 0, nil
};
