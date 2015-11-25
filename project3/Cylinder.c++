#include "Cylinder.h"
#include "ShaderIF.h"

#include <math.h>
	
cryph::AffVector dir;

#define N_POINTS_AROUND_SLICE 40

typedef float vec3[3];

vec3 Cylinder::ka = { 0.135, 0.2225, 0.1575 };
vec3 Cylinder::kd = {0.54, 0.89, 0.63};
vec3 Cylinder::ks = {0.316228, 0.316228, 0.316228};
float Cylinder::m = 12.8;
	
Cylinder::Cylinder(float topRad, float botRad, const cryph::AffPoint& t, const cryph::AffPoint& b, float inColor[3], bool toCap) : displayCylEdges(false), displayCylFill(true)
{
	bottomRadius 	= botRad;
	topRadius 		= topRad;
	maxRadius		= fmax(botRad, topRad);
	CylinderBottom 	= b;
	CylinderTop		= t;
	isATree = false;
	capped = toCap;
	dir 			= (CylinderTop - CylinderBottom);
	dir.normalize();
	topToBottom = CylinderTop - CylinderBottom;
	defineCylinder();


/*

	topCap[0][0] = CylinderTop.x;
	topCap[0][1] = CylinderTop.y;
	topCap[0][2] = CylinderTop.z;

	bottomCap[0][0] = CylinderBottom.x;
	bottomCap[0][1] = CylinderBottom.y;
	bottomCap[0][2] = CylinderBottom.z; */

	colors[0] 		= inColor[0];
	colors[1]		= inColor[1];
	colors[2]		= inColor[2];
}

Cylinder::Cylinder(float topRad, float botRad, const cryph::AffPoint& t, const cryph::AffPoint& b, float inColor[3], bool toCap, bool isTree) : displayCylEdges(false), displayCylFill(true)
{
	bottomRadius 	= botRad;
	topRadius 		= topRad;
	maxRadius		= fmax(botRad, topRad);
	CylinderBottom 	= b;
	CylinderTop		= t;
	isATree = isTree;
	capped = toCap;
	dir 			= (CylinderTop - CylinderBottom);
	dir.normalize();
	topToBottom = CylinderTop - CylinderBottom;
	defineCylinder();


/*

	topCap[0][0] = CylinderTop.x;
	topCap[0][1] = CylinderTop.y;
	topCap[0][2] = CylinderTop.z;

	bottomCap[0][0] = CylinderBottom.x;
	bottomCap[0][1] = CylinderBottom.y;
	bottomCap[0][2] = CylinderBottom.z; */

	colors[0] 		= inColor[0];
	colors[1]		= inColor[1];
	colors[2]		= inColor[2];
}
	
Cylinder::~Cylinder()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}
	
	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Cylinder::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = bounds[0];
	xyzLimits[1] = bounds[1];
	xyzLimits[2] = bounds[2];
	xyzLimits[3] = bounds[3];
	xyzLimits[4] = bounds[4];
	xyzLimits[5] = bounds[5];
}

void pwlApproxCircle(const cryph::AffPoint& Center, const cryph::AffVector& normal, double radius,
	bool includeCenter, cryph::AffPoint* pwlApprox, int nPoints)
{
	int nPointsAroundPerimeter = includeCenter ? nPoints-1 : nPoints;
	if (nPointsAroundPerimeter < 4)
		// degenerate
		return;
	// Determine the "local x" (uHat) and "local y" (vHat) axes
	cryph::AffVector uHat, vHat; // constructor initializes to (0,0,0)
	cryph::AffVector wHat(normal); // following will normalize the copy
	cryph::AffVector::coordinateSystemFromUW(uHat, vHat, wHat);

	// Now create the PWL approximation
	double dTheta = 2.0 * M_PI / (nPointsAroundPerimeter - 1);
	double theta = 0.0;
	int loc = 0, first = 0;
	if (includeCenter)
	{
		pwlApprox[loc++] = Center;
		first = 1;
	}
	for (int i=0 ; i<(nPointsAroundPerimeter-1) ; i++, theta+=dTheta)
		pwlApprox[loc++] = Center + radius * (cos(theta)*uHat + sin(theta)*vHat);
	pwlApprox[nPoints-1] = pwlApprox[first];
}
	
