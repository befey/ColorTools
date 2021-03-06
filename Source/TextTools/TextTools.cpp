#include "IllustratorSDK.h"


#include "ATEFuncs.h"
#include "ArtTree.h"
#include "ColorFuncs.h"
#include "DictionaryWriter.h"
#include "BtAteTextFeatures.h"
#include "SafeguardFileConstants.h"
#include "BtArtHandle.hpp"

#include "TextTools.h"

#include <regex>

bool gBoundsSetFlag = FALSE;

bool ConvertToPointType()
{
	//Set the undo/redo text
    sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Make Point Type"), ai::UnicodeString("Redo Make Point Type"));
	
	int docCount = 0;
	sAIDocumentList->Count(&docCount);
	if (docCount > 0)
    {
		//Create an art set of only the selected text objects
		//CREATE THE ART SET
		AIArtSet artSet;
		sAIArtSet->NewArtSet(&artSet);
        
		AIArtSpec selectedSpecs[] =
        {
            { kTextFrameArt , kArtFullySelected , kArtFullySelected }
        };
        
		sAIArtSet->MatchingArtSet( selectedSpecs , 1, artSet );
		
		//Loop the art set
		size_t count;
        sAIArtSet->CountArtSet( artSet, &count );
		for ( int i=0 ; i < count ; i++ )
        {
			AIArtHandle currArtHandle = nullptr;
			sAIArtSet->IndexArtSet( artSet, i, &currArtHandle );
			
			AITextFrameType type;
			sAITextFrame->GetType(currArtHandle, &type);
			//We only want text frames, not path or point type
			if (type == kInPathTextType)
            {
				//Create an ITextFrame object from the original TextFrame art
				TextFrameRef origATETextFrameRef;
				sAITextFrame->GetATETextFrame(currArtHandle, &origATETextFrameRef);
				ATE::ITextFrame origITextFrame(origATETextFrameRef);
												
				//Create the runs iterator
				ATE::TextRangeRef currATETextRangeRef;
				sAITextFrame->GetATETextRange(currArtHandle, &currATETextRangeRef);
				ATE::ITextRange currTextRange(currATETextRangeRef);
				
				ATE::ITextRunsIterator currTextRunsIterator = currTextRange.GetTextRunsIterator();
				
				//If there is stuff to iterate through
				if ( !currTextRunsIterator.IsEmpty() )
                {
					//Create the new point text that we will add the text from the frame to
					AITextOrientation orientation;
					sAITextFrame->GetOrientation(currArtHandle, &orientation);
					AIRealPoint anchor = {0,0};
					AIArtHandle theCopy = nullptr;
					sAITextFrame->NewPointText(kPlaceAbove, currArtHandle, orientation, anchor, &theCopy );
					
									
					//Store the justification of the first line
					ATE::ParagraphJustification firstRunJustification = currTextRunsIterator.Item().GetParaInspector().GetJustification().GetFirst();
					
					//Find the bounds of the original with rotation removed
					AIRealMatrix origMatrix = origITextFrame.GetMatrix();
					AIReal angle, hscale, vscale, transx, transy;
					sAIRealMath->AIRealMatrixGetRotate(&origMatrix, &angle);
					sAIRealMath->AIRealMatrixGetScale(&origMatrix, &hscale, &vscale);
					sAIRealMath->AIRealMatrixGetTranslate(&origMatrix, &transx, &transy);
					
					//Set scale (which in our case means reflection) of the new text object to match the original
					AIRealMatrix scaleMatrix;
					sAIRealMath->AIRealMatrixSetIdentity(&scaleMatrix);
					sAIRealMath->AIRealMatrixSetScale(&scaleMatrix, hscale, vscale);
					sAITransformArt->TransformArt(theCopy, &scaleMatrix, 0, kTransformObjects);
					
					
					//Get bounds of unrotated text
					AIRealRect convertedToPathsBounds, unRotateBounds;
					AIRealMatrix unrotateMatrix;
					sAIRealMath->AIRealMatrixSetIdentity(&unrotateMatrix);
					sAIRealMath->AIRealMatrixConcatRotate(&unrotateMatrix, angle);
					sAITransformArt->TransformArt(currArtHandle, &unrotateMatrix, 0, kTransformObjects);
					sAIArt->GetArtTransformBounds(currArtHandle, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds|kNoExtendedBounds, &unRotateBounds);
					
					//Duplicate the art, convert to outlines so we can get the tightest bounds
					AIArtHandle tempDuplicate = nullptr;
					sAITextFrame->CreateOutline(currArtHandle, &tempDuplicate);					
					
					//Get the bounds of our outline text and delete it
					sAIArt->GetArtTransformBounds(tempDuplicate, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds, &convertedToPathsBounds);
					sAIArt->DisposeArt(tempDuplicate);
					
					//We'll just use the top and bottom from the converted to paths one
					if ((firstRunJustification == ATE::kLeftJustify || firstRunJustification == ATE::kFullJustifyLastLineLeft ||
						 firstRunJustification == ATE::kFullJustifyLastLineFull || firstRunJustification == ATE::kFullJustifyLastLineCenter ||
						 firstRunJustification == ATE::kFullJustifyLastLineRight))
                    {
						convertedToPathsBounds.left = unRotateBounds.left;
					}
                    else if (firstRunJustification == ATE::kRightJustify)
                    {
						convertedToPathsBounds.right = unRotateBounds.right;
					}
					
					//Check if there are no bounds after we convert to paths, this means that the entire text was in overflow
					if ( !sAIRealMath->EqualWithinTol(convertedToPathsBounds.right, convertedToPathsBounds.left, 10000) )
                    {
						convertedToPathsBounds = unRotateBounds;
					}
					
					sAIRealMath->AIRealMatrixSetIdentity(&unrotateMatrix);
					
					if (firstRunJustification == ATE::kLeftJustify || firstRunJustification == ATE::kFullJustifyLastLineLeft ||
						firstRunJustification == ATE::kFullJustifyLastLineFull || firstRunJustification == ATE::kFullJustifyLastLineCenter ||
						firstRunJustification == ATE::kFullJustifyLastLineRight)
                    {
						sAIRealMath->AIRealMatrixConcatTranslate(&unrotateMatrix, -convertedToPathsBounds.top, -convertedToPathsBounds.left);
					}
                    else if (firstRunJustification == ATE::kRightJustify)
                    {
						sAIRealMath->AIRealMatrixConcatTranslate(&unrotateMatrix, -convertedToPathsBounds.top, -convertedToPathsBounds.right);
					}
                    else if (firstRunJustification == ATE::kCenterJustify)
                    {
						sAIRealMath->AIRealMatrixConcatTranslate(&unrotateMatrix, -convertedToPathsBounds.top, -(convertedToPathsBounds.left+((convertedToPathsBounds.right - convertedToPathsBounds.left)/2)) );
					}
					
					sAITransformArt->TransformArt(currArtHandle, &unrotateMatrix, 0, kTransformObjects);
					
					//Loop through the runs
					while ( currTextRunsIterator.IsNotDone() )
                    {
						//Get the range for the new copy
						ATE::TextRangeRef theCopyRangeRef;
						sAITextFrame->GetATETextRange(theCopy, &theCopyRangeRef);
						ATE::ITextRange theCopyRange(theCopyRangeRef);
						
						//Put the run from the old text into the new text
						theCopyRange.InsertAfter(currTextRunsIterator.Item());
						
						currTextRunsIterator.Next();
						
						if ( currTextRunsIterator.IsDone() )
                        {
							//If this was the last run (or an empty text), we can delete art object we're pulling from
							sAIArt->DisposeArt(currArtHandle);
							//break;
						}
					}
					
					//Move the new text into the same position as the old text
					//Get the new bounds
					//Duplicate the art, convert to point type so we can get the tightest bounds
					tempDuplicate = nullptr;
					sAITextFrame->CreateOutline(theCopy, &tempDuplicate);
					
					//Get unconverted bounds
					AIRealRect origCopyBounds;
					sAIArt->GetArtTransformBounds(theCopy, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds, &origCopyBounds);
					
					//Get the bounds of our outline text and delete it
					AIRealRect newBounds;
					sAIArt->GetArtTransformBounds(tempDuplicate, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds, &newBounds);
					sAIArt->DisposeArt(tempDuplicate);
					
					//Find the x and y translation
					AIReal tx = 0.0, ty = 0.0;	
					
					if (firstRunJustification == ATE::kLeftJustify || firstRunJustification == ATE::kFullJustifyLastLineLeft ||
						firstRunJustification == ATE::kFullJustifyLastLineFull || firstRunJustification == ATE::kFullJustifyLastLineCenter ||
						firstRunJustification == ATE::kFullJustifyLastLineRight)
                    {
						tx = convertedToPathsBounds.left - origCopyBounds.left;
						ty = convertedToPathsBounds.top - newBounds.top;	
					}
                    else if (firstRunJustification == ATE::kRightJustify)
                    {
						tx = convertedToPathsBounds.right - origCopyBounds.right;
						ty = convertedToPathsBounds.top - newBounds.top;
					}
                    else if (firstRunJustification == ATE::kCenterJustify)
                    {
						tx = (convertedToPathsBounds.left+((convertedToPathsBounds.right - convertedToPathsBounds.left)/2)) - 
						(origCopyBounds.left+((origCopyBounds.right - origCopyBounds.left)/2));
						ty = convertedToPathsBounds.top - newBounds.top;
					}
					
					//Apply the translation to the new one
					AIRealMatrix newMatrix;
					sAIRealMath->AIRealMatrixSetIdentity(&newMatrix);
					sAIRealMath->AIRealMatrixConcatTranslate(&newMatrix, tx, ty);
					sAITransformArt->TransformArt(theCopy, &newMatrix, 0, kTransformObjects);
					
					//Apply the rotation to the new one
					sAIRealMath->AIRealMatrixSetIdentity(&newMatrix);
					sAIRealMath->AIRealMatrixConcatRotate(&newMatrix, -angle);
					sAITransformArt->TransformArt(theCopy, &newMatrix, 0, kTransformObjects);
					
				}
			}
		}
        
        sAIArtSet->DisposeArtSet(&artSet);
	}
	return TRUE;
}


