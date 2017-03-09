//
//  ColorListDuplicateChecker.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/24/17.
//
//

#include "ColorListDuplicateChecker.hpp"

bool ColorListDuplicateChecker::operator() (const BtColor& compare) const
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

ColorListDuplicateChecker::ColorListDuplicateChecker(const BtColor& base)
: base(base)
{
    //Will run each of these tests, in order
    //********** Must make sure two tests will not both set found1 = true for the same pair or when base == compare, base will be removed and crash
    
    typedef std::function<bool(const BtColor&, const BtColor&)> predFunc;
    
    predFunc NonColor = [] (const BtColor& base, const BtColor& compare)
    {
        if (compare.Kind() == kPattern || compare.Kind() == kGradient || compare.Kind() == kNoneColor)
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(NonColor));
    
    predFunc ExactMatch = [&found1 = found1] (const BtColor& base, const BtColor& compare)
    {
        if (base == compare && &base != &compare)
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(ExactMatch));
    
    predFunc GrayColor = [&found1 = found1] (const BtColor& base, const BtColor& compare)
    {
        if (base.Kind() == kGrayColor && base.Kind() == compare.Kind() && base.Method() == compare.Method())
        {
            if (base.AiColor().c.g == compare.AiColor().c.g)
            {
                if (!found1)
                {
                    found1 = true;
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(GrayColor));

    predFunc ProcessColors = [&found1 = found1] (const BtColor& base, const BtColor& compare)
    {
        if (base.PrintsAsProcess() && compare.PrintsAsProcess())
        {
            if (!found1)
            {
                found1 = true;
                return false;
            }
            else
            {
                return true;
            }
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(ProcessColors));
    
    predFunc CustomColor = [&found1 = found1] (const BtColor& base, const BtColor& compare)
    {
        if (base.Kind() == kCustomColor && base.Kind() == compare.Kind() && base.Method() == compare.Method())
        {
            if (base.AiColor().c.c.color == compare.AiColor().c.c.color && !base.PrintsAsProcess())
            {
                if (!found1)
                {
                    found1 = true;
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(CustomColor));
        
    predFunc RemoveBlackIfProcess = [&found1 = found1] (const BtColor& base, const BtColor& compare)
    {
        if (base.PrintsAsProcess() && compare.IsBlack(false) && base.Method() == compare.Method())
        {
            return true;
        }
        return false;
    };
    AddPredicate(ColorListDuplicatePredicate(RemoveBlackIfProcess));
}
