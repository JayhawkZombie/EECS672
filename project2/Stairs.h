 
// Stairs.h

#ifndef Stairs_H
#define Stairs_H

#include <vector>
#include "Block.h"

#include "ModelViewWithPhongLighting.h"
#include "GLFWController.h"

class Stairs : public ModelViewWithPhongLighting
{
public:
	Stairs(GLFWController &c, int numSteps, cryph::AffPoint bottomLeftCornerOfBottomStep, float heightOfStep, float widthOfStep, float lengthOfStep);
	virtual ~Stairs();

	std::vector<Block> steps;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
};

#endif
