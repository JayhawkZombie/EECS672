#include "Cylinder.h"
#include "ShaderIF.h"
	
cryph::AffVector dir;

#define N_POINTS_AROUND_SLICE 20

typedef float vec3[3];
	
Cylinder::Cylinder(float rad, const cryph::AffPoint& b, const cryph::AffPoint& t, float inColor[3])
{
	radius 			= rad;
	CylinderBottom 	= b;
	CylinderTop		= t;
	dir 			= (CylinderTop - CylinderBottom);
	dir.normalize();
	
	defineCylinder();

	color[0] 		= inColor[0];
	color[1]		= inColor[1];
	color[2]		= inColor[2];
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
	
void Cylinder::defineCylinder()
{
	bounds[0] = CylinderBottom.x 	- radius;
	bounds[1] = CylinderBottom.x 	+ radius;
	bounds[2] = CylinderBottom.y 	- radius;
	bounds[3] = CylinderTop.y 		+ radius;
	bounds[4] = CylinderBottom.z 	- radius;
	bounds[5] = CylinderBottom.z 	+ radius;


	double 	theta 	= 0.0;
	double 	deltaTheta 	= 2.0 * M_PI / N_POINTS_AROUND_SLICE;
	
	int 	nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
	vec3* 	coords 	= new vec3[nPoints];
	vec3* 	normals	= new vec3[nPoints];
	
	
	
	cryph::AffVector point 			= cryph::AffVector(dir.dx - 1, dir.dy, dir.dz + 1).cross(dir);
	cryph::AffPoint  b 				= CylinderBottom 	+ radius * point;
	cryph::AffPoint  t 				= CylinderTop 		+ radius * point;
			
	cryph::AffVector bPerpendicular = (b - CylinderBottom).cross(t - b).cross(t - b);
	
	
	for (int i=0 ; i <= N_POINTS_AROUND_SLICE ; i++)
	{


		/* Set the values for the new coordinates after moving to the next point */
		coords[2 * i][0] 		= b.x; 
		coords[2 * i][1] 		= b.y; 
		coords[2 * i][2] 		= b.z;
		coords[2 * i + 1][0] 	= t.x; 
		coords[2 * i + 1][1] 	= t.y; 
		coords[2 * i + 1][2] 	= t.z;

		/* Set the values for the normal vectors */		
		normals[2 * i][0] 		= bPerpendicular.dx; 
		normals[2 * i][1] 		= bPerpendicular.dy; 
		normals[2 * i][2] 		= bPerpendicular.dz;
		
		normals[2 * i + 1][0] 	= bPerpendicular.dx; 
		normals[2 * i + 1][1] 	= bPerpendicular.dy; 
		normals[2 * i + 1][2] 	= bPerpendicular.dz;

		b 		= CylinderBottom 	+ ((radius * cos(deltaTheta)) * point + ((radius * sin(deltaTheta)) * dir.cross(point)));
		t 		= CylinderTop 		+ ((radius * cos(deltaTheta)) * point + ((radius * sin(deltaTheta)) * dir.cross(point)));
		point 	= (b - CylinderBottom);
		point.normalize();
		
		cryph::AffVector helperV = (b - CylinderBottom).cross(t - b);
		bPerpendicular = helperV.cross(t - b);
	
	}


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
	
	delete [] coords;
	delete [] normals;
	
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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));
		
	glUseProgram(pgm);
}
