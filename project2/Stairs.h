#ifndef Stairs_H
#define Stairs_H

#include <vector>
#include "Block.h"

#include "ModelViewWithPhongLighting.h"
#include "GLFWController.h"

class Stairs : public ModelViewWithPhongLighting
{
public:
	Stairs(GLFWController &c, int numSteps, cryph::AffPoint bottomLeftCornerOfBottomStep, float heightOfStep, float widthOfStep, float lengthOfStep, float color[3]);
	virtual ~Stairs();

	std::vector<Block> steps;

	float getStairWidth();
	float getStairHeight();
	float getStairLength();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	float stairHeight;
	float stairWidth;
	float stairLength;
};

#endif
