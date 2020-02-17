PROJECT(Tools)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
        ../Creature/Interface
        ../Environment/Interface
        ../Shapes/Interface
	../Genome/Interface
	../Math/Interface
	../System/Interface
	../Tools/Interface
	../Physics/Interface
)

SET(SOURCES
	Src/random.cpp
	Src/setting.cpp
	Src/setting_parser.cpp
        Src/cladeMap.cpp
)

ADD_LIBRARY(tools ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
