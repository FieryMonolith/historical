PROJECT(Genome)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Tools/Interface
	../Math/Interface
	../System/Interface
	../Creature/Interface
	../Environment/Interface
	../Physics/Interface
	../Shapes/Interface
	..
)

SET(SOURCES
	Src/ec_settings.cpp
	Src/instruction_set_settings.cpp
	Src/genome.cpp
	Src/instruction.cpp
	Src/instruction_basic.cpp
	Src/instruction_set.cpp
	Src/language_engine.cpp
	Src/parser.cpp
	Src/parser_basic.cpp
	Src/spider.cpp
	Src/spider_basic.cpp
	Src/turtle.cpp
	Src/turtle_basic.cpp
)

ADD_LIBRARY(genome ${SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
