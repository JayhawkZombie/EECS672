// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "CustomGLFWController.h"
#include "Block.h"
//#include "Stairs.h"
#include "Cylinder.h"
//#include "Desk.h"
//#include "Room.h"
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
	zmin = -(distEyeCenter + 2 * r);
	zmax = zmin + 2 * d;
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
	//c.addModel(new Block(bottomLeft.x, bottomLeft.y - height, bottomLeft.z, width, height, thickness, color));
} 

void addTree(CustomGLFWController &c, cryph::AffPoint tip, cryph::AffPoint bottom, float rad, float leafC[3], float trunkC[3])
{

	cryph::AffPoint centers[6];

	cryph::AffVector up(bottom - tip);

	float height = up.length();

	up.normalize();

	centers[0] = cryph::AffPoint(tip);
	centers[1] = cryph::AffPoint(tip + (0.5) * height * up);
	centers[2] = cryph::AffPoint(tip + (0.33) * height * up);
	centers[3] = cryph::AffPoint(tip + (0.75) * height * up);
	centers[4] = centers[3];
	centers[5] = bottom;

	c.addModel(new Cylinder(0.0f, rad, centers[0], centers[1], leafC, false, true));
	c.addModel(new Cylinder(0.0f, rad, centers[2], centers[3], leafC, false, true));
	c.addModel(new Cylinder(rad / 2, rad / 2, centers[4], centers[5], trunkC, false, true));
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
	CustomGLFWController c("Temple Of Time", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);
	//c.setRunWaitsForAnEvent(false);

	// create your scene, adding things to the Controller....

	
	float bottomLeftX = 230;
	//Let's try to create a DESK


	cryph::AffPoint bottomLeftPoint(0,-500, 1130);

	float ruby[3] 	= { 255.0f / 255 , 	0.0f, 			0.0f };
	float gold[3] 	= { 255.0f / 255, 	217.0f / 255, 	0 };
	float green[3] 	= { 6.0f / 255.0f, 	173.0f / 255, 	0 };
	float purple[3] = { 127.0f / 255, 	0.0f / 255, 	173.0f / 255 };
	float orange[3] = { 255.0f / 255, 	128.0f / 255, 	0.0f };
	float blue[3] 	= { 0.0f, 			30.0f / 255, 	255.0f / 255 };
	float grey[3] 	= { 163.0f / 255, 	163.0f / 255, 	163.0f / 255 };
	float pink[3] 	= { 255.0f / 255,	0.0f / 255, 	144.0f / 255 };
	float brown[3]  = { 128.0f / 255,	68.0f / 255,	0.0f / 255 };
	float darkgreen[3] = {26.0f / 255,	120.0f / 255, 	0.0f / 255 };



	//Stairs s(c, numOfStairs, stairStart, height, width, length, stairsColor);


	float wall1Attribs[6] 	= {0, 	-300, 0, 	1500, 	800, 	15};
	float wall2Attribs[6] 	= {0, 	-300, 0, 	1500, 	15, 	2000};
	float floorAttribs[6]	= {0, 	 500, 0, 	1500, 	15, 	2000};
	float ceilingAttribs[6] = {-15, -300, 0,	15,		800,	2000};

	cryph::AffPoint deskCoord(0, -300, 1200);
	float triforceColor[3] = {255 / 255.0f, 251 / 255.0f, 0 / 255.0f };

	//Desk d(c, deskCoord, deskWidth, deskLength, deskHeight, legHeight, legWidth, desktopcolor, legsColor);
	//Room r(c, wall1Attribs, wallColor, wall2Attribs, wallColor, ceilingAttribs, wallColor, floorAttribs, wallColor);

	float floorAlterValues[3] = { 0.03, 0.03, 0.03 };
	float noChangeValues[3] = { 1.0, 1.0, 1.0 };

	c.addModel(new Block(-10000.0f, -10.0f, -10000.0f, 20000.0f, 10.0f, 20000.0f, brown, floorAlterValues));

	float wedgeWidth = 100.0f;
	float wedgeHeight = 150.0f;
	float wedgeThickness = 50.0f;

	float wedgePts1[3];
	float wedgePts2[3];
	float wedgePts3[3];
	wedgePts1[0] = -500.0f; wedgePts1[1] = 0.0f; wedgePts1[2] = 0.0f + 750.0f;		//Z MOVES LEFT AND RIGHT - X MOVES FORWARD AND BACKWARD - Y STILL MOVES UP AND DOWN
	wedgePts2[0] = -500.0f; wedgePts2[1] = wedgeHeight; wedgePts2[2] = -wedgeWidth + 750.0f;
	wedgePts3[0] = -500.0f; wedgePts3[1] = 0.0f; wedgePts3[2] = -2 * wedgeWidth + 750.0f;

	float pyramidPts1[3];
	float pyramidPts2[3];
	float pyramidPts3[3];
	float pyramidPts4[3];

	//There's a whole bunch of rupees in the scene!

	pyramidPts1[0] = -400.0f;	pyramidPts1[1] = 50.0f;			pyramidPts1[2] = 250.0f + 300.0f;
	pyramidPts2[0] = -400.0f;	pyramidPts2[1] = 180.0f;		pyramidPts2[2] = 250.0f + 300.0f;
	pyramidPts3[0] = -400.0f;	pyramidPts3[1] = 180.0f; 		pyramidPts3[2] = 380.0f + 300.0f;
	pyramidPts4[0] = -400.0f;	pyramidPts4[1] = 50.0f; 		pyramidPts4[2] = 380.0f + 300.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, purple));

	pyramidPts1[0] = -200.0f;	pyramidPts1[1] = 100.0f;		pyramidPts1[2] = 250.0f;
	pyramidPts2[0] = -200.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 250.0f;
	pyramidPts3[0] = -200.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 300.0f;
	pyramidPts4[0] = -200.0f;	pyramidPts4[1] = 100.0f; 		pyramidPts4[2] = 300.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, ruby));

	pyramidPts1[0] = -400.0f;	pyramidPts1[1] = 55.0f;			pyramidPts1[2] = 250.0f;
	pyramidPts2[0] = -400.0f;	pyramidPts2[1] = 130.0f;		pyramidPts2[2] = 250.0f;
	pyramidPts3[0] = -400.0f;	pyramidPts3[1] = 130.0f; 		pyramidPts3[2] = 325.0f;
	pyramidPts4[0] = -400.0f;	pyramidPts4[1] = 55.0f; 		pyramidPts4[2] = 325.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, blue));

	pyramidPts1[0] = -800.0f;	pyramidPts1[1] = 55.0f;			pyramidPts1[2] = 250.0f;
	pyramidPts2[0] = -800.0f;	pyramidPts2[1] = 130.0f;		pyramidPts2[2] = 250.0f;
	pyramidPts3[0] = -800.0f;	pyramidPts3[1] = 130.0f; 		pyramidPts3[2] = 325.0f;
	pyramidPts4[0] = -800.0f;	pyramidPts4[1] = 55.0f; 		pyramidPts4[2] = 325.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, grey));

	pyramidPts1[0] = -700.0f;	pyramidPts1[1] = 40.0f;			pyramidPts1[2] = 750.0f;
	pyramidPts2[0] = -700.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 750.0f;
	pyramidPts3[0] = -700.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 860.0f;
	pyramidPts4[0] = -700.0f;	pyramidPts4[1] = 40.0f; 		pyramidPts4[2] = 860.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, grey));

	pyramidPts1[0] = -1300.0f;	pyramidPts1[1] = 40.0f;			pyramidPts1[2] = 350.0f;
	pyramidPts2[0] = -1300.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 350.0f;
	pyramidPts3[0] = -1300.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 460.0f;
	pyramidPts4[0] = -1300.0f;	pyramidPts4[1] = 40.0f; 		pyramidPts4[2] = 460.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, green));
	pyramidPts1[0] = -600.0f;	pyramidPts1[1] = 40.0f;			pyramidPts1[2] = 1250.0f;
	pyramidPts2[0] = -600.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 1250.0f;
	pyramidPts3[0] = -600.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 1360.0f;
	pyramidPts4[0] = -600.0f;	pyramidPts4[1] = 40.0f; 		pyramidPts4[2] = 1360.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, purple));
	pyramidPts1[0] = -1000.0f;	pyramidPts1[1] = 40.0f;			pyramidPts1[2] = 750.0f;
	pyramidPts2[0] = -1000.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 750.0f;
	pyramidPts3[0] = -1000.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 860.0f;
	pyramidPts4[0] = -1000.0f;	pyramidPts4[1] = 40.0f; 		pyramidPts4[2] = 860.0f;

	c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, blue));

	float treeTop[3];
	treeTop[0] = -200.0f;
	treeTop[1] = 500.0f;
	treeTop[2] = 500.0f;

	float treeBottom[3];
	treeBottom[0] = -200.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 500.0f;

	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	treeTop[0] = 1600.0f;
	treeTop[1] = 700.0f;
	treeTop[2] = 500.0f;

	treeBottom[0] = 1600.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 500.0f;

	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 140.0f, darkgreen, brown);

	treeTop[0] = -200.0f;
	treeTop[1] = 500.0f;
	treeTop[2] = -200.0f;

	treeBottom[0] = -200.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = -200.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	treeTop[0] = -450.0f;
	treeTop[1] = 500.0f;
	treeTop[2] = 1200.0f;

	treeBottom[0] = -450.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 1200.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	treeTop[0] = -900.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 200.0f;

	treeBottom[0] = -900.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 200.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 70.0f, darkgreen, brown);

	treeTop[0] = -800.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 1500.0f;

	treeBottom[0] = -800.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 1500.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 70.0f, darkgreen, brown);

	treeTop[0] = -300.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 1400.0f;

	treeBottom[0] = -300.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 1400.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 90.0f, darkgreen, brown);

	treeTop[0] = -1200.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 1400.0f;

	treeBottom[0] = -1200.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 1400.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 90.0f, darkgreen, brown);

	treeTop[0] = -1600.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 1300.0f;

	treeBottom[0] = -1600.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 1300.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 90.0f, darkgreen, brown);


	treeTop[0] = -480.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = -200.0f;

	treeBottom[0] = -900.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = -200.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	treeTop[0] = -300.0f;
	treeTop[1] = 400.0f;
	treeTop[2] = 200.0f;

	treeBottom[0] = -300.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = 200.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 80.0f, darkgreen, brown);

	treeTop[0] = -1300.0f;
	treeTop[1] = 700.0f;
	treeTop[2] = -500.0f;

	treeBottom[0] = -1300.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = -500.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	treeTop[0] = -1900.0f;
	treeTop[1] = 900.0f;
	treeTop[2] = -wedgeWidth + 750.0f + 75.0f;

	treeBottom[0] = -1900.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = -wedgeWidth + 750.0f + 75.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 200.0f, darkgreen, brown);

	treeTop[0] = -1300.0f;
	treeTop[1] = 700.0f;
	treeTop[2] = -500.0f;

	treeBottom[0] = -1300.0f;
	treeBottom[1] = 0.0f;
	treeBottom[2] = -500.0f;
	addTree(c, cryph::AffPoint(treeTop), cryph::AffPoint(treeBottom), 100.0f, darkgreen, brown);

	pyramidPts1[0] = 0.0f;	pyramidPts1[1] = 100.0f;		pyramidPts1[2] = 250.0f;
	pyramidPts2[0] = 0.0f;	pyramidPts2[1] = 150.0f;		pyramidPts2[2] = 250.0f;
	pyramidPts3[0] = 0.0f;	pyramidPts3[1] = 150.0f; 		pyramidPts3[2] = 300.0f;
	pyramidPts4[0] = 0.0f;	pyramidPts4[1] = 100.0f; 		pyramidPts4[2] = 300.0f;

	//c.addModel(new Rupee(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, blue));

	//Alright, let's start defining the scene

	float templeHeight = 700.0f;
	float templeWidth = 1500.0f;
	float templeDepth = 1200.0f;

	cryph::AffPoint templeFront[4];
	templeFront[0].x = 0.0f;		templeFront[0].y = 0.0f;			templeFront[0].z = 0.0f;
	templeFront[1].x = 0.0f;		templeFront[1].y = templeHeight;	templeFront[1].z = 0.0f;
	templeFront[2].x = templeWidth; templeFront[2].y = 0.0f;			templeFront[2].z = 0.0f;
	templeFront[3].x = templeWidth;	templeFront[3].y = templeHeight;	templeFront[3].z = 0.0f;

	cryph::AffPoint templeBack[4];
	templeBack[0].x = 0.0f;			templeBack[0].y = 0.0f;				templeBack[0].z = templeDepth;
	templeBack[1].x = 0.0f;			templeBack[1].y = templeHeight;		templeBack[1].z = templeDepth;
	templeBack[2].x = templeWidth;	templeBack[2].y = 0.0f;				templeBack[2].z = templeDepth;
	templeBack[3].x = templeWidth;	templeBack[3].y = templeHeight;		templeBack[3].z = templeDepth;

	//float wedgePts1[3];
	//float wedgePts2[3];
	//float wedgePts3[3];
	wedgePts1[0] = -1200.0f; wedgePts1[1] = 0.0f + templeHeight / 2; 		wedgePts1[2] = 0.0f + 750.0f + 75.0f;		//Z MOVES LEFT AND RIGHT - X MOVES FORWARD AND BACKWARD - Y STILL MOVES UP AND DOWN
	wedgePts2[0] = -1200.0f; wedgePts2[1] = wedgeHeight + templeHeight / 2; 	wedgePts2[2] = -wedgeWidth + 750.0f + 75.0f;
	wedgePts3[0] = -1200.0f; wedgePts3[1] = 0.0f + templeHeight / 2; 		wedgePts3[2] = -2 * wedgeWidth + 750.0f + 75.0f;

	cryph::AffPoint B1(wedgePts2);
	cryph::AffPoint B2(wedgePts1);
	cryph::AffPoint B3(wedgePts3);

	float triforceWidth = 100.0f;
	float triforceHeight = 100.0f;

	cryph::AffPoint triforceTopLeft, triforceTopMiddle, triforceTopRight;
	triforceTopMiddle.x = 500.0f;
	triforceTopMiddle.y = 500.0f;
	triforceTopMiddle.z = 0.0f;

	triforceTopLeft.x = 375.0f;
	triforceTopLeft.y = 400.0f;
	triforceTopLeft.z = 0.0f;

	triforceTopRight.x = 625.0f;
	triforceTopRight.y = 400.0f;
	triforceTopRight.z = 0.0f;

	
 //Block renders as DEPTH - HEIGHT - WIDTH
	c.addModel(new Block(templeFront[0].x, templeFront[0].y, templeFront[0].z, templeDepth, templeHeight + 200.0f, templeWidth, grey, noChangeValues));

