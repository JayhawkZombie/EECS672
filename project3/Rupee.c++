#include <iostream>
#include <math.h>

#include "Rupee.h"
#include "ShaderIF.h"
#include "ModelViewWithPhongLighting.h"


typedef float vec3[3];

/* 
| P1 - points[0]
| P2 - points[1]
| P3 - points[2]
| P4 - points[3]
| T  - points[4]
| B  - points[5]
| V1 - points[6]
| V2 - points[7]
*/

GLuint Rupee::indexList[12][3] = {
	{4, 1, 6} /* S1  */, {1, 0, 6} /* S2  */,
	{0, 5, 6} /* S3  */, {5, 3, 6} /* S4  */,
	{2, 3, 6} /* S5  */, {4, 2, 6} /* S6  */,
	{4, 1, 7} /* S7  */, {1, 0, 7} /* S8  */,
	{0, 5, 7} /* S9  */, {5, 3, 7} /* S10 */,
	{2, 3, 7} /* S11 */, {4, 2, 7} /* S12 */
};

vec3 Rupee::ka = { 0.1745, 0.01175, 0.01175};
vec3 Rupee::kd = { 0.61424, 0.04136, 0.04136};
vec3 Rupee::ks = { 0.727811, 0.626959, 0.626959};
float Rupee::m = 76.8;

Rupee::Rupee(float P1[3], float P2[3], float P3[3], float P4[3], float c[]) :
	displayRupeeEdges(false), displayRupeeFill(true)
{
	points[0] = cryph::AffPoint(P1);
	points[1] = cryph::AffPoint(P2);
	points[2] = cryph::AffPoint(P3);
	points[3] = cryph::AffPoint(P4);

	colors[0] = c[0];
	colors[1] = c[1];
	colors[2] = c[2];

	//The height and normals will be determined later

	defineRupee();
}

Rupee::~Rupee()
{
	glDeleteBuffers(12, ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Rupee::defineRupee()
{
	//Alright, there's a lot of work to do
	//First, let's get the center point on the plane

	cryph::AffPoint c1 = 0.5 * (points[1] + points[2]);
	cryph::AffPoint c2 = 0.5 * (points[0] + points[3]);

	cryph::AffPoint centerOfPlane = 0.5 * (c1 + c2);

	upVectorParallelToPlane = cryph::AffVector(points[1] - points[0]);
	upVectorParallelToPlane.normalize();

	//Now we need a vector parallel to the plane  to get the Top and Bottom points
	cryph::AffVector v1(points[3] - points[0]);
	cryph::AffVector v2(points[1] - points[0]);
	//we also now need a vector perpendicular to the plane
	upVectorPerpToPlane = v1.cross(v2);
	upVectorPerpToPlane.normalize();

	//What is the distance between the two points on each side of the rupee?

	double distanceBetweenTopPoints = c1.distanceTo(c2);
	height = distanceBetweenTopPoints / 2;

	//Now we can get the Top and Bottom points of the rupee
	points[4] = c1 + height * upVectorParallelToPlane;
	points[5] = c2 - height * upVectorParallelToPlane;

	//Now how thick is the rupee?
	thickness = height / 2; //We'll see how it looks - may tweak later

	//Now we can get the two points jutting out of the rupee
	points[6] = centerOfPlane + thickness * upVectorPerpToPlane;
	points[7] = centerOfPlane - thickness * upVectorPerpToPlane;

	std::cerr << "Thickness: " << thickness;

	//Let's get xmin, xmax, etc...

	xmin = xmax = ymin = ymax = zmin = zmax = 0;

	for (int i = 0; i < 12; i++)
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

	zmax *= 1.2;

	//Alright...Now we have all 8 points, but now we need to compute 12 vectors!

	//Let's get started...
	//First face (top of rupee, top left hand corner) = (top - V1) cross (P2 - V1)
	v1 = cryph::AffVector(points[4] - points[6]);
	v2 = cryph::AffVector(points[1] - points[6]);
	normals[0] = v1.cross(v2);	normals[0].normalize();

	//Second face - directly below first on the same side
	v1 = cryph::AffVector(points[1] - points[6]);
	v2 = cryph::AffVector(points[0] - points[6]);
	normals[1] = v1.cross(v2);	normals[1].normalize();

	//Third face - bottom left corner of the front face
	v1 = cryph::AffVector(points[0] - points[6]);
	v2 = cryph::AffVector(points[5] - points[6]);
	normals[2] = v1.cross(v2);	normals[2].normalize();

	//Fourth face - bottom right corner of the front face
	v1 = cryph::AffVector(points[5] - points[6]);
	v2 = cryph::AffVector(points[3] - points[6]);
	normals[3] = v1.cross(v2);	normals[3].normalize();

	//Fifth face - middle of the right side of the front face
	v1 = cryph::AffVector(points[3] - points[6]);
	v2 = cryph::AffVector(points[2] - points[6]);
	normals[4] = v1.cross(v2);	normals[4].normalize();

	//Sixth face - top right corner of the front face
	v1 = cryph::AffVector(points[2] - points[6]);
	v2 = cryph::AffVector(points[4] - points[6]);
	normals[5] = v1.cross(v2);	normals[5].normalize();

	//Seventh face - directly BEHIND face 1 = (P2 - V2) cross (T - V2)
	v1 = cryph::AffVector(points[1] - points[7]);
	v2 = cryph::AffVector(points[4] - points[7]);
	normals[6] = v2.cross(v1);	normals[6].normalize();

	//Eighth face - directly behind face 2 = (P1 - V2) cross (P2 - V2)
	v1 = cryph::AffVector(points[0] - points[7]);
	v2 = cryph::AffVector(points[1] - points[7]);
	normals[7] = v2.cross(v1);	normals[7].normalize();

	//Ninth face - directly behind face 3 = (B - V2) cross (P1 - V2)
	v1 = cryph::AffVector(points[5] - points[7]);
	v2 = cryph::AffVector(points[0] - points[7]);
	normals[8] = v2.cross(v1);	normals[8].normalize();

	//Tenth face - directly behind face 4 = (P4 - V2) cross (B - V2)
	v1 = cryph::AffVector(points[3] - points[7]);
	v2 = cryph::AffVector(points[5] - points[7]);
	normals[9] = v2.cross(v1);	normals[9].normalize();

	//Eleventh face - directly behind face 5 = (P3 - V2) cross (P4 - V2)
	v1 = cryph::AffVector(points[2] - points[7]);
	v2 = cryph::AffVector(points[3] - points[7]);
	normals[10] = v2.cross(v1);	normals[10].normalize();

	//Face 12 - directly behind face 6 = (T - V2) cross (P3 - V2)
	v1 = cryph::AffVector(points[5] - points[7]);
	v2 = cryph::AffVector(points[2] - points[7]);
	normals[11] = v2.cross(v1);	normals[11].normalize();

	vec3 verts[] = {
		{points[0].x, points[0].y, points[0].z}, {points[1].x, points[1].y, points[1].z}, {points[2].x, points[2].y, points[2].z},
		{points[3].x, points[3].y, points[3].z}, {points[4].x, points[4].y, points[4].z}, {points[5].x, points[5].y, points[5].z},
		{points[6].x, points[6].y, points[6].z}, {points[7].x, points[7].y, points[7].z}
	};

	//Generate the vao and bind
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	//We have 6 vertices, so tell that to OpenGL
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glGenBuffers(12, ebo);

	for (int i = 0; i < 12; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	//Disable the pva for normals - we will set them manually
	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

void Rupee::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Rupee::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'b')
		displayRupeeFill = !displayRupeeFill;
	else if (key == 'B')
		displayRupeeEdges = !displayRupeeEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Rupee::renderRupee(float* color)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	// The 12 faces that are drawn with glDrawElements

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[0].dx, normals[0].dy, normals[0].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[1].dx, normals[1].dy, normals[1].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[2].dx, normals[2].dy, normals[2].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[3].dx, normals[3].dy, normals[3].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[4].dx, normals[4].dy, normals[4].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[4]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[5].dx, normals[5].dy, normals[5].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[6].dx, normals[6].dy, normals[6].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[6]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[7].dx, normals[7].dy, normals[7].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[7]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[8].dx, normals[8].dy, normals[8].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[8]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[9].dx, normals[9].dy, normals[9].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[9]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[10].dx, normals[10].dy, normals[10].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[10]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	//Front 1
	glVertexAttrib3f(pvaLoc_mcNormal, normals[11].dx, normals[11].dy, normals[11].dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[11]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);


}

void Rupee::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	ModelViewWithPhongLighting::sendToGPU(Rupee::ka, Rupee::kd, Rupee::ks, Rupee::m);

	float black[] = { 0.0, 0.0, 0.0 };
	float bColor[] = { 1.0, 0.0, 1.0 };
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];

	if (displayRupeeFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderRupee(bColor);
	}
	if (displayRupeeEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderRupee(black);
	}

	glUseProgram(pgm);

}

/* 
| P1 - points[0]
| P2 - points[1]
| P2 - points[2]
| P2 - points[3]
| T  - points[4]
| B  - points[5]
| V1 - points[6]
| V2 - points[7]
*/
