#ifndef Cylinder_H
#define Cylinder_H

#define N_POINTS_AROUND_SLICE 40


#include "ModelViewWithPhongLighting.h"
	
class Cylinder : public ModelViewWithPhongLighting
{
public:
	Cylinder(float topRad, float botRad, const cryph::AffPoint& t, const cryph::AffPoint& b, float inColor[3], bool toCap);
	Cylinder(float topRad, float botRad, const cryph::AffPoint& t, const cryph::AffPoint& b, float inColor[3], bool toCap, bool isTree);
virtual ~Cylinder();
	
	cryph::AffPoint CylinderBottom;
	cryph::AffPoint CylinderTop;

	float bounds[6];
	float colors[3];
	float topRadius;
	float bottomRadius;
	float maxRadius;

	bool isATree;

	bool capped;

	cryph::AffVector topToBottom;

	static vec3 ka;
	static vec3 kd;
	static vec3 ks;
	static float m;

	//vec3 topCap[N_POINTS_AROUND_SLICE];
	//vec3 topCapNormals[N_POINTS_AROUND_SLICE];
	/*vec3 bottomCap[N_POINTS_AROUND_SLICE + 1];
	vec3 bottomCapNormals[N_POINTS_AROUND_SLICE]; */
	
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[3]; // 0: coordinates; 1: normal vectors 3: topCapCoords

	bool displayCylEdges, displayCylFill;
	
	void defineCylinder();
	void renderCylinder(float *color);
};
	
#endif
