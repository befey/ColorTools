//
//  TokenCreator.hpp
//  SafeguardTools
//
//  Created by T431962 on 12/2/16.
//
//

#ifndef TokenCreator_hpp
#define TokenCreator_hpp

namespace SafeguardFile
{
    static constexpr auto NO_TOKEN_DESIG = "F";
    static constexpr auto DEFAULT_ARTBOARD_NAME = "ARTBOARD";

    class TokenCreator
    {
    public:
        TokenCreator(string plateNumber, int artboardIndex);
        TokenCreator(string plateNumber, string artboardName);
        
        string GetToken();
    private:
        string plateNumber;
        string artboardName;
        bool isDefaultName = false;
    };
}

#endif /* TokenCreator_hpp */