bool Align(ATE::ParagraphJustification alignStyle)
{
	short selectedArtTypes = 0;
	
	//Create an art set of the selected objects
	//CREATE THE ART SET
	AIArtSet artSet;
	sAIArtSet->NewArtSet(&artSet);
    
	AIArtSpec selectedSpecs[] =
    {
        { kAnyArt , kArtSelected , kArtSelected }
    };
	
    sAIArtSet->MatchingArtSet( selectedSpecs , 1, artSet );
		
	//Next, we need to find out what kind of objects we're dealing with
	selectedArtTypes = GetTypesFromArtSet(&artSet);
	
	try
    {
        if (selectedArtTypes == TTArtTypes::ttTextArt)
        {
			AlignText(&artSet, alignStyle);
		}
		else if (selectedArtTypes == TTArtTypes::ttUnknownArt)
        {
			throw TTArtTypes::ttUnknownArt;
		}
		else if ( ( selectedArtTypes == TTArtTypes::ttObjectArt ) ||
				  ( selectedArtTypes == (TTArtTypes::ttObjectArt | TTArtTypes::ttTextArt)) ||
				  ( selectedArtTypes == (TTArtTypes::ttObjectArt | TTArtTypes::ttUnknownArt) ) ||
				  ( selectedArtTypes == (TTArtTypes::ttTextArt | TTArtTypes::ttUnknownArt) ) ||
				  ( selectedArtTypes == (TTArtTypes::ttObjectArt | TTArtTypes::ttTextArt | TTArtTypes::ttUnknownArt)) )
        {
			AlignObject(&artSet, alignStyle);
		}
	}
	catch (int e)
    {
		sAIArtSet->DisposeArtSet(&artSet);
		
		sAIUndo->UndoChanges();
		return false;
	}
	
	sAIArtSet->DisposeArtSet(&artSet);
	return true;
}


