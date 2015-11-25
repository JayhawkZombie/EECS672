#version 410 core

#define NUM_LIGHTS 3

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

// Naming convention for variables holding coordinates:
// mc - model coordinates
// ec - eye coordinates
// lds - logical device space
// "p_" prefix on any of the preceding indicates the coordinates have been
//      embedded in projective space
// (gl_Position would be called something like: p_ldsPosition)

// Transformation Matrices
uniform mat4 mc_ec =   // (dynamic rotations) * (ViewOrientation(E,C,up))
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> no dynamic rotations -AND- MC = EC
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
uniform mat4 ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
             
// There are MANY ways to deal with the basic object color.
// For now we will  simply assume:
//uniform vec3 kd = // "kd" - diffuse reflectivity; basic object color
	//vec3(0.8, 0.0, 0.0); // default: darkish red

// There are also MANY ways to deal with light sources (number, type,
// strength, etc.).  For now we simply assume one directional source.
// You will generalize this in future projects.
uniform vec4 p_ecLightSource = vec4(0.7, 0.2, 1.0, 0.0);

// Per-vertex attributes
// 1. incoming vertex position in model coordinates

layout (location = 0) in vec3 mcPosition;

// 2. incoming vertex normal vector in model coordinates

in vec3 mcNormal; // incoming normal vector in model coordinates


//I need this space - too much to read above



uniform vec3 kd = // "kd" - diffuse reflectivity; basic object color
	vec3(0.8, 0.0, 0.0); // default: darkish red

uniform vec3 ka = vec3(0.0215, 0.1745, 0.0215);
uniform vec3 ks = vec3(0.0215, 0.1745, 0.0215);

uniform vec4 sources[NUM_LIGHTS];// = vec4(1.0, 1.0, 1.0, 0.0);

uniform vec3 strengths[NUM_LIGHTS];// = vec3(1.0, 1.0, 1.0);

uniform vec3 ambientLight = vec3(0.8, 0.8, 0.8);

uniform float m = 20.0f;

uniform int numLights = 1;


float attenuation = 0.0f;
float oneOverAttenuation = 1.0f;
float maxDistanceForLocalLight = 10000;


out vec3 colorToFS;

vec3 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	// Simplistic lighting model:
	// Reflected color will be a function of angle between the normal
	// vector and the light source direction. Specifically:

	// NOTE: The use of "abs" here is a temporary hack. As we study
	//       lighting models more carefully, we will REMOVE "abs" while
	//       incorporating more sophisticated processing.

	vec3 Iq = vec3(0.0, 0.0, 0.0);

	Iq += ka * ambientLight;


	//Need a vector from the point to the eye
	//Am I calculating this correctly?
	vec3 ec_v = vec3(0.0, 0.0, 1.0);

	if (ec_lds[3][3] != 0.0f)
		ec_v = -normalize(ec_Q);
	else if (ec_lds[1][0] != 0.0f && ec_lds[2][0] != 0.0f)
		ec_v = vec3(0.0, 0.0, 1.0); 

	//Normalize
	ec_v = normalize(ec_v);

	//If we are below, flip the normal
	if (dot(ec_v, ec_nHat) < 0)
		ec_nHat = -ec_nHat;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{

		//Default directional light source if something goes wrong
		vec3 ecLi = vec3(0.0, 0.0, 1.0);

		if (sources[i].w == 0.0f) //If directional
		{
			if (false) //How do I check for orthogonal?  What am I supposed to do here?
			{
				//Orthogonal
				ecLi = vec3(0.0, 0.0, 1.0);
			}

			ecLi = normalize(vec3(0.0, 0.0, 1.0));
			attenuation = 0.0f;
		}
		
		//If it's not directional, then find the vector from the point to the light source
		else
		{
			ecLi = vec3(sources[i].xyz);
			attenuation = length(ecLi);
			//ecLi = -vec3(ec_Q);
		}

		//Convert to eye coordinates
		//If I don't do this, it looks like garbage
		//Don't need to do this anymore
		//ecLi = (mc_ec * vec4(ecLi, 0.0)).xyz;

		ecLi = normalize(ecLi);

		if (dot(ec_nHat, ecLi) < 0)
		{
			//Don't add specular or diffuse - just stop here for this light source
		}
		else
		{
			//Add the diffuse portion

			Iq += (1 - (attenuation / maxDistanceForLocalLight)) * kd * max((dot(ecLi, ec_nHat)), 0.0) * strengths[i]; //Either add the positive amount of light, or add nothing - we don't want to subtract light


			//Need to get ri - angle of incidence = angle of reflection
			vec3 ri = vec3(0.0, 0.0, 0.0);
			//reflect(ri, ecLi);
			/*
			I - 2.0 * dot(N, I) * N
			*/
			ri = normalize((ecLi - 2.0 * dot(ec_nHat, ecLi)) * ec_nHat);
			//ri = dot(ri, ec_nHat);
			//ri = ri * 2.0;
			//ri = ecLi - ri;
			//ri = normalize(reflect(ecLi, ec_nHat));

			if (dot(ri, ec_v) < 0)
			{
				//ignore specular lighting
				//We are on the same side of the point as the light source - we won't be able to see the specular part
			}
			else
			{

				//Add specular lighting
				//Add ks * strenghts[i] * (riHat dot vHat)^m
				//float specularComponent = max((dot(ri, ec_v)), 0.0);
				float dotted = dot(ri, ec_v);
				if (dotted < 0)
				{
					//Do nothing - ignore specular component
				}
				else
				{
					//there is something wrong with this equation right here
					//Commenting this out removes the "invalid operation error - damn it

					//Iq += ks * strengths[i]; // * pow(dot(ri, ec_v), m);
					//It apparently can't find ks and m...BUT THEY'RE RIGHT HERE!!!
					Iq += (1 - (attenuation / maxDistanceForLocalLight)) * ks * pow(dot(ri, ec_v), m) * strengths[i];
				}
				
			}
		

		}

	}


	float maxLightValue = max(Iq.x, max(Iq.y, Iq.z));

	//Dear god, could anything actually be below zero?
	//Just in case......
	Iq.x = max(Iq.x, 0.0);
	Iq.y = max(Iq.y, 0.0);
	Iq.z = max(Iq.z, 0.0);

	if (maxLightValue > 1.0f)
	{
		Iq.x = Iq.x / maxLightValue;
		Iq.y = Iq.y / maxLightValue;
		Iq.z = Iq.z / maxLightValue;
	}

	//This is just for debugging purposes - will remove
	if (false)
	{
		//Iq = vec3(0.0, 0.0, 0.0) + ka * strengths[i];

		float factor = abs(dot(normalize(p_ecLightSource.xyz), ec_nHat));

		Iq = factor * kd;
	}

	//float factor = abs(dot(normalize(p_ecLightSource.xyz), ec_nHat));

	//Iq += factor * kd;

	//return factor * kd;

	return Iq;
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	vec3 ec_nHat = normalize(normalMatrix * mcNormal);

	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);

	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position = ec_lds * p_ecPosition;
}
