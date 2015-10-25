// Stairs.c++

#include "Stairs.h"
#include "ShaderIF.h"

Stairs::Stairs(GLFWController &c, int numSteps, cryph::AffPoint bottomLeftCorner, float height, float width, float length)
{
	//steps.reserve(numSteps);
	for(int i = 0; i < numSteps; i++)
	{
		c.addModel(new Block(bottomLeftCorner.x + i * width, bottomLeftCorner.y - i * height,	bottomLeftCorner.z, 	width, height, length));
	}
}

Stairs::~Stairs()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Stairs::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyzLimits[0]; // xmin  Give real values!
	xyzLimits[1] = xyzLimits[1];  // xmax        |
	xyzLimits[2] = xyzLimits[2]; // ymin         |
	xyzLimits[3] = xyzLimits[3]; // ymax         |
	xyzLimits[4] = xyzLimits[4]; // zmin         |
	xyzLimits[5] = xyzLimits[5]; // zmax        \_/
}

void Stairs::render()
{
	for(int i = 0; i < steps.size(); i++)
	{
		steps[i].render();
	}
}
