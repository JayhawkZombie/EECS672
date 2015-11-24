// Wedge.c++

#include <iostream>
#include <math.h>

#include "Wedge.h"
#include "CustomGLFWController.h"
#include "ShaderIF.h"

typedef float vec3[3];
typedef float vec4[4];

// index lists for the three faces that can't be drawn with glDrawArrays
// there are 3 faces that we cant draw
GLuint Wedge::indexList[2][3] = {
	{ 0, 2, 4 }, // FRONT FACE
	{ 1, 3, 5 } // BACK FACE
};
GLuint Wedge::indexforBottomFace[1][4] = {
	{0, 1, 4, 5}
};

vec3 Wedge::ka = {0.24725, 0.2245, 0.0645};
vec3 Wedge::kd = {0.34615, 0.3143, 0.0903};
vec3 Wedge::ks = {0.797357, 0.723991, 0.208006};
float Wedge::m = 12.8;

Wedge::Wedge(float P1[3], float P2[3], float P3[3], float de,  float c[]) :
	displayWedgeEdges(false), displayWedgeFill(true)
{
	colors[0] = c[0];
	colors[1] = c[1];
	colors[2] = c[2];

	depth = de;

	//We will determine the min/max when we are defining the points in defineWedge()...

	points[0] = cryph::AffPoint(P1);
	points[1] = cryph::AffPoint(P2);
	points[2] = cryph::AffPoint(P3);

	defineWedge();
}

Wedge::~Wedge()
{
	glDeleteBuffers(2, ebo);
	glDeleteBuffers(1, eboBottom);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Wedge::defineWedge()
{
	//Alright, so we need to determinet the last 3 points
	//We will ASSUME the points were given in the constructor as <BOTTOM_ELFT>, <TOP>, <BOTTOM_RIGHT>

	//So we need a vector going backwards
	//backVector = (BOTTOM_RIGHT - TOP) X (BOTTOM_LEFT - TOP)

	cryph::AffVector BRT = points[2] - points[1];
	cryph::AffVector BLT = points[0] - points[1];

	backVector = BLT.cross(BRT);

	/* OH MY GOD - NORMALIZE YOUR FREAKING VECTORS */
	/* I get some obviously unwanted results if I don't normalize (like xmax = 760,000) */ 
	backVector.normalize();


	//Now we need to find the other 3 points...

	//The points will be defined as follow <BackBottomLeft = points[3], BackTop = points[4], BackBottomRight = points[5]>
	points[3] = points[0] + depth * backVector;
	points[4] = points[1] + depth * backVector;
	points[5] = points[2] + depth * backVector;
	/*
	| points[0] (F1)
	| points[3] (B3) (BACK TOP)
	| points[1] (F3) (FRONT TOP)
	| points[4] (B1)
	| points[2] (F2)
	| points[5] (B2)
	*/

	vec3 verts[] = { //The 6 vertices - defined as frontFace then backFace
		{points[0].x, points[0].y, points[0].z}, {points[3].x, points[3].y, points[3].z}, {points[1].x, points[1].y, points[1].z},
		{points[4].x, points[4].y, points[4].z}, {points[2].x, points[2].y, points[2].z}, {points[5].x, points[5].y, points[5].z}
	};

	//Now that we have all the points, we need to compute the normal vectors

	//normal for front face = -backVector
	normals[0] = -backVector;
	normals[0].normalize();

	//normal for back face = backVector
	normals[4] = backVector;
	normals[4].normalize();

	//normal for bottom face = (B2 - F2) cross (F1 - F2)
	cryph::AffVector pt4normal1, pt4normal2;
	pt4normal1 = (points[5] - points[2]);
	pt4normal2 = (points[0] - points[2]);
	normals[3] = pt4normal1.cross(pt4normal2);
	normals[3].normalize();

	//normal for left face = (F3 - F1) cross (F3 - B3)
	pt4normal1 = (points[1] - points[0]);
	pt4normal2 = (points[1] - points[4]);
	normals[1] = pt4normal1.cross(pt4normal2);
	normals[1].normalize();

	//normal for right face = (F2 - F3) cross (F2 - B2)
	pt4normal1 = (points[2] - points[1]);
	pt4normal2 = (points[2] - points[5]);
	normals[2] = pt4normal1.cross(pt4normal2);
	normals[2].normalize();

	//hmm, so we need to find xmin, xmax, ymin, ymax, zmin, zmax
	xmin = xmax = ymin = ymax = zmin = zmax = 0.0f;

	for (int pointIndex = 0; pointIndex < 6; pointIndex++)
	{
		if (points[pointIndex].x < xmin)
			xmin = points[pointIndex].x;
		if (points[pointIndex].x > xmax)
			xmax = points[pointIndex].x;
		if (points[pointIndex].y < ymin)
			ymin = points[pointIndex].y;
		if (points[pointIndex].y > ymax)
			ymax = points[pointIndex].y;
		if (points[pointIndex].z < zmin)
			zmin = points[pointIndex].z;
		if (points[pointIndex].z > zmax)
			zmax = points[pointIndex].z;
	}

	//Generate the vao and bind
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//We have 6 vertices, so tell that to OpenGL
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(vec3), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	//Bind the buffers for the 2 triangle ebos (they each hold 3 indices for each point)
	glGenBuffers(2, ebo);
	for (int i=0 ; i<2 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	//Generate and bind for the one ebo that will have a rectangle (the bottom face)
	glGenBuffers(1, eboBottom);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBottom[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexforBottomFace[0], GL_STATIC_DRAW);

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Wedge::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Wedge::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'b')
		displayWedgeFill = !displayWedgeFill;
	else if (key == 'B')
		displayWedgeEdges = !displayWedgeEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Wedge::renderWedge(float* color)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	// The two faces that can be drawn with glDrawArrays
	glVertexAttrib3f(pvaLoc_mcNormal, normals[1].dx, normals[1].dy, normals[1].dz);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, normals[2].dx, normals[2].dy, normals[2].dz);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);

	// The three faces that are drawn with glDrawElements

	//Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, normals[0].dx, normals[0].dy, normals[0].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Back face
	glVertexAttrib3f(pvaLoc_mcNormal, normals[4].dx, normals[4].dy, normals[5].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Bottom face (the rectangle)
	glVertexAttrib3f(pvaLoc_mcNormal, normals[3].dx, normals[3].dy, normals[3].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBottom[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
}

void Wedge::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	ModelViewWithPhongLighting::sendToGPU(Wedge::ka, Wedge::kd, Wedge::ks, Wedge::m);

	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayWedgeFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderWedge(bColor);
	}
	if (displayWedgeEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderWedge(black);
	}

	glUseProgram(pgm);

}
