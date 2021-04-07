//
//  FilePlacer.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/13/17.
//
//

#ifndef FilePlacer_hpp
#define FilePlacer_hpp

#include "AIPlaced.h"

extern AIPlacedSuite* sAIPlaced;

namespace PlaceFileSearch
{
    class FilePlacer
    {
    public:
        FilePlacer(string path) : path(path) {};
        void Place() const;
    private:
        string path;
    };
}

#endif /* FilePlacer_hpp */
