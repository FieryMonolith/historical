PROJECT(WhirlingDervish C++)

OPTION(USE_FLTK "Use FLTK" ON)
OPTION(USE_QT "Use QT" OFF)

SUBDIRS(
	Creature
	Environment
	Genome
	Graphics
	Math
	Physics
	Shapes
	Tools
	Play
	Viewer
)

LINK_DIRECTORIES(
	Creature/Lib
	Environment/Lib
	Genome/Lib
	Graphics/Lib
	Math/Lib
	Physics/Lib
	Shapes/Lib
	Tools/Lib
	Play/Lib
	Viewer/Lib
	/usr/lib
)

SET(EXECUTABLE_OUTPUT_PATH "./bin")

ADD_EXECUTABLE(WhirlingDervishEnvironment System/Src/dummy.cpp)
ADD_EXECUTABLE(WhirlingDervishAnalysis System/Src/dummy.cpp)
ADD_EXECUTABLE(WhirlingDervishPlay System/Src/dummy.cpp)
ADD_EXECUTABLE(WhirlingDervishViewer System/Src/dummy.cpp)

SET(EVERYONE_LIBS environment creature genome math physics shapes tools)
SET(ANALYSIS_LIBS creature genome math physics shapes tools)

TARGET_LINK_LIBRARIES(WhirlingDervishEnvironment
	environment_main ${EVERYONE_LIBS}
)

TARGET_LINK_LIBRARIES(WhirlingDervishAnalysis
	environment_analysis ${EVERYONE_LIBS}
)

TARGET_LINK_LIBRARIES(WhirlingDervishPlay
	play qt ${EVERYONE_LIBS} graphics
)

TARGET_LINK_LIBRARIES(WhirlingDervishViewer
	viewer ${EVERYONE_LIBS} graphics qt
)
