//
//  PrintToPdfCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#ifndef PrintToPdfCommand_hpp
#define PrintToPdfCommand_hpp

#include "IBtCommand.hpp"
#include "SafeguardFileConstants.h"
#include "PrintToPdfConstants.h"

class PrintToPdfCommand : public IBtCommand
{
public:
    PrintToPdfCommand() {};
    
    void AddCommand(shared_ptr<VerifySuccessCommand> command) { commands.push_back(command); };
    bool Execute() const override;
    
private:
    vector<shared_ptr<VerifySuccessCommand>> commands;
};

class SetLayerVisibilityCommand : public VerifySuccessCommand
{
public:
    SetLayerVisibilityCommand(SafeguardFile::ProductType productType, PrintToPdf::PdfPreset preset, bool requireSuccess) : VerifySuccessCommand(requireSuccess), productType(productType), preset(preset) {};
    
    bool Execute() const override;
    
private:
    SafeguardFile::ProductType productType;
    PrintToPdf::PdfPreset preset;
};

class ConvertTypeToPathsCommand : public VerifySuccessCommand
{
public:
    ConvertTypeToPathsCommand(bool requireSuccess) : VerifySuccessCommand(requireSuccess) {};
    
    bool Execute() const override;
};


#endif /* PrintToPdfCommand_hpp */
