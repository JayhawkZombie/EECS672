// ModelView.c++ - a basic combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"
#include "Controller.h"
#include "ShaderIF.h"

ShaderIF* ModelView::shaderIF = NULL;
int ModelView::numInstances = 0;
GLuint ModelView::shaderProgram = 0;

double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

GLint ModelView::ppuLoc_color = -2;
GLint ModelView::ppuLoc_scaleTrans = -2;
GLint ModelView::pvaLoc_mcPosition = -2;


ModelView::ModelView(std::vector<float> coord, float colors[3])
{
    drawAsLines = false;
    numVertices = coord.size();

	if (ModelView::shaderProgram == 0)
	{
		// Create the common shader program:
		ModelView::shaderIF = new ShaderIF("project1.vsh", "project1.fsh");
		ModelView::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
        drawColor[0] = colors[0]; drawColor[1] = colors[1]; drawColor[2] = colors[2];
        vec2* c = new vec2[coord.size()];

	// TODO: define and call method(s) to initialize your model and send data to GPU

        xmin = 0.0; xmax = coord.size();
        ymin = ymax = 0;

        for(int i = 0; i < 121; i++)
        {
            c[i][0] = static_cast<float>(i);

            /* MONITOR MIN/MAX HERE */

            if(coord[i] < ymin)
                ymin = coord[i];
            else if (coord[i] > ymax)
                ymax = coord[i];

            c[i][1] = coord[i];

        }

        initModelGeometry(c, coord.size());
	ModelView::numInstances++;

        xmax = xmax + 0.2;
        //ymax = ymax + 0.2;
        xmin = xmin - 0.2;
        ymin = ymin - 0.02;

        delete[] c;
        //std::cerr << "drawColor[0] = " << drawColor[0] << " | drawColor[1] = " << drawColor[1] << " | drawColor[2] = " << drawColor[2] << std::endl;
}

ModelView::ModelView(vec2 *coords, float colors[], int numVerts)
{
    drawAsLines = true;

    numVertices = numVerts;

    if (ModelView::shaderProgram == 0)
    {
            // Create the common shader program:
            ModelView::shaderIF = new ShaderIF("project1.vsh", "project1.fsh");
            ModelView::shaderProgram = shaderIF->getShaderPgmID();
            fetchGLSLVariableLocations();
    }
    drawColor[0] = colors[0]; drawColor[1] = colors[1]; drawColor[2] = colors[2];
    vec2* c = new vec2[numVerts];

    // TODO: define and call method(s) to initialize your model and send data to GPU

    xmin = 0.0; xmax = numVerts;
    ymin = ymax = 0;

    for(int i = 0; i < numVerts; i++)
    {
        /* MONITOR MIN/MAX HERE */

        if (coords[i][1] > ymax)
            ymax = coords[i][1];
        if (coords[i][0] > xmax)
            xmax = coords[i][1];

        //std::cout << "X: " << coords[i][0] << ", Y: " << coords[i][1] << std::endl;
        c[i][0] = coords[i][0];
        c[i][1] = coords[i][1];
    }

    xmax = xmax + 0.2;
    //ymax = ymax + 0.2;
    xmin = xmin - 0.2;
    ymin = ymin - 0.02;

    initModelGeometry(c, numVerts);
    ModelView::numInstances++;

    delete[] c;
    //std::cerr << "drawColor[0] = " << drawColor[0] << " | drawColor[1] = " << drawColor[1] << " | drawColor[2] = " << drawColor[2] << std::endl;
}

void ModelView::initModelGeometry(vec2 *coords, int numVerts)
{
    glGenVertexArrays(1, vao);

    glGenBuffers(1, vbo);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    int numBytesInBuffer = numVerts * sizeof(vec2);
    glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, coords, GL_STATIC_DRAW);
    glVertexAttribPointer(ModelView::pvaLoc_mcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ModelView::pvaLoc_mcPosition);
}

