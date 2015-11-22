// Wedge.h

#ifndef Wedge_H
#define Wedge_H

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];

class Wedge : public ModelViewWithPhongLighting
{
public:
	Wedge(float P1[3], float P2[3], float P3[3], float de, float c[]);

	virtual ~Wedge();

	float colors[3];

	cryph::AffPoint points[6];
	float depth;

	cryph::AffVector backVector;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();

private:
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[2]; //The two ebos for the two triangle faces
	GLuint eboBottom[1]; //The ebo for the bottom face

	float xmin, xmax, ymin, ymax, zmin, zmax;

	cryph::AffVector normals[5];

	static GLuint indexList[2][3];
	static GLuint indexforBottomFace[1][4];

	bool displayWedgeEdges, displayWedgeFill;

	void defineWedge();
	void renderWedge(float* color);
};

#endif