void AlignText(AIArtSet* artSet, ATE::ParagraphJustification alignStyle)
{
	size_t count;
    sAIArtSet->CountArtSet( *artSet, &count );
	
    if (count == 1)
    {
		AIArtHandle currArtHandle = nullptr;
		sAIArtSet->IndexArtSet( *artSet, 0, &currArtHandle );
		try
        {
			AlignText(currArtHandle, alignStyle);
		}
		catch (int e)
        {
			throw;
		}
	}
    else if (count > 1)
    {
		AIArtHandle keyArt = nullptr;
		sAIArt->GetKeyArt(&keyArt);
		if (keyArt)
        {
			try
            {
				AlignObject(artSet, alignStyle);
			}
			catch (int e)
            {
				throw;
			}
		}
        else
        {
			for ( int i=0 ; i < count ; i++ )
            {
				AIArtHandle currArtHandle = nullptr;
				sAIArtSet->IndexArtSet( *artSet, i, &currArtHandle );
				try
                {
					AlignText(currArtHandle, alignStyle);
				}
				catch (int e)
                {
					throw;
				}
			}
		}
	}
}

void AlignText(AIArtHandle currArt, ATE::ParagraphJustification alignStyle)
{
	AITextFrameType type;
	sAITextFrame->GetType(currArt, &type);
	try
    {
		if (type == kInPathTextType || type == kOnPathTextType)
        {
			//Create an ITextFrame object from the original TextFrame art
			TextRangeRef ATETextRangeRef;
			sAITextFrame->GetATETextRange(currArt, &ATETextRangeRef);
			ATE::ITextRange iTextRange(ATETextRangeRef);
			ATE::IParaFeatures pFeat;
			pFeat.SetJustification(alignStyle);
			iTextRange.ReplaceOrAddLocalParaFeatures(pFeat);
		}
		else if (type == kPointTextType)
        {
			AlignPointType(currArt, alignStyle);
		}
		else if (type == kUnknownTextType)
        {
			throw ttUnknownArt;
		}
	}
	catch (int e)
    {
		throw;
	}
}