ModelView::~ModelView()
{
	// TODO: delete the vertex array objects and buffers here

    if (vao[0] > 0) // hasn't already been deleted
    {
            glDeleteBuffers(1, &vbo[0]);
            glDeleteVertexArrays(1, &vao[0]);
            vao[0] = vbo[0] = 0;
    }

	if (--numInstances == 0)
	{
		ModelView::shaderIF->destroy();
		delete ModelView::shaderIF;
		ModelView::shaderIF = NULL;
		ModelView::shaderProgram = 0;
	}
}

void ModelView::compute2DScaleTrans(float* scaleTransF) // CLASS METHOD
{
	// TODO: This code can be used as is, BUT be absolutely certain you
	//       understand everything about how it works.

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	// If we wish to preserve aspect ratios, make "region of interest"
	// wider or taller to match the Controller's viewport aspect ratio.
	//double vAR = Controller::getCurrentController()->getViewportAspectRatio();
	//matchAspectRatio(xmin, xmax, ymin, ymax, vAR);

    // We are only concerned with the xy extents for now, hence we will
    // ignore mcRegionOfInterest[4] and mcRegionOfInterest[5].
    // Map the overall limits to the -1..+1 range expected by the OpenGL engine:
	double scaleTrans[4];
	linearMap(xmin, xmax, -1.0, 1.0, scaleTrans[0], scaleTrans[1]);
	linearMap(ymin, ymax, -1.0, 1.0, scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

void ModelView::fetchGLSLVariableLocations()
{
	if (ModelView::shaderProgram > 0)
	{
		// TODO: Set GLSL variable locations here
            ModelView::ppuLoc_color = ppUniformLocation(shaderProgram, "color");
            ModelView::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
            // "mc" in "mcPosition" refers to "model coordinates"
            ModelView::pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ModelView::getMCBoundingBox(double* xyzLimits) const
{
	// TODO:
	// Put this ModelView instance's min and max x, y, and z extents
	// into xyzLimits[0..5]. (-1 .. +1 is OK for z direction for 2D models)
        xyzLimits[0] = xmin; xyzLimits[1] = xmax;
        xyzLimits[2] = ymin; xyzLimits[3] = ymax;
        xyzLimits[4] = -1; xyzLimits[5] = 1;

}

void ModelView::handleCommand(unsigned char key, double ldsX, double ldsY)
{
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
					  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

void ModelView::matchAspectRatio(double& xmin, double& xmax,
        double& ymin, double& ymax, double vAR)
{
	// TODO: This code can be used as is, BUT be absolutely certain you
	//       understand everything about how it works.

	double wHeight = ymax - ymin;
	double wWidth = xmax - xmin;
	double wAR = wHeight / wWidth;
	if (wAR > vAR)
	{
		// make window wider
		wWidth = wHeight / vAR;
		double xmid = 0.5 * (xmin + xmax);
		xmin = xmid - 0.5*wWidth;
		xmax = xmid + 0.5*wWidth;
	}
	else
	{
		// make window taller
		wHeight = wWidth * vAR;
		double ymid = 0.5 * (ymin + ymax);
		ymin = ymid - 0.5*wHeight;
		ymax = ymid + 0.5*wHeight;
	}
}

GLint ModelView::ppUniformLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetUniformLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ModelView::pvAttribLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetAttribLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}

void ModelView::render() const
{
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

    // draw the triangles using our vertex and fragment shaders
    glUseProgram(shaderProgram);

    // TODO: set scaleTrans (and all other needed) uniform(s)
    float scaleTrans[4];
    compute2DScaleTrans(scaleTrans);

    glUniform4fv(ModelView::ppuLoc_scaleTrans, 1, scaleTrans);
    glUniform3fv(ModelView::ppuLoc_color, 1, drawColor);

    // TODO: make require primitive call(s)
    glBindVertexArray(vao[0]);

    // Draw the i-th triangle.
    if (drawAsLines)
        glDrawArrays(GL_LINES, 0, numVertices);
    else
        glDrawArrays(GL_LINE_STRIP, 0, 121);

    // restore the previous program
    glUseProgram(pgm);
}



void ModelView::setMCRegionOfInterest(double xyz[6])
{
	for (int i=0 ; i<6 ; i++)
		mcRegionOfInterest[i] = xyz[i];
}
