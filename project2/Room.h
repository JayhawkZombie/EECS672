#ifndef Room_H
#define Room_H

#include <vector>
#include "Block.h"

#include "ModelViewWithPhongLighting.h"
#include "GLFWController.h"

class Room : public ModelViewWithPhongLighting
{
public:
	Room(GLFWController &c, float wall1Attribs[6], float wall1Color[3], float wall2Attribs[6], float wall2Color[3], float ceilingAttribs[6], float ceilingColor[3], float floorAttribs[6], float floorColor[3]);
	virtual ~Room();

	std::vector<Block> walls;

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
 
