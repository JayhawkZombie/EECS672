// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Block.h"
#include "Stairs.h"

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

	cryph::AffPoint eye(xmid + 900, ymid + 250, zmid + 0.75*distEyeCenter);

	cryph::AffVector up = cryph::AffVector::yu;

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp, ecZmin, ecZmax;
	ecZpp = -(distEyeCenter - 0.5 * maxDelta);
	ecZmin = ecZpp - maxDelta * 2.0;
	ecZmax = ecZpp + 0.5 * maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}
/*
void addStairs(GLFWController &c, cryph::AffPoint bottomLeftCorner, float length, float height, float width, int numStairs)
{

	for(int i = 0; i < numStairs; i++)
	{
		c.addModel(new Block(bottomLeftCorner.x + i * width, bottomLeftCorner.y - i * height,	bottomLeftCorner.z, 	width, height, length));
	}

} */

void addWall(GLFWController &c, cryph::AffPoint bottomLeft, float height, float width, float thickness)
{
	c.addModel(new Block(bottomLeft.x, bottomLeft.y - height, bottomLeft.z, width, height, thickness));
}

void addGround(GLFWController &c, float height, bool onTopOfStairs = false)
{
	if (!onTopOfStairs)
		c.addModel(new Block(0, height, 150, 1000, 30, 1000));
	else
		c.addModel(new Block(0, height - 15, 200, 400, 15, 1000));
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
/*
	cryph::AffPoint bottomLeftCorner(230, -130, 130);
	cryph::AffPoint bottomLeftCorner2(330, -30, 130);
	cryph::AffPoint bottomLeftCornerWall1(230, 0, 130);
	cryph::AffPoint bottomLeftCornerWall2(30, 0, 1130); */

	//Rotate to see different angles arounf the stairs - bleh, looks like crap, but it's a start

	cryph::AffPoint bottomLeftCorner(0, -130, 130);
	cryph::AffPoint bottomLeftCorner2(0, -30, 130);
	cryph::AffPoint bottomLeftCornerWall1(0, 0, 130);
	cryph::AffPoint bottomLeftCornerWall2(-200, 0, 1130);


	//addStairs(c, bottomLeftCorner, length, height, width, 5);
	//addStairs(c, bottomLeftCorner2, length, height, width, 5);
	//addGround(c, -545);
	//addGround(c, 0, true);
	addWall(c, bottomLeftCornerWall1, 600, 600, 15);
	addWall(c, bottomLeftCornerWall2, 600, 600, 15);

	//OH MY GOD ----- IT FREAKING WORKS!
	//It's a little weird to do it this way.... but it works!!!!!!
	Stairs s(c, 5, bottomLeftCorner, height, width, length);



	//c.addModel(new Block(230, -130, 130, 100, 100, 100));

	glClearColor(1.0, 1.0, 1.0, 1.0);


	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
