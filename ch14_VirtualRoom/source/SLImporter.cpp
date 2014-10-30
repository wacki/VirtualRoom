//#############################################################################
//  File:      VRSkeletonKinectBinding.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "SLImporter.h"
#include "assimp/cimport.h"
#include "SLScene.h"


//-----------------------------------------------------------------------------
/*! 
Clears all the reference lists. 
Does not delete any data, the data has to be loaded into a scene before this is called
*/
void SLImporter::clear()
{
	// TODO: switch to shared_ptr for the loaded resources so we can clear the data safely
	_meshes.clear();
	_boneNames.clear();
	_bones.clear();
	_materials.clear();
	_textures.clear();
	_meshRefList.clear();
}


//-----------------------------------------------------------------------------
/*! 
Main import function, this loads the file and generates the data.
If this importer has just loaded data then loadIntoScene should be called before this or else there will be memory leaks
*/
void SLImporter::import(const string &file)
{
	_file = file;

	// clear previous data
	clear();
	_meshes.clear();
	Assimp::Importer aiImporter;
	
	const aiScene *assimpScene = aiImporter.ReadFile(file.c_str(),	
                                                   aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
													aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
													aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
													0);

    
	// Extract the directory part from the file name
    std::string::size_type slashIndex = _file.find_last_of("/");

    if (slashIndex == std::string::npos) {
        _dir = ".";
    }
    else if (slashIndex == 0) {
        _dir = "/";
    }
    else {
        _dir = _file.substr(0, slashIndex);
    }
	
	// load materials
	for(SLint i = 0; i < assimpScene->mNumMaterials; i++)
		loadMaterial(i, assimpScene->mMaterials[i]);

	// load the node tree
	loadNodeTree(NULL, assimpScene->mRootNode);

	// load each mesh from the scene
	for(SLint i = 0; i < assimpScene->mNumMeshes; i++)
		loadMesh(assimpScene->mMeshes[i]);
	
	// traverse the node tree again to find the pose matrices
	findPoseMatrices();
	
	// load animations
	for(SLint i = 0; i < assimpScene->mNumAnimations; i++)
		loadAnimation(assimpScene->mAnimations[i]);

}

//-----------------------------------------------------------------------------
/*! 
Loads node tree as bone nodes
*/
void SLImporter::loadNodeTree(SLBoneNode *curNode, aiNode *aiNode)
{
	// we're at the root
	if(!curNode) {
		curNode = new SLBoneNode(aiNode->mName.data);
		_root = curNode;
	}
	
	// load local transform
	memcpy(&curNode->_transform, &aiNode->mTransformation, sizeof(SLMat4f));
	curNode->_transform.transpose();

	for(SLint i = 0; i < aiNode->mNumChildren; i++) {
		SLBoneNode *child = new SLBoneNode(aiNode->mChildren[i]->mName.data);
		curNode->addChild(child);
		loadNodeTree(child, aiNode->mChildren[i]);
	}
}


//-----------------------------------------------------------------------------
/*! 
Load the mesh data into a SLNewMesh
*/
void SLImporter::loadMesh(aiMesh *mesh)
{
	// create a new mesh
	SLNewMesh *curMesh = new SLNewMesh;

	// keep a reference map to attach the mesh to the correct node later
	_meshRefList[mesh] = curMesh;

	curMesh->_hasNormals = mesh->HasNormals();
	curMesh->_hasTexCoords = mesh->HasTextureCoords(0); // we only support 1 set of coords atm
	curMesh->_hasVertColors = mesh->HasVertexColors(0); // we only support 1 set of colors atm
	curMesh->_hasTangents = mesh->HasTangentsAndBitangents();
	curMesh->_hasBones = mesh->HasBones();

	_meshes.push_back(curMesh);
	
	curMesh->_numVerts = mesh->mNumVertices;
	curMesh->_numFaces = mesh->mNumFaces;

	curMesh->_positions.resize(curMesh->_numVerts);


	bool test = mesh->HasTangentsAndBitangents();
	const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D *pos		= &(mesh->mVertices[i]);
		curMesh->_positions[i] = SLVec3f(pos->x, pos->y, pos->z);

		if(curMesh->_hasNormals) {
			const aiVector3D *normal	= &(mesh->mNormals[i]);
			curMesh->_normals.push_back(SLVec3f(normal->x, normal->y, normal->z));
		}
		if(curMesh->_hasTexCoords) {
			const aiVector3D *texcoord	= mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zeroVec;
			curMesh->_texCoords.push_back(SLVec2f(texcoord->x, texcoord->y));
		}
		if(curMesh->_hasTangents) {
			const aiVector3D *tangent  = &(mesh->mTangents[i]);
			curMesh->_tangents.push_back(SLVec3f(tangent->x, tangent->y, tangent->z));
		}
	}
	
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		// should not trigger since we triangulated the mesh
		assert(face.mNumIndices <= 3);
        if (face.mNumIndices == 3) //We only allow faces with 3 vertices, no point or line meshes for now
        {
            curMesh->_faces.push_back(face.mIndices[0]);
            curMesh->_faces.push_back(face.mIndices[1]);
            curMesh->_faces.push_back(face.mIndices[2]);
        }
	}
	
	//curMesh->calcNormals();

	// load materials
	SLint matIndex = mesh->mMaterialIndex;
	curMesh->material(_materials[matIndex]);

	
	curMesh->_bones.resize(mesh->mNumBones);
	curMesh->_boneData.resize(mesh->mNumVertices);

	const aiBone *bone;
	BoneStruct *curBone;
	for(SLint i = 0; i < mesh->mNumBones; i++) {
		bone = mesh->mBones[i];
		curBone = &curMesh->_bones[i];

		curBone->name = string(bone->mName.data);
		memcpy(&curBone->offset, &bone->mOffsetMatrix, sizeof(SLMat4f));
		curBone->offset.transpose();
		curBone->node = findNode(curBone->name);
		curBone->node->_offset = curBone->offset;
		curBone->node->_globalTransform = curBone->offset.inverse();

		// keep track of this bone's name to filter out its nodes later
		_boneNames.insert(curBone->name);


		for(SLint j = 0; j < bone->mNumWeights; j++) {
			SLint vertId = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;

			curMesh->_boneData[vertId].addBone(i, weight);
		}

	}
    	
}

//-----------------------------------------------------------------------------
/*! 
find and set the local transformations based on the bind pose of an imported mesh
*/
void SLImporter::findPoseMatrices(SLBoneNode* parentNode)
{
	// traverse the node tree and update the pose matrices
	if(!parentNode) parentNode = _root;
	/*
	// temporary hack for astroboy to get the "root" bone out of the root to use it for hip rotations
	bool nametest = parentNode->name() == "root";
	if(nametest)
		parentNode->_offset.translate(0, -7, 0);
	*/

	SLBoneNode* child;

	for(SLuint i = 0; i < parentNode->_children.size(); i++)
	{
		child = parentNode->_children[i];
		
		// update our pose and local transform
		child->_transform = parentNode->_offset * child->_offset.inverse();
		child->_poseTransform = child->_transform;

		findPoseMatrices(child);
	}
}

//-----------------------------------------------------------------------------
/*! 
Loads the materials, the textures will be loaded as following:
DIFFUSE 0
NORMALS 1
SPECULAR 2
EMISSIVE 3
SHININESS 4
*/
void SLImporter::loadMaterial(SLint index, aiMaterial *material)
{
	aiString		aipath;
	SLstring		path;
	const SLint		textureCount = 4;
	aiTextureType	textureTypes[textureCount];
	SLGLTexture*	textures[textureCount] = { 0 };
	SLMaterial*		currentMat = NULL;
	aiColor3D		ambient, diffuse, specular, emissive;


	// set the texture types to import into our material
	textureTypes[0] = aiTextureType_DIFFUSE;
	textureTypes[1] = aiTextureType_NORMALS;
	textureTypes[2] = aiTextureType_SPECULAR;
	textureTypes[3] = aiTextureType_HEIGHT;
	
	// load all the textures for this material first
	for(SLint i = 0; i < textureCount; ++i) {
		if(material->GetTextureCount(textureTypes[i]) > 0) {
			material->GetTexture(textureTypes[i], 0, &aipath, NULL, NULL, NULL, NULL, NULL);
			path = aipath.data;
		
			textures[i] = loadTexture(path);
		}
	}
	
	currentMat = new SLMaterial("imported material");

	// add the textures to the material
	for(SLint i = 0; i < 4; ++i) {
		if(textures[i]) 
            currentMat->textures().push_back(textures[i]);
	}
    
    SLfloat shininess, refracti, reflectivity, opacity;
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    material->Get(AI_MATKEY_SHININESS, shininess);
    material->Get(AI_MATKEY_REFRACTI, refracti);
    material->Get(AI_MATKEY_REFLECTIVITY, reflectivity);
    material->Get(AI_MATKEY_OPACITY, opacity);
    
    // hack, all the models come with 0 ambient, in the old project we hacked in the lighting shader
    ambient.r = 0.5f;
    ambient.g = 0.5f;
    ambient.b = 0.5f;

    // set color data
    currentMat->ambient(SLCol4f(ambient.r, ambient.g, ambient.b));
    currentMat->diffuse(SLCol4f(diffuse.r, diffuse.g, diffuse.b));
    currentMat->specular(SLCol4f(specular.r, specular.g, specular.b));
    currentMat->emission(SLCol4f(emissive.r, emissive.g, emissive.b));
    currentMat->shininess(shininess);

	_materials[index] = currentMat;
}

//-----------------------------------------------------------------------------
/*! 
load textures only once
*/
SLGLTexture* SLImporter::loadTexture(SLstring &path)
{
	SLGLTexture* texture;
	if(_textures.find(path) == _textures.end()) 
	{
		// load the texture since it doesn't exist already
		SLstring finalPath = _dir + "/" + path;
		_textures[path] = new SLGLTexture(finalPath);
	} 

	texture = _textures[path];

	return texture;
}

//-----------------------------------------------------------------------------
/*! 
Laod the imported data into an SLScene instance
*/
void SLImporter::loadIntoScene(SLScene* s, SLNode* dest)
{
	if(!dest)
		dest = s->root3D();

	// for now just put the meshes into the root
	for(auto mesh : _meshes) {
		dest->addMesh(mesh);
	}
}

//-----------------------------------------------------------------------------
/*! 
Not yet implemented, load animations
*/
void SLImporter::loadAnimation(aiAnimation *animation)
{
	// TODO: import the code after the animation part is implemented
}

//-----------------------------------------------------------------------------
/*! 
Search for a node by name, return NULL if not found
*/
SLBoneNode* SLImporter::findNode(string &name, SLBoneNode* curNode)
{
	SLBoneNode* result = NULL;

	if(!curNode && _root) 
		curNode = _root;
	
	if(!curNode)
		return NULL;

	if(curNode->_name == name)
		return curNode;

	for(SLuint i = 0; i < curNode->_children.size(); i++)
	{
		result = findNode(name,  curNode->_children[i]);
		if(result)
			return result;
	}

	return NULL;
}