void AlignPointType(AIArtHandle currArtHandle, ATE::ParagraphJustification alignStyle)
{
	AIBoolean focus = FALSE;
	sAIDocument->HasTextFocus(&focus);
	if (focus) sAIDocument->LoseTextFocus();

	//Create an ITextFrame object from the original TextFrame art
	TextFrameRef origATETextFrameRef;
	sAITextFrame->GetATETextFrame(currArtHandle, &origATETextFrameRef);
	ATE::ITextFrame origITextFrame(origATETextFrameRef);
	
	//Find the bounds of the original with rotation removed
	AIRealMatrix origMatrix = origITextFrame.GetMatrix();
	AIReal angle, hscale, vscale;
	sAIRealMath->AIRealMatrixGetRotate(&origMatrix, &angle);
	sAIRealMath->AIRealMatrixGetScale(&origMatrix, &hscale, &vscale);
	
	//Get bounds of unrotated text
	AIRealRect unRotateBounds, postAlignBounds;
	AIRealMatrix unrotateMatrix;
	sAIRealMath->AIRealMatrixSetIdentity(&unrotateMatrix);
	sAIRealMath->AIRealMatrixConcatRotate(&unrotateMatrix, angle);
	sAITransformArt->TransformArt(currArtHandle, &unrotateMatrix, 0, kTransformObjects);
	sAIArt->GetArtTransformBounds(currArtHandle, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds|kNoExtendedBounds, &unRotateBounds);
	
	//Create an ITextFrame object from the original TextFrame art
	TextRangeRef ATETextRangeRef;
	sAITextFrame->GetATETextRange(currArtHandle, &ATETextRangeRef);
	ATE::ITextRange iTextRange(ATETextRangeRef);
	ATE::IParaFeatures pFeat;
	pFeat = iTextRange.GetUniqueLocalParaFeatures();
	
	pFeat.SetJustification(alignStyle);
	
	if (!iTextRange.GetSize())
    {
        iTextRange.InsertAfter(ai::UnicodeString(" ").as_ASUnicode().c_str());
		iTextRange.SetLocalParaFeatures(pFeat);
		iTextRange.Remove();
	}
    else
    {
		iTextRange.SetLocalParaFeatures(pFeat);
	}

	sAIArt->GetArtTransformBounds(currArtHandle, nullptr, kVisibleBounds|kExcludeHiddenObjectBounds, &postAlignBounds);
	
	//Find the x and y translation
	AIReal tx = 0.0, ty = 0.0;	
	
	if (alignStyle == ATE::kLeftJustify || alignStyle == ATE::kFullJustifyLastLineLeft ||
		alignStyle == ATE::kFullJustifyLastLineFull || alignStyle == ATE::kFullJustifyLastLineCenter ||
		alignStyle == ATE::kFullJustifyLastLineRight)
    {
		tx = unRotateBounds.left - postAlignBounds.left;
		ty = unRotateBounds.top - postAlignBounds.top;	
	}
    else if (alignStyle == ATE::kRightJustify)
    {
		tx = unRotateBounds.right - postAlignBounds.right;
		ty = unRotateBounds.top - postAlignBounds.top;
	}
    else if (alignStyle == ATE::kCenterJustify)
    {
		tx = (unRotateBounds.left+((unRotateBounds.right - unRotateBounds.left)/2)) - (postAlignBounds.left + ((postAlignBounds.right - postAlignBounds.left) / 2));
		ty = unRotateBounds.top - postAlignBounds.top;
	}
	
	//Apply the translation to the new one
	AIRealMatrix newMatrix;
	sAIRealMath->AIRealMatrixSetIdentity(&newMatrix);
	sAIRealMath->AIRealMatrixConcatTranslate(&newMatrix, tx, ty);
	//Add the rotation
	sAIRealMath->AIRealMatrixConcatRotate(&newMatrix, -angle);
	sAITransformArt->TransformArt(currArtHandle, &newMatrix, 0, kTransformObjects | kTransformChildren);
	
	if (focus)
    {
        sAIArt->SetArtUserAttr(currArtHandle, kArtSelected, kArtSelected);
    }
}

