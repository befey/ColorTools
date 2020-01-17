//
//  BtColor.h
//  SafeguardTools
//
//  Created by T431962 on 2/16/15.
//
//

#ifndef __SafeguardTools__BtColor__
#define __SafeguardTools__BtColor__


//=================================
// included dependencies
#include <string>
#include "AIColor.h"
#include "AIColorConversion.h"
#include "AISwatchList.h"
#include "SafeguardFileConstants.h"
#include "ColorFuncs.h"
#include "ICanBeTextRange.h"
#include "AIATETextUtil.h"
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"

extern AIATETextUtilSuite* sAIATETextUtil;
extern AICustomColorSuite* sAICustomColor;
extern AIColorConversionSuite* sAIColorConversion;
extern AISwatchListSuite *sAISwatchList;

static constexpr AIReal TOLERANCE = .002;
static constexpr bool DoNotIncludeCMYKBuilds = false;
#define	kCustomProcessColor 0x0000

namespace Bt
{
    //=================================
    // BtColor - wrapper for a AICustomColor definition
    class BtColor : public ICanBeTextRange
    {
    public:
        //Constuctors
        BtColor() {};
        BtColor(AIColor aiColor) :  BtColor(aiColor, SafeguardFile::InkMethod::INVAL) {};
        BtColor(AIColor aiColor, SafeguardFile::InkMethod method);
        
        BtColor(AICustomColor aiCustomColor, std::string name) : BtColor(aiCustomColor, name, 0) {};
        BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint) : BtColor(aiCustomColor, name, tint, SafeguardFile::InkMethod::INVAL) {};
        BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint, SafeguardFile::InkMethod method);
        
        BtColor(AIColorTag kind, AIColorUnion c) : BtColor(kind, c, SafeguardFile::InkMethod::INVAL) {};
        BtColor(AIColorTag kind, AIColorUnion c, SafeguardFile::InkMethod method);
        
        BtColor(std::string name); //Not guaranteed to create a complete color definition if color is not in book!!
        BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag) : BtColor(name, kind, c, flag, 0) {};
        BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint) : BtColor(name, kind, c, flag, tint, SafeguardFile::InkMethod::INVAL) {};
        BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint, SafeguardFile::InkMethod method);
        
        bool Null() const;
        
        //Getters/Setters
        BtColor& AiColor(AIColor newVal, SafeguardFile::InkMethod method);
        AIColor AiColor() const { return aiColor; };
        
        BtColor& AiCustomColor(AICustomColor newVal, std::string name, AIReal tint, SafeguardFile::InkMethod method);
        const AICustomColor AiCustomColor() const;
        BtColor& AiCustomColorHandle(AICustomColorHandle newVal);
        const AICustomColorHandle AiCustomColorHandle() const { return aiCustomColorHandle; };
        
        BtColor& Name(std::string newVal);
        std::string Name() const;
        bool CompareName(std::string name) const;
        
        AISwatchRef CheckSwatchListForColor() const;
        
        AIColorTag Kind() const { return aiColor.kind; };
        
        AICustomColorFlags CustomFlag() const;
        BtColor& CustomFlag(AICustomColorFlags newVal);
        
        BtColor& Method(SafeguardFile::InkMethod newVal) { method = newVal; return *this; };
        SafeguardFile::InkMethod Method() const { return method; };
        
        BtColor& Tint(AIReal newVal) { aiColor.c.c.tint = newVal; return *this; };
        AIReal Tint() const;
        
        //Color Type checks
        bool PrintsAsProcess() const;
        bool PrintsAsSpot() const;
        
        AILabColorStyle GetLabApproximation() const;
        
        bool IsBlack(bool includeCMYKBuilds = true) const;
        bool IsWhite() const;
        bool IsGripper() const;
        bool IsKeyline() const;
        bool IsPantone() const; //Returns true if the color name includes PANTONE
        bool IsNumbering() const; //Returns true if the color name includes NUMBERING
        bool IsNonPrinting() const;
        bool IsRegistration() const;
        
        static BtColor RegistrationColor();
        
        bool ColorIsEqual (const BtColor& color2 , const bool ignoreTints ) const;
        
        friend bool operator==(const BtColor& lhs, const BtColor& rhs);
        friend bool operator!=(const BtColor& lhs, const BtColor& rhs) { return !operator==(lhs,rhs); };
        friend bool operator< (const BtColor& lhs, const BtColor& rhs);
        
        inline operator AIColor(void) const { return aiColor; }
        
    private:
        AIColor aiColor;
        AICustomColor aiCustomColor;
        AICustomColorHandle aiCustomColorHandle = nullptr;
        
        SafeguardFile::InkMethod method = SafeguardFile::InkMethod::INVAL;
        
        void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const override;
        
        friend class cereal::access;
        template <class Archive,
        cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
        void save( Archive & ar ) const;
        friend class cereal::access;
        template <class Archive,
        cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
        void load( Archive & ar );
    };
    
    #include "BtColor.tpp"
    
    class BtStandardColors
    {
    public:
        BtColor Black();
        BtColor White();
        BtColor MicrBlack();
        BtColor Keyline();
        BtColor Gripper();
        BtColor NumberingRed();
        BtColor NumberingBlack();
        
        std::unordered_map<std::string, Bt::BtColor> BasicColorDefinitions();
        std::unordered_map<std::string, Bt::BtColor> SafeguardColorDefinitions();
    };
}

#endif /* defined(__SafeguardTools__BtColor__) */
