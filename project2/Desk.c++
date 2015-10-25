// Desk.c++

#include "Desk.h"
#include "ShaderIF.h"
#include "Cylinder.h"
#include "Block.h"
#include "GLFWController.h"

Desk::Desk(GLFWController &c, cryph::AffPoint bottomLeft, float deskWidth, float deskLength, float deskHeight, float legHeight, float legWidth, float colorOfDeskTop[3], float colorOfLegs[3])
{
	//Alright, let's determine the locations that we will need for all of the desk legs!
	//Need to cryph::AffPoints for each leg, a TOP and BOTTOM point, then we'll just construct cylinders for them - the desktop will just be a block
	//50 is just an offset that kind of made it look good

	cryph::AffPoint leg1_top(		bottomLeft.x + legWidth / 2.0, 				bottomLeft.y, 				bottomLeft.z + legWidth / 2.0),					//Adding legWidth / 2.0 to move the center of the cylinder away from the corner
					leg1_bottom(	bottomLeft.x + legWidth / 2.0, 				bottomLeft.y + legHeight, 	bottomLeft.z + legWidth / 2.0),					//Otherwise half of the leg will be protruding from the desk - and that looks stupid
					leg2_top(		bottomLeft.x + deskWidth - legWidth / 2.0, 	bottomLeft.y, 				bottomLeft.z + legWidth / 2.0),					//So now the edge of the cylinder should coencide with the edge of the desk
					leg2_bottom(	bottomLeft.x + deskWidth - legWidth / 2.0, 	bottomLeft.y + legHeight, 	bottomLeft.z + legWidth / 2.0),
					leg3_top(		bottomLeft.x + legWidth / 2.0, 				bottomLeft.y, 				bottomLeft.z + deskLength - legWidth / 2.0),
					leg3_bottom(	bottomLeft.x + legWidth / 2.0, 				bottomLeft.y + legHeight, 	bottomLeft.z + deskLength - legWidth / 2.0),
					leg4_top(		bottomLeft.x + deskWidth - legWidth / 2.0,	bottomLeft.y, 				bottomLeft.z + deskLength - legWidth / 2.0),
					leg4_bottom(	bottomLeft.x + deskWidth - legWidth / 2.0, 	bottomLeft.y + legHeight, 	bottomLeft.z + deskLength - legWidth / 2.0);

c.addModel(new Block    	(bottomLeft.x, 	bottomLeft.y + legHeight, 	bottomLeft.z, 	deskWidth, deskHeight, deskLength, colorOfDeskTop));

c.addModel(new Cylinder 	(legWidth/2.0,  leg1_top, 				 	leg1_bottom, 		colorOfLegs));

c.addModel(new Cylinder 	(legWidth/2.0,  leg2_top, 					leg2_bottom, 		colorOfLegs));

c.addModel(new Cylinder 	(legWidth/2.0,  leg3_top, 					leg3_bottom,		colorOfLegs));

c.addModel(new Cylinder 	(legWidth/2.0,  leg4_top, 					leg4_bottom, 		colorOfLegs));
}

Desk::~Desk()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}

void Desk::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0; // xmin  Give real values!
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -3000.0; // zmin         |
	xyzLimits[5] = -2000.0; // zmax        \_/
}

void Desk::render()
{
}
