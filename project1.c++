// project1.c++

#include "GLFWController.h"
#include "ModelView.h"

#include <vector>
#include <string>
#include <fstream>

std::vector<float> line1;
std::vector<float> line2;
std::vector<float> line3;
std::vector<float> line4;

void readFile()
{
    std::ifstream is;
    is.open("ExchangeRates.csv");
    if(is.fail())
    {
        std::cout << "OOPS" << std::endl;
    }
    std::string date;
    float rate1, rate2, rate3, rate4;
    char comma;

    int nLines = 0;

    line1.reserve(121);
    line2.reserve(121);
    line3.reserve(121);
    line4.reserve(121);

    while(is >> date)
    {
        is >> rate1 >> comma >> rate2 >> comma >> rate3 >> comma >> rate4;
        line1.push_back(rate1);
        line2.push_back(rate2);
        line3.push_back(rate3);
        line4.push_back(rate4);
        //std::cout << rate1 << comma << rate2 << comma << rate3 << comma << rate4 << std::endl;
        //std::cin >> comma;
    }
    std::cout << "SIZES: " << line1.size() << ", " << line2.size() << ", " << line3.size() << ", " << line4.size() << std::endl;
}

int main(int argc, char* argv[])
{
    readFile();

        float color[3] = {0.0, 0.0, 1.0};

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);


        //Color = { 0.0, 0.0, 1.0 }
        c.addModel(new ModelView(line1, color));

        //color = {0.0, 1.0, 0.0};
        color[0] = 0.0; color[1] = 1.0; color[2] = 0.0;
        c.addModel(new ModelView(line2, color));

        //color = {1.0, 0.0, 0.0};
        color[0] = 1.0; color[1] = 0.0; color[2] = 0.0;
        c.addModel(new ModelView(line3, color));

        //color = {1.0, 1.0, 0.0};
        color[0] = 1.0; color[1] = 0.0; color[2] = 1.0;
        c.addModel(new ModelView(line4, color));

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

        glClearColor(1.0, 1.0, 1.0, 1.0);

	c.run();

	return 0;
}
