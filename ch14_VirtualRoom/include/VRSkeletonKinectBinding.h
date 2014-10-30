//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VR_SKELETONKINECTBINDING_H
#define VR_SKELETONKINECTBINDING_H

#include "VRPerson.h"
#include "SLBoneNode.h"
#include "SLSkeleton.h"

namespace VirtualRoom
{
	
	//-----------------------------------------------------------------------------
	//! Kinect bone to SLSkeleton bone binding and conversion
	struct KinectBoneBinding
	{
		SLstring	boneName;						//<! Name of the bone in SLSkeleton
		SLBoneNode* bone;							//<! direct reference to the bone
		SLbool		hasKinectToSkeletonTransform;	//<! Does this bone have a different orientation than the corresponding bone in the kinect space
		SLQuat4f	kinectToSkeletonSpace;			//<! conversion from kinect space to bone space
		SLbool		hasRotationOffset;				//<! Some bones may need to be offset because they have a different orientation in the skeleton
		SLQuat4f	rotationOffset;					//<! Offset rotation to correct for different relative binding orientations in the SLSkeleton
	};

	typedef std::map<VR_SKELETON_POSITION_INDEX, KinectBoneBinding> BoneBindMap;

	//-----------------------------------------------------------------------------
	//! Kinect to SLSkeleton bone bindings and conversions
	/*!      
	This class allows the kinect bone orientations to be bound to a skeleton of a mesh.
	It provides functionality to convert and offset these orientations if needed.
	*/
	//-----------------------------------------------------------------------------
	class VRSkeletonKinectBinding
	{
	public:
		VRSkeletonKinectBinding();

		void			setSkeleton(SLSkeleton* skeleton);
		void			setPerson(VRPerson* person);
		bool			addBinding(VR_SKELETON_POSITION_INDEX kinectBone, const SLstring &boneName);
		bool			setKinectToSkeleton(VR_SKELETON_POSITION_INDEX kinectBone, SLQuat4f kinectToSkeleton);
		bool			setRotationOffset(VR_SKELETON_POSITION_INDEX kinectBone, SLQuat4f offset);
		void			updateSkeleton(SkeletonData* data);

		void			useOculus(bool value);
		void			setOculusBone(const SLstring &boneName);
		void			setOculusOffset(SLQuat4f offset);
		void			setOculusToSkeleton(SLQuat4f oculusToSkeleton);

		KinectBoneBinding* findBinding(VR_SKELETON_POSITION_INDEX kinectBone);

	private:
		SLSkeleton*		_skeleton;						//!< SLSkeleton reference
		VRPerson*		_person;						//!< reference to the kinect data
		BoneBindMap		_bindings;						//!< Kinect to bone bindings
		bool			_useOculus;						//!< using an oculus flag
		KinectBoneBinding _oculusBinding;				//!< Seperate binding for the oculus rotation

	};
	
	// inline functions
	inline void VRSkeletonKinectBinding::setSkeleton(SLSkeleton* skeleton) {
		_skeleton = skeleton;
	}
	
	inline void VRSkeletonKinectBinding::setPerson(VRPerson* person) {
		_person = person;
	}
	
	inline void VRSkeletonKinectBinding::useOculus(bool value) {
		_useOculus = value;
	}
	

	
}

#endif