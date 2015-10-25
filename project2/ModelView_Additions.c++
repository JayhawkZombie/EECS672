// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
	std::cout << "For project 3, you must implement ModelView::addToGlobalRotationDegrees in ModelView_Additions.c++\n";
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. Use dynamic_view in ModelView::getMatrices
}

void ModelView::addToGlobalZoom(double increment)
{
	dynamic_zoomScale += increment;
	// TODO: Project 3: Use dynamic_zoomScale in ModelView::getMatrices
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
	//std::cout << "You must implement ModelView::getMatrices in ModelView_Additions.c++\n";
	// TODO:
	// 1. Create the mc_ec matrix:
	//    Create a local variable of type Matria4x4 calledx M_ECu from the eye,
	//    center, and up. Recall that those values are set in the main program
	//    and are stored in protected class variables that you directly access
	//    (ModelView::eye, ModelView::center, and ModelView::up). You can use the
	//    following utility from Matrix4x4:
	//
	//    cryph::Matrix4x4 cryph::Matrix4x4::lookAt(
	//          const cryph::AffPoint& eye, const cryph::AffPoint& center,
	//          const cryph::AffVector& up);
	//
	//    NOTE: eye, center, and up are specified in MODEL COORDINATES (MC)
	//
	//    a) For project 2: mc_ec = M_ECu
	//    b) For project 3: mc_ec = dynamic_view * M_ECu
	//
	// 2. Create the ec_lds matrix:
	//    Use the mcRegionOfInterest (again, set in main; accessible to you via
	//    the protected ModelView class variable ModelView::mcRegionOfInterest) to
	//    determine the radius of the circumscribing sphere. Recall that this radius
	//    is used to determine an initial ecXmin, ecXmax, ecYmin, and ecYmax.
	//      i) Adjust the width in the x OR y direction to match the viewport aspect ratio;
	//     ii) Project 3 and 4 only: Scale both widths by dynamic_zoom;
	//    iii) Use the adjusted widths along with the ecZmin, ecZmax, and ecZpp set
	//         in main and stored in the ModelView class variables to create the
	//         ec_lds matrix. Use the class variable ModelView::projType to determine
	//         which of the routines below to use. (Also use ModelView::obliqueProjectionDir
	//         if the projType is OBLIQUE.
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!   All coordinate data in the parameter lists below are specified      !!!!!!
	// !!!!!   in EYE COORDINATES (EC)! Be VERY sure you understand what that      !!!!!!
	// !!!!!   means!  (This is why I emphasized "WIDTHS" above.)                  !!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	/* The three ec_lds matrix generation choices:

	cryph::Matrix4x4 cryph::Matrix4x4::orthogonal(double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::Matrix4x4::perspective(double ecZpp, double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::Matrix4x4::oblique(double ecZpp, double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax,
		const cryph::AffVector& projDir);

	*/

	// THEN IN THE CALLER OF THIS METHOD:
	//
	// float mat[16];
	// glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	// glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));
	//
	// (The extractColMajor method copies the elements of the matrix into the given
	// array which is assumed to be of length 16. It then returns the array pointer
	// so it can be used as indicated in the two calls. Since the array is immediately
	// copied by glUniformMatrix to the GPU, "mat" can be reused as indicated.)

	//Alright, here we go

	cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(ModelView::eye, ModelView::center, ModelView::up);
	mc_ec = M_ECu;

	float deltaX = mcRegionOfInterest[1] - mcRegionOfInterest[0];
	float deltaY = mcRegionOfInterest[3] - mcRegionOfInterest[2];
	float deltaZ = mcRegionOfInterest[5] - mcRegionOfInterest[4];

	float radius = sqrt(  (deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ)  ) / 2;

	double xmin = -radius;
	double xmax = radius;
	double ymin = -radius;
	double ymax = radius;

	double vAR = Controller::getCurrentController()->getViewportAspectRatio();

	matchAspectRatio(xmin, xmax, ymin, ymax, vAR);


	if(projType == PERSPECTIVE)
	{
		ec_lds = cryph::Matrix4x4::perspective(ecZpp, xmin, xmax, ymin, ymax, ecZmin, ecZmax);
	}
	else if (projType == ORTHOGONAL)
	{
		ec_lds = cryph::Matrix4x4::orthogonal(xmin, xmax, ymin, ymax, ecZmin, ecZmax);
	}
	else
	{
		ec_lds = cryph::Matrix4x4::oblique(ecZpp, xmin, xmax, ymin, ymax, ecZmin, ecZmax, obliqueProjectionDir);
	}


}
