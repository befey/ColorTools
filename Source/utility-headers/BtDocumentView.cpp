//
//  BtDocumentView.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#include "BtDocumentView.hpp"
#include "DictionaryWriter.h"


void BtDocumentView::StoreCurrentDocumentView()
{
    sAIDocumentView->GetDocumentViewCenter(nullptr, &center);
    sAIDocumentView->GetDocumentViewZoom(nullptr, &zoom);
    DictionaryWriter dw;
    dw.AddAIRealToDictionary(center.h, BTDOCVIEW_CENTERH);
    dw.AddAIRealToDictionary(center.v, BTDOCVIEW_CENTERV);
    dw.AddAIRealToDictionary(zoom, BTDOCVIEW_ZOOM);
}

void BtDocumentView::RecallDocumentView()
{
    DictionaryWriter dw;
    center.h = dw.GetAIRealFromIdentifier(BTDOCVIEW_CENTERH);
    center.v = dw.GetAIRealFromIdentifier(BTDOCVIEW_CENTERV);
    zoom = dw.GetAIRealFromIdentifier(BTDOCVIEW_ZOOM);
    sAIDocumentView->SetDocumentViewCenter(nullptr, &center);
    sAIDocumentView->SetDocumentViewZoom(nullptr, zoom);
}

void BtDocumentView::SetViewOnArtboard(ai::ArtboardID id)
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, id, props);
    
    AIRealRect bounds;
    sAIArtboard->GetPosition(props, bounds);
    AIReal abWidth = bounds.right - bounds.left;
    AIReal abHeight = bounds.top - bounds.bottom;
    AIRealPoint center {.h = bounds.left + ( abWidth / 2 ),
                        .v = bounds.top - ( abHeight / 2 ) };
    
    sAIDocumentView->SetDocumentViewCenter(nullptr, &center);
    
    AIRealRect viewBounds;
    sAIDocumentView->GetDocumentViewBounds(nullptr, &viewBounds);
    
    AIReal viewWidth = viewBounds.right - viewBounds.left;
    AIReal viewHeight = viewBounds.top - viewBounds.bottom;
    
    auto abRatio = abWidth / abHeight;
    auto viewRatio = viewWidth / viewHeight;

    AIReal zoom = viewRatio > abRatio ? viewHeight / abHeight : viewWidth / abWidth;
    AIReal currZoom;
    sAIDocumentView->GetDocumentViewZoom(nullptr, &currZoom);
    sAIDocumentView->SetDocumentViewZoom(nullptr, currZoom * zoom * .9);
    sAIDocument->RedrawDocument();
    sAIUser->AppIdle();
}
