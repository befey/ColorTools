//
//  PdfArtObjectCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 2/12/18.
//
//

#ifndef PdfArtObjectCommand_hpp
#define PdfArtObjectCommand_hpp

#include "IBtCommand.hpp"
#include "AIArtSet.h"

extern AIArtSetSuite* sAIArtSet;

namespace PrintToPdf
{
    class PdfArtObjectCommand : public VerifySuccessCommand
    {
    public:
        void AddCommand(shared_ptr<PdfArtObjectCommand> command) { commands.push_back(command); };
        
        bool Execute() const override;
        
    protected:
        PdfArtObjectCommand(bool requireSuccess) : VerifySuccessCommand(requireSuccess) { };
        ~PdfArtObjectCommand() {};
        
        vector<shared_ptr<IBtCommand>> commands;
    };
    
    class ManufacturingPdfArtObjectCommand : public PdfArtObjectCommand
    {
    public:
        ManufacturingPdfArtObjectCommand(bool requireSuccess) : PdfArtObjectCommand(requireSuccess) {};
        
        bool Execute() const override;
    private:
        
    };
}

#endif /* PdfArtObjectCommand_hpp */
