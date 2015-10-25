// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Block.h"
#include "Stairs.h"
#include "Cylinder.h"
#include "Desk.h"
#include "Room.h"

typedef float vec3[3];

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);


	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);

	cryph::AffPoint center(xmid, ymid, zmid);

	double maxDelta = xyz[1] - xyz[0];
	double delta 	= xyz[3] - xyz[2];

	if(delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if(delta > maxDelta)
		maxDelta = delta;

	double distEyeCenter = 2.0 * maxDelta;

	cryph::AffPoint eye(xmid + 1200, ymid + 350, zmid + 0.75*distEyeCenter);

	cryph::AffVector up = cryph::AffVector::yu;

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp, ecZmin, ecZmax;
	ecZpp 	= -(distEyeCenter - 0.5 * maxDelta);
	ecZmin 	= ecZpp - maxDelta * 2.0;
	ecZmax 	= ecZpp + 0.5 * maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

void addWall(GLFWController &c, cryph::AffPoint bottomLeft, float height, float width, float thickness, float color[3])
{
	c.addModel(new Block(bottomLeft.x, bottomLeft.y - height, bottomLeft.z, width, height, thickness, color));
} 


int main(int argc, char* argv[])
{
	GLFWController c("Room with stairs and a desk", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	// create your scene, adding things to the Controller....

	
	float bottomLeftX = 230;
	//Let's try to create a DESK


	cryph::AffPoint bottomLeftPoint(0,-500, 1130);

	/* Using coordinates devised on paper */
	int  	numOfStairs 	= 8;
	float 	deskHeight 		= 50;
	float 	deskWidth 		= 500;
	float 	deskLength 		= 500;
	float 	legHeight 		= 200;
	float 	legWidth 		= 100;
	float 	height 			= 100;
	float 	width 			= 100;
	float 	length 			= 1000;
	float 	wallColor[3] 	= {0.5, 0.5, 0.5};
	float 	groundColor[3] 	= {0, 0.15, 0.37};
	float 	stairsColor[3] 	= {0.2, 0.75, 0.5};
	float 	desktopcolor[3] = {0.2, 0.2, 0.7};
	float 	legsColor[3] 	= {0.5, 0.2, 0.2};

	cryph::AffPoint origin		(-300, 0, -300);

	cryph::AffPoint stairStart	(0, 400, 0);



	Stairs s(c, numOfStairs, stairStart, height, width, length, stairsColor);


	float wall1Attribs[6] 	= {0, 	-300, 0, 	1500, 	800, 	15};
	float wall2Attribs[6] 	= {0, 	-300, 0, 	1500, 	15, 	2000};
	float floorAttribs[6]	= {0, 	 500, 0, 	1500, 	15, 	2000};
	float ceilingAttribs[6] = {-15, -300, 0,	15,		800,	2000};

	cryph::AffPoint deskCoord(0, -300, 1200);

	Desk d(c, deskCoord, deskWidth, deskLength, deskHeight, legHeight, legWidth, desktopcolor, legsColor);
	Room r(c, wall1Attribs, wallColor, wall2Attribs, wallColor, ceilingAttribs, wallColor, floorAttribs, wallColor);

	c.addModel(new Block(0, 	-300, 1000, 800, 	500, 	15, 	wallColor)); //Wall right next to stair (small wall)

	glClearColor(1.0, 1.0, 1.0, 1.0);


	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
