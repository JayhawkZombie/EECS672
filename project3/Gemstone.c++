#include <iostream>
#include <math.h>

#include "Gemstone.h"
#include "ShaderIF.h"
#include "ModelViewWithPhongLighting.h"


typedef float vec3[3];

/* 
| B1 - points[0]
| B2 - points[1]
| B3 - points[2]
| B4 - points[3]
| T  - points[4]
| B  - points[5]
*/

vec3 Gemstone::ka = { 0.135, 0.2225, 0.1575 };
vec3 Gemstone::kd = {0.54, 0.89, 0.63};
vec3 Gemstone::ks = {0.316228, 0.316228, 0.316228};
float Gemstone::m = 12.8;

GLuint Gemstone::indexList[7][3] = {
	{0, 1, 4} /* S1 */, {0, 3, 4} /* S2 */,
	{2, 1, 4} /* S4 */, {0, 1, 5} /* S5 */,
	{0, 3, 5} /* S6 */, {2, 3, 5} /* S7 */,
	{1, 2, 5} /* S8 */
};

Gemstone::Gemstone(float P1[3], float P2[3], float P3[3], float P4[3], float he, float c[]) :
	displayGemstoneEdges(false), displayGemstoneFill(true)
{
	//The 4 points we are initally give
	points[0] = cryph::AffPoint(P1);
	points[1] = cryph::AffPoint(P2);
	points[2] = cryph::AffPoint(P3);
	points[3] = cryph::AffPoint(P4);

	colors[0] = c[0];
	colors[1] = c[1];
	colors[2] = c[2];

	height = he;

	defineGemstone();

}

