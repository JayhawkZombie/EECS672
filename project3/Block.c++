// Block.c++

#include <iostream>
#include <math.h>

#include "Block.h"
#include "ShaderIF.h"

typedef float vec3[3];

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Block::indexList[3][4] = {
	{ 6, 7, 0, 1 }, // xmin face
	{ 6, 0, 4, 2 }, // ymin face
	{ 1, 7, 3, 5 }  // ymax face
};

vec3 Block::ka = {0.24725, 0.2245, 0.0645};
vec3 Block::kd = {0.34615, 0.3143, 0.0903};
vec3 Block::ks = {0.797357, 0.723991, 0.208006};
float Block::m = 12.8;

Block::Block(float cx, float cy, float cz, float lx, float ly, float lz, float c[3], float alterVals[3]) :
	displayBlockEdges(false), displayBlockFill(true)
{
	colors[0] = c[0];
	colors[1] = c[1];
	colors[2] = c[2];
	xmin = cx; xmax = cx + lx;
	ymin = cy; ymax = cy + ly;
	zmin = cz; zmax = cz + lz;
	zmax *= 1.0;

	alterMinMaxReporting(alterVals[0], alterVals[1], alterVals[2]);

	defineBlock();
}

void Block::alterMinMaxReporting(float xFactor, float yFactor, float zFactor)
{
	alterValues[0] = xFactor;
	alterValues[1] = yFactor;
	alterValues[2] = zFactor;
}

Block::~Block()
{
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Block::defineBlock()
{
	vec3 vtx[] = { // The 8 unique vertices (Note the order)
		{xmin ,ymin, zmax}, {xmin, ymax, zmax},
		{xmax, ymin, zmax}, {xmax, ymax, zmax},
		{xmax, ymin, zmin}, {xmax, ymax, zmin},
		{xmin, ymin, zmin}, {xmin, ymax, zmin}
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glGenBuffers(3, ebo);
	for (int i=0 ; i<3 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Block::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = alterValues[0] * xmin;
	xyzLimits[1] = alterValues[0] * xmax;
	xyzLimits[2] = alterValues[1] * ymin;
	xyzLimits[3] = alterValues[1] * ymax;
	xyzLimits[4] = alterValues[2] * zmin;
	xyzLimits[5] = alterValues[2] * zmax;
}

void Block::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'b')
		displayBlockFill = !displayBlockFill;
	else if (key == 'B')
		displayBlockEdges = !displayBlockEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Block::renderBlock(float* color)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, -1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
}

void Block::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	ModelViewWithPhongLighting::sendToGPU(Block::ka, Block::kd, Block::ks, Block::m);

	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayBlockFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderBlock(bColor);
	}
	if (displayBlockEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderBlock(black);
	}

	glUseProgram(pgm);
}
