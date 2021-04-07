//
//  ArtObjectCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 2/12/18.
//
//

#ifndef ArtObjectCommand_hpp
#define ArtObjectCommand_hpp

#include "IBtCommand.hpp"
#include <vector>
#include "BtArtHandle.hpp"

class ArtObjectCommand : public IBtCommand
{

protected:
    ArtObjectCommand() {};
    ~ArtObjectCommand() {};
};


class RemoveArtObjectsCommand : public ArtObjectCommand
{
public:
    RemoveArtObjectsCommand(std::vector<BtArtHandle> handles) : handles(handles) {};
    
    bool Execute() const override;
    
private:
    std::vector<BtArtHandle> handles;
};


#endif /* ArtObjectCommand_hpp */
