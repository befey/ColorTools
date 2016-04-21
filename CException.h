/*
 *  CException.h
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>

@interface CException : NSObject {
    enum ErrorCode {
        ecOK =                              0, //No Error
        
        //****Master Updater Errors
        ecUpdaterNotRun =                   1, //Master Updater not run
        ecPlateNumberNotFound =             2, //The Plate number text or the registration box could not be found
        
        ecNoDocOpen =                       11, //There is no current document
        ecBadOriginPoint =                  12, //The document origin is not in the correct location relative to the artboard
        ecFaceMismatch =                    13, //Front and backer could not be determined
        
        ecCropToolSelected =                15, //The crop tool was selected
        
        ecNoValidArtInLayer =               25, //The layer does not have any valid art in it
        
        ecDictArtObjectNotValid =           50, //The art object referred to in the plate dictionary was invalid
        ecPlateStructArtObjectNotValid =    51, //The art object stored in the Plate node was invalid
        
        //Incorrectly loaded errors
        ecPlateNotFound =                   101, //Could not locate plate
        ecPlateNotCopied =                  102, //Could not copy folder from archive to wip
        ecWIPFolderChanged =                103, //The WIP folder could not be found
        ecArchiveFolderChanged =            104, //The Archive folder could not be found
        ecManufacturingFolderChanged =      105, //The Manufacturing folder could not be found
        
        ecMissingLink =                     150, //There are missing linked files
        ecNoFontsFolder =                   151, //The Temp Fonts folder could not be found
        ecMissingFont =                     152, //There are missing fonts
        ecFontNotCopied =                   153, //A font file could not be copied to Temp Fonts
        ecNoIllustratorFile =               154, //Could not find an Illustrator file for the plate
        ecNoJobOptions =                    155, //There was a problem finding the pdf joboptions file
        
        //Incorrect PDF print settings
        ecBadOutputPath =                   201, //No output path selected or output path doesn't exist
        ecBadPlant =                        202, //No or invalid plant selected
        ecBadPlate =                        203, //No or invalid plate selected
        ecPDFError =                        204, //Some error outputting PDF file
        
        //Key objects not found
        ecKeyObjectNotFound =               500, //One of several required objects was not found
        ecKeyObjectNotFoundRegBox =         501, //Registration Box wasn't found
        ecKeyObjectNotFoundPlateNum =       502, //Plate number wasn't found
        ecKeyObjectNotFoundDate =           503, //Date wasn't found
        ecKeyObjectNotFoundColorList =      504  //Color List wasn't found
    };
    
	ErrorCode code;
}

//**************************************************************
//*********** Getters and Setters ******************************
//**************************************************************
- (ErrorCode) code;
- (void) setCode: (ErrorCode) ec;

- (id)init;
+ (CException*)withCode:(ErrorCode) ec;
- (id)initWithCode: (ErrorCode) ec;
- (void) dealloc;

- (NSString*) getDescription;

@end