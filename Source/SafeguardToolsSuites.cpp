#include "IllustratorSDK.h"
#include "SafeguardToolsSuites.h"

// Suite externs
	SPBlocksSuite*			sSPBlocks = nullptr;
	AIUnicodeStringSuite*	sAIUnicodeString = nullptr;
    AIMenuSuite*			sAIMenu = nullptr;
    AIPreferenceSuite*      sAIPreference = nullptr;
    
    AIArtSuite*             sAIArt = nullptr;
    AIGroupSuite*           sAIGroup = nullptr;
    AILayerSuite*           sAILayer = nullptr;
    AIPluginGroupSuite*     sAIPluginGroup = nullptr;
    AIPlacedSuite*          sAIPlaced = nullptr;
    AIRasterSuite*          sAIRaster = nullptr;
    AIFOConversionSuite*    sAIFOConversion = nullptr;
    AISymbolSuite*          sAISymbol = nullptr;

    AIUndoSuite*            sAIUndo = nullptr;
    AISwatchLibrariesSuite* sAISwatchLibrary = nullptr;
	AISwatchListSuite*      sAISwatchList = nullptr;
	AISwatchGroupSuite*     sAISwatchGroup = nullptr;
	AICustomColorSuite*     sAICustomColor = nullptr;
	AIColorConversionSuite* sAIColorConversion = nullptr;

    AIDocumentSuite*        sAIDocument = nullptr;
	AIDocumentListSuite*    sAIDocumentList = nullptr;
    AIDocumentViewSuite*    sAIDocumentView = nullptr;
    AIArtboardSuite*        sAIArtboard = nullptr;
    AIArtboardRangeSuite*   sAIArtboardRange = nullptr;
    AIHardSoftSuite*        sAIHardSoft = nullptr;
    AIIsolationModeSuite*   sAIIsolationMode = nullptr;

	AIArtSetSuite*          sAIArtSet = nullptr;
	AIArtStyleSuite*        sAIArtStyle = nullptr;
    
    AIPathSuite*            sAIPath = nullptr;
	AIPathStyleSuite*       sAIPathStyle = nullptr;
    AIGeometrySuite*        sAIGeometry = nullptr;

    AITransformArtSuite*    sAITransformArt = nullptr;
    AIRealMathSuite*        sAIRealMath = nullptr;
	AIMatchingArtSuite*     sAIMatchingArt = nullptr;
	AIMdMemorySuite*        sAIMdMemory = nullptr;
	AIPatternSuite*         sAIPattern = nullptr;
	
	AITextFrameSuite*       sAITextFrame = nullptr;
	AIATEPaintSuite*        sAIATEPaint = nullptr;
	AIATETextUtilSuite*     sAIATETextUtil = nullptr;
    AICSXSExtensionSuite*   sAICSXSExtension = nullptr;
    AIFontSuite*            sAIFont = nullptr;
    
    AIUIDSuite*             sAIUID = nullptr;
    AIUIDREFSuite*          sAIUIDREF = nullptr;
    AIUIDUtilsSuite*        sAIUIDUtils = nullptr;
    AIUIDPoolSuite*         sAIUIDPool = nullptr;
    
    AIDictionarySuite*      sAIDictionary = nullptr;
    AIEntrySuite*           sAIEntry = nullptr;
    AIArraySuite*           sAIArray = nullptr;

    AIActionManagerSuite*   sAIActionManager = nullptr;

    
	EXTERN_TEXT_SUITES

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
    
    kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
    kAIPreferenceSuite, kAIPreferenceSuiteVersion, &sAIPreference,
    
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
    kAIGeometrySuite, kAIGeometrySuiteVersion, &sAIGeometry,

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
