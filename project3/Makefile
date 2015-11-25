CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = project3.o ModelViewWithPhongLighting.o  Block.o ModelView_Additions.o Cylinder.o CustomGLFWController.o Wedge.o Pyramid.o Gemstone.o Rupee.o
project3: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -o project3 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project3.o: project3.c++
	$(CPP) $(C_FLAGS) project3.c++
ModelView_Additions.o: ModelView_Additions.c++
	$(CPP) $(C_FLAGS) ModelView_Additions.c++
ModelViewWithPhongLighting.o: ModelViewWithPhongLighting.h ModelViewWithPhongLighting.c++
	$(CPP) $(C_FLAGS) ModelViewWithPhongLighting.c++
Block.o: Block.h Block.c++
	$(CPP) $(C_FLAGS) Block.c++
Cylinder.o: Cylinder.h Cylinder.c++
	$(CPP) $(C_FLAGS) Cylinder.c++
CustomGLFWController.o: CustomGLFWController.h CustomGLFWController.cpp
	$(CPP) $(C_FLAGS) CustomGLFWController.cpp
Wedge.o: Wedge.h Wedge.c++
	$(CPP) $(C_FLAGS) Wedge.c++
Pyramid.o: Pyramid.h Pyramid.c++
	$(CPP) $(C_FLAGS) Pyramid.c++
Gemstone.o: Gemstone.h Gemstone.c++
	$(CPP) $(C_FLAGS) Gemstone.c++
Rupee.o: Rupee.h Rupee.c++
	$(CPP) $(C_FLAGS) Rupee.c++

clean:
	rm *.o
	rm project3
