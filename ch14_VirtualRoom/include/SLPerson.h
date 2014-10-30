//#############################################################################
//  File:      SLPerson.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef SLPERSON_H
#define SLPERSON_H

#include "SLNode.h"
#include "SLCamera.h"
#include "SLCylinder.h"
#include "SLBox.h"
#include "VRPerson.h"
#include "VRSkeletonKinectBinding.h"

//-----------------------------------------------------------------------------
//! defines player hands
enum SL_PERSON_TYPE
{
	SL_JOINTS_ONLY,
	SL_HIERARCHICAL_BONES
};

//-----------------------------------------------------------------------------
//! represents a skeleton of a person
/*!      
builds the skeleton for the view
*/
//-----------------------------------------------------------------------------
class SLPerson : public SLNode
{   public:
							SLPerson() : SLNode("Person"){};
							SLPerson(SLMaterial* mat, SL_PERSON_TYPE type, SkeletonData* data, SLstring name="Person", VR_MODEL model = VR_STICKMAN);
							~SLPerson();

							void			rebuildFigure(SkeletonData* data);
							void			makeCamActive();
							void			importModel();
														
							SLVec3f			getLeftHandPos() { return _lastLeftHandPos; }
							SLVec3f			getRightHandPos() { return _lastRightHandPos; }
	private:
							SLMaterial*		_mat;											//!< skeleton material
							SLNode*			_bones[VR_SKELETON_POSITION_COUNT];				//!< bones
							SLNode*			_skeletonGroups[VR_SKELETON_POSITION_COUNT];	//!< bone groups
							SL_PERSON_TYPE	_type;											//!< person display type
							SLCamera*		_cam;											//!< person POV
							SLSkeleton*					_skeleton;							//!< the sl skeleton bound to the mesh
							VRSkeletonKinectBinding		_skeletonKinectBinding;				//!< the kinect bindings for the sl skeleton
							VR_MODEL		_model;											//!< the model used by this person
							SLNode*			_modelGroup;									//!< SLGroup holding the model
							SLVec3f			_lastLeftHandPos;								//!< left hand position of the current model
							SLVec3f			_lastRightHandPos;								//!< right hand position of the current model

							SLNode*			leftHandSphere;
							SLNode*			rightHandSphere;

							void			calcHandPositions			(SkeletonData* data);
							void			BuildFigureGroup			(SkeletonData* data);
							void			rebuildFigureJointsAndBones	(SkeletonData* data);
							void			rebuildFigureJointsOnly		(SkeletonData* data);
							
};
//-----------------------------------------------------------------------------
#endif