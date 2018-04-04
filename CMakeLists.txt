cmake_minimum_required(VERSION 2.8)
project(cubeVR1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -gdwarf-2 ")

set(SOURCE_FILES main.cpp)

add_executable(cubeVR1 main.cpp model/Camera.cpp util/ObjFileLoader.cpp util/ObjFileLoader.h model/ModelObject.cpp model/ModelObject.h util/MapFileLoader.cpp util/MapFileLoader.h lib/Node.cpp lib/Node.h lib/Tree.cpp lib/Tree.h model/MapTree.cpp model/MapTree.h util/MtlFileLoader.cpp util/MtlFileLoader.h model/Material.cpp model/Material.h model/MaterialContainer.cpp model/MaterialContainer.h model/Animation.cpp model/Animation.h model/FrameState.cpp model/FrameState.h model/Animation.cpp model/Animation.h model/AnimationEngine.cpp model/AnimationEngine.h model/Animation.cpp model/Animation.h util/AnimationParser.h util/AnimationParser.cpp util/animationcontainer.h util/animationcontainer.cpp
model/animationtemplate.h model/animationtemplate.cpp model/AabbDetector.cpp model/AabbDetector.h)

find_package(FreeGLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(SOIL REQUIRED)

include_directories( ${FREEGLUT_INCLUDE_DIR} ${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${SOIL_INCLUDE_DIR} )
target_link_libraries(cubeVR1 ${FREEGLUT_LIBRARY} ${OPENGL_LIBRARIES} ${SOIL_LIBRARY} )
