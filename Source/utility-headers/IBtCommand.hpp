//
//  IBtCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#ifndef IBtCommand_hpp
#define IBtCommand_hpp

class IBtCommand
{
public:
    virtual bool Execute() const = 0;
protected:
    IBtCommand() {};
    ~IBtCommand() {};
};

class VerifySuccessCommand : public IBtCommand
{
public:
    virtual bool Execute() const = 0;
    
    bool RequireSuccess() const { return requireSuccess; };
protected:
    VerifySuccessCommand(bool requireSuccess) : requireSuccess(requireSuccess) {};
    ~VerifySuccessCommand() {};
    
    bool requireSuccess = false;
};

#endif /* IBtCommand_hpp */
