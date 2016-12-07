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
        virtual unique_ptr<PasswordRetriever> Clone() const = 0;
        
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
        
        unique_ptr<PasswordRetriever> Clone() const override { return make_unique<NonePasswordRetriever>(); };
        
        string GetUserPassword() const override { return ""; }
        string GetMasterPassword() const override { return ""; }
    };
    
    class ProofPasswordRetriever : public PasswordRetriever
    {
    public:
        ProofPasswordRetriever() { LoadFromFile(); };
        
        unique_ptr<PasswordRetriever> Clone() const override { return make_unique<ProofPasswordRetriever>(); };
        
        string GetUserPassword() const override { return ""; }
        string GetMasterPassword() const override { return MasterPassword; }
    };
    
    class MicrPasswordRetriever : public PasswordRetriever
    {
    public:
        MicrPasswordRetriever() { LoadFromFile(); };
        
        unique_ptr<PasswordRetriever> Clone() const override { return make_unique<MicrPasswordRetriever>(); };
        
        string GetUserPassword() const override { return UserPassword; }
        string GetMasterPassword() const override { return MasterPassword; }
    };
}

#endif /* defined(__SafeguardTools__PasswordRetriever__) */
