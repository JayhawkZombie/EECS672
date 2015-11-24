// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "CustomGLFWController.h"
#include "Block.h"
#include "Stairs.h"
#include "Cylinder.h"
#include "Desk.h"
#include "Room.h"
#include "Wedge.h"
#include "Pyramid.h"
#include "Gemstone.h"
#include "Rupee.h"
#include "Cone.h"

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

	float width = xyz[1] - xyz[0];
	float height = xyz[3] - xyz[2];
	float depth = xyz[5] - xyz[4];

	//float pd = sqrt(height * height + width * width);
	float d = 1.5 * sqrt(height * height + width * width + depth * depth);

	float r = d / 2;

	double distEyeCenter = 2.0 * d;

	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);



	cryph::AffVector up = cryph::AffVector::yu;

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp, ecZmin, ecZmax;
	double zpp, zmin, zmax;
	zmin = - (distEyeCenter + r);
	zmax = zmin + d;
	zpp = 0.85 * zmax;

	ecZpp 	= -(distEyeCenter - 0.5 * maxDelta);
	ecZmin 	= ecZpp - maxDelta * 2.0;
	ecZmax 	= ecZpp + 0.5 * maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(zpp);
	ModelView::setECZminZmax(zmin, zmax);
	//ModelViewWithPhongLighting::setViewingRadius(r);
}

void addWall(GLFWController &c, cryph::AffPoint bottomLeft, float height, float width, float thickness, float color[3])
{
	c.addModel(new Block(bottomLeft.x, bottomLeft.y - height, bottomLeft.z, width, height, thickness, color));
} 

void addTriforce(CustomGLFWController &c, cryph::AffPoint B1, cryph::AffPoint B2, cryph::AffPoint B3, float wedgeThickness, float wedgeHeight, float wedgeWidth, float color[3])
{
	cryph::AffVector leftVector(B2 - B1);
	cryph::AffVector rightVector(B3 - B1);
	cryph::AffVector bottomVector(B3 - B2);

	float sideLength = leftVector.length();

	/* Don't make the same stupid mistake again - NORMALIZE!!!!! */
	leftVector.normalize();
	rightVector.normalize();
	bottomVector.normalize();

	cryph::AffPoint wedge2_pt1(  B2 + sideLength * leftVector  );
	cryph::AffPoint wedge2_pt2(B2);
	cryph::AffPoint wedge2_pt3(  B2 + sideLength * rightVector  );

	cryph::AffPoint wedge3_pt1(  B3 + sideLength * leftVector  );
	cryph::AffPoint wedge3_pt2(B3);
	cryph::AffPoint wedge3_pt3(  B3 + sideLength * rightVector  );

	float wedge1_1[3], wedge1_2[3], wedge1_3[3];
	float wedge2_1[3], wedge2_2[3], wedge2_3[3];
	float wedge3_1[3], wedge3_2[3], wedge3_3[3];

	wedge1_1[0] = B1.x; wedge1_1[1] = B1.y; wedge1_1[2] = B1.z;
	wedge1_2[0] = B2.x; wedge1_2[1] = B2.y; wedge1_2[2] = B2.z;
	wedge1_3[0] = B3.x; wedge1_3[1] = B3.y; wedge1_3[2] = B3.z;

	wedge2_1[0] = wedge2_pt1.x; wedge2_1[1] = wedge2_pt1.y; wedge2_1[2] = wedge2_pt1.z;
	wedge2_2[0] = wedge2_pt2.x; wedge2_2[1] = wedge2_pt2.y; wedge2_2[2] = wedge2_pt2.z;
	wedge2_3[0] = wedge2_pt3.x; wedge2_3[1] = wedge2_pt3.y; wedge2_3[2] = wedge2_pt3.z;

	wedge3_1[0] = wedge3_pt1.x; wedge3_1[1] = wedge3_pt1.y; wedge3_1[2] = wedge3_pt1.z;
	wedge3_2[0] = wedge3_pt2.x; wedge3_2[1] = wedge3_pt2.y; wedge3_2[2] = wedge3_pt2.z;
	wedge3_3[0] = wedge3_pt3.x; wedge3_3[1] = wedge3_pt3.y; wedge3_3[2] = wedge3_pt3.z;

	c.addModel(new Wedge(wedge1_1, wedge1_2, wedge1_3, -wedgeThickness, color));
	c.addModel(new Wedge(wedge2_1, wedge2_2, wedge2_3, wedgeThickness, color));
	c.addModel(new Wedge(wedge3_1, wedge3_2, wedge3_3, wedgeThickness, color));

	std::cerr << "Center of triforce = " << wedge2_pt2.x << wedge2_pt2.y <<  wedge2_pt2.z << std::endl;

}


