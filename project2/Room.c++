 
#include "Room.h"
#include "ShaderIF.h"

Room::Room(GLFWController &c, float wall1Attribs[6], float wall1Color[3], float wall2Attribs[6], float wall2Color[3], float ceilingAttribs[6], float ceilingColor[3], float floorAttribs[6], float floorColor[3])
{
	c.addModel(new Block(wall1Attribs[0], 	wall1Attribs[1], 	wall1Attribs[2], 	wall1Attribs[3], 	wall1Attribs[4], 	wall1Attribs[5], 	wall1Color));
	c.addModel(new Block(wall2Attribs[0], 	wall2Attribs[1], 	wall2Attribs[2], 	wall2Attribs[3], 	wall2Attribs[4], 	wall2Attribs[5], 	wall2Color));
	c.addModel(new Block(floorAttribs[0], 	floorAttribs[1], 	floorAttribs[2], 	floorAttribs[3], 	floorAttribs[4], 	floorAttribs[5], 	floorColor));
	c.addModel(new Block(ceilingAttribs[0], ceilingAttribs[1], 	ceilingAttribs[2], 	ceilingAttribs[3], 	ceilingAttribs[4], 	ceilingAttribs[5], 	ceilingColor));
}

Room::~Room()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Room::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyzLimits[0]; // xmin  Give real values!
	xyzLimits[1] = xyzLimits[1]; // xmax        |
	xyzLimits[2] = xyzLimits[2]; // ymin         |
	xyzLimits[3] = xyzLimits[3]; // ymax         |
	xyzLimits[4] = xyzLimits[4]; // zmin         |
	xyzLimits[5] = xyzLimits[5]; // zmax        \_/
}

void Room::render()
{
	//for(int i = 0; i < steps.size(); i++)
	//{
	//	walls[i].render();
	//}
}
