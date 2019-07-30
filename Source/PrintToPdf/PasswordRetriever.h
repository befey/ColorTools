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

namespace PrintToPdf
{
    static constexpr auto PATH_TO_PASSWORD_FILE =     "/Volumes/WIP/WIP/General Resources/AI PLUGINS/Plugin Data/passwords.txt";
    
    class PasswordRetriever
    {
    public:
        virtual string GetUserPassword() const = 0;
        virtual string GetMasterPassword() const = 0;
        
        bool IsValid() const { return isValid; };
    protected:
        bool isValid;
        string UserPassword;
        string MasterPassword;
        
        void LoadFromFile();
    };
    
    class NonePasswordRetriever : public PasswordRetriever
    {
    public:
        NonePasswordRetriever() { isValid = true; };
        
        string GetUserPassword() const { return ""; }
        string GetMasterPassword() const { return ""; }
    };
    
    class ProofPasswordRetriever : public PasswordRetriever
    {
    public:
        ProofPasswordRetriever() { LoadFromFile(); };
        
        string GetUserPassword() const { return ""; }
        string GetMasterPassword() const { return MasterPassword; }
    };
    
    class MicrPasswordRetriever : public PasswordRetriever
    {
    public:
        MicrPasswordRetriever() { LoadFromFile(); };
        
        string GetUserPassword() const { return UserPassword; }
        string GetMasterPassword() const { return MasterPassword; }
    };
    
    class PlateRequestPasswordRetriever : public PasswordRetriever
    {
    public:
        PlateRequestPasswordRetriever() { LoadFromFile(); };
        
        string GetUserPassword() const { return UserPassword; }
        string GetMasterPassword() const { return MasterPassword; }
    };
}

#endif /* defined(__SafeguardTools__PasswordRetriever__) */
