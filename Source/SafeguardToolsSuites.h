#ifndef __SafeguardToolsSuites_H__
#define __SafeguardToolsSuites_H__

#include "IllustratorSDK.h"
#include "Suites.hpp"

// AI suite headers
#include "AIMenu.h"
#include "AIPreference.h"

#include "AISwatchLibraries.h"
#include "AISwatchList.h"
#include "AICustomColor.h"
#include "AIColorConversion.h"

#include "AIDocument.h"
#include "AIDocumentList.h"
#include "AIDocumentView.h"
#include "AIArtboard.h"
#include "AIArtboardRange.h"
#include "AIHardSoft.h"
#include "AIIsolationMode.h"

#include "AIGroup.h"
#include "AILayer.h"
#include "AIArtSet.h"
#include "AIArtStyle.h"
#include "AIPluginGroup.h"
#include "AIPlaced.h"
#include "AIRaster.h"
#include "AIFOConversion.h"
#include "AISymbol.h"

#include "AIPath.h"
#include "AIPathStyle.h"
#include "AIGeometry.h"

#include "AITransformArt.h"
#include "AIRealMath.h"
#include "AIGradient.h"
#include "AIPattern.h"

#include "AITextFrame.h"
#include "AIFont.h"
#include "ATETextSuitesImportHelper.h"
#include "AIATEPaint.h"
#include "AIATETextUtil.h"

#include "AICSXSExtension.h"

#include "AIDictionary.h"
#include "AIUID.h"
#include "AIArray.h"

#include "AIActionManager.h"

// Suite externs
extern SPBlocksSuite *sSPBlocks;
extern AIUnicodeStringSuite*	sAIUnicodeString;

extern AIMenuSuite *sAIMenu;
extern AIPreferenceSuite *sAIPreference;

extern AIArtSuite *sAIArt;
extern AIGroupSuite *sAIGroup;
extern AILayerSuite *sAILayer;
extern AIPluginGroupSuite *sAIPluginGroup;
extern AIPlacedSuite *sAIPlaced;
extern AIRasterSuite *sAIRaster;
extern AIFOConversionSuite *sAIFOConversion;
extern AISymbolSuite *sAISymbol;

extern AIUndoSuite *sAIUndo;
extern AISwatchLibrariesSuite *sAISwatchLibrary;
extern AISwatchListSuite *sAISwatchList;
extern AISwatchGroupSuite *sAISwatchGroup;
extern AICustomColorSuite *sAICustomColor;
extern AIColorConversionSuite *sAIColorConversion;

extern AIDocumentSuite *sAIDocument;
extern AIDocumentListSuite *sAIDocumentList;
extern AIDocumentViewSuite* sAIDocumentView;
extern AIArtboardSuite* sAIArtboard;
extern AIArtboardRangeSuite* sAIArtboardRange;
extern AIHardSoftSuite* sAIHardSoft;
extern AIIsolationModeSuite* sAIIsolationMode;

extern AIArtSetSuite *sAIArtSet;
extern AIArtStyleSuite *sAIArtStyle;

extern AIPathSuite *sAIAPath;
extern AIPathStyleSuite *sAIPathStyle;
extern AIGeometrySuite *sAIGeometry;

extern AITransformArtSuite *sAITransformArt;
extern AIRealMathSuite *sAIRealMath;
extern AIMatchingArtSuite *sAIMatchingArt;
extern AIMdMemorySuite *sAIMdMemory;
extern AITextFrameSuite *sAITextFrame;
extern AIFontSuite *sAIFont;
extern AIATEPaintSuite *sAIATEPaint;
extern AIATETextUtilSuite *sAIATETextUtil;
extern AIPatternSuite *sAIPattern;
extern AICSXSExtensionSuite *sAICSXSExtension;

extern AIUIDSuite *sAIUID;
extern AIUIDREFSuite *sAIUIDREF;
extern AIUIDUtilsSuite *sAIUIDUtils;
extern AIUIDPoolSuite *sAIUIDPool;

extern AIDictionarySuite *sAIDictionary;
extern AIEntrySuite *sAIEntry;
extern AIArraySuite *sAIArray;

extern AIActionManagerSuite *sAIActionManager;


#endif
