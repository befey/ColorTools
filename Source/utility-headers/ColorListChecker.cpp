//
//  ColorListChecker.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/24/17.
//
//

#include "ColorListChecker.hpp"

typedef std::function<bool(const BtColor&, const BtColor&)> predFunc;

bool ColorListDuplicateChecker::operator() (const BtColor& compare) const
{
    for (auto predicate : predicates)
    {
        if ( &base == &compare )
        {
            return false;
        }
        else if ( predicate(base, compare) )
        {
            return true;
        }
    }
    return false;
}

ColorListDuplicateChecker::ColorListDuplicateChecker(const BtColor& base)
: ColorListChecker(base)
{
    //Will run each of these tests, in order
    
    predFunc ExactMatch = [] (const BtColor& base, const BtColor& compare)
    {
        if (base == compare && base.Name() == compare.Name())
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(ExactMatch));
    
    predFunc BothBlack = [] (const BtColor& base, const BtColor& compare)
    {
        if (base.IsBlack() && compare.IsBlack())
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(BothBlack));
    
    predFunc GrayColor = [] (const BtColor& base, const BtColor& compare)
    {
        if (base.Kind() == kGrayColor && base.Kind() == compare.Kind() && base.Method() == compare.Method())
        {
            if (base.AiColor().c.g == compare.AiColor().c.g)
            {
                    return true;
            }
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(GrayColor));

    predFunc ProcessColors = [] (const BtColor& base, const BtColor& compare)
    {
        if (base.PrintsAsProcess() && compare.PrintsAsProcess())
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(ProcessColors));
    
    predFunc CustomColor = [] (const BtColor& base, const BtColor& compare)
    {
        if (base.Kind() == kCustomColor && base.Kind() == compare.Kind() && base.Method() == compare.Method())
        {
            if (base.AiColor().c.c.color == compare.AiColor().c.c.color && !base.PrintsAsProcess())
            {
                return true;
            }
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(CustomColor));
}


bool ColorListNonSolidColorChecker::operator() (const BtColor& compare) const
{
    for (auto predicate : predicates)
    {
        if ( predicate(base, compare) )
        {
            return true;
        }
    }
    return false;
}

ColorListNonSolidColorChecker::ColorListNonSolidColorChecker(const BtColor& base)
: ColorListChecker(base)
{
    //Will run each of these tests, in order
    
    predFunc NonColor = [] (const BtColor& base, const BtColor& compare)
    {
        if (compare.Kind() == kPattern || compare.Kind() == kGradient || compare.Kind() == kNoneColor)
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(NonColor));
        
    predFunc RemoveBlackIfProcess = [] (const BtColor& base, const BtColor& compare)
    {
        if (base.PrintsAsProcess() && compare.IsBlack(false) && base.Method() == compare.Method())
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListCheckerPredicate(RemoveBlackIfProcess));
}
