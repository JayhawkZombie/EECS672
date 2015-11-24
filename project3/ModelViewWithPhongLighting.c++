// ModelViewWithPhongLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithPhongLighting.h"
#include "ShaderIF.h"

typedef float vec3[3];
typedef float vec4[4];

ShaderIF* ModelViewWithPhongLighting::shaderIF = NULL;
int ModelViewWithPhongLighting::numInstances = 0;
GLuint ModelViewWithPhongLighting::shaderProgram = 0;
GLint ModelViewWithPhongLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithPhongLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;

GLint ModelViewWithPhongLighting::ppuLoc_ambient = -2;

vec3 ModelViewWithPhongLighting::ambientLight = { 0.15, 0.15, 0.15 };

int ModelViewWithPhongLighting::numLights = NUM_LIGHTS;
GLint ModelViewWithPhongLighting::ppuLoc_numLights;

GLint ModelViewWithPhongLighting::ppuLoc_ka = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ks = -2;
GLint ModelViewWithPhongLighting::ppuLoc_m = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightSources = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightStrengths = -2;

vec3 ModelViewWithPhongLighting::ka = {0.24725, 0.2245, 0.0645};
vec3 ModelViewWithPhongLighting::kd = {0.34615, 0.3143, 0.0903};
vec3 ModelViewWithPhongLighting::ks = {0.797357, 0.723991, 0.208006};
float ModelViewWithPhongLighting::m = 12.8;

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

ModelViewWithPhongLighting::ModelViewWithPhongLighting()
{
	if (shaderProgram == 0)
	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;


	//set sources and strengths
	//have NUM_LIGHTS of them

	sources[0][0] = 0.0f;		sources[0][1] = 0.0f; 		sources[0][2] = 1.0f;  		sources[0][3] = 0.0f;
	sources[1][0] = -300.0f; 	sources[1][1] = 600.0f; 	sources[1][2] = 200.0f;  	sources[1][3] = 1.0f;
	sources[2][0] = -300.0f;		sources[2][1] = -200.0f; 	sources[2][2] = 50.0f;  	sources[2][3] = 1.0f;

	strengths[0][0] = 0.5f;			strengths[0][1] = 0.5f;			strengths[0][2] = 0.5f;
	strengths[1][0] = 255 / 255.0f;	strengths[1][1] = 131 / 255.0f;	strengths[1][2] = 8 / 255.0f;
	//strengths[1][0] = 0 / 255.0f;	strengths[1][1] = 0 / 255.0f;	strengths[1][2] = 0 / 255.0f;
	strengths[2][0] = 0 / 255.0f;	strengths[2][1] = 204 / 255.0f;	strengths[2][2] = 109 / 255.0f;


}

ModelViewWithPhongLighting::~ModelViewWithPhongLighting()
{
	if (--numInstances == 0)
	{
		shaderIF->destroy();
		delete shaderIF;
		shaderIF = NULL;
		shaderProgram = 0;
	}
}

void ModelViewWithPhongLighting::fetchGLSLVariableLocations()
{
	if (shaderProgram > 0)
	{
		pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		pvaLoc_mcNormal = pvAttribLocation(shaderProgram, "mcNormal");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");

		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
		ppuLoc_ks = ppUniformLocation(shaderProgram, "ks");
		ppuLoc_ambient = ppUniformLocation(shaderProgram, "ambientLight");
		ppuLoc_m = ppUniformLocation(shaderProgram, "m");
		ppuLoc_lightSources = ppUniformLocation(shaderProgram, "sources");
		ppuLoc_lightStrengths = ppUniformLocation(shaderProgram, "strengths");
		ppuLoc_numLights = ppUniformLocation(shaderProgram, "numLights");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::sendToGPU(const vec3 &_ka, const vec3 &_kd, const vec3 &_ks, const float _m)
{
	//So, we need to send it down to the GPU
	//We haven't really talked much about doing this
	//Sigh, let's try converting them to eye coordinates

	float pos[ 4 * NUM_LIGHTS];
	float str[ 3 * NUM_LIGHTS];

	for ( int i = 0; i < NUM_LIGHTS; i++)
	{

		pos[4 * i] = sources[i][0];
		pos[4 * i + 1] = sources[i][1];
		pos[4 * i + 2] = sources[i][2];
		pos[4 * i + 3] = sources[i][3]; 

		str[3 * i] = strengths[i][0];
		str[3 * i + 1] = strengths[i][1];
		str[3 * i + 2] = strengths[i][2]; 

	}

	glUniform4fv(ppuLoc_lightSources, NUM_LIGHTS, pos);
	glUniform3fv(ppuLoc_lightStrengths, NUM_LIGHTS, str);
	glUniform1i(ppuLoc_numLights, NUM_LIGHTS);
	glUniform3fv(ppuLoc_ambient, 1, ambientLight);

	glUniform3fv(ppuLoc_ka, 1, _ka);
	glUniform3fv(ppuLoc_kd, 1, _kd);
	glUniform3fv(ppuLoc_ks, 1, _ks);
	glUniform1f(ppuLoc_m, _m);


}
