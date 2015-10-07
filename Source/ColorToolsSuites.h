#ifndef __ColorToolsSuites_H__
#define __ColorToolsSuites_H__

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
#include "AIArtSet.h"
#include "AIArtStyle.h"
#include "AIPathStyle.h"
#include "AIRealMath.h"
#include "AIGradient.h"
#include "AIPattern.h"

#include "AITextFrame.h"
#include "ATETextSuitesImportHelper.h"
#include "AIATEPaint.h"
#include "AIATETextUtil.h"



// Suite externs
extern "C"  SPBlocksSuite *sSPBlocks;
extern "C"  AIUnicodeStringSuite*	sAIUnicodeString;

extern "C" 	AIMenuSuite *sAIMenu;

extern "C"  AIArtSuite *sAIArt;

extern "C"	AIUndoSuite *sAIUndo;
extern "C"  AISwatchLibrariesSuite *sAISwatchLibrary;
extern "C"  AISwatchListSuite *sAISwatchList;
extern "C"  AISwatchGroupSuite *sAISwatchGroup;
extern "C"  AICustomColorSuite *sAICustomColor;
extern "C"	AIColorConversionSuite *sAIColorConversion;
extern "C"	AIDocumentSuite *sAIDocument;
extern "C"  AIDocumentListSuite *sAIDocumentList;
extern "C"  AIArtSetSuite *sAIArtSet;
extern "C"  AIArtStyleSuite *sAIArtStyle;
extern "C"  AIPathStyleSuite *sAIPathStyle;
extern "C"  AIRealMathSuite *sAIRealMath;
extern "C"  AIMatchingArtSuite *sAIMatchingArt;
extern "C"  AIMdMemorySuite *sAIMdMemory;
extern "C"  AITextFrameSuite *sAITextFrame;
extern "C"  AIATEPaintSuite *sAIATEPaint;
extern "C"  AIATETextUtilSuite *sAIATETextUtil;
extern "C"	AIPatternSuite *sAIPattern;

#endif