void AlignObject(AIArtSet* artSet, ATE::ParagraphJustification alignStyle)
{
	size_t count;
    sAIArtSet->CountArtSet( *artSet, &count );
	
	//Initialize the bounds
	AIRealRect foundBounds;
	
	//Loop the art set.
	//Each time through, we remove the art we've processed, so the artset shrinks each iteration
	//The first object in the set on each pass will be the next "root" object
	int i = 0;
	while ( i < count )
    {
		AIArtHandle currArtHandle = nullptr;
		sAIArtSet->IndexArtSet( *artSet, i, &currArtHandle );
				
        foundBounds = BtArtHandle(currArtHandle).Bounds();
		
		AlignObject(currArtHandle, currArtHandle, foundBounds, artSet, alignStyle);
		
		//Reset the count
		i = 0;
		sAIArtSet->CountArtSet( *artSet, &count );
	}
}

void AlignObject(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect selBounds, AIArtSet* artSet, ATE::ParagraphJustification alignStyle)
{
	AIArtHandle child, sibling;
	child = sibling = nullptr;
	AIReal tx = 0;
	
	//GET ALIGNMENT BOUNDS
	AIRealRect keyBounds;
	AIArtHandle keyArt = nullptr;
	sAIArt->GetKeyArt(&keyArt);
	if (keyArt)
    {
		sAIArt->GetArtTransformBounds(keyArt, nullptr, kVisibleBounds | kExcludeHiddenObjectBounds | kExcludeGuideBounds, &keyBounds);
	}
    else
    {
		AIPageTiling pageTiling;
		sAIDocumentView->GetPageTiling(&pageTiling);
		sAIRealMath->AIRealRectSet(&keyBounds, pageTiling.bounds.left, pageTiling.bounds.top, pageTiling.bounds.right, pageTiling.bounds.bottom);
	}
	
	//Store the amount to move
	if (alignStyle == ATE::kLeftJustify || alignStyle == ATE::kFullJustifyLastLineLeft ||
		alignStyle == ATE::kFullJustifyLastLineFull || alignStyle == ATE::kFullJustifyLastLineCenter ||
		alignStyle == ATE::kFullJustifyLastLineRight)
    {
		tx = keyBounds.left - selBounds.left;
	}
    else if (alignStyle == ATE::kRightJustify)
    {
		tx = keyBounds.right - selBounds.right;
	}
    else if (alignStyle == ATE::kCenterJustify)
    {
		tx = (keyBounds.left+((keyBounds.right - keyBounds.left)/2)) - (selBounds.left+((selBounds.right - selBounds.left)/2));
	}
	
	short artType = 0;
	sAIArt->GetArtType(currArtHandle, &artType);
	
	//CHECK IF CURR IS FULLY SELECTED
	int attr = 0;
	sAIArt->GetArtUserAttr(currArtHandle, kArtSelected | kArtFullySelected, &attr);
	AIBoolean pathFullySelected = TRUE;
	sAIPath->GetPathAllSegmentsSelected(currArtHandle, &pathFullySelected);
	
	if ((attr & kArtFullySelected) && pathFullySelected)
    {
		//******************* FULLY SELECTED OBJECTS
		//Apply the translation to the new one
		AIRealMatrix newMatrix;
		sAIRealMath->AIRealMatrixSetIdentity(&newMatrix);
		sAIRealMath->AIRealMatrixConcatTranslate(&newMatrix, tx, 0);
		sAITransformArt->TransformArt(currArtHandle, &newMatrix, 0,
									  kTransformObjects |
									  kTransformFillGradients |
									  kTransformFillPatterns |
									  kTransformStrokePatterns |
									  kTransformLinkedMasks |
									  kTransformChildren);
		RemoveChildrenFromArtSet(currArtHandle, currArtHandle, artSet);
	}
    else if (attr & kArtSelected)
    {
		//IF OBJECT IS NOT FULLY SELECTED
		//IF ITS A GROUP, WE'LL JUST GET THE CHILD
		if (artType == kGroupArt)
        {
			sAIArt->GetArtFirstChild(currArtHandle, &child);
			if (child)
            {
				//***MOVE CHILD
				AlignObject(root, child, selBounds, artSet, alignStyle);
			}
		}
        else
        {
			//IF ITS NOT A GROUP, IT MUST BE EITHER A PATH OR A COMPOUND PATH
			AIRealMatrix newMatrix;
			sAIRealMath->AIRealMatrixSetIdentity(&newMatrix);
			sAIRealMath->AIRealMatrixConcatTranslate(&newMatrix, tx, 0);
			
			if (artType == kCompoundPathArt)
            {
				sAITransformArt->TransformArt(currArtHandle, &newMatrix, 0,
											  kTransformObjects |
											  kTransformFillGradients |
											  kTransformFillPatterns |
											  kTransformStrokePatterns |
											  kTransformLinkedMasks |
											  kTransformChildren |
											  kTransformSelectionOnly);
			}
            else
            {
				sAITransformArt->TransformArt(currArtHandle, &newMatrix, 0,
											  kTransformObjects |
											  kTransformFillGradients |
											  kTransformFillPatterns |
											  kTransformStrokePatterns |
											  kTransformLinkedMasks |
											  kTransformSelectionOnly);
			}
		}
	}
	
	//CHECK CURR != ROOT
	if (currArtHandle != root)
    {
		sAIArt->GetArtSibling(currArtHandle, &sibling);
		if (sibling)
        {
			//*** MOVE SIBLING
			AlignObject(root, sibling, selBounds, artSet, alignStyle);
		}
	}
	
	sAIArtSet->RemoveArtFromArtSet(*artSet, currArtHandle);
}

