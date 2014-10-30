//#############################################################################
//  File:      VRPerson.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRPERSON_H
#define VRPERSON_H

#include "VRTypes.h"
#include <NuiApi.h>

using namespace VirtualRoom;

const SLint PAST_FRAME_COUNT		 = 1;
const SLint VALUE_INDEX				 = 0;
const SLint	MIN_HEIGHT_SAMPLE_COUNT	 = 50;
const SLint BONE_LENGTH_SAMPLE_COUNT = 100;

//-----------------------------------------------------------------------------
//! storage for the skeleton data
enum SKELETON_DATA_STORAGE
	{
		DATA_STORAGE_ROTATE_X			= 0,
		DATA_STORAGE_ROTATE_Y			= ( DATA_STORAGE_ROTATE_X + 1 ) ,
		DATA_STORAGE_ROTATE_Z			= ( DATA_STORAGE_ROTATE_Y + 1 ) ,
		DATA_STORAGE_ROTATE_W			= ( DATA_STORAGE_ROTATE_Z + 1 ) ,
		DATA_STORAGE_POS_X				= ( DATA_STORAGE_ROTATE_W + 1 ) ,
		DATA_STORAGE_POS_Y				= ( DATA_STORAGE_POS_X + 1 ) ,
		DATA_STORAGE_POS_Z				= ( DATA_STORAGE_POS_Y + 1 ) ,
		DATA_STORAGE_LENGTH				= ( DATA_STORAGE_POS_Z + 1 ) ,
		DATA_STORAGE_ABSOLUTE_ROTATE_X	= ( DATA_STORAGE_LENGTH + 1 ) ,
		DATA_STORAGE_ABSOLUTE_ROTATE_Y	= ( DATA_STORAGE_ABSOLUTE_ROTATE_X + 1 ) ,
		DATA_STORAGE_ABSOLUTE_ROTATE_Z	= ( DATA_STORAGE_ABSOLUTE_ROTATE_Y + 1 ) ,
		DATA_STORAGE_ABSOLUTE_ROTATE_W	= ( DATA_STORAGE_ABSOLUTE_ROTATE_Z + 1 ) ,
		DATA_STORAGE_SIZE				= ( DATA_STORAGE_ABSOLUTE_ROTATE_W + 1 ) ,
	};
//-----------------------------------------------------------------------------
//! storage value
struct DataStorageValue
	{
		SLfloat value;
		SLint	frame;
	};
//-----------------------------------------------------------------------------
//! storage value
struct BoneLengthStorage
{
	SLfloat length;
	SLint	insertIndex;
	SLfloat	values[BONE_LENGTH_SAMPLE_COUNT];
};
//-----------------------------------------------------------------------------
//! skeleton data manager
/*!      
gets the raw input data of the kinects and calculates the output skeleton for the view
*/
//-----------------------------------------------------------------------------
class VRPerson
{   public:
												VRPerson		();
												~VRPerson		();
							void				addFrame		(SkeletonDataRaw* data);
							SkeletonData*		getData			();
							SkeletonData*		getDefaultData	();
							void				scaleUp			();
							void				scaleDown		();
							void				setSkeletonModel(VR_MODEL model) {_skeletonModel = model;};
	private:
							SkeletonData*		_current;																		//!< current skeleton
							SkeletonData*		_default;																		//!< default skeleton
							SLbool				_tracked;																		//!< is the person tracked
							DataStorageValue	_skeletonData[VR_SKELETON_POSITION_COUNT][PAST_FRAME_COUNT][DATA_STORAGE_SIZE];	//!< skeleton data storage
							SLfloat				_minHeight[MIN_HEIGHT_SAMPLE_COUNT];											//!< minimum distance to the floor
							SLint				_minHeightLatest;																//!< last minimum
							SLint				_minHeightCurrent;																//!< current minimum
							BoneLengthStorage	_boneLength[VR_SKELETON_POSITION_COUNT];										//!< calculated bone length
							VR_MODEL			_skeletonModel;																	//!< skelton model

							void				initalize			();
							void				calculateCurrent	();
							void				saveData			(VR_SKELETON_POSITION_INDEX joint, SKELETON_DATA_STORAGE storageId, SLfloat value);
							void				addMinHeight		(SLfloat minHeight);
							void				addBoneLength		(VR_SKELETON_POSITION_INDEX bone, float length);

};
//-----------------------------------------------------------------------------
#endif