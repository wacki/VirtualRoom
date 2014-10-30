//#############################################################################
//  File:      VRTypes.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTYPES_H
#define VRTYPES_H

#include "stdafx.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! define the different tracking state each joint can have
	enum VR_TRACKING_STATE
	{
		VR_SKELETON_POSITION_NOT_TRACKED	= 0,
		VR_SKELETON_POSITION_INFERRED	= ( VR_SKELETON_POSITION_NOT_TRACKED + 1 ) ,
		VR_SKELETON_POSITION_TRACKED	= ( VR_SKELETON_POSITION_INFERRED + 1 ) 
	};
	//-----------------------------------------------------------------------------
	//! define the different game types
	enum VR_GAME_TYPE
	{
		VR_GAME_TYPE_DEFAULT		=	0,
		VR_GAME_TYPE_COLOR_SPHERE	= ( VR_GAME_TYPE_DEFAULT + 1),
		VR_GAME_TYPE_TIC_TAC_TOE	= ( VR_GAME_TYPE_COLOR_SPHERE + 1 ),
		VR_GAME_TYPE_CONNECT_FOUR	= ( VR_GAME_TYPE_TIC_TAC_TOE + 1 ),
		VR_GAME_TYPE_TURN_TABLE		= ( VR_GAME_TYPE_CONNECT_FOUR + 1 ),
	};
	//-----------------------------------------------------------------------------
	//! define the different client types
	enum VR_CLIENT_TYPE
	{
		VR_CLIENT_TYPE_PLAYER	= 0,
		VR_CLIENT_TYPE_OBSERVER	= ( VR_CLIENT_TYPE_PLAYER + 1 ),
		VR_CLIENT_NOT_CONNECTED	= ( VR_CLIENT_TYPE_OBSERVER + 1 )
	};
	//-----------------------------------------------------------------------------
	//! define each skeleton joint
	enum VR_SKELETON_POSITION_INDEX
	{
		VR_SKELETON_POSITION_SPINE_BASE		= 0,
		VR_SKELETON_POSITION_SPINE_MID		= 1,
		VR_SKELETON_POSITION_NECK			= 2,
		VR_SKELETON_POSITION_HEAD			= 3,
		VR_SKELETON_POSITION_SHOULDER_LEFT	= 4,
		VR_SKELETON_POSITION_ELBOW_LEFT		= 5,
		VR_SKELETON_POSITION_WRIST_LEFT		= 6,
		VR_SKELETON_POSITION_HAND_LEFT		= 7,
		VR_SKELETON_POSITION_SHOULDER_RIGHT	= 8,
		VR_SKELETON_POSITION_ELBOW_RIGHT	= 9,
		VR_SKELETON_POSITION_WRIST_RIGHT	= 10,
		VR_SKELETON_POSITION_HAND_RIGHT		= 11,
		VR_SKELETON_POSITION_HIP_LEFT		= 12,
		VR_SKELETON_POSITION_KNEE_LEFT		= 13,
		VR_SKELETON_POSITION_ANKLE_LEFT		= 14,
		VR_SKELETON_POSITION_FOOT_LEFT		= 15,
		VR_SKELETON_POSITION_HIP_RIGHT		= 16,
		VR_SKELETON_POSITION_KNEE_RIGHT		= 17,
		VR_SKELETON_POSITION_ANKLE_RIGHT	= 18,
		VR_SKELETON_POSITION_FOOT_RIGHT		= 19,
		VR_SKELETON_POSITION_SPINE_SHOULDER	= 20,
		VR_SKELETON_POSITION_HAND_TIP_LEFT	= 21,
		VR_SKELETON_POSITION_THUMB_LEFT		= 22,
		VR_SKELETON_POSITION_HAND_TIP_RIGHT	= 23,
		VR_SKELETON_POSITION_THUM_RIGHT		= 24,
		VR_SKELETON_POSITION_COUNT			= 25 
	};

	//-----------------------------------------------------------------------------
	//! defines player hands
	enum VR_HANDS
	{
		VR_LEFT_HAND,
		VR_RIGHT_HAND,
		VR_HAND_COUNT
	};
	
	//-----------------------------------------------------------------------------
	//! predefined models to display
	enum VR_MODEL
	{
		VR_STICKMAN,
		VR_ASTROBOY,
		VR_HL2_CIV_MALE,
		VR_HL2_CIV_FEMALE
	};


	//-----------------------------------------------------------------------------
	//! represents a single bone in the skeleton of a person
	/*!
	each bone has a position indicating the end joint of the bone. additionally 
	a relative and an absolute rotation as well as the length of the bone
	is stored in a SkeletonBone object
	*/
	struct SkeletonBone
	{
		VR_SKELETON_POSITION_INDEX	parent;
		SLQuat4f					rotation;
		SLQuat4f					absoluteRotation;
		SLfloat						length;
	};
	//-----------------------------------------------------------------------------
	//! represents the whole skeleton data of a person
	/*!
	containing the position of a person, his head rotation and for each bone
	the effective skeleton bone data
	*/
	struct SkeletonData
	{
		SLVec3f				position;
		SLQuat4f			headRotation;
		SkeletonBone		bones[VR_SKELETON_POSITION_COUNT];
	};
	//-----------------------------------------------------------------------------
	//! Raw unprocessed skeleton bone data
	/*!
	Raw unprocessed skeleton bone data received directly from the kinect
	*/
	struct SkeletonBoneRaw
	{
		VR_SKELETON_POSITION_INDEX	parent;
		SLQuat4f					rotation;
		SLQuat4f					absoluteRotation;
		SLfloat						length;
		VR_TRACKING_STATE			state;
	};
	//-----------------------------------------------------------------------------
	//! Raw unprocessed skeleton data
	/*!
	Raw unprocessed skeleton data received directly from the kinect
	*/
	struct SkeletonDataRaw
	{
		SLVec3f					position;
		SLQuat4f				headRotation;
		SkeletonBoneRaw			bones[VR_SKELETON_POSITION_COUNT];
	};
	//-----------------------------------------------------------------------------
	/*!
	calculates the effective position of a joint based on the hierarchical 
	length and rotations of the skeletal bones
	*/
	inline SLVec3f calculatePositionOfJoint(SkeletonData* data, VR_SKELETON_POSITION_INDEX pos)
	{
		VR_SKELETON_POSITION_INDEX parent = data->bones[pos].parent;
		VR_SKELETON_POSITION_INDEX current = pos;
		SLMat4f res;
		SLMat4f tmp;

		do{
			tmp.identity();
			tmp.translation(0, data->bones[parent].length, 0);
			tmp *= data->bones[current].rotation.toMat4();
			res = tmp * res;
			current = parent;
			parent = data->bones[parent].parent;		
		}while(current != parent);

		tmp.identity();
		tmp.translation(data->position.x, data->position.y, data->position.z);
		tmp *= data->bones[current].rotation.toMat4();
		res = tmp * res;

		return res.translation();
	}
}
//-----------------------------------------------------------------------------
#endif