short GetTypesFromArtSet(AIArtSet* artSet)
{
	short selectedArtTypes = 0;

	AIArtSet artToRemove;
	sAIArtSet->NewArtSet(&artToRemove);
	
	//Loop the art set
	size_t count;
    sAIArtSet->CountArtSet( *artSet, &count );
    
	for ( int i=0 ; i < count ; i++ )
    {
		AIArtHandle currArtHandle = nullptr;
		sAIArtSet->IndexArtSet( *artSet, i, &currArtHandle );
		
		short artType = 0;
		sAIArt->GetArtType(currArtHandle, &artType);
		
		switch (artType)
        {
			case kGroupArt:
			{
				//Check if the group art is a layer, we don't want the layer groups in our artset
				AIBoolean isLayer = FALSE;
				sAIArt->IsArtLayerGroup(currArtHandle, &isLayer);
				if (isLayer) { sAIArtSet->AddArtToArtSet(artToRemove, currArtHandle); break;}
				
				//We don't actually need to account for groups at all, since they are not visible artwork themselves
//				selectedArtTypes = selectedArtTypes | ttObjectArt;
				break; 
			}
			case kPathArt:
			{
				//We need to check path art and get rid of anything that is part of a compound path
				int attr = 0;
				sAIArt->GetArtUserAttr(currArtHandle, kArtPartOfCompound, &attr);
				if ((attr & kArtPartOfCompound))
                {
					AIArtHandle parent = nullptr;
					bool cmpPathFnd = FALSE;
					while (!cmpPathFnd)
                    {
						sAIArt->GetArtParent(currArtHandle, &parent);
						if (parent)
                        {
							short parArtType = 0;
							sAIArt->GetArtType(parent, &parArtType);
							if (parArtType == kCompoundPathArt)
                            {
//								sAIArt->GetArtUserAttr(parent, kArtFullySelected, &attr);
//								if ((attr & kArtFullySelected))
//                              {
									sAIArtSet->AddArtToArtSet(artToRemove, currArtHandle);
									cmpPathFnd = TRUE;
									break;
//								}
							}
						}
					}
				}
				//We also need to get rid of guide paths
				AIBoolean isGuide = FALSE;
				sAIPath->GetPathGuide(currArtHandle, &isGuide);
				if (isGuide)
                {
                    sAIArtSet->AddArtToArtSet(artToRemove, currArtHandle);
                    break;
                }
				
				selectedArtTypes = selectedArtTypes | ttObjectArt;
				break;
			}
			case kCompoundPathArt:
			case kPlacedArt:
			case kRasterArt:
			case kPluginArt:
			case kMeshArt:
			case kSymbolArt:
			case kForeignArt:
                selectedArtTypes = selectedArtTypes | TTArtTypes::ttObjectArt;
				break;
				
			case kTextFrameArt:
			case kLegacyTextArt:
				selectedArtTypes = selectedArtTypes | TTArtTypes::ttTextArt;
				break;
				
			case kUnknownArt:
				selectedArtTypes = selectedArtTypes | TTArtTypes::ttUnknownArt;
				break;
		}
	}
	
	//Loop through the removal set and take them out of the main set
	sAIArtSet->CountArtSet( artToRemove, &count );
	for ( int i=0 ; i < count ; i++ )
    {
		AIArtHandle currArtHandle = nullptr;
		sAIArtSet->IndexArtSet( artToRemove, i, &currArtHandle );
		sAIArtSet->RemoveArtFromArtSet(*artSet, currArtHandle);
	}
	sAIArtSet->DisposeArtSet(&artToRemove);
	
	return selectedArtTypes;
}

void RemoveChildrenFromArtSet(AIArtHandle root, AIArtHandle currArtHandle, AIArtSet* artSet)
{
	AIArtHandle sibling = nullptr;
	AIArtHandle child = nullptr;

	sAIArt->GetArtFirstChild(currArtHandle, &child);
	if (child)
    {
		RemoveChildrenFromArtSet(root, child, artSet);
	}
	if (currArtHandle != root)
    {
		sAIArt->GetArtSibling(currArtHandle, &sibling);
		if (sibling)
        {
			RemoveChildrenFromArtSet(root, sibling, artSet);
		}
		
		sAIArtSet->RemoveArtFromArtSet(*artSet, currArtHandle);
	}
}


bool CreateMICRBarcode()
{
	//CHECK IF THE BARCODE FONT IS INSTALLED
	AIFontKey currFontKey = nullptr;
	sAIFont->FindFont(BARCODE_FONT_NAME, kAIAnyFontTechnology, kUnknownAIScript, FALSE, &currFontKey);
	
	if (!currFontKey)
    {
		sAIUser->MessageAlert(ai::UnicodeString("The Barcode font was not found. Please make sure the correct font is loaded."));
		return FALSE;
	}
	
	//Check if we already have a micr line in the document dictionary
    DictionaryWriter dw;
	AIArtHandle micrLineHandle = nullptr;
	micrLineHandle = dw.GetArtHandleFromIdentifier(MICR_LINE_LABEL);
	
	//Create an art set of the selected objects
	//CREATE THE ART SET
	AIArtSet artSet;
	sAIArtSet->NewArtSet(&artSet);
    
	AIArtSpec selectedSpecs[] =
    {
        { kAnyArt , kArtSelected , kArtSelected }
    };
	
    sAIArtSet->MatchingArtSet( selectedSpecs , 1, artSet );
	size_t count;
    sAIArtSet->CountArtSet( artSet, &count );
	
	if ( count )
    {
		//Loop the art set
		for ( int i=0 ; i < count ; i++ )
        {
			AIArtHandle currArtHandle = nullptr;
			sAIArtSet->IndexArtSet( artSet, i, &currArtHandle );
			
			short atype = 0;
			sAIArt->GetArtType(currArtHandle, &atype);
			if (atype != kTextFrameArt)
            {
				continue;
			}
						
			//Create the ATE range
			ATE::TextRangeRef currATETextRangeRef;
			sAITextFrame->GetATETextRange(currArtHandle, &currATETextRangeRef);
			ATE::ITextRange currTextRange(currATETextRangeRef);
			
			//Create the runs iterator
			ATE::ITextRunsIterator it = currTextRange.GetTextRunsIterator();
			
			//Get the Features
			ATE::ICharFeatures currCharFeatures = it.Item().GetUniqueCharFeatures();
			
			//Get the font applied to the range
			bool isAssigned = false;
			ATE::IFont currFont = currCharFeatures.GetFont(&isAssigned);
			if (isAssigned)
            {
				//CHECK IF THE FONT ON THE SELECTED TEXT IS MICR
				//Get the FontRef
				FontRef currFontRef = currFont.GetRef();
				
				//Get the FontKey
				AIFontKey currFontKey = nullptr;
				sAIFont->FontKeyFromFont(currFontRef, &currFontKey);
				
				char fontName [256];
				sAIFont->GetFullFontName(currFontKey, fontName, 256);
				
				ai::UnicodeString usFontName(fontName);
				
				//Get the Font info
				AIFontStyle currFontStyle;
				sAIFont->GetFontInfo(currFontKey, &currFontStyle);
				
				if (usFontName != ai::UnicodeString(MICR_FONT_NAME))
                {
					continue;
				}
				
				//Make sure the text is in the MICR black color
				Bt::BtColor currRangeColor(GetAIColorFromATETextRange(currTextRange));
				
                if (! currRangeColor.ColorIsEqual(Bt::BtStandardColors().MicrBlack(), TRUE ) )
                {
                    continue;
                }
				
				
				//Tag the MICR line when we've found it, and store the handle in the dictionary
				if (micrLineHandle && (micrLineHandle != currArtHandle))
                {
					dw.RemoveIdentifierFromDictionary(MICR_LINE_LABEL);
				}
				micrLineHandle = currArtHandle;
				sAIArt->SetArtName(currArtHandle, ai::UnicodeString(MICR_LINE_LABEL));
				dw.AddArtHandleToDictionary(currArtHandle, MICR_LINE_LABEL);
				break;
			}
		}
	}
	
	sAIArtSet->DisposeArtSet(&artSet);
	
	//Now, if we managed to find a micr line
	if (micrLineHandle)
    {
		//Create the ATE range
		TextRangeRef currRangeRef = nullptr;
		sAITextFrame->GetATETextRange(micrLineHandle, &currRangeRef);
		ATE::TextRangeRef currATETextRangeRef(currRangeRef);
		ATE::ITextRange currTextRange(currATETextRangeRef);
		
		//currTextRange.ClearLocalCharFeatures();
		
		//Now we have all the info we need, so we'll convert the micr text string into the barcode text string
		char micrContents[256];
		currTextRange.GetContents(micrContents, 256);
		ai::UnicodeString barcodeString(CreateBarcodeStringFromMICRString(ai::UnicodeString(micrContents)));
		if (barcodeString.empty())
        {
            sAIUser->MessageAlert(ai::UnicodeString("The MICR number doesn't fit the expected format."));
			return FALSE;
		}
		
		//Check if we already have a MICR barcode, if not, create the new point text
		AIArtHandle barcodeTextFrame = nullptr;
		barcodeTextFrame = dw.GetArtHandleFromIdentifier(MICR_BARCODE_LABEL);
		
		if (!barcodeTextFrame)
        {
			AIRealPoint anchor;
			FindBarcodeAnchorPoint(&anchor);
			sAITextFrame->NewPointText(kPlaceAbove, micrLineHandle, kHorizontalTextOrientation, anchor, &barcodeTextFrame);
			sAIArt->SetArtName(barcodeTextFrame, ai::UnicodeString(MICR_BARCODE_LABEL));
			
			dw.AddArtHandleToDictionary(barcodeTextFrame, MICR_BARCODE_LABEL);
		}
		
		//Create the ATE range
		ATE::TextRangeRef barcodeATETextRangeRef;
		sAITextFrame->GetATETextRange(barcodeTextFrame, &barcodeATETextRangeRef);
		ATE::ITextRange barcodeTextRange(barcodeATETextRangeRef);
		
		//Clear the range of whatever was there
		barcodeTextRange.Remove();
		
        BtAteTextFeatures barcodeFeatures;
        barcodeFeatures.FontSize(12).Font(BARCODE_FONT_NAME).Justification(ATE::kCenterJustify).FillStyle({Bt::BtStandardColors().MicrBlack(), true});
        
		barcodeFeatures.AddTextToRangeWithFeatures(barcodeString.as_Platform(), barcodeTextRange);
	}
    else
    {
		sAIUser->MessageAlert(ai::UnicodeString("Could not locate the MICR number. Please select the MICR text box before using this tool."));
		return FALSE;
	}
	
	return TRUE;
}