//We need to add a triforce that is in the middle of the temple, but pushed forward a few hundred units
	addTriforce(c, B1, B2, B3, wedgeThickness, wedgeHeight, wedgeWidth, gold);

	cryph::AffPoint platformBottom(-1200.0f, 0.0f, wedgePts2[2]);
	cryph::AffPoint platformTop(-1200.0f, 150.0f, wedgePts2[2]);

	//c.addModel(new Cylinder(500.0f, 300.0f, platformTop, platformBottom, grey, false));

	//Add the two gems that sit atop the temple

	pyramidPts1[0] = 300.0f / 2 - 50.0f;	pyramidPts1[1] = templeHeight + 400.0f;		pyramidPts1[2] = 300.0f / 2 - 50.0f; //X VALUE MOVES BACKWARDS
	pyramidPts2[0] = 300.0f / 2 - 50.0f;	pyramidPts2[1] = templeHeight + 400.0f;		pyramidPts2[2] = 600.0f / 2 - 50.0f;
	pyramidPts3[0] = 600.0f / 2 - 50.0f;	pyramidPts3[1] = templeHeight + 400.0f; 	pyramidPts3[2] = 600.0f / 2 - 50.0f;
	pyramidPts4[0] = 600.0f / 2 - 50.0f;	pyramidPts4[1] = templeHeight + 400.0f; 	pyramidPts4[2] = 300.0f / 2 - 50.0f; //Z VALUE MOVES TOWARDS THE SCREEN

	c.addModel(new Gemstone(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, 300.0f / 2, green));

	pyramidPts1[0] = 300.0f / 2 - 50.0f;	pyramidPts1[1] = templeHeight + 400.0f;		pyramidPts1[2] = 300.0f / 2 + 1500.0f - 450.0f; //X VALUE MOVES BACKWARDS
	pyramidPts2[0] = 300.0f / 2 - 50.0f;	pyramidPts2[1] = templeHeight + 400.0f;		pyramidPts2[2] = 600.0f / 2 + 1500.0f - 450.0f;
	pyramidPts3[0] = 600.0f / 2 - 50.0f;	pyramidPts3[1] = templeHeight + 400.0f; 	pyramidPts3[2] = 600.0f / 2 + 1500.0f - 450.0f;
	pyramidPts4[0] = 600.0f / 2 - 50.0f;	pyramidPts4[1] = templeHeight + 400.0f; 	pyramidPts4[2] = 300.0f / 2 + 1500.0f - 450.0f; //Z VALUE MOVES TOWARDS THE SCREEN

	c.addModel(new Gemstone(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, 300.0f / 2, green));

	//The wedge that sits on top of the temple

	wedgePts1[0] = 0.0f; wedgePts1[1] = templeHeight + 200.0f; 				wedgePts1[2] = 0.0f + 850.0f + 150.0f;		//Z MOVES LEFT AND RIGHT - X MOVES FORWARD AND BACKWARD - Y STILL MOVES UP AND DOWN
	wedgePts2[0] = 0.0f; wedgePts2[1] = wedgeHeight + templeHeight + 400.0f; wedgePts2[2] = -wedgeWidth + 700.0f + 150.0f;
	wedgePts3[0] = 0.0f; wedgePts3[1] = templeHeight + 200.0f; 				wedgePts3[2] = -2 * wedgeWidth + 550.0f + 150.0f;

	c.addModel(new Wedge(wedgePts1, wedgePts2, wedgePts3, templeDepth, blue));

	//Add the gem that sits on top of the wedge on top of the temple

	pyramidPts1[0] = 200.0f / 2 - 50.0f;	pyramidPts1[1] = wedgePts2[1] + 600.0f;		pyramidPts1[2] = 260.0f + 350.0f;// / 2 - templeWidth;// - 450.0f; //X VALUE MOVES BACKWARDS
	pyramidPts2[0] = 200.0f / 2 - 50.0f;	pyramidPts2[1] = wedgePts2[1] + 600.0f;		pyramidPts2[2] = 510.0f + 350.0f;// / 2 - templeWidth;// - 450.0f;
	pyramidPts3[0] = 700.0f / 2 - 50.0f;	pyramidPts3[1] = wedgePts2[1] + 600.0f; 	pyramidPts3[2] = 510.0f + 350.0f;// / 2 - templeWidth;// - 450.0f;
	pyramidPts4[0] = 700.0f / 2 - 50.0f;	pyramidPts4[1] = wedgePts2[1] + 600.0f; 	pyramidPts4[2] = 260.0f + 350.0f;// / 2 - templeWidth;// - 450.0f; //Z VALUE MOVES TOWARDS THE SCREEN

	c.addModel(new Gemstone(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, 500.0f / 2, pink));

	float gem1[4][4];

	gem1[0][0] = -300.0f;	gem1[0][1] = 700.0f;	gem1[0][2] = -100.0f;
	gem1[1][0] = -400.0f;	gem1[1][1] = 700.0f;	gem1[1][2] = 100.0f;
	gem1[2][0] = -400.0f;	gem1[2][1] = 700.0f;	gem1[2][2] = 100.0f;
	gem1[3][0] = -300.0f;	gem1[3][1] = 700.0f;	gem1[3][2] = -100.0f;

	float gemPts1[3];
	float gemPts2[3];
	float gemPts3[3];
	float gemPts4[3];

	cryph::AffPoint cylinderBottom(-300.0f, 0.0f, 100.0f);
	cryph::AffPoint cylinderTop(-300.0f, 600.0f, 100.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-500.0f, 0.0f, -150.0f);
	cylinderTop = cryph::AffPoint(-500.0f, 500.0f, -150.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-800.0f, 0.0f, -300.0f);
	cylinderTop = cryph::AffPoint(-800.0f, 400.0f, -300.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1200.0f, 0.0f, -350.0f);
	cylinderTop = cryph::AffPoint(-1200.0f, 300.0f, -350.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1500.0f, 0.0f, -300.0f);
	cylinderTop = cryph::AffPoint(-1500.0f, 200.0f, -300.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1800.0f, 0.0f, -150.0f);
	cylinderTop = cryph::AffPoint(-1800.0f, 100.0f, -150.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	gemPts1[0] = -225.0f;	gemPts1[1] = 800.0f;	gemPts1[2] = 0.0f;
	gemPts2[0] = -225.0f;	gemPts2[1] = 800.0f;	gemPts2[2] = 150.0f;
	gemPts3[0] = -375.0f;	gemPts3[1] = 800.0f;	gemPts3[2] = 150.0f;
	gemPts4[0] = -375.0f;	gemPts4[1] = 800.0f;	gemPts4[2] = 0.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, purple));

	gemPts1[0] = -425.0f;	gemPts1[1] = 700.0f;	gemPts1[2] = -225.0f;
	gemPts2[0] = -425.0f;	gemPts2[1] = 700.0f;	gemPts2[2] = -75.0f;
	gemPts3[0] = -575.0f;	gemPts3[1] = 700.0f;	gemPts3[2] = -75.0f;
	gemPts4[0] = -575.0f;	gemPts4[1] = 700.0f;	gemPts4[2] = -225.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, orange));


	gemPts1[0] = -725.0f;	gemPts1[1] = 600.0f;	gemPts1[2] = -375.0f;
	gemPts2[0] = -725.0f;	gemPts2[1] = 600.0f;	gemPts2[2] = -225.0f;
	gemPts3[0] = -875.0f;	gemPts3[1] = 600.0f;	gemPts3[2] = -225.0f;
	gemPts4[0] = -875.0f;	gemPts4[1] = 600.0f;	gemPts4[2] = -375.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, ruby));

	gemPts1[0] = -1125.0f;	gemPts1[1] = 500.0f;	gemPts1[2] = -425.0f;
	gemPts2[0] = -1125.0f;	gemPts2[1] = 500.0f;	gemPts2[2] = -275.0f;
	gemPts3[0] = -1275.0f;	gemPts3[1] = 500.0f;	gemPts3[2] = -275.0f;
	gemPts4[0] = -1275.0f;	gemPts4[1] = 500.0f;	gemPts4[2] = -425.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, blue));

	gemPts1[0] = -1425.0f;	gemPts1[1] = 400.0f;	gemPts1[2] = -375.0f;
	gemPts2[0] = -1425.0f;	gemPts2[1] = 400.0f;	gemPts2[2] = -225.0f;
	gemPts3[0] = -1575.0f;	gemPts3[1] = 400.0f;	gemPts3[2] = -225.0f;
	gemPts4[0] = -1575.0f;	gemPts4[1] = 400.0f;	gemPts4[2] = -375.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, green));

	gemPts1[0] = -1725.0f;	gemPts1[1] = 300.0f;	gemPts1[2] = -225.0f;
	gemPts2[0] = -1725.0f;	gemPts2[1] = 300.0f;	gemPts2[2] = -75.0f;
	gemPts3[0] = -1875.0f;	gemPts3[1] = 300.0f;	gemPts3[2] = -75.0f;
	gemPts4[0] = -1875.0f;	gemPts4[1] = 300.0f;	gemPts4[2] = -225.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, grey));





	cylinderBottom = cryph::AffPoint(-300.0f, 0.0f, 1500.0f);
	cylinderTop = cryph::AffPoint(-300.0f, 600.0f, 1500.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-500.0f, 0.0f, 1750.0f);
	cylinderTop = cryph::AffPoint(-500.0f, 500.0f, 1750.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-800.0f, 0.0f, 1900.0f);
	cylinderTop = cryph::AffPoint(-800.0f, 400.0f, 1900.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1200.0f, 0.0f, 1950.0f);
	cylinderTop = cryph::AffPoint(-1200.0f, 300.0f, 1950.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1500.0f, 0.0f, 1900.0f);
	cylinderTop = cryph::AffPoint(-1500.0f, 200.0f, 1900.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	cylinderBottom = cryph::AffPoint(-1800.0f, 0.0f, 1750.0f);
	cylinderTop = cryph::AffPoint(-1800.0f, 100.0f, 1750.0f);
	c.addModel(new Cylinder(50.0f, 50.0f, cylinderTop, cylinderBottom, ruby, false));

	gemPts1[0] = -225.0f;	gemPts1[1] = 800.0f;	gemPts1[2] = 1425.0f;
	gemPts2[0] = -225.0f;	gemPts2[1] = 800.0f;	gemPts2[2] = 1575.0f;
	gemPts3[0] = -375.0f;	gemPts3[1] = 800.0f;	gemPts3[2] = 1575.0f;
	gemPts4[0] = -375.0f;	gemPts4[1] = 800.0f;	gemPts4[2] = 1425.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, purple));

	gemPts1[0] = -425.0f;	gemPts1[1] = 700.0f;	gemPts1[2] = 1650.0f;
	gemPts2[0] = -425.0f;	gemPts2[1] = 700.0f;	gemPts2[2] = 1800.0f;
	gemPts3[0] = -575.0f;	gemPts3[1] = 700.0f;	gemPts3[2] = 1800.0f;
	gemPts4[0] = -575.0f;	gemPts4[1] = 700.0f;	gemPts4[2] = 1650.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, orange));

	gemPts1[0] = -725.0f;	gemPts1[1] = 600.0f;	gemPts1[2] = 1800.0f;
	gemPts2[0] = -725.0f;	gemPts2[1] = 600.0f;	gemPts2[2] = 1950.0f;
	gemPts3[0] = -875.0f;	gemPts3[1] = 600.0f;	gemPts3[2] = 1950.0f;
	gemPts4[0] = -875.0f;	gemPts4[1] = 600.0f;	gemPts4[2] = 1800.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, ruby));

	gemPts1[0] = -1125.0f;	gemPts1[1] = 500.0f;	gemPts1[2] = 1850.0f;
	gemPts2[0] = -1125.0f;	gemPts2[1] = 500.0f;	gemPts2[2] = 2000.0f;
	gemPts3[0] = -1275.0f;	gemPts3[1] = 500.0f;	gemPts3[2] = 2000.0f;
	gemPts4[0] = -1275.0f;	gemPts4[1] = 500.0f;	gemPts4[2] = 1850.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, blue));

	gemPts1[0] = -1425.0f;	gemPts1[1] = 400.0f;	gemPts1[2] = 1800.0f;
	gemPts2[0] = -1425.0f;	gemPts2[1] = 400.0f;	gemPts2[2] = 1950.0f;
	gemPts3[0] = -1575.0f;	gemPts3[1] = 400.0f;	gemPts3[2] = 1950.0f;
	gemPts4[0] = -1575.0f;	gemPts4[1] = 400.0f;	gemPts4[2] = 1800.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, green));

	gemPts1[0] = -1725.0f;	gemPts1[1] = 300.0f;	gemPts1[2] = 1650.0f;
	gemPts2[0] = -1725.0f;	gemPts2[1] = 300.0f;	gemPts2[2] = 1800.0f;
	gemPts3[0] = -1875.0f;	gemPts3[1] = 300.0f;	gemPts3[2] = 1800.0f;
	gemPts4[0] = -1875.0f;	gemPts4[1] = 300.0f;	gemPts4[2] = 1650.0f;

	c.addModel(new Gemstone(gemPts1, gemPts2, gemPts3, gemPts4, 300.0f / 2, grey));

	cryph::AffPoint pyramidCenter = B1;
	float pyramidWidth = 100.0f;

//Z MOVES LEFT AND RIGHT - X MOVES FORWARD AND BACKWARD - Y STILL MOVES UP AND DOWN
	cryph::AffPoint pyramid1, pyramid2, pyramid3, pyramid4;

	pyramid1.x = pyramidCenter.x + pyramidWidth;
	pyramid1.y = 0.0f;
	pyramid1.z = pyramidCenter.z - pyramidWidth;

	pyramid2.x = pyramidCenter.x + pyramidWidth;
	pyramid2.y = 0.0f;
	pyramid2.z = pyramidCenter.z + pyramidWidth;

	pyramid3.x = pyramidCenter.x - pyramidWidth;
	pyramid3.y = 0.0f;
	pyramid3.z = pyramidCenter.z + pyramidWidth;

	pyramid4.x = pyramidCenter.x - pyramidWidth;
	pyramid4.y = 0.0f;
	pyramid4.z = pyramidCenter.z - pyramidWidth;

	pyramidPts1[0] = pyramid1.x;	pyramidPts1[1] = pyramid1.y;	pyramidPts1[2] = pyramid1.z;
	pyramidPts2[0] = pyramid2.x;	pyramidPts2[1] = pyramid2.y;	pyramidPts2[2] = pyramid2.z;
	pyramidPts3[0] = pyramid3.x;	pyramidPts3[1] = pyramid3.y;	pyramidPts3[2] = pyramid3.z;
	pyramidPts4[0] = pyramid4.x;	pyramidPts4[1] = pyramid4.y; 	pyramidPts4[2] = pyramid4.z;

	c.addModel(new Pyramid(pyramidPts1, pyramidPts2, pyramidPts3, pyramidPts4, -150.0f, grey));

	//addTriforce(c, triforceTopLeft, triforceTopMiddle, triforceTopRight, 50.0f, triforceWidth, triforceHeight, gold);

	glClearColor(0.0, 0.0, 0.0, 1.0);


	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
