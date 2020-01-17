//
//  BleedInfoWriter.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/16/16.
//
//

#ifndef BleedInfoWriter_hpp
#define BleedInfoWriter_hpp

#include "BleedInfo.h"
#include <string>

namespace PlateBleedInfo
{
    class IBleedInfoWriterStrategy
    {
    public:
        virtual void Write(const BleedInfo* bleedInfo) const = 0;
    protected:
        ~IBleedInfoWriterStrategy() {};
    };
    
    class BleedInfoToArtStrategy : public IBleedInfoWriterStrategy
    {
    public:
        BleedInfoToArtStrategy(AIArtHandle art) : art(art) {};
        void Write(const BleedInfo* bleedInfo) const override;
    private:
        AIArtHandle art;
    };
    
    class BleedInfoToJsonStrategy : public IBleedInfoWriterStrategy
    {
    public:
        BleedInfoToJsonStrategy(std::string& json, bool fullColorName = false) : json(json), fullColorName(fullColorName) {};
        void Write(const BleedInfo* bleedInfo) const override;
    private:
        std::string& json;
        bool fullColorName;
    };
    
    class BleedInfoWriter
    {
    public:
        BleedInfoWriter(const BleedInfo* bleedInfo, shared_ptr<IBleedInfoWriterStrategy> strategy) : strategy(strategy) { DoIt(bleedInfo); };
    private:
        shared_ptr<IBleedInfoWriterStrategy> strategy;
        
        void DoIt(const BleedInfo* bleedInfo) const { strategy->Write(bleedInfo); };
    };
}

#endif /* BleedInfoWriter_hpp */