void Cylinder::defineCylinder()
{
	bounds[0] = CylinderBottom.x 	- maxRadius;
	bounds[1] = CylinderBottom.x 	+ maxRadius;
	bounds[2] = CylinderBottom.y 	- maxRadius;
	bounds[3] = CylinderTop.y 		+ maxRadius;
	bounds[4] = CylinderBottom.z 	- maxRadius;
	bounds[5] = CylinderBottom.z 	+ maxRadius;


	double 	theta 	= 0.0;
	double 	deltaTheta 	= 2.0 * M_PI / ( N_POINTS_AROUND_SLICE );
	
	int 	nPoints = 2 * (N_POINTS_AROUND_SLICE);
	vec3* 	coords 	= new vec3[nPoints];
	vec3* 	normals	= new vec3[nPoints];
	vec3*	topCap  = new vec3[N_POINTS_AROUND_SLICE];
	
	
	
	cryph::AffVector point 			= cryph::AffVector(dir.dx - 1, dir.dy, dir.dz + 1).cross(dir);
	cryph::AffPoint  b 				= CylinderBottom 	+ bottomRadius * point;
	cryph::AffPoint  t 				= CylinderTop 		+ topRadius * point;
			
	cryph::AffVector bPerpendicular = (b - CylinderBottom).cross(t - b).cross(t - b);

	//cryph::AffVector topToBottom = CylinderTop - CylinderBottom;

	//cryph::AffPoint p[N_POINTS_AROUND_SLICE];

	//topCap[0][0] = p[0].x;
	//topCap[0][1] = p[0].y;
	//topCap[0][2] = p[0].z;

	//pwlApproxCircle(CylinderTop, -topToBottom, topRadius, true, p, N_POINTS_AROUND_SLICE);
	
	
	for (int i = 0 ; i < N_POINTS_AROUND_SLICE; i++)
	{
/*
		topCapNormals[i][0] = topToBottom.dx;
		topCapNormals[i][1] = topToBottom.dy;
		topCapNormals[i][2] = topToBottom.dz;

		bottomCapNormals[i][0] = -topToBottom.dx;
		bottomCapNormals[i][1] = -topToBottom.dy;
		bottomCapNormals[i][2] = -topToBottom.dz; */


		/* Set the values for the new coordinates after moving to the next point */
		coords[2 * i][0] 		= t.x; 
		coords[2 * i][1] 		= t.y; 
		coords[2 * i][2] 		= t.z;
		coords[2 * i + 1][0] 	= b.x; 
		coords[2 * i + 1][1] 	= b.y; 
		coords[2 * i + 1][2] 	= b.z;
/*
		topCap[i][0] = p[i].x;
		topCap[i][1] = p[i].y;
		topCap[i][2] = p[i].z;

		bottomCap[i + 1][0] = b.x;
		bottomCap[i + 1][1] = b.y;
		bottomCap[i + 1][2] = b.z; */

		/* Set the values for the normal vectors */		
		normals[2 * i][0] 		= bPerpendicular.dx; 
		normals[2 * i][1] 		= bPerpendicular.dy; 
		normals[2 * i][2] 		= bPerpendicular.dz;
		
		normals[2 * i + 1][0] 	= bPerpendicular.dx; 
		normals[2 * i + 1][1] 	= bPerpendicular.dy; 
		normals[2 * i + 1][2] 	= bPerpendicular.dz;

		point 	= (b - CylinderBottom);
		point.normalize();

		b 		= CylinderBottom 	+ ((bottomRadius * cos(deltaTheta)) * point + ((bottomRadius * sin(deltaTheta)) * dir.cross(point)));
		t 		= CylinderTop 		+ ((topRadius * cos(deltaTheta)) * point + ((topRadius * sin(deltaTheta)) * dir.cross(point)));
		
		cryph::AffVector helperV = (b - CylinderBottom).cross(t - b);
		bPerpendicular = helperV.cross(t - b);

		theta += deltaTheta;
	
	}
	/* 
	coords[nPoints - 2][0] = coords[0][0];
	coords[nPoints - 2][1] = coords[0][1];
	coords[nPoints - 2][2] = coords[0][2]; 

	coords[nPoints - 1][0] = coords[1][0];
	coords[nPoints - 1][1] = coords[1][1];
	coords[nPoints - 1][2] = coords[1][2]; */

	coords[0][0] = coords[nPoints - 2][0];
	coords[0][1] = coords[nPoints - 2][1];
	coords[0][2] = coords[nPoints - 2][2];

	coords[1][0] = coords[nPoints - 1][0];
	coords[1][1] = coords[nPoints - 1][1];
	coords[1][2] = coords[nPoints - 1][2];

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	
	glGenBuffers(2, vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), coords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

if (false)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, N_POINTS_AROUND_SLICE * sizeof(vec3), topCap, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(pvaLoc_mcNormal);
	glEnableVertexAttribArray(pvaLoc_mcPosition);
}
/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, N_POINTS_AROUND_SLICE * sizeof(vec3), topCap, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDisableVertexAttribArray(pvaLoc_mcNormal);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), bottomCap, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FALSE, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(pvaLoc_mcNormal); */

	delete [] coords;
	delete [] normals;
	delete [] topCap;
	
}

void Cylinder::renderCylinder(float *color)
{
	//std::cerr << "GOING TO RENDER CYLINDER" << std::endl;
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	glEnableVertexAttribArray(pvaLoc_mcNormal);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE));

	//std::cerr << "RENDERED THE EDGE OF THE CONE" << std::endl;

	if(false)
	{
		//std::cerr << "WE ARE DRAWING A CAPPED CONE!" << std::endl;
		//Draw the caps first
		glDisableVertexAttribArray(pvaLoc_mcNormal);
		glVertexAttrib3f(pvaLoc_mcNormal, topToBottom.dx, topToBottom.dy, topToBottom.dz);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N_POINTS_AROUND_SLICE);
	} 

}

	
void Cylinder::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);
	
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 	1, false, mc_ec.	extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 	1, false, ec_lds.	extractColMajor(mat));

	if (isATree)
	{
		vec3 newKa = {0.02, 0.02, 0.02};
		vec3 newKd = {0.01, 0.01, 0.01};
		vec3 newKs = {0.4, 0.4, 0.4};
		float newM = 10.0;

		ModelViewWithPhongLighting::sendToGPU(newKa, newKd, newKs, newM);
	}
	else
	{
		ModelViewWithPhongLighting::sendToGPU(Cylinder::ka, Cylinder::kd, Cylinder::ks, Cylinder::m);
	}
	
	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayCylFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderCylinder(bColor);
	}
	if (displayCylEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderCylinder(black);
	}
		
	glUseProgram(pgm);
}
