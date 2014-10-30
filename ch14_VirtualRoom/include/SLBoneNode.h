//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef SL_BONENODE_H
#define SL_BONENODE_H

#include <stdafx.h>

//-----------------------------------------------------------------------------
//! Defines a bone node structure
/*! 
This class holds a tree of SLBoneNodes. It is still a very basic class for now. It should hold all functionality that relates to rigged meshes.
*/  
class SLBoneNode : public SLObject
{
	friend class SLImporter;

public:
	SLBoneNode(SLstring name = "BoneNode")
		: SLObject(name), _parent(NULL), useWStemp(false)
	{}
	~SLBoneNode();
	
	void addChild(SLBoneNode *bone);
	void removeChild(SLBoneNode *bone);

	SLBoneNode* findBone(const SLstring &name);

	void update(SLbool parentChanged);

	const SLMat4f& globalTransform() { return _globalTransform; }
	const SLMat4f& poseTransform() { return _poseTransform; }
	const SLMat4f& offset() { return _offset; }

	void rotate(SLfloat angle, SLfloat x, SLfloat y, SLfloat z);
	void rotation(SLQuat4f& rot);
	void translation(SLVec3f& pos);

protected:
	SLMat4f		_poseTransform;
	SLMat4f		_offset;
	SLMat4f		_transform;
	SLMat4f		_globalTransform;

	bool useWStemp;
	SLBoneNode*					_parent;
	std::vector<SLBoneNode*>	_children;
};

#endif