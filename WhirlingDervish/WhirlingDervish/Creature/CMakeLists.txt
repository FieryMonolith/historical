PROJECT(Creature)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Shapes/Interface
	../Genome/Interface
	../Environment/Interface
	../Math/Interface
	../System/Interface
	../Tools/Interface
	../Physics/Interface
)

SET(SOURCES
	Src/body.cpp
	Src/creature.cpp
	Src/creature_info.cpp
	Src/population_storage.cpp
	Src/hfc_population_storage_base.cpp
	Src/hfc_population_storage.cpp
	Src/hfc_population_storage_4.cpp
        Src/hfc_population_storage_5.cpp
	Src/hfc_population_storage_6.cpp	
)

ADD_LIBRARY(creature ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
