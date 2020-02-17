PROJECT(Environment)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	../Tools/Interface
	../Physics/Interface
	../Genome/Interface
	../Creature/Interface
	../Math/Interface
	../System/Interface
	../Shapes/Interface
)

SET(SOURCES
	Src/command_line_settings.cpp
	Src/environment_data.cpp
	Src/environment.cpp
	Src/file_handler.cpp
	Src/interactive_environment.cpp
	Src/run_settings.cpp
)

SET(ANALYSIS_SOURCES
	Src/command_line_settings.cpp
	Src/environment_data.cpp
	Src/environment_analysis.cpp
	Src/file_handler.cpp
	Src/interactive_environment.cpp
	Src/run_settings.cpp
	Src/main_analysis.cpp
)

ADD_LIBRARY(environment ${SOURCES})
ADD_LIBRARY(environment_main Src/main.cpp)
ADD_LIBRARY(environment_analysis ${ANALYSIS_SOURCES})

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
