//
//  PasswordRetriever.cpp
//  SafeguardTools
//
//  Created by T431962 on 5/4/16.
//
//

#include "PasswordRetriever.h"

using PrintToPdf::PasswordRetriever;

void PasswordRetriever::LoadFromFile()
{
    ifstream in(PATH_TO_PASSWORD_FILE);
    
    if (in.is_open())
    {
        string passwordType, password;
        while ( getline(in, passwordType, ';') )
        {
            getline(in, password);

            if (passwordType == "UserPassword")
            {
                UserPassword = password;
            }
            if (passwordType == "MasterPassword")
            {
                MasterPassword = password;
            }
        }
        in.close();
        isValid = true;
    }
    else
    {
        isValid = false;
        sAIUser->ErrorAlert(ai::UnicodeString("The PDF passwords file could not be found."));
    }
}
