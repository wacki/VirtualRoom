//#############################################################################
//  File:      SLMesh.h
//  Author:    Marcus Hudritsch
//  Date:      February 2014
//  Copyright: 2002-2014 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLNEWMESH_H
#define SLNEWMESH_H

#include <stdafx.h>
#include "SLAABBox.h"
#include "SLGLBuffer.h"
#include "SLBoneNode.h"
#include "SLMesh.h"

// temporary solution for storing bone information
// TODO: put the offset mat in the BoneNode class in the skeleton tree!
class SLBoneNode;
struct BoneStruct
{
	SLstring	name;
	SLMat4f		offset;
	SLBoneNode*	node;
};

// struct to hold the bone data for a single vertex
// max 4 bone weights per vertex
struct SLBoneData
{
	SLuint	ids[4];
	SLfloat	weights[4];

	SLBoneData()
	{
		// TODO: switch to c++11 pls so we can initialize the member arrays properly
		for(SLint i = 0; i < 4; i++) {
			weights[i] = 0.0f;
			ids[i] = 0;
		}
	}

	void addBone(SLuint id, float weight)
	{
		for(SLint i = 0; i < 4; i++) {
			if(weights[i] == 0.0f) {
				ids[i] = id;
				weights[i] = weight;
				return;
			}
		}
	}
};



class SLNewMesh : public SLMesh
{
	friend class SLImporter;


public:
	SLNewMesh(SLstring name = "NewMesh");
	~SLNewMesh();

	void         init(SLSceneView* sv, SLNode* node);
	void         draw(SLSceneView* sv, SLNode* node);
	SLbool       hit(SLRay* ray);
    void         buildAABB(SLAABBox &aabb, SLMat4f wmNode);

	void         preShade(SLRay* ray);
	void         addStats(SLNodeStats &parent);

	void         calcMinMax();
	void         calcCenterRad(SLVec3f& center, SLfloat& radius);

	void         material(SLMaterial* mat) { _material = mat; }
	SLMaterial*	 material() { return _material; }

	bool hasNormals()		{ return _hasNormals; }
	bool hasTexCoords()		{ return _hasTexCoords; }
	bool hasVertexColors()	{ return _hasVertColors; }
	bool hasTangents()		{ return _hasTangents; }
	bool hasBones()			{ return _hasBones; }

protected:
	// delete opengl buffers and clear all vertex data
	void		clear();
	void		calcNormals();


	// opengl vertex data buffers
	SLGLState*				_stateGL;//!< pointer to the global SLGLState instance
	GLuint					_bufPos;
	GLuint					_bufNormal;
	GLuint					_bufTexCoord;
	GLuint					_bufVertColor;
	GLuint					_bufTangents;
	GLuint					_bufBoneData;
	GLuint					_bufIndex;

	// buffer data
	SLuint					_numVerts;
	SLuint					_numFaces;

	std::vector<SLVec3f>	_positions;
	std::vector<SLVec3f>	_normals;
	std::vector<SLVec2f>	_texCoords;
	std::vector<SLVec4f>	_vertColors;
	std::vector<SLVec3f>	_tangents;
	std::vector<SLBoneData>	_boneData;
	std::vector<SLushort>	_faces;

	SLMaterial*				_material;

	// reference containing the bones that influence this mesh
	// note: the BoneData.id references the id in this list and is only valid for this mesh
	//std::vector<std::shared_ptr<Node>>	_bones; // make a small bone struct that contains a shared ptr to a node and an offset matrix for now! and make it work, then switch to this
	std::vector<BoneStruct>		_bones;

	bool					_hasNormals;
	bool					_hasTexCoords;
	bool					_hasVertColors;
	bool					_hasTangents;
	bool					_hasBones;
};

//-----------------------------------------------------------------------------
#endif //SLNEWMESH_H

