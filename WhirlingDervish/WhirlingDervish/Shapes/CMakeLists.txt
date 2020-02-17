PROJECT(Shapes)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Math/Interface
	../System/Interface
)

SET(SOURCES
	Src/physical_point.cpp
	Src/physical_point_info.cpp
	Src/shape_engine.cpp
	Src/spring.cpp
	Src/spring_info.cpp
)

ADD_LIBRARY(shapes ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