Gemstone::~Gemstone()
{
	glDeleteBuffers(7, ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Gemstone::defineGemstone()
{
	//Two points left to find - the top and bottom points

	cryph::AffVector v1(points[3] + points[0]);
	cryph::AffVector v2(points[1] + points[0]);

	upVector = v2.cross(v1);
	upVector.normalize();

	//Now we need to calculate the center of the base
	cryph::AffPoint center1 = 0.5 * cryph::AffPoint(points[0] + points[3]);
	cryph::AffPoint center2 = 0.5 * cryph::AffPoint(points[1] + points[2]);
	cryph::AffPoint center = 0.5 * (  center1 + center2  );

	//Top = center + height * up
	points[4] = center + height * upVector;
	points[5] = center - height * upVector;

	xmin = xmax = ymin = ymax = zmin = zmax = 0;

	for (int i = 0; i < 6; i++)
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

	zmax *= 1.2; //there seems to be some strange clipping if I don't adjust this here...
/*
	std::cerr << "Bounds: " << std::endl;
	std::cerr << "xmin: " << xmin << std::endl;
	std::cerr << "xmax: " << xmax << std::endl;
	std::cerr << "ymin: " << ymin << std::endl;
	std::cerr << "ymax: " << ymax << std::endl;
	std::cerr << "zmin: " << zmin << std::endl;
	std::cerr << "zmax: " << zmax << std::endl; */

	//Since we can only draw one face with glDrawArrays, we'll just put them into the VBO in order
	vec3 verts[] = {
		{points[0].x, points[0].y, points[0].z}, {points[1].x, points[1].y, points[1].z}, 
		{points[2].x, points[2].y, points[2].z}, {points[3].x, points[3].y, points[3].z}, 
		{points[4].x, points[4].y, points[4].z}, {points[5].x, points[5].y, points[5].z}
	};

/*
	std::cerr << "Points: " << std::endl;
	std::cerr << "B1: ( " << points[0].x << ", " << points[0].y << " , " << points[0].z << ") " << std::endl;
	std::cerr << "B2: ( " << points[1].x << ", " << points[1].y << " , " << points[1].z << ") " << std::endl;
	std::cerr << "B3: ( " << points[2].x << ", " << points[2].y << " , " << points[2].z << ") " << std::endl;
	std::cerr << "B4: ( " << points[3].x << ", " << points[3].y << " , " << points[3].z << ") " << std::endl;
	std::cerr << "T : ( " << points[4].x << ", " << points[4].y << " , " << points[4].z << ") " << std::endl;
	std::cerr << "B : ( " << points[5].x << ", " << points[5].y << " , " << points[5].z << ") " << std::endl; */

	//And now comes the fun part - making the normal vectors

	v1 = cryph::AffVector(points[0] - points[1]);
	v2 = cryph::AffVector(points[4] - points[1]);
	normals[0] = v1.cross(v2);	normals[0].normalize();

	v1 = cryph::AffVector(points[3] - points[0]);
	v2 = cryph::AffVector(points[4] - points[0]);
	normals[1] = v1.cross(v2);	normals[1].normalize();

	v1 = cryph::AffVector(points[2] - points[3]);
	v2 = cryph::AffVector(points[4] - points[3]);
	normals[2] = v1.cross(v2);	normals[2].normalize();

	v1 = cryph::AffVector(points[1] - points[2]);
	v2 = cryph::AffVector(points[4] - points[2]);
	normals[3] = v1.cross(v2);	normals[3].normalize();

	v1 = cryph::AffVector(points[1] - points[0]);
	v2 = cryph::AffVector(points[5] - points[0]);
	normals[4] = v1.cross(v2);	normals[4].normalize();

	v1 = cryph::AffVector(points[0] - points[3]);
	v2 = cryph::AffVector(points[5] - points[3]);
	normals[5] = v1.cross(v2);	normals[5].normalize();

	v1 = cryph::AffVector(points[3] - points[2]);
	v2 = cryph::AffVector(points[5] - points[1]);
	normals[6] = v1.cross(v2);	normals[5].normalize();

	v1 = cryph::AffVector(points[2] - points[1]);
	v2 = cryph::AffVector(points[5] - points[1]);
	normals[7] = v1.cross(v2);	normals[6].normalize();

	//Generate the vao and bind
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//We have 6 vertices, so tell that to OpenGL
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(vec3), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glGenBuffers(7, ebo);

	for (int i = 0; i < 7; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	//Disable the pva for normals - we will set them manually
	glDisableVertexAttribArray(pvaLoc_mcNormal);

}

void Gemstone::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Gemstone::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'b')
		displayGemstoneFill = !displayGemstoneFill;
	else if (key == 'B')
		displayGemstoneEdges = !displayGemstoneEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Gemstone::renderGemstone(float* color)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	// The one faces that can be drawn with glDrawArrays (S3 - going counterclockwise from the leftmost top face)
	glVertexAttrib3f(pvaLoc_mcNormal, normals[2].dx, normals[2].dy, normals[2].dz);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	// The 7 faces that are drawn with glDrawElements

	//S1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[0].dx, normals[0].dy, normals[0].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//S2
	glVertexAttrib3f(pvaLoc_mcNormal, normals[1].dx, normals[1].dy, normals[1].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//S4
	glVertexAttrib3f(pvaLoc_mcNormal, normals[3].dx, normals[3].dy, normals[3].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);

	//S5
	glVertexAttrib3f(pvaLoc_mcNormal, normals[4].dx, normals[4].dy, normals[4].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);

	//S6
	glVertexAttrib3f(pvaLoc_mcNormal, normals[5].dx, normals[5].dy, normals[5].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[4]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);

	//S7
	glVertexAttrib3f(pvaLoc_mcNormal, normals[6].dx, normals[6].dy, normals[6].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);

	//S8
	glVertexAttrib3f(pvaLoc_mcNormal, normals[7].dx, normals[7].dy, normals[7].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[6]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);


}

void Gemstone::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	ModelViewWithPhongLighting::sendToGPU(Gemstone::ka, Gemstone::kd, Gemstone::ks, Gemstone::m);

	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayGemstoneFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderGemstone(bColor);
	}
	if (displayGemstoneEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderGemstone(black);
	}

	glUseProgram(pgm);

}

/* 
| B1 - points[0]
| B2 - points[1]
| B3 - points[2]
| B4 - points[3]
| T  - points[4]
| B  - points[5]
*/
