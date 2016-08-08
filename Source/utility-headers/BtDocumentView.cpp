//
//  BtDocumentView.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#include "BtDocumentView.hpp"


void BtDocumentView::StoreCurrentDocumentView()
{
    sAIDocumentView->GetDocumentViewCenter(NULL, &center);
    sAIDocumentView->GetDocumentViewZoom(NULL, &zoom);
}

void BtDocumentView::RecallDocumentView()
{
    sAIDocumentView->SetDocumentViewCenter(NULL, &center);
    sAIDocumentView->SetDocumentViewZoom(NULL, zoom);
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
    
    sAIDocumentView->SetDocumentViewCenter(NULL, &center);
    
    AIRealRect viewBounds;
    sAIDocumentView->GetDocumentViewBounds(NULL, &viewBounds);
    
    AIReal viewWidth = viewBounds.right - viewBounds.left;
    AIReal viewHeight = viewBounds.top - viewBounds.bottom;
     
    auto abRatio = abWidth / abHeight;
    auto viewRatio = viewWidth / viewHeight;

    AIReal zoom = viewRatio > abRatio ? viewHeight / abHeight : viewWidth / abWidth;
    AIReal currZoom;
    sAIDocumentView->GetDocumentViewZoom(NULL, &currZoom);
    sAIDocumentView->SetDocumentViewZoom(NULL, currZoom * zoom * .9);

}