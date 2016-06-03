//
//  PasswordRetriever.h
//  SafeguardTools
//
//  Created by T431962 on 5/4/16.
//
//

#ifndef __SafeguardTools__PasswordRetriever__
#define __SafeguardTools__PasswordRetriever__

#include <string.h>
#include "passwords.h"

namespace PrintToPdf
{
    class PasswordRetriever
    {
    public:
        virtual string GetUserPassword() = 0;
        virtual string GetMasterPassword() = 0;
    };
    
    class NonePasswordRetriever : public PasswordRetriever
    {
    public:
        string GetUserPassword() { return ""; }
        string GetMasterPassword() { return ""; }
    };
    
    class ProofPasswordRetriever : public PasswordRetriever
    {
    public:
        string GetUserPassword() { return ""; }
        string GetMasterPassword() { return MasterPassword; }
    };
    
    class MicrPasswordRetriever : public PasswordRetriever
    {
    public:
        string GetUserPassword() { return UserPassword; }
        string GetMasterPassword() { return MasterPassword; }
    };
}

#endif /* defined(__SafeguardTools__PasswordRetriever__) */
