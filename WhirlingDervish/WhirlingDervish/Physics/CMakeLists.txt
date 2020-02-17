PROJECT(Physics)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Math/Interface
	../System/Interface
	../Tools/Interface
)

SET(SOURCES
	Src/physics.cpp
	Src/physics_settings.cpp
)

ADD_LIBRARY(physics ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
