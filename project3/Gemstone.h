// Gemstone.h

#ifndef Gemstone_H
#define Gemstone_H

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];

class Gemstone : public ModelViewWithPhongLighting
{
public:
	Gemstone(float P1[3], float P2[3], float P3[3], float P4[3], float he, float c[]);

	virtual ~Gemstone();

	float colors[3];

	cryph::AffPoint points[6];
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
	GLuint ebo[7]; //We can draw 7 faces with ebos, but only 1 face with drawArrays :(

	float xmin, xmax, ymin, ymax, zmin, zmax;

	cryph::AffVector normals[8];

	static GLuint indexList[7][3];

	bool displayGemstoneEdges, displayGemstoneFill;

	void defineGemstone();
	void renderGemstone(float* color);
};

#endif
