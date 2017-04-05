//
//  ExistingFileDeleter.h
//  SafeguardTools
//
//  Created by T431962 on 6/6/16.
//
//

#ifndef __SafeguardTools__ExistingFileDeleter__
#define __SafeguardTools__ExistingFileDeleter__

#include "FilesystemResults.hpp"

class ExistingFileDeleter
    {
    public:
        ExistingFileDeleter(bool doNotDelete) : doNotDelete(doNotDelete) {};
        
        static unique_ptr<ExistingFileDeleter> GetExistingFileDeleter(bool doNotDelete);
        
        FilesystemResults Delete(string matchString, ai::FilePath);
        
    private:
        bool doNotDelete;
    };

#endif /* defined(__SafeguardTools__ExistingFileDeleter__) */