int main(int argc, char* argv[])
{
	CustomGLFWController c("Room with stairs and a desk", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);
	//c.setRunWaitsForAnEvent(false);

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

	float ruby[3] 	= { 255.0f / 255 , 	0.0f, 			0.0f };
	float gold[3] 	= { 255.0f / 255, 	217.0f / 255, 	0 };
	float green[3] 	= { 6.0f / 255.0f, 	173.0f / 255, 	0 };
	float purple[3] = { 127.0f / 255, 	0.0f / 255, 	173.0f / 255 };
	float orange[3] = { 255.0f / 255, 	128.0f / 255, 	0.0f };
	float blue[3] 	= { 0.0f, 			30.0f / 255, 	255.0f / 255 };
	float grey[3] 	= { 163.0f / 255, 	163.0f / 255, 	163.0f / 255 };



	//Stairs s(c, numOfStairs, stairStart, height, width, length, stairsColor);


	float wall1Attribs[6] 	= {0, 	-300, 0, 	1500, 	800, 	15};
	float wall2Attribs[6] 	= {0, 	-300, 0, 	1500, 	15, 	2000};
	float floorAttribs[6]	= {0, 	 500, 0, 	1500, 	15, 	2000};
	float ceilingAttribs[6] = {-15, -300, 0,	15,		800,	2000};

	cryph::AffPoint deskCoord(0, -300, 1200);
	float triforceColor[3] = {255 / 255.0f, 251 / 255.0f, 0 / 255.0f };

	//Desk d(c, deskCoord, deskWidth, deskLength, deskHeight, legHeight, legWidth, desktopcolor, legsColor);
	//Room r(c, wall1Attribs, wallColor, wall2Attribs, wallColor, ceilingAttribs, wallColor, floorAttribs, wallColor);

	float wedgeWidth = 100.0f;
	float wedgeHeight = 150.0f;
	float wedgeThickness = 50.0f;

	float wedgePts1[3];
	float wedgePts2[3];
	float wedgePts3[3];
	wedgePts1[0] = 0.0f; wedgePts1[1] = 0.0f; wedgePts1[2] = 0.0f;
	wedgePts2[0] = 0.0f; wedgePts2[1] = wedgeHeight; wedgePts2[2] = -wedgeWidth;
	wedgePts3[0] = 0.0f; wedgePts3[1] = 0.0f; wedgePts3[2] = -2 * wedgeWidth;

	cryph::AffPoint B1(wedgePts2);
	cryph::AffPoint B2(wedgePts1);
	cryph::AffPoint B3(wedgePts3);

	float pyramidPts1[3];
	float pyramidPts2[3];
	float pyramidPts3[3];
	float pyramidPts4[3];

	pyramidPts1[0] = -75.0f;	pyramidPts1[1] = -300.0f;	pyramidPts1[2] = -wedgeWidth * 2;
	pyramidPts2[0] = -75.0f;	pyramidPts2[1] = -300.0f;	pyramidPts2[2] = 200.0f - wedgeWidth * 2;
	pyramidPts3[0] = 125.0f;	pyramidPts3[1] = -300.0f; pyramidPts3[2] = 200.0f - wedgeWidth * 2;
	pyramidPts4[0] = 125.0f;	pyramidPts4[1] = -300.0f; pyramidPts4[2] = -wedgeWidth * 2;

	c.addModel(new Pyramid(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, 150.0f, grey));

	pyramidPts1[0] = 500.0f / 2;	pyramidPts1[1] = 0.0f;	pyramidPts1[2] = 1000.0f / 2;
	pyramidPts2[0] = 500.0f / 2;	pyramidPts2[1] = 0.0f;	pyramidPts2[2] = 1500.0f / 2;
	pyramidPts3[0] = 1000.0f / 2;	pyramidPts3[1] = 0.0f; pyramidPts3[2] = 1500.0f / 2;
	pyramidPts4[0] = 1000.0f / 2;	pyramidPts4[1] = 0.0f; pyramidPts4[2] = 1000.0f / 2;

	c.addModel(new Gemstone(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, 500.0f / 2, green));

	pyramidPts1[0] = 0.0f;	pyramidPts1[1] = 0.0f;		pyramidPts1[2] = 100.0f;
	pyramidPts2[0] = 0.0f;	pyramidPts2[1] = 150.0f;	pyramidPts2[2] = 100.0f;
	pyramidPts3[0] = 0.0f;	pyramidPts3[1] = 150.0f; 	pyramidPts3[2] = 250.0f;
	pyramidPts4[0] = 0.0f;	pyramidPts4[1] = 0.0f; 		pyramidPts4[2] = 250.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, ruby));

	addTriforce(c, B1, B2, B3, wedgeThickness, wedgeHeight, wedgeWidth, gold);

	//c.addModel(new Block(0, -300, 1000, 800, 500, 15, wallColor)); //Wall right next to stair (small wall)

	wedgePts1[0] = 400.0f; wedgePts1[1] = 700.0f; 	wedgePts1[2] = 150.0f - 500.0f;
	wedgePts2[0] = 400.0f; wedgePts2[1] = 1200.0f;	wedgePts2[2] = 400.0f - 500.0f;
	wedgePts3[0] = 400.0f; wedgePts3[1] = 700.0f; 	wedgePts3[2] = 650.0f - 500.0f;

	c.addModel(new Block(400.0, -2 * wedgeHeight, -625.0, 1000, 1000, 1000, wallColor));
	//c.addModel(new Wedge(wedgePts1, wedgePts2, wedgePts3, -1000, blue));

	float conet[3];
	conet[0] = 0;
	conet[1] = 500.0f;
	conet[2] = 1000.0f;

	float coneb[3];
	coneb[0] = 0;
	coneb[1] = 0;
	coneb[2] = 1000.0f;
	cryph::AffPoint coneTop(conet);
	cryph::AffPoint coneBottom(coneb);

	glClearColor(0.0, 0.0, 0.0, 1.0);


	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
