#ifndef Cylinder_H
#define Cylinder_H


#include "ModelViewWithPhongLighting.h"
	
class Cylinder : public ModelViewWithPhongLighting
{
public:
	Cylinder(float inRadius, const cryph::AffPoint& bottom_midpoint, const cryph::AffPoint& top_midpoint, float inColor[3]);
virtual ~Cylinder();
	
	cryph::AffPoint CylinderBottom;
	cryph::AffPoint CylinderTop;

	float bounds[6];
	float color[3];
	float radius;
	
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors
	
	void defineCylinder();
};
	
#endif
