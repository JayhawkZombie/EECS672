#include <iostream>
#include <math.h>

#include "Pyramid.h"
#include "ShaderIF.h"
#include "ModelViewWithPhongLighting.h"

/* 
| B1 - points[0]
| B2 - points[1]
| B3 - points[2]
| B4 - points[3]
| T  - points[4]
*/


//VBO ORDER
/*
| B1 (0)
| B4 (1)
| B2 (2)
| B3 (3)
| T  (4)
*/

typedef float vec3[3];

GLuint Pyramid::indexList[3][3] = {
	{0, 2, 4}, {0, 1, 4}, {1, 3, 4}
};

Pyramid::Pyramid(float P1[3], float P2[3], float P3[3], float P4[3], float he, float c[]) :
	displayPyramidEdges(false), displayPyramidFill(true)
{
	colors[0] = c[0];
	colors[1] = c[1];
	colors[1] = c[2];

	height = he;

	points[0] = cryph::AffPoint(P1);
	points[1] = cryph::AffPoint(P2);
	points[2] = cryph::AffPoint(P3);
	points[3] = cryph::AffPoint(P4);

	definePyramid();

}

Pyramid::~Pyramid()
{
	glDeleteBuffers(3, ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Pyramid::definePyramid()
{

	//There is one point left to find - the top point - but we need an UP vector

	cryph::AffVector v1(points[2] - points[3]);
	cryph::AffVector v2(points[0] - points[3]);

	upVector = v2.cross(v1);
	upVector.normalize();

	//Calculate the center point of the base of the pyramid
	cryph::AffPoint center1 = 0.5 * cryph::AffPoint(points[0] + points[3]);
	cryph::AffPoint center2 = 0.5 * cryph::AffPoint(points[1] + points[2]);
	cryph::AffPoint center = 0.5 * (  center1 + center2  );

	//Calculate point at top of pyramid
	points[4] = center + height * upVector; 

	//Now let's get xmin, ymin, etc

	xmin = xmax = ymin = ymax = zmin = zmax = 0;

	for (int i = 0; i < 5; i++)
	{
		if (points[i].x < xmin)
			xmin = points[i].x;
		if (points[i].x > xmax)
			xmax = points[i].x;
		if (points[i].y < ymin)
			ymin = points[i].y;
		if (points[i].y > ymax)
			ymax = points[i].y;
		if (points[i].z < zmin)
			zmin = points[i].z;
		if (points[i].z > zmax)
			zmax = points[i].z;
	}

	//Our VBO ordering is NOT in numerical order
	//VBO ORDER
	/*
	| B1 (0)
	| B4 (1)
	| B2 (2)
	| B3 (3)
	| T  (4)
	*/
	vec3 verts[] = {
		{points[0].x, points[0].y, points[0].z}, {points[3].x, points[3].y, points[3].z},
		{points[1].x, points[1].y, points[1].z}, {points[2].x, points[2].y, points[2].z},
		{points[4].x, points[4].y, points[4].z}
	};
/*
	std::cerr << "Points: " << std::endl;
	std::cerr << "B1: ( " << points[0].x << ", " << points[0].y << " , " << points[0].z << ") " << std::endl;
	std::cerr << "B2: ( " << points[1].x << ", " << points[1].y << " , " << points[1].z << ") " << std::endl;
	std::cerr << "B3: ( " << points[2].x << ", " << points[2].y << " , " << points[2].z << ") " << std::endl;
	std::cerr << "B4: ( " << points[3].x << ", " << points[3].y << " , " << points[3].z << ") " << std::endl;
	std::cerr << "T : ( " << points[4].x << ", " << points[4].y << " , " << points[4].z << ") " << std::endl; */

	//Now let's calculate our normal vectors
	//They will be defined as such: BOTTOM, S1, S2, S3, S4

	//Normal for bottom face (Rectangular bottom face)

	normals[0] = -1 * upVector;

	//S1 normal vector
	v1 = cryph::AffVector(points[0] - points[1]);
	v2 = cryph::AffVector(points[4] - points[1]);
	normals[1] = v1.cross(v2);	normals[1].normalize();

	//S2 normal vector
	v1 = cryph::AffVector(points[3] - points[0]);
	v2 = cryph::AffVector(points[4] - points[0]);
	normals[2] = v1.cross(v2);	normals[2].normalize();

	//S3 normal vector
	v1 = cryph::AffVector(points[2] - points[3]);
	v2 = cryph::AffVector(points[4] - points[3]);
	normals[3] = v1.cross(v2);	normals[3].normalize();

	//S4 normal vector
	v1 = cryph::AffVector(points[1] - points[2]);
	v2 = cryph::AffVector(points[4] - points[2]);
	normals[4] = v1.cross(v2);	normals[4].normalize();


	//Generate the vao and bind
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//We have 5 vertices, so tell that to OpenGL
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(vec3), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glGenBuffers(3, ebo);

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	//Disable the pva for normals - we will set them manually
	glDisableVertexAttribArray(pvaLoc_mcNormal);

}

/* 
| B1 - points[0]
| B2 - points[1]
| B3 - points[2]
| B4 - points[3]
| T  - points[4]
*/

void Pyramid::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Pyramid::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'b')
		displayPyramidFill = !displayPyramidFill;
	else if (key == 'B')
		displayPyramidEdges = !displayPyramidEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Pyramid::renderPyramid(float* color)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	// The two faces that can be drawn with glDrawArrays
	glVertexAttrib3f(pvaLoc_mcNormal, normals[0].dx, normals[0].dy, normals[0].dz);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, normals[4].dx, normals[4].dy, normals[4].dz);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	// The three faces that are drawn with glDrawElements

	//S1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[1].dx, normals[1].dy, normals[1].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//S2
	glVertexAttrib3f(pvaLoc_mcNormal, normals[2].dx, normals[2].dy, normals[2].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//S3
	glVertexAttrib3f(pvaLoc_mcNormal, normals[3].dx, normals[3].dy, normals[3].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);
}

void Pyramid::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayPyramidFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderPyramid(bColor);
	}
	if (displayPyramidEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderPyramid(black);
	}

	glUseProgram(pgm);

}
