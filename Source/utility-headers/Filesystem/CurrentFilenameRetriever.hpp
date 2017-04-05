//
//  CurrentFilenameRetriever.hpp
//  SafeguardTools
//
//  Created by T431962 on 12/2/16.
//
//

#ifndef CurrentFilenameRetriever_hpp
#define CurrentFilenameRetriever_hpp

#include "AIDocument.h"
#include "AIFilePath.h"

extern AIDocumentSuite* sAIDocument;

class CurrentFilenameRetriever
{
public:
    static string GetFilenameNoExt()
    {
        ai::FilePath openedFilePath;
        sAIDocument->GetDocumentFileSpecification(openedFilePath);
        return openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding);
    }
};

#endif /* CurrentFilenameRetriever_hpp */
