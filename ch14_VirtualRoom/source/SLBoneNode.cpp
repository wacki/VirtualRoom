#include "SLBoneNode.h"


//-----------------------------------------------------------------------------
SLBoneNode::~SLBoneNode()
{
	for(auto child : _children) {
		delete child;
	}
}

//-----------------------------------------------------------------------------
/*! 
Add a child node
*/  
void SLBoneNode::addChild(SLBoneNode *bone)
{
	bone->_parent = this;
	_children.push_back(bone);
}

//-----------------------------------------------------------------------------
/*! 
remove a child
*/  
void SLBoneNode::removeChild(SLBoneNode *bone)
{
	bone->_parent = NULL;
	_children.erase(std::remove(_children.begin(), _children.end(), bone), _children.end());
}

//-----------------------------------------------------------------------------
/*! 
find a bone by name, returns NULL if not found
*/  
SLBoneNode* SLBoneNode::findBone(const SLstring &name)
{
	SLBoneNode *result = NULL;

	if(_name == name)
		return this;

	for(SLuint i = 0; i < _children.size(); i++)
	{
		result = _children[i]->findBone(name);
		if(result)
			return result;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
/*! 
update this node and all child node's globalTransform
*/  
void SLBoneNode::update(SLbool parentChanged)
{
	bool needsUpdate = parentChanged;

	if(needsUpdate && !useWStemp) {
		if(_parent) {
			_globalTransform = _parent->_globalTransform;
			_globalTransform *= _transform;
		}
		else
			_globalTransform = _transform;
	}

	// call update on children
	for(SLuint i = 0; i < _children.size(); i++)
		_children[i]->update(needsUpdate);
}


//-----------------------------------------------------------------------------
/*! 
rotate this node 
*/  
void SLBoneNode::rotate(SLfloat angle, SLfloat x, SLfloat y, SLfloat z)
{
	_transform.rotate(angle, x, y, z);

}

//-----------------------------------------------------------------------------
/*! 
set a rotation for this node
*/  
void SLBoneNode::rotation(SLQuat4f& rot)
{
	_globalTransform = _poseTransform;
	_transform = _poseTransform;

	SLMat4f rotation = rot.toMat4();

	_transform *= rotation;
}

//-----------------------------------------------------------------------------
/*! 
set the translation of this node
*/  
void SLBoneNode::translation(SLVec3f& pos)
{
	_transform.translation(pos);
}
