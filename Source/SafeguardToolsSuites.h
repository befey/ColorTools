#ifndef __SafeguardToolsSuites_H__
#define __SafeguardToolsSuites_H__

#include "IllustratorSDK.h"
#include "Suites.hpp"

// AI suite headers
#include "AIMenu.h"

#include "AISwatchLibraries.h"
#include "AISwatchList.h"
#include "AICustomColor.h"
#include "AIColorConversion.h"

#include "AIDocument.h"
#include "AIDocumentList.h"
#include "AIDocumentView.h"
#include "AIArtboard.h"

#include "AIGroup.h"
#include "AIArtSet.h"
#include "AIArtStyle.h"

#include "AIPath.h"
#include "AIPathStyle.h"

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

#include "AIActionManager.h"

// Suite externs
extern "C"  SPBlocksSuite *sSPBlocks;
extern "C"  AIUnicodeStringSuite*	sAIUnicodeString;

extern "C" 	AIMenuSuite *sAIMenu;

extern "C"  AIArtSuite *sAIArt;
extern "C"  AIGroupSuite *sAIGroup;

extern "C"	AIUndoSuite *sAIUndo;
extern "C"  AISwatchLibrariesSuite *sAISwatchLibrary;
extern "C"  AISwatchListSuite *sAISwatchList;
extern "C"  AISwatchGroupSuite *sAISwatchGroup;
extern "C"  AICustomColorSuite *sAICustomColor;
extern "C"	AIColorConversionSuite *sAIColorConversion;

extern "C"	AIDocumentSuite *sAIDocument;
extern "C"  AIDocumentListSuite *sAIDocumentList;
extern "C"  AIDocumentViewSuite* sAIDocumentView;
extern "C"  AIArtboardSuite* sAIArtboard;

extern "C"  AIArtSetSuite *sAIArtSet;
extern "C"  AIArtStyleSuite *sAIArtStyle;

extern "C"  AIPathSuite *sAIAPath;
extern "C"  AIPathStyleSuite *sAIPathStyle;

extern "C"  AITransformArtSuite *sAITransformArt;
extern "C"  AIRealMathSuite *sAIRealMath;
extern "C"  AIMatchingArtSuite *sAIMatchingArt;
extern "C"  AIMdMemorySuite *sAIMdMemory;
extern "C"  AITextFrameSuite *sAITextFrame;
extern "C"  AIFontSuite *sAIFont;
extern "C"  AIATEPaintSuite *sAIATEPaint;
extern "C"  AIATETextUtilSuite *sAIATETextUtil;
extern "C"	AIPatternSuite *sAIPattern;
extern "C"  AICSXSExtensionSuite *sAICSXSExtension;

extern "C"  AIUIDSuite *sAIUID;
extern "C"  AIUIDREFSuite *sAIUIDREF;
extern "C"  AIUIDUtilsSuite *sAIUIDUtils;
extern "C"  AIUIDPoolSuite *sAIUIDPool;

extern "C"  AIDictionarySuite *sAIDictionary;
extern "C"  AIEntrySuite *sAIEntry;

extern "C"  AIActionManagerSuite *sAIActionManager;


#endif
