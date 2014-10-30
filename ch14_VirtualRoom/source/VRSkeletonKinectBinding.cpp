//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRSkeletonKinectBinding.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
VRSkeletonKinectBinding::VRSkeletonKinectBinding()
	: _skeleton(NULL), _useOculus(false)
{
}

//-----------------------------------------------------------------------------
/*! 
Trys to create a new binding pair for the kinectBone and the corresponding boneName in SLSkeleton
If a bone binding already exists, it will do nothing.
returns true on sucess
false if boneName was not found
*/
bool VRSkeletonKinectBinding::addBinding(VR_SKELETON_POSITION_INDEX kinectBone, const SLstring &boneName) {
	SLBoneNode* bone = _skeleton->root()->findBone(boneName);

	// bone name not found
	if(!bone)
		return false;

	// binding already exists, do nothing
	if(findBinding(kinectBone))
		return true;

	// create the new binding
	KinectBoneBinding& binding = _bindings[kinectBone];
	binding.boneName = boneName;
	binding.bone = bone;
	binding.hasKinectToSkeletonTransform = false;
	binding.hasRotationOffset = false;

	return true;
}

//-----------------------------------------------------------------------------
/*! 
Set a rotation to convert from kinect space to bone space
*/
bool VRSkeletonKinectBinding::setKinectToSkeleton(VR_SKELETON_POSITION_INDEX kinectBone, SLQuat4f kinectToSkeleton)
{
	KinectBoneBinding* binding;
	if(!(binding = findBinding(kinectBone)))
		return false;
		
	binding->hasKinectToSkeletonTransform = true;
	binding->kinectToSkeletonSpace = kinectToSkeleton;
}

//-----------------------------------------------------------------------------
/*! 
Set an offset rotation
*/
bool VRSkeletonKinectBinding::setRotationOffset(VR_SKELETON_POSITION_INDEX kinectBone, SLQuat4f offset)
{
	KinectBoneBinding* binding;
	if(!(binding = findBinding(kinectBone)))
		return false;
		
	binding->hasRotationOffset = true;
	binding->rotationOffset = offset;
}	


//-----------------------------------------------------------------------------
/*! 
Sets the rotations based on the current kinect data and updates the skeleton
*/
void VRSkeletonKinectBinding::updateSkeleton(SkeletonData* data)
{
	//return if no skeleton is present
	if(!_skeleton) return;

	//set all bones that have a binding
	for(auto bindingPair : _bindings) {
		KinectBoneBinding& binding = bindingPair.second;
		SLQuat4f rotation = data->bones[bindingPair.first].rotation;
		
		if(binding.hasRotationOffset) {
			rotation = binding.rotationOffset * rotation;
		}

		if(binding.hasKinectToSkeletonTransform) {
			rotation = binding.kinectToSkeletonSpace.inverted() * rotation * binding.kinectToSkeletonSpace;
		}
		
		//the bone bound to VR_SKELETON_POSITION_SPINE_MID needs to be combined with the SPINE_SHOULDER bone or else we experience problems
		if(bindingPair.first == VR_SKELETON_POSITION_SPINE_MID)
			rotation *= data->bones[VR_SKELETON_POSITION_SPINE_SHOULDER].rotation;

		binding.bone->rotation(rotation);

	}

	// apply oculus rotation seperately from the kinect rotations
	if(_useOculus) {
		SLQuat4f rotation = data->headRotation;

		if(_oculusBinding.hasRotationOffset)
			rotation = _oculusBinding.rotationOffset * rotation;

		if(_oculusBinding.hasKinectToSkeletonTransform)
			rotation = _oculusBinding.kinectToSkeletonSpace.inverted() * rotation * _oculusBinding.kinectToSkeletonSpace;

		_oculusBinding.bone->rotation(rotation);
	}

	// temporary hack for position.. 
	// TODO: set a bone to recieve the position data, also fix the scale hack (0.12f)

	// TODO: allow the user to set a general space transformation (e.g.: if the bone data is in Z up etc...) the problem
	// comes from blender where you cant export "real" y up data taht is converted, but rather just a rotated root node, the bones are still in Z up space
	// and we need to apply our rotations directly in that space
	SLBoneNode* tempBone = _skeleton->root()->findBone("ValveBiped.Bip01_Pelvis");
	if(tempBone)
		tempBone->translation(1.0f/0.022f * (SLMat4f(180, 1, 0, 0) * data->position)/* - SLVec3f(0, 65, 0)*/);

	// call update on skeleton
	_skeleton->update();
}

//-----------------------------------------------------------------------------
/*! 
Set the head or neck bone that should react to oculus rotations
*/
void VRSkeletonKinectBinding::setOculusBone(const SLstring &boneName)
{
	_oculusBinding.boneName = boneName;
	_oculusBinding.bone = _skeleton->root()->findBone(boneName);
	_oculusBinding.hasRotationOffset = false;
	_oculusBinding.hasKinectToSkeletonTransform = false;
}

//-----------------------------------------------------------------------------
/*! 
Set the oculus rotation offset
*/
void VRSkeletonKinectBinding::setOculusOffset(SLQuat4f offset)
{
	_oculusBinding.hasRotationOffset = true;
	_oculusBinding.rotationOffset = offset;
}

//-----------------------------------------------------------------------------
/*! 
Set the oculus to skeleton transformation
*/
void VRSkeletonKinectBinding::setOculusToSkeleton(SLQuat4f oculusToSkeleton)
{
	_oculusBinding.hasKinectToSkeletonTransform = true;
	_oculusBinding.kinectToSkeletonSpace = oculusToSkeleton;
}


//-----------------------------------------------------------------------------
/*! 
Check if a binding exists

returns NULL if no binding exists
*/
KinectBoneBinding* VRSkeletonKinectBinding::findBinding(VR_SKELETON_POSITION_INDEX kinectBone)
{
	KinectBoneBinding* result = NULL;

	if(_bindings.find(kinectBone) != _bindings.end())
 		result = &_bindings[kinectBone];

	return result;
}