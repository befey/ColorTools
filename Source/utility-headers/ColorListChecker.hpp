//
//  ColorListChecker.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/24/17.
//
//

#ifndef ColorListChecker_hpp
#define ColorListChecker_hpp

#include <functional>
#include "BtColor.h"

class ColorListCheckerPredicate
{
public:
    ColorListCheckerPredicate(std::function<bool(const Bt::BtColor&, const Bt::BtColor&)> predicate) : predicate(predicate) {};
    bool operator() (const Bt::BtColor& base, const Bt::BtColor& compare) const { return predicate(base, compare); };
private:
    std::function<bool(const Bt::BtColor&, const Bt::BtColor&)> predicate;
};

class ColorListChecker
{
public:
    virtual bool operator() (const Bt::BtColor& compare) const = 0;
    
    void AddPredicate(ColorListCheckerPredicate predicate) { predicates.push_back(predicate); };
    
protected:
    ColorListChecker(const Bt::BtColor& base) : base(base) {};
    ~ColorListChecker() {};
    
    vector<ColorListCheckerPredicate> predicates;
    const Bt::BtColor& base;
};


class ColorListDuplicateChecker : public ColorListChecker
{
public:
    ColorListDuplicateChecker(const Bt::BtColor& base);
    
    bool operator() (const Bt::BtColor& compare) const override;
    
private:
};

class ColorListNonSolidColorChecker : public ColorListChecker
{
public:
    ColorListNonSolidColorChecker(const Bt::BtColor& base);
    
    bool operator() (const Bt::BtColor& compare) const override;
        
private:
};


#endif /* ColorListChecker_hpp */
