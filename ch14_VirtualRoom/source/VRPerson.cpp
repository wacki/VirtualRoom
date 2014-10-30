//#############################################################################
//  File:      VRPerson.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRPerson.h"
#include <SLSceneView.h>

//-----------------------------------------------------------------------------
VRPerson::VRPerson()
{
	this->initalize();	
}
//-----------------------------------------------------------------------------
VRPerson::~VRPerson()
{
	delete this->_current;
	delete this->_default;
}
//-----------------------------------------------------------------------------
/*!
initialize the skeleton manager, sets the default values
*/
void VRPerson::initalize()
{
	_tracked			= false;
	_skeletonModel		= VR_STICKMAN;
	_current			= new SkeletonData;
	_default			= new SkeletonData;
	_minHeightCurrent	= 0;
	_minHeightLatest	= 0;
	_minHeight[0]		= 0;
	
	// set default bone length
	_default->bones[VR_SKELETON_POSITION_SPINE_BASE].length		= 0.0f;
	_default->bones[VR_SKELETON_POSITION_SPINE_MID].length		= 0.0852846652f;
	_default->bones[VR_SKELETON_POSITION_NECK].length			= 0.0f;
	_default->bones[VR_SKELETON_POSITION_HEAD].length			= 0.177198395f;
	_default->bones[VR_SKELETON_POSITION_SHOULDER_LEFT].length	= 0.220885217f;
	_default->bones[VR_SKELETON_POSITION_ELBOW_LEFT].length		= 0.245449364f;
	_default->bones[VR_SKELETON_POSITION_WRIST_LEFT].length		= 0.221833274f;
	_default->bones[VR_SKELETON_POSITION_HAND_LEFT].length		= 0.100904375f;
	_default->bones[VR_SKELETON_POSITION_SHOULDER_RIGHT].length = 0.212600321f;
	_default->bones[VR_SKELETON_POSITION_ELBOW_RIGHT].length	= 0.281918973f;
	_default->bones[VR_SKELETON_POSITION_WRIST_RIGHT].length	= 0.222151190f;
	_default->bones[VR_SKELETON_POSITION_HAND_RIGHT].length		= 0.0792074576f;
	_default->bones[VR_SKELETON_POSITION_HIP_LEFT].length		= 0.113522060f;
	_default->bones[VR_SKELETON_POSITION_KNEE_LEFT].length		= 0.438629806f;
	_default->bones[VR_SKELETON_POSITION_ANKLE_LEFT].length		= 0.306646287f;
	_default->bones[VR_SKELETON_POSITION_FOOT_LEFT].length		= 0.0869859233f;
	_default->bones[VR_SKELETON_POSITION_HIP_RIGHT].length		= 0.112729155f;
	_default->bones[VR_SKELETON_POSITION_KNEE_RIGHT].length		= 0.421579510f;
	_default->bones[VR_SKELETON_POSITION_ANKLE_RIGHT].length	= 0.299472094f;
	_default->bones[VR_SKELETON_POSITION_FOOT_RIGHT].length		= 0.0869859084f;
	_default->bones[VR_SKELETON_POSITION_SPINE_SHOULDER].length = 0.365488291f;
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_LEFT].length	= 0.0f;
	_default->bones[VR_SKELETON_POSITION_THUMB_LEFT].length		= 0.0f;
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_RIGHT].length = 0.0f;
	_default->bones[VR_SKELETON_POSITION_THUM_RIGHT].length		= 0.0f;

	// set default bone rotation
	_default->bones[VR_SKELETON_POSITION_SPINE_BASE].rotation		= SLQuat4f(0.00119716825f, 0.995545626f, 0.0919951573f, -0.0206019171f);
	_default->bones[VR_SKELETON_POSITION_SPINE_MID].rotation		= SLQuat4f(-0.281525970f, 0.00518858153f, 0.00856242701f, 0.959501386f);
	_default->bones[VR_SKELETON_POSITION_NECK].rotation				= SLQuat4f();
	_default->bones[VR_SKELETON_POSITION_HEAD].rotation				= SLQuat4f(0.00558449049f, -0.000112827503f, 0.0201996006f, 0.999780357f);
	_default->bones[VR_SKELETON_POSITION_SHOULDER_LEFT].rotation	= SLQuat4f(0.00359512423f, -0.00665517617f, 0.879806757f, -0.475271463f);
	_default->bones[VR_SKELETON_POSITION_ELBOW_LEFT].rotation		= SLQuat4f(-0.131647199f, 0.318682581f, 0.162161231f, 0.924561679f);
	_default->bones[VR_SKELETON_POSITION_WRIST_LEFT].rotation		= SLQuat4f(-0.00720653264f, 0.000717031886f, 0.0990079492f, 0.995060265f);
	_default->bones[VR_SKELETON_POSITION_HAND_LEFT].rotation		= SLQuat4f(-0.0501062982f, 0.00107819820f, -0.0214856416f, 0.998512089f);
	_default->bones[VR_SKELETON_POSITION_SHOULDER_RIGHT].rotation	= SLQuat4f(-0.00359475822f, -0.00698859477f, 0.889228165f, 0.457396418f);
	_default->bones[VR_SKELETON_POSITION_ELBOW_RIGHT].rotation		= SLQuat4f(-0.0555501133f, 0.0277222469f, -0.228456646f, 0.971572578f);
	_default->bones[VR_SKELETON_POSITION_WRIST_RIGHT].rotation		= SLQuat4f(0.00190796878f, 0.000204227559f, -0.106430978f, 0.994318306f);
	_default->bones[VR_SKELETON_POSITION_HAND_RIGHT].rotation		= SLQuat4f(-0.0548249334f, -0.0174149126f, 0.302238494f, 0.951495051f);
	_default->bones[VR_SKELETON_POSITION_HIP_LEFT].rotation			= SLQuat4f(-5.03854558e-009f, -2.01541828e-009f, 0.924197793f, -0.381914258f);
	_default->bones[VR_SKELETON_POSITION_KNEE_LEFT].rotation		= SLQuat4f(-0.109537631f, 0.0489522144f, -0.352943718f, 0.927920282f);
	_default->bones[VR_SKELETON_POSITION_ANKLE_LEFT].rotation		= SLQuat4f(-0.0269402564f, -0.000696260482f, -0.0258266199f, 0.999303102f);
	_default->bones[VR_SKELETON_POSITION_FOOT_LEFT].rotation		= SLQuat4f(-0.0785508379f, -0.0185852945f, -0.229493931f, 0.969957232f);
	_default->bones[VR_SKELETON_POSITION_HIP_RIGHT].rotation		= SLQuat4f(1.00624298e-009f, 4.02497191e-009f, 0.925544441f, 0.378639162f);
	_default->bones[VR_SKELETON_POSITION_KNEE_RIGHT].rotation		= SLQuat4f(-0.101064041f, -0.0430051461f, 0.363443136f, 0.925119281f);
	_default->bones[VR_SKELETON_POSITION_ANKLE_RIGHT].rotation		= SLQuat4f(-0.00344927423f, 4.27195700e-005f, 0.0123845693f, 0.999917388f);
	_default->bones[VR_SKELETON_POSITION_FOOT_RIGHT].rotation		= SLQuat4f(0.132611156f, -0.0575776622f, 0.394080430f, 0.907634139f);
	_default->bones[VR_SKELETON_POSITION_SPINE_SHOULDER].rotation	= SLQuat4f(0.360050827f, 0.00190352427f, -0.00170757785f, 0.932929158f);
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_LEFT].rotation	= SLQuat4f();
	_default->bones[VR_SKELETON_POSITION_THUMB_LEFT].rotation		= SLQuat4f();
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_RIGHT].rotation	= SLQuat4f();
	_default->bones[VR_SKELETON_POSITION_THUM_RIGHT].rotation		= SLQuat4f();

	// set bone hierarchy
	_default->bones[VR_SKELETON_POSITION_SPINE_BASE].parent		= VR_SKELETON_POSITION_SPINE_BASE;
	_default->bones[VR_SKELETON_POSITION_SPINE_MID].parent		= VR_SKELETON_POSITION_SPINE_BASE;
	_default->bones[VR_SKELETON_POSITION_NECK].parent			= VR_SKELETON_POSITION_SPINE_SHOULDER;
	_default->bones[VR_SKELETON_POSITION_HEAD].parent			= VR_SKELETON_POSITION_NECK;
	_default->bones[VR_SKELETON_POSITION_SHOULDER_LEFT].parent	= VR_SKELETON_POSITION_SPINE_SHOULDER;
	_default->bones[VR_SKELETON_POSITION_ELBOW_LEFT].parent		= VR_SKELETON_POSITION_SHOULDER_LEFT;
	_default->bones[VR_SKELETON_POSITION_WRIST_LEFT].parent		= VR_SKELETON_POSITION_ELBOW_LEFT;
	_default->bones[VR_SKELETON_POSITION_HAND_LEFT].parent		= VR_SKELETON_POSITION_WRIST_LEFT;
	_default->bones[VR_SKELETON_POSITION_SHOULDER_RIGHT].parent = VR_SKELETON_POSITION_SPINE_SHOULDER;
	_default->bones[VR_SKELETON_POSITION_ELBOW_RIGHT].parent	= VR_SKELETON_POSITION_SHOULDER_RIGHT;
	_default->bones[VR_SKELETON_POSITION_WRIST_RIGHT].parent	= VR_SKELETON_POSITION_ELBOW_RIGHT;
	_default->bones[VR_SKELETON_POSITION_HAND_RIGHT].parent		= VR_SKELETON_POSITION_WRIST_RIGHT;
	_default->bones[VR_SKELETON_POSITION_HIP_LEFT].parent		= VR_SKELETON_POSITION_SPINE_BASE;
	_default->bones[VR_SKELETON_POSITION_KNEE_LEFT].parent		= VR_SKELETON_POSITION_HIP_LEFT;
	_default->bones[VR_SKELETON_POSITION_ANKLE_LEFT].parent		= VR_SKELETON_POSITION_KNEE_LEFT;
	_default->bones[VR_SKELETON_POSITION_FOOT_LEFT].parent		= VR_SKELETON_POSITION_ANKLE_LEFT;
	_default->bones[VR_SKELETON_POSITION_HIP_RIGHT].parent		= VR_SKELETON_POSITION_SPINE_BASE;
	_default->bones[VR_SKELETON_POSITION_KNEE_RIGHT].parent		= VR_SKELETON_POSITION_HIP_RIGHT;
	_default->bones[VR_SKELETON_POSITION_ANKLE_RIGHT].parent	= VR_SKELETON_POSITION_KNEE_RIGHT;
	_default->bones[VR_SKELETON_POSITION_FOOT_RIGHT].parent		= VR_SKELETON_POSITION_ANKLE_RIGHT;
	_default->bones[VR_SKELETON_POSITION_SPINE_SHOULDER].parent = VR_SKELETON_POSITION_SPINE_MID;
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_LEFT].parent	= VR_SKELETON_POSITION_HAND_LEFT;
	_default->bones[VR_SKELETON_POSITION_THUMB_LEFT].parent		= VR_SKELETON_POSITION_HAND_LEFT;
	_default->bones[VR_SKELETON_POSITION_HAND_TIP_RIGHT].parent = VR_SKELETON_POSITION_HAND_RIGHT;
	_default->bones[VR_SKELETON_POSITION_THUM_RIGHT].parent		= VR_SKELETON_POSITION_HAND_RIGHT;

	// set default skeleton data
	_default->headRotation.fromEulerAngles(0, SL_DEG2RAD * 180, 0);
	_default->position		= SLVec3f(0.0281617995f, -0.0122061409f, 0.0f);
	_default->position.y	+= 0.85f;

	// initialize _current and dataStorage
	for(SLint i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		_current->position.x = 0;
		_current->position.y = 0;
		_current->position.z = 0;

		_current->bones[i].parent	= _default->bones[i].parent;
		_current->bones[i].rotation	= SLQuat4f();
		_current->bones[i].absoluteRotation = SLQuat4f();
		_default->bones[i].absoluteRotation = SLQuat4f();
		
		_current->bones[i].length = _default->bones[i].length;

		// initialize data storage
		for(SLint frame = 0; frame < PAST_FRAME_COUNT; frame++){
			_skeletonData[i][frame][DATA_STORAGE_ROTATE_X].value			= 0;
			_skeletonData[i][frame][DATA_STORAGE_ROTATE_Y].value			= 0;
			_skeletonData[i][frame][DATA_STORAGE_ROTATE_Z].value			= 0;
			_skeletonData[i][frame][DATA_STORAGE_ROTATE_W].value			= 1;
			_skeletonData[i][frame][DATA_STORAGE_ABSOLUTE_ROTATE_X].value	= 0;
			_skeletonData[i][frame][DATA_STORAGE_ABSOLUTE_ROTATE_Y].value	= 0;
			_skeletonData[i][frame][DATA_STORAGE_ABSOLUTE_ROTATE_Z].value	= 0;
			_skeletonData[i][frame][DATA_STORAGE_ABSOLUTE_ROTATE_W].value	= 1;
			_skeletonData[i][frame][DATA_STORAGE_POS_X].value				= 0;
			_skeletonData[i][frame][DATA_STORAGE_POS_Y].value				= 0;
			_skeletonData[i][frame][DATA_STORAGE_POS_Z].value				= 0;
			_skeletonData[i][frame][DATA_STORAGE_LENGTH].value				= 0;

			for(SLint storageId = 0; storageId < DATA_STORAGE_SIZE; storageId++){
				_skeletonData[i][frame][storageId].frame = frame;
			}
		}

		// initialize bone length storage
		_boneLength[i].insertIndex	= -1;
		_boneLength[i].length		= _default->bones[i].length;
	}
}
//-----------------------------------------------------------------------------
/*!
add raw skeleton data to the manager
*/
void VRPerson::addFrame(SkeletonDataRaw* data)
{
	// create data struct
	_tracked = false;

	for(SLint i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		// update joint
		if(data->bones[i].state == VR_SKELETON_POSITION_TRACKED)
		{
			_tracked = true;
			if(i == VR_SKELETON_POSITION_SPINE_BASE)
			{
				// save data
				this->saveData(VR_SKELETON_POSITION_SPINE_BASE, DATA_STORAGE_POS_X, data->position.x);
				this->saveData(VR_SKELETON_POSITION_SPINE_BASE, DATA_STORAGE_POS_Y, data->position.y);
				this->saveData(VR_SKELETON_POSITION_SPINE_BASE, DATA_STORAGE_POS_Z, data->position.z);
			}
			
			// save data
			SLVec4f rotation = data->bones[i].rotation.toVec4();
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ROTATE_X, rotation.x);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ROTATE_Y, rotation.y);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ROTATE_Z, rotation.z);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ROTATE_W, rotation.w);

			SLVec4f absoluteRotation = data->bones[i].absoluteRotation.toVec4();
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ABSOLUTE_ROTATE_X, absoluteRotation.x);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ABSOLUTE_ROTATE_Y, absoluteRotation.y);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ABSOLUTE_ROTATE_Z, absoluteRotation.z);
			this->saveData((VR_SKELETON_POSITION_INDEX)i, DATA_STORAGE_ABSOLUTE_ROTATE_W, absoluteRotation.w);

			// add bone length
			if(data->bones[i].state == NUI_SKELETON_POSITION_TRACKED)
			{
				this->addBoneLength((VR_SKELETON_POSITION_INDEX)i, data->bones[i].length);
			}
		}
	}

	// add data to queue
	this->calculateCurrent();
}
//-----------------------------------------------------------------------------
/*!
returns the current calculated skeleton data for the view
*/
SkeletonData* VRPerson::getData()
{
	if(_tracked){
		return _current;
	}else{
		return _default;
	}
}
//-----------------------------------------------------------------------------
/*!
returns the default skeleton data
*/
SkeletonData* VRPerson::getDefaultData()
{
	return _default;
}
//-----------------------------------------------------------------------------
/*!
calculates the new current
*/
void VRPerson::calculateCurrent()
{
	// check oculus rotation
    SLbool oculus = SLScene::current->oculus()->isConnected();

    _current->headRotation = SLScene::current->oculus()->orientation(leftEye);

	// set position
	_current->position.x = _skeletonData[VR_SKELETON_POSITION_SPINE_BASE][VALUE_INDEX][DATA_STORAGE_POS_X].value;
	_current->position.y = _skeletonData[VR_SKELETON_POSITION_SPINE_BASE][VALUE_INDEX][DATA_STORAGE_POS_Y].value;
	_current->position.z = _skeletonData[VR_SKELETON_POSITION_SPINE_BASE][VALUE_INDEX][DATA_STORAGE_POS_Z].value;

	for(SLint i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		// set rotation
		_current->bones[i].rotation	= SLQuat4f(	_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ROTATE_X].value,
												_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ROTATE_Y].value,
												_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ROTATE_Z].value,
												_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ROTATE_W].value
												);
		_current->bones[i].absoluteRotation	= SLQuat4f(	_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ABSOLUTE_ROTATE_X].value,
														_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ABSOLUTE_ROTATE_Y].value,
														_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ABSOLUTE_ROTATE_Z].value,
														_skeletonData[i][VALUE_INDEX][DATA_STORAGE_ABSOLUTE_ROTATE_W].value
												);
		// set default length
		if(_skeletonModel == VR_STICKMAN)
			_current->bones[i].length = _boneLength[i].length;
		else
			_current->bones[i].length = _default->bones[i].length;
	}
	// invert rotations for head if there is oculus
	if(oculus)
		_current->headRotation *= _current->bones[VR_SKELETON_POSITION_SPINE_BASE].rotation * _current->bones[VR_SKELETON_POSITION_SPINE_MID].rotation
								* _current->bones[VR_SKELETON_POSITION_SPINE_SHOULDER].rotation * _current->bones[VR_SKELETON_POSITION_NECK].rotation
								* _current->bones[VR_SKELETON_POSITION_HEAD].rotation;

	// set head rotation
	if(oculus){
		_current->headRotation.invert();
	}else{
		_current->headRotation.fromEulerAngles(0, SL_DEG2RAD * 180, 0);
	}

	// add min height of the frame
	SLVec3f leftFoot = calculatePositionOfJoint(_current, VR_SKELETON_POSITION_FOOT_LEFT);
	SLVec3f rightFoot = calculatePositionOfJoint(_current, VR_SKELETON_POSITION_FOOT_RIGHT);
	this->addMinHeight(min(leftFoot.y, rightFoot.y));
	_current->position.y -= _minHeight[_minHeightCurrent];
}
//-----------------------------------------------------------------------------
/*!
save data package into the storage
*/
void VRPerson::saveData(VR_SKELETON_POSITION_INDEX joint, SKELETON_DATA_STORAGE storageId, SLfloat value)
{
	SLint	newDataPos;
	bool	newValueIsSmaler = false;

	// update data
	for(SLint i = 0; i < PAST_FRAME_COUNT; i++)
	{
		_skeletonData[joint][i][storageId].frame++;
		if(_skeletonData[joint][i][storageId].frame == PAST_FRAME_COUNT)
		{
			newDataPos = i;
			if(value < _skeletonData[joint][i][storageId].value){
				newValueIsSmaler = true;
			}
			_skeletonData[joint][i][storageId].frame = 0;
			_skeletonData[joint][i][storageId].value = value;
		};
	}

	// reorder list
	DataStorageValue tmp;
	if(newValueIsSmaler){
		for(SLint i = newDataPos; i > 0; i--){
			if(_skeletonData[joint][i-1][storageId].value > _skeletonData[joint][i][storageId].value){
				tmp = _skeletonData[joint][i-1][storageId];
				_skeletonData[joint][i-1][storageId] = _skeletonData[joint][i][storageId];
				_skeletonData[joint][i][storageId] = tmp;
			}else{
				break;
			}
		}
	}else{
		for(SLint i = newDataPos; i < PAST_FRAME_COUNT-1; i++){
			if(_skeletonData[joint][i+1][storageId].value < _skeletonData[joint][i][storageId].value){
				tmp = _skeletonData[joint][i+1][storageId];
				_skeletonData[joint][i+1][storageId] = _skeletonData[joint][i][storageId];
				_skeletonData[joint][i][storageId] = tmp;
			}else{
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*!
adds a minimum height to the storage
*/
void VRPerson::addMinHeight(SLfloat minHeight)
{
	_minHeightLatest = (_minHeightLatest + 1) % MIN_HEIGHT_SAMPLE_COUNT;
	if(minHeight <= _minHeight[_minHeightCurrent])
	{
		_minHeight[_minHeightLatest] = minHeight;
		_minHeightCurrent = _minHeightLatest;
	}else{
		_minHeight[_minHeightLatest] = minHeight;
		for(SLint i = 0; i < MIN_HEIGHT_SAMPLE_COUNT; i++){
			if(_minHeight[_minHeightCurrent] > _minHeight[i]){
				_minHeightCurrent = i;
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*!
adds a bone length to the storage
*/
void VRPerson::addBoneLength(VR_SKELETON_POSITION_INDEX bone, float length)
{
	_boneLength[bone].insertIndex++;

	if(_boneLength[bone].insertIndex < BONE_LENGTH_SAMPLE_COUNT)
	{
		// add bone length
		_boneLength[bone].values[_boneLength[bone].insertIndex] = length;

		if(_boneLength[bone].insertIndex == BONE_LENGTH_SAMPLE_COUNT-1){
			SLfloat length = 0;
			for(int i = 0; i < BONE_LENGTH_SAMPLE_COUNT; i++)
			{
				length += _boneLength[bone].values[i];
			}
			_boneLength[bone].length = length / BONE_LENGTH_SAMPLE_COUNT;

		}
	}
}
//-----------------------------------------------------------------------------
/*!
scales the skeleton up
*/
void VRPerson::scaleUp()
{
	for(int i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		_boneLength[i].length *= 1.1f;
	}
}
//-----------------------------------------------------------------------------
/*!
scales the skeleton down
*/
void VRPerson::scaleDown()
{
		for(int i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		_boneLength[i].length *= 0.9f;
	}
}
//-----------------------------------------------------------------------------