// ModelViewWithPhongLighting.h

#ifndef MODELVIEWWITHPHONGLIGHTING_H
#define MODELVIEWWITHPHONGLIGHTING_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"

#define NUM_LIGHTS 3

typedef float vec3[3];
typedef float vec4[4];

class ModelViewWithPhongLighting : public ModelView
{
public:
	ModelViewWithPhongLighting();
	virtual ~ModelViewWithPhongLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);

	static float globalXRot;
	static float globalYRot;

	void sendToGPU(const vec3 &ka, const vec3 &kd, const vec3 &ks, const float m);

	static vec3 ka;
	static vec3 ks;
	static vec3 kd;
	static float m;

protected:

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_kd;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;

	//Variables for lighting and their associated storage elements
	static GLint ppuLoc_ambient;
	static vec3 ambientLight;

	static GLint ppuLoc_ka;
	static GLint ppuLoc_ks;
	static GLint ppuLoc_m;
	static GLint ppuLoc_lightSources;
	vec4 sources[NUM_LIGHTS];
	static GLint ppuLoc_lightStrengths;
	vec3 strengths[NUM_LIGHTS];

	static GLint ppuLoc_numLights;

	static int numLights; //Does not include ambient lights


	static void fetchGLSLVariableLocations();



private:
	static std::string vShaderSource, fShaderSource;
};

#endif
