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
    ColorListCheckerPredicate(std::function<bool(const BtColor&, const BtColor&)> predicate) : predicate(predicate) {};
    bool operator() (const BtColor& base, const BtColor& compare) const { return predicate(base, compare); };
private:
    std::function<bool(const BtColor&, const BtColor&)> predicate;
};

class ColorListChecker
{
public:
    virtual bool operator() (const BtColor& compare) const = 0;
    
    void AddPredicate(ColorListCheckerPredicate predicate) { predicates.push_back(predicate); };
    
protected:
    ColorListChecker(const BtColor& base) : base(base) {};
    ~ColorListChecker() {};
    
    vector<ColorListCheckerPredicate> predicates;
    const BtColor& base;
};


class ColorListDuplicateChecker : public ColorListChecker
{
public:
    ColorListDuplicateChecker(const BtColor& base);
    
    bool operator() (const BtColor& compare) const override;
    
private:
};

class ColorListNonSolidColorChecker : public ColorListChecker
{
public:
    ColorListNonSolidColorChecker(const BtColor& base);
    
    bool operator() (const BtColor& compare) const override;
        
private:
};


#endif /* ColorListChecker_hpp */
