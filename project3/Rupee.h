// Rupee.h

#ifndef Rupee_H
#define Rupee_H

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];
typedef float vec4[4];

class Rupee : public ModelViewWithPhongLighting
{
public:
	Rupee(float P1[3], float P2[3], float P3[3], float P4[3], float c[]);

	virtual ~Rupee();

	float colors[3];

	cryph::AffPoint points[8];
	float height;
	float thickness;

	cryph::AffVector upVectorParallelToPlane;
	cryph::AffVector upVectorPerpToPlane;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();

	static vec4 ka;
	static vec4 kd;
	static vec4 ks;
	static float m;

private:
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[12]; //We can't draw ANY faces with glDrawArrays :()

	float xmin, xmax, ymin, ymax, zmin, zmax;

	cryph::AffVector normals[12];

	static GLuint indexList[12][3];

	bool displayRupeeEdges, displayRupeeFill;

	void defineRupee();
	void renderRupee(float* color);
};

#endif
