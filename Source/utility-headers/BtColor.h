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

//=================================
// forward declared dependencies



//=================================
// BtColor - wrapper for a AICustomColor definition
class BtColor
{
public:
    //Constuctor
    BtColor(string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag);
    BtColor();

    //Getters/Setters
    std::string GetName() const;
    AICustomColorTag GetKind() const;
    AICustomColorUnion GetCustomColorUnion() const;
    AICustomColorFlags GetCustomColorFlags() const;
    
    //Behaviors
    
    
private:
    
    //Members
    string pName;
    AICustomColorTag pKind;
    AICustomColorUnion pC;
    AICustomColorFlags pFlag;
};


#endif /* defined(__SafeguardTools__BtColor__) */
