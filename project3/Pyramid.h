 
// Pyramid.h

#ifndef Pyramid_H
#define Pyramid_H

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];

class Pyramid : public ModelViewWithPhongLighting
{
public:
	Pyramid(float P1[3], float P2[3], float P3[3], float P4[3], float he, float c[]);

	virtual ~Pyramid();

	float colors[3];

	cryph::AffPoint points[5];
	float height;

	cryph::AffVector upVector;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();

	static vec3 ka;
	static vec3 kd;
	static vec3 ks;
	static float m;

private:
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[3]; //The two ebos for the two triangle faces

	float xmin, xmax, ymin, ymax, zmin, zmax;

	vec3 normalVectorsVec3[5]; //The siz normals that I'll have to define myself with cross products
	cryph::AffVector normals[5];

	static GLuint indexList[3][3];

	bool displayPyramidEdges, displayPyramidFill;

	void definePyramid();
	void renderPyramid(float* color);
};

#endif
