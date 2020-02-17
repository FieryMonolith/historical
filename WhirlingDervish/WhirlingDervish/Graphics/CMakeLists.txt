PROJECT(Graphics)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Math/Interface
	../System/Interface
	../Creature/Interface
	../Genome/Interface
	../Shapes/Interface
)

SET(SOURCES
	Src/grid_implementation.cpp
	Src/eyepoint_implementation.cpp
	Src/jgl.cpp
	Src/jgl_private.cpp
)

ADD_LIBRARY(graphics ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
