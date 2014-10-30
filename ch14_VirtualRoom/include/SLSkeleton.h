//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################


#ifndef SL_SKELETON_H
#define SL_SKELETON_H

#include "SLBoneNode.h"

//-----------------------------------------------------------------------------
//! Class to hold a single skeleton bone tree
/*! 
This class holds a tree of SLBoneNodes. It is still a very basic class for now. It should hold all functionality that relates to rigged meshes.
*/  
class SLSkeleton
{
public:
	SLSkeleton(SLBoneNode* root)
	{
		assert(root);
		_root = root;
	}
	~SLSkeleton()
	{
		if(_root)
			delete _root;
	}

	SLbool		update() { _root->update(true); return false; }
	SLBoneNode* root() { return _root; }

protected:
	SLBoneNode*		_root;			//!< root node

};

#endif