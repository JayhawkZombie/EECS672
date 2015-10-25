// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Block.h"
#include "Stairs.h"
#include "Cylinder.h"
#include "Desk.h"

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
	double delta = xyz[3] - xyz[2];

	if(delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if(delta > maxDelta)
		maxDelta = delta;

	double distEyeCenter = 2.0 * maxDelta;

	cryph::AffPoint eye(xmid + 1200, ymid + 250, zmid + 0.75*distEyeCenter);

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

void addGround(GLFWController &c, float height, bool onTopOfStairs, float color[3])
{
	if (!onTopOfStairs)
		c.addModel(new Block(0, height, 		150, 1000, 	30, 1000, color));
	else
		c.addModel(new Block(0, height - 15, 	200, 400, 	15, 1000, color));
} 

int main(int argc, char* argv[])
{
	GLFWController c("FUCK THIS", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	// create your scene, adding things to the Controller....

	
	float bottomLeftX = 230;
	float height = 100;
	float width = 100;
	float length = 1000;

	float groundColor[3] = {0, 0.15, 0.37};

	float stairsColor[3] = {0.2, 0.75, 0.5};

	int  numOfStairs = 5;

	//Let's try to create a DESK
	float desktopcolor[3] 	= {0.2, 0.2, 0.7};
	float legsColor[3] 		= {0.5, 0.2, 0.2};

	cryph::AffPoint bottomLeftPoint(0,-500,1130);

	float deskHeight 	= 50;
	float deskWidth 	= 500;
	float deskLength 	= 500;
	float legHeight 	= 200;
	float legWidth 		= 100;

	cryph::AffPoint deskPoint(0, 400, 2230);

	/* Using coordinates devised on paper */

	cryph::AffPoint origin(-300, 0, -300);

	cryph::AffPoint stairStart(3RT00, 100, 0);
	//float stairsColor[3] = {0.21, 0.23, 0.87};
	float wallColor[3] = {0.5, 0.5, 0.5};

	Stairs s(c, numOfStairs, stairStart, height, width, length, stairsColor);

//GOES X, Y, Z, WIDTH, HEIGHT UPWARDs, THICKNESS, COLOR
	c.addModel(new Block(0, -300, 1000, 800, 	500, 	15, 	wallColor));

	c.addModel(new Block(0, -300, 0, 	1500, 	15, 	2000, 	wallColor));

	c.addModel(new Block(-15, -300, 0,	15,		800,	2000,	wallColor));

	cryph::AffPoint deskCoord(0, -300, 1200);

	Desk d(c, deskCoord, deskWidth, deskLength, deskHeight, legHeight, legWidth, desktopcolor, legsColor);

	glClearColor(1.0, 1.0, 1.0, 1.0);


	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
