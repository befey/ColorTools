//
//  ColorListDuplicateChecker.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/24/17.
//
//

#ifndef ColorListDuplicateChecker_hpp
#define ColorListDuplicateChecker_hpp

#include <functional>
#include "BtColor.h"

class ColorListDuplicatePredicate
{
public:
    ColorListDuplicatePredicate(std::function<bool(const BtColor&, const BtColor&)> predicate) : predicate(predicate) {};
    bool operator() (const BtColor& base, const BtColor& compare) const { return predicate(base, compare); };
private:
    std::function<bool(const BtColor&, const BtColor&)> predicate;
};

class ColorListDuplicateChecker
{
public:
    ColorListDuplicateChecker(const BtColor& base);
    
    bool operator() (const BtColor& compare) const;
    
    void AddPredicate(ColorListDuplicatePredicate predicate) { predicates.push_back(predicate); };
    
private:
    vector<ColorListDuplicatePredicate> predicates;
    bool found1 = false;
    const BtColor& base;
};

#endif /* ColorListDuplicateChecker_hpp */
