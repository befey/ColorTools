/*
 *  Node.h
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 1/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef NODE_H
#define NODE_H

#include "AIArtboard.h"

#include "string.h"

#include "Plate.h"
class Plate;

extern "C" AIArtboardSuite* sAIArtboard;
extern "C" AIPathSuite* sAIPath;


#define BSVERSION 
//#define CUSTVERSION

//CONSTANT DEFINITIONS

#ifdef BSVERSION
	constexpr auto KEYLINE_LAYER =              "KEY LINE";
	constexpr auto REG_LAYER =                  "Registration Guide for Proof";
	constexpr auto SLUG_LAYER =                 "SLUG";
	constexpr auto FOREGROUND_LAYER =           "Foreground";
	constexpr auto GUIDE_LAYER =                "Guides";
	constexpr auto BACKGROUND_LAYER =           "Background";

    constexpr auto LAYER_WITH_PLATE_INFO =          SLUG_LAYER;
    constexpr auto LAYER_WITH_PROOF_INFO =          FOREGROUND_LAYER;
    constexpr auto LAYER_WITH_PROOF_INFO_ALT =      BACKGROUND_LAYER;
    constexpr auto LAYER_WITH_EXTRA_COLOR_GROUP =   SLUG_LAYER;
    constexpr auto LAYER_WITH_KEYLINE_INFO = 		KEYLINE_LAYER;
    constexpr auto LAYER_WITH_REGISTRATION =		REG_LAYER;
    constexpr auto LAYER_WITH_BLEED = 				BACKGROUND_LAYER;
    constexpr auto LAYER_WITH_GRIPPER =				BACKGROUND_LAYER;
#endif

constexpr auto PLATE_NUMBER_LABEL =             "__plate_number__";
constexpr auto DATE_LABEL =                     "__date__";
constexpr auto COLOR_LIST_LABEL =               "__color_list__";
constexpr auto EXTRA_COLOR_GROUP_LABEL =        "__extra_colors__";
constexpr auto PROOF_TAG_LABEL =                "__proof_tag__";
constexpr auto KEYLINE_FACE_LABEL =             "__keyline_face__";
constexpr auto KEYLINE_ENV_FLAP_LABEL =         "__keyline_envelope_flap__";
constexpr auto KEYLINE_REGISTRATION_LABEL =     "__keyline_registration__";
constexpr auto KEYLINE_SAFE_EMBOSS_LABEL =      "__keyline_safe_emboss__";
constexpr auto REGISTRATION_BOX_LABEL =         "__registration_box__";
constexpr auto REGISTRATION_ENV_FLAP_LABEL =    "__registration_envelope_flap__";
constexpr auto REGISTRATION_SAFE_EMBOSS_LABEL = "__registration_safe_emboss__";
constexpr auto BLEED_BOX_LABEL =                "__bleed_guide__";
constexpr auto GRIPPER_TOP_LABEL =              "__gripper_top__";
constexpr auto GRIPPER_BOTTOM_LABEL =           "__gripper_bottom__";
constexpr auto GRIPPER_LEFT_LABEL =             "__gripper_left__";
constexpr auto GRIPPER_RIGHT_LABEL =            "__gripper_right__";


constexpr auto PLATE_NUMBER_TEXT =              "PLATE NUMBER";
constexpr auto PLATE_NUMBER_MIN_LENGTH =        6;
constexpr auto PLATE_NUMBER_MAX_LENGTH =        20;
constexpr auto DATE_TEXT =                      "00/00";
constexpr auto DATE_MIN_LENGTH =                4;
constexpr auto DATE_MAX_LENGTH =                7;

enum Corner
{
    NONE =          0,
    UPPER_LEFT =	1,
    UPPER_RIGHT =	2,
    LOWER_RIGHT =	3,
    LOWER_LEFT =	4
};



void SetUpNodes();
void CleanUpNodes();


class Node {
public:
	Plate* p;
	Node* nextNode;
	Node* prevNode;
	
	//Find Object Callback prototype
	typedef AIArtHandle (Node::*FindObjectFunc) ( Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere );
	
//Default constructor
	Node(Node* const prevNode = NULL, const int CAIndex = -1);
	//prevNode - Passed in should be the pointer to the last node that was made, so we can set this node's previous node to it
	//CAIndex - The crop area's index position in the list of crop areas -- passed on to the Plate constructor via the initializer list
//Default destructor
	~Node();

//Member functions
	//Fills in some node specific plate variables
	void SetupAdditionalVars();
		
	//Locates the extra color group
	//If its already been named, it returns that object, otherwise it tries to find it in the document
	AIArtHandle FindExtraColorListGroup(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);

	//Locates the color list text frame
	//Extracts the color list and creates a new point text if no named object is found
	AIArtHandle FindColorList(Node* const startNode, AIArtHandle currArtHandle, Corner const lookWhere);
	
	//Locates the plate number text frame
	//Extracts the plate number and creates a new point text if no named object is found
	AIArtHandle FindPlateNumber(Node* const startNode, AIArtHandle currArtHandle, Corner const lookWhere);
	
	//Locates the date text frame
	//Extracts the date and creates a new point text if no named object is found
	AIArtHandle FindDate(Node* const startNode, AIArtHandle currArtHandle, Corner const lookWhere);
	
	//Locates the proof tag
	//Creates one if none exists
	AIArtHandle FindProofTag(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the keyline registration box
	AIArtHandle FindKeylineRegBox(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the keyline Envelope Flap
	AIArtHandle FindKeylineEnvFlap(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);

	//Locates the keyline safe emboss box
	AIArtHandle FindKeylineSafeEmbossBox(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the keyline face indicator
	AIArtHandle FindKeylineFace(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the registration box
	AIArtHandle FindRegBox(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the Registration Envelope Flap
	AIArtHandle FindRegEnvFlap(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the registration box
	AIArtHandle FindRegSafeEmbossBox(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the bleed guide path
	AIArtHandle FindBleedBox(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	
	//Locates the gripper areas
	AIArtHandle FindGripperTop(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	AIArtHandle FindGripperBottom(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	AIArtHandle FindGripperLeft(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
	AIArtHandle FindGripperRight(Node* const startNode, AIArtHandle const currArtHandle, Corner const lookWhere);
};

struct FIND_FUNCTIONS {
	Node::FindObjectFunc callback;
	ai::UnicodeString layerLabel;
	ai::UnicodeString objectLabel;
	Corner lookWhere;
};

//Find everything
void FindAllObjects(Node* const startNode, Node* const currNode);
//Main Find Function
AIArtHandle Find(Node* const startNode, Node* const inNode, FIND_FUNCTIONS const &find);


Node* FindClosestNode(AIArtHandle object, Corner CORNER);
// object -- the object we're using
// CORNER -- the corner of the node to check distance to
// Returns a pointer to the closest Node, or NULL if there was an error

Corner FindClosestCorner(AIArtHandle object, Node* const node );
// object -- the object we're checking
// node -- the Node we're checking
// Returns the Corner Constant defined above or 0 if some were equal distance

std::string CreatePlateNumberStringFromCharArray(const char* inChars);
// inChars -- the array of chars to search for the plate number
// Returns a standard string that contains the plate number

std::string CreateDateStringFromCharArray(const char* inChars);
// inChars -- the array of chars to search for the date
// Returns a standard string that contains the date

#endif