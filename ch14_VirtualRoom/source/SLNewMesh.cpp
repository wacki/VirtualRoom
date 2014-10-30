//#############################################################################
//  File:      SLMesh.cpp
//  Author:    Marcus Hudritsch
//  Date:      February 2014
//  Copyright: 2002-2014 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>           // precompiled headers
#ifdef SL_MEMLEAKDETECT       // set in SL.h for debug config only
#include <debug_new.h>        // memory leak detector
#endif

#include "SLNewMesh.h"
#include "SLRay.h"
#include "SLRaytracer.h"
#include "SLSceneView.h"
#include "SLCamera.h"
#include "SLUniformGrid.h"
#include "SLLightSphere.h"
#include "SLLightRect.h"
#include "SLGLShaderProg.h"


//###########################################################
//
//						NEW MESH
//
//







SLNewMesh::SLNewMesh(SLstring name)
	: SLMesh(name),
	_numVerts(0), _numFaces(0), _bufPos(0), 
	_bufNormal(0), 	_bufTexCoord(0), _bufVertColor(0), 
	_bufTangents(0), _bufBoneData(0), _bufIndex(0),
	_hasNormals(false), _hasTexCoords(false),
	_hasVertColors(false), _hasTangents(false),
	_hasBones(false)
{
	_stateGL = SLGLState::getInstance();
}

SLNewMesh::~SLNewMesh()
{
	clear();
}
	
void SLNewMesh::init(SLSceneView* sv, SLNode* node)
{

}

void SLNewMesh::draw(SLSceneView* sv, SLNode* node)
{
	if(!_faces.size())
		return;

	// build buffers on first call
	// TODO: switch to using the SLGLBuffer class for this
	if(!_bufPos) {
		glGenBuffers(1, &_bufPos);
		glBindBuffer(GL_ARRAY_BUFFER, _bufPos);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLVec3f), &_positions[0], GL_STATIC_DRAW);
	}
	if(!_bufNormal && _hasNormals)	{
		glGenBuffers(1, &_bufNormal);
		glBindBuffer(GL_ARRAY_BUFFER, _bufNormal);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLVec3f), &_normals[0], GL_STATIC_DRAW);
	}
	if(!_bufTexCoord && _hasTexCoords) {
		glGenBuffers(1, &_bufTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, _bufTexCoord);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLVec2f), &_texCoords[0], GL_STATIC_DRAW);
	}
	if(!_bufVertColor && _hasVertColors) {
		glGenBuffers(1, &_bufVertColor);
		glBindBuffer(GL_ARRAY_BUFFER, _bufVertColor);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLVec4f), &_vertColors[0], GL_STATIC_DRAW);
	}
	if(!_bufTangents && _hasTangents) {
		glGenBuffers(1, &_bufTangents);
		glBindBuffer(GL_ARRAY_BUFFER, _bufTangents);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLVec3f), &_tangents[0], GL_STATIC_DRAW);
	}
	if(!_bufBoneData && _hasBones) {
		glGenBuffers(1, &_bufBoneData);
		glBindBuffer(GL_ARRAY_BUFFER, _bufBoneData);
		glBufferData(GL_ARRAY_BUFFER, _numVerts*sizeof(SLBoneData), &_boneData[0], GL_STATIC_DRAW);
	}
	if(!_bufIndex) {
		glGenBuffers(1, &_bufIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numFaces*sizeof(SLushort)*3, &_faces[0], GL_STATIC_DRAW);
	}
	
    if (_hasNormals)
        _stateGL->buildNormalMatrix();
	
	
	if(_material && (_material != SLMaterial::current || SLMaterial::current->shaderProg()==0)) {
		_material->activate(_stateGL, *node->drawBits());
	}

	// get current shader prog
	SLGLShaderProg* sp = SLMaterial::current->shaderProg();
	sp->uniformMatrix4fv("u_mvMatrix",    1, (SLfloat*)&_stateGL->modelViewMatrix);
	sp->uniformMatrix4fv("u_mvpMatrix", 1, (SLfloat*)_stateGL->mvpMatrix());
	sp->uniformMatrix3fv("u_nMatrix", 1, (SLfloat*)_stateGL->normalMatrix());
	sp->uniformMatrix4fv("u_invMvMatrix", 1, (SLfloat*)_stateGL->invModelViewMatrix());


	// get vertex attrbute locations for the shader program
	GLuint attrPos =			sp->getAttribLocation("a_position");
	GLuint attrNormal = 0;
	GLuint attrTexCoord = 0;
	GLuint attrVertColor = 0;
	GLuint attrTangent = 0;
	GLuint attrBoneIds = 0;
	GLuint attrBoneWeights = 0;

	if(_bufNormal && _hasNormals)
		attrNormal = sp->getAttribLocation("a_normal");

	if(_bufTexCoord && _hasTexCoords)
		attrTexCoord =		sp->getAttribLocation("a_texCoord");

	if(_bufVertColor && _hasVertColors)
		attrVertColor =		sp->getAttribLocation("a_vertColor");

	if(_bufTangents && _hasTangents)
		attrTangent =		sp->getAttribLocation("a_tangent");

	if(_bufBoneData && _hasBones) {
		attrBoneIds =		sp->getAttribLocation("a_boneIds");
		attrBoneWeights =	sp->getAttribLocation("a_boneWeights");	

		std::vector<SLMat4f> bonesTransforms;
		bonesTransforms.resize(_bones.size());
		// push the bone skinning data
		for(SLuint i = 0; i < _bones.size(); i++) {
			assert(_bones[i].node);
			bonesTransforms[i] = _bones[i].node->globalTransform() * _bones[i].node->offset();
		}

		sp->uniformMatrix4fv("u_bones", bonesTransforms.size(), (GLfloat*)&bonesTransforms[0], GL_FALSE);
	}

	//enable attribute arrays
	glBindBuffer(GL_ARRAY_BUFFER, _bufPos);
	glEnableVertexAttribArray(attrPos);
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	if(_hasNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, _bufNormal);
		glEnableVertexAttribArray(attrNormal);
		glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if(_hasTexCoords && _material->textures().size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _bufTexCoord);
		glEnableVertexAttribArray(attrTexCoord);
		glVertexAttribPointer(attrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if(_hasVertColors) {
		glBindBuffer(GL_ARRAY_BUFFER, _bufVertColor);
		glEnableVertexAttribArray(attrVertColor);
		glVertexAttribPointer(attrVertColor, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if(_hasTangents) {
		glBindBuffer(GL_ARRAY_BUFFER, _bufTangents);
		glEnableVertexAttribArray(attrTangent);
		glVertexAttribPointer(attrTangent, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if(_hasBones) {
		glBindBuffer(GL_ARRAY_BUFFER, _bufBoneData);
		glEnableVertexAttribArray(attrBoneIds);
		glVertexAttribIPointer(attrBoneIds, 4, GL_INT, sizeof(SLBoneData), (void*)0);
		glEnableVertexAttribArray(attrBoneWeights);
		glVertexAttribPointer(attrBoneWeights, 4, GL_FLOAT, GL_FALSE, sizeof(SLBoneData), (void*)16);
	}

	
	// draw call
	// TODO: fix importer bug that ignores line meshes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufIndex);
	glDrawElements(GL_TRIANGLES, _faces.size()*3, GL_UNSIGNED_SHORT, (void*)0);

	
	glDisableVertexAttribArray(attrPos);
	if(_hasNormals) 
		glDisableVertexAttribArray(attrNormal);
	if(_hasTexCoords && _material->textures().size() > 0) 
		glDisableVertexAttribArray(attrTexCoord);
	if(_hasVertColors) 
		glDisableVertexAttribArray(attrVertColor);
	if(_hasTangents) 
		glDisableVertexAttribArray(attrTangent);
	if(_hasBones) {
		glDisableVertexAttribArray(attrBoneIds);
		glDisableVertexAttribArray(attrBoneWeights);
	}
}


//-----------------------------------------------------------------------------
/*! 
SLMesh::buildAABB builds the passed axis-aligned bounding box.
*/
void SLNewMesh::buildAABB(SLAABBox &aabb, SLMat4f wmNode)
{   
    // Calculate min & max in object space only once
    if(minP==SLVec3f( FLT_MAX,  FLT_MAX,  FLT_MAX) &&
       maxP==SLVec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX))
    {  calcMinMax();
   
        // Enlarge aabb for avoiding rounding errors 0.5%
        SLVec3f distMinMax = maxP - minP;
        SLfloat addon = distMinMax.length() * 0.005f;
        minP -= addon;
        maxP += addon;
   
        // Recreate acceleration structure for triangle meshes
        delete _accelStruct;
        _accelStruct = 0;
        if (_primitive == SL_TRIANGLES)
            _accelStruct = new SLUniformGrid(this);

        // Build accelerations structure for more than 5 triangles
        if (_accelStruct && numI > 5*3) 
            _accelStruct->build(minP, maxP);
    }
   
    // Apply world matrix
    aabb.fromOStoWS(minP, maxP, wmNode);
}


void SLNewMesh::clear()
{
	// TODO: put buffers in an array and make this a little nicer
	if(_bufPos) glDeleteBuffers(1, &_bufPos);	
	if(_bufNormal) glDeleteBuffers(1, &_bufNormal);	
	if(_bufTexCoord) glDeleteBuffers(1, &_bufTexCoord);	
	if(_bufVertColor) glDeleteBuffers(1, &_bufVertColor);	
	if(_bufTangents) glDeleteBuffers(1, &_bufTangents);
	if(_bufBoneData) glDeleteBuffers(1, &_bufBoneData);
	if(_bufIndex) glDeleteBuffers(1, &_bufIndex);
	
	_bufPos = 0;
	_bufNormal = 0;	
	_bufTexCoord = 0;	
	_bufVertColor = 0;	
	_bufTangents = 0;
	_bufBoneData = 0;
	_bufIndex = 0;

	// clear local data
	_positions.clear();
	_normals.clear();
	_texCoords.clear();
	_vertColors.clear();
	_tangents.clear();
	_boneData.clear();
	_faces.clear();
}


void SLNewMesh::calcNormals()
{
    /*
   // Create array for the normals & Zero out the normals array
   _normals.clear();
   _normals.resize(_numVerts);
   for (SLuint i = 0; i < _numVerts; ++i) _normals[i] = SLVec3f::ZERO;
     
   // Loop over all triangles
   for (SLuint f = 0; f < _numFaces; ++f)
   {  
      // Calculate the face's normal
      SLVec3f e1, e2, n;
      
      // Calculate edges of triangle
      e1.sub(_positions[_faces[f].iB],_positions[_faces[f].iC]);   // e1 = B - C
      e2.sub(_positions[_faces[f].iB],_positions[_faces[f].iA]);   // e2 = B - A
      
      // Build normal with cross product but do NOT normalize it.
      n.cross(e1,e2);                  // n = e1 x e2

      // Add this normal to its vertices normals
      _normals[_faces[f].iA] += n;
      _normals[_faces[f].iB] += n;
      _normals[_faces[f].iC] += n;
   }
   
   
   // normalize vertex normals
   for (SLuint i=0; i < _numVerts; ++i) {
	   _normals[i].normalize();
   }
   */
}

SLbool SLNewMesh::hit(SLRay* ray)
{
	// TODO: ...
	return true;
}

void SLNewMesh::calcMinMax()
{  // init min & max points
   /*minV.set( SL_FLOAT_MAX,  SL_FLOAT_MAX,  SL_FLOAT_MAX);
   maxV.set(-SL_FLOAT_MAX, -SL_FLOAT_MAX, -SL_FLOAT_MAX);

   // calc min and max point of all vertices
   for (SLuint i=0; i<_positions.size(); ++i)
   {  if (_positions[i].x < minV.x) minV.x = _positions[i].x;
      if (_positions[i].x > maxV.x) maxV.x = _positions[i].x;
      if (_positions[i].y < minV.y) minV.y = _positions[i].y;
      if (_positions[i].y > maxV.y) maxV.y = _positions[i].y;
      if (_positions[i].z < minV.z) minV.z = _positions[i].z;
      if (_positions[i].z > maxV.z) maxV.z = _positions[i].z;
   } */
}

void SLNewMesh::calcCenterRad(SLVec3f& center, SLfloat& radius)
{  /*SLint    i;
   SLfloat  dx, dy, dz;
   SLfloat  radius2, xspan, yspan, zspan, maxspan;
   SLfloat  old_to_p, old_to_p_sq, old_to_new;
   SLVec3f  xmin, xmax, ymin, ymax, zmin, zmax, dia1, dia2;

   // FIRST PASS: find 6 minima/maxima points
   xmin.x = ymin.y = zmin.z=  SL_FLOAT_MAX;
   xmax.x = ymax.y = zmax.z= -SL_FLOAT_MAX;
   
   for (i=0; i<_positions.size(); ++i)
   {
      if (_positions[i].x < xmin.x) xmin = _positions[i]; else
      if (_positions[i].x > xmax.x) xmax = _positions[i];
      if (_positions[i].y < ymin.y) ymin = _positions[i]; else
      if (_positions[i].y > ymax.y) ymax = _positions[i];
      if (_positions[i].z < zmin.z) zmin = _positions[i]; else
      if (_positions[i].z > zmax.z) zmax = _positions[i];
   }
   
   // Set xspan = distance between the 2 points xmin & xmax (squared)
   dx = xmax.x - xmin.x;
   dy = xmax.y - xmin.y;
   dz = xmax.z - xmin.z;
   xspan = dx*dx + dy*dy + dz*dz;

   // Same for y & z spans
   dx = ymax.x - ymin.x;
   dy = ymax.y - ymin.y;
   dz = ymax.z - ymin.z;
   yspan = dx*dx + dy*dy + dz*dz;

   dx = zmax.x - zmin.x;
   dy = zmax.y - zmin.y;
   dz = zmax.z - zmin.z;
   zspan = dx*dx + dy*dy + dz*dz;

   // Set points dia1 & dia2 to the maximally separated pair
   dia1 = xmin; dia2 = xmax; // assume xspan biggest
   maxspan = xspan;
   if (yspan > maxspan)
   {  maxspan = yspan;
      dia1 = ymin; dia2 = ymax;
   }
   if (zspan > maxspan)
   {  dia1 = zmin; dia2 = zmax;
   }

   // dia1,dia2 is a diameter of initial sphere
   // calc initial center
   center.x = (dia1.x + dia2.x)*0.5f;
   center.y = (dia1.y + dia2.y)*0.5f;
   center.z = (dia1.z + dia2.z)*0.5f;
   
   // calculate initial radius*radius and radius
   dx = dia2.x - center.x; // x component of radius vector
   dy = dia2.y - center.y; // y component of radius vector
   dz = dia2.z - center.z; // z component of radius vector
   radius2 = dx*dx + dy*dy + dz*dz;
   radius  = sqrt(radius2);

   // SECOND PASS: increment current sphere
   for (i=0; i<_positions.size(); ++i)
   {
      dx = _positions[i].x - center.x;
      dy = _positions[i].y - center.y;
      dz = _positions[i].z - center.z;
      old_to_p_sq = dx*dx + dy*dy + dz*dz;
      
      if (old_to_p_sq > radius2) 	// do r**2 test first
      { 	
         // this point is outside of current sphere
         old_to_p = sqrt(old_to_p_sq);
         
         // calc radius of new sphere
         radius  = (radius + old_to_p) * 0.5f;
         radius2 = radius*radius; 	// for next r**2 compare
         old_to_new = old_to_p - radius;
         
         // calc center of new sphere
         center.x = (radius*center.x + old_to_new*_positions[i].x) / old_to_p;
         center.y = (radius*center.y + old_to_new*_positions[i].y) / old_to_p;
         center.z = (radius*center.z + old_to_new*_positions[i].z) / old_to_p;
         
         // Suppress if desired
         SL_LOG("\n New sphere: center,radius = %f %f %f   %f",
                center.x,center.y,center.z, radius);
      }
   }*/
}




void SLNewMesh::preShade(SLRay* ray)
{
	// TODO: implement
}
