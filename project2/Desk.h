#ifndef Desk_H
#define Desk_H

#include "ModelViewWithPhongLighting.h"
#include "GLFWController.h"

class Desk : public ModelViewWithPhongLighting
{
public:
	Desk(GLFWController &c, cryph::AffPoint bottomLeft, float deskWidth, float deskLength, float deskHeight, float legHeight, float legWidth, float colorOfDeskTop[3], float colorOfLegs[3]);
	virtual ~Desk();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	float xmin, xmax, ymin, ymax, zmin, zmax;
};

#endif
