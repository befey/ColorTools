//
//  BleedInfoPluginArtToArtboardMatcher.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/15/16.
//
//

#ifndef BleedInfoPluginArtToArtboardMatcher_hpp
#define BleedInfoPluginArtToArtboardMatcher_hpp

#include <map>

namespace PlateBleedInfo
{
    class BleedInfoPluginArtToArtboardMatcher
    {
    public:
        BleedInfoPluginArtToArtboardMatcher();
        
        bool IsBleedInfoPluginArtCreated(int artboardIndex = 0);
    private:
        std::map<int, AIArtHandle> sortedPluginArts;
        
        void DisposePluginArtHandlesWithNoArtboard(vector<AIArtHandle> savedInDictPluginArts) const;
    };
}

#endif /* BleedInfoPluginArtToArtboardMatcher_hpp */