ai::UnicodeString CreateBarcodeStringFromMICRString(ai::UnicodeString micrString)
{
	/*  We need to strip any leading/trailing consecutive numbering and spaces
	 We need to add ! at the start and end
	 We need to replace any spaces with =
	 */
    
    string s = micrString.as_Platform();
    
    std::regex r("(?:[C]*[\\d]*[C]*[ ]*)((([A][\\d|D]{9}[A])[ ]*([C]|[D]|[B]|[\\d]|[ ])+[C])([ ]*[\\d]*)?)");
    std::smatch result;
    std::regex_search(s,result, r);
    
    if (result.length() < 2) return ai::UnicodeString();
    
    ai::UnicodeString formattedResult(result[1]);
    
    formattedResult = ai::UnicodeString("!").append(formattedResult);
    formattedResult += ai::UnicodeString("!");
    
    string::size_type loc = 0;
    while (loc != string::npos)
    {
        loc = formattedResult.find_first_of(ai::UnicodeString(" "));
        if (loc != string::npos)
        {
            formattedResult.replace(loc, 1, ai::UnicodeString("="));
        }
    }
    
    return formattedResult;
}


void FindBarcodeAnchorPoint(AIRealPoint* anchor)
{
	//We get it from the Page Tiling
	AIPageTiling pageTiling;
	sAIDocumentView->GetPageTiling(&pageTiling);
	
	AIRealRect cropAreaRect;
	
	sAIRealMath->AIRealRectSet(&cropAreaRect, pageTiling.bounds.left, pageTiling.bounds.top, pageTiling.bounds.right, pageTiling.bounds.bottom);
	
	anchor->h = (cropAreaRect.right - cropAreaRect.left) / 2;
	anchor->v = cropAreaRect.bottom - LASER_BARCODE_OFFSET;
	
	return;
}
