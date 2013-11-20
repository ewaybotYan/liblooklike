
# for qt precompliation operations
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# find include dir
find_package(Qt5Gui REQUIRED)
find_package(OpenGL REQUIRED)

if (Qt5Gui_FOUND)
	message(STATUS "qt5 found")
else (Qt5Gui_FOUND)
	message(ERROR "qt5 not found") 
endif (Qt5Gui_FOUND)

include_directories(
	${Qt5Gui_INCLUDES}
	${OPENGL_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include 
	"/usr/include/qt"
	)

#link_directories(${Qt5Gui_LIBRARIES})

message( STATUS "include path ${Qt5Gui_INCLUDES}" )

add_definitions(${Qt5Gui_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

file(
	GLOB
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
	)

add_executable(
	test_preview 
	test_preview.cpp 
	../src/openglwindow.cpp
	../src/previewwindow.cpp
	)

target_link_libraries(test_preview ${OPENGL_LIBRARIES})

qt5_use_modules(test_preview Gui)
