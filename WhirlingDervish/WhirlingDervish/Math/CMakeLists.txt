PROJECT(Math)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
)

SET(SOURCES
	Src/geometry.cpp
	Src/jvector.cpp
	Src/matrix.cpp
	Src/spacial_description.cpp
)

ADD_LIBRARY(math ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
