//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef SL_IMPORTER_H
#define SL_IMPORTER_H

#include <stdafx.h>
#include "SLMesh.h"
#include "SLNewMesh.h"
#include "SLBoneNode.h"
#include "SLGLTexture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <set>

//-----------------------------------------------------------------------------
//! Assimp Importer Wrapper Class
/*! 
This class serves as a wrapper class around Assimp. It gives the functionality to load scenes from a file.
*/  
class SLImporter
{
public:
		void import(const string &file);
		void loadNodeTree(SLBoneNode *curNode, aiNode *aiNode);
		void loadMesh(aiMesh *mesh);
		void findPoseMatrices(SLBoneNode* parentNode = NULL);
		void loadMaterial(SLint index, aiMaterial *material);
		void loadAnimation(aiAnimation *animation);
		SLGLTexture* SLImporter::loadTexture(SLstring &path);

		void loadIntoScene(SLScene* s, SLNode* dest = NULL);

		std::vector<SLNewMesh*>& meshes() { return _meshes; }

		SLBoneNode* root() { return _root; }

		SLBoneNode* findNode(string &name, SLBoneNode* curNode = NULL);

protected:
	void clear();

	SLstring							_file;			//!< name of the file to be loaded
	SLstring							_dir;			//!< directory path to the loaded file

	std::vector<SLNewMesh*>				_meshes;		//!< references to all loaded meshes
	SLBoneNode*							_root;			//!< root node of the scene
	std::set<SLstring>					_boneNames;		//!< set of all unique bone names in this scene
	std::map<SLstring, SLMat4f>			_bones;			//!< set of bone matrices
	std::map<SLint, SLMaterial*>		_materials;		//!< references to all loaded materials
	std::map<SLstring, SLGLTexture*>	_textures;		//!< references to all loaded textures
	std::map<void* const, SLNewMesh*>	_meshRefList;	//!< assimp mesh to SLNewMesh reference map
};

#endif