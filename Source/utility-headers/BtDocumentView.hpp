//
//  BtDocumentView.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#ifndef BtDocumentView_hpp
#define BtDocumentView_hpp

#include "AIDocumentView.h"
#include "AIArtboard.h"

extern AIDocumentViewSuite* sAIDocumentView;
extern AIArtboardSuite* sAIArtboard;

class BtDocumentView
{
public:
    static constexpr auto BTDOCVIEW_ZOOM =    "btdocview_zoom";
    static constexpr auto BTDOCVIEW_CENTERH = "btdocview_centerh";
    static constexpr auto BTDOCVIEW_CENTERV = "btdocview_centerv";
    
    void StoreCurrentDocumentView();
    void RecallDocumentView();
    void SetViewOnArtboard(ai::ArtboardID id);
private:
    AIRealPoint center;
    AIReal zoom;
};

#endif /* BtDocumentView_hpp */
