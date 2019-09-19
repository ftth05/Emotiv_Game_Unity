# ---------------------------------
# Finds Matlab toolkit
# ---------------------------------
SET(Matlab_EXECUTABLE "Matlab_EXECUTABLE-NOTFOUND")

IF (WIN32)
	FIND_PROGRAM(Matlab_EXECUTABLE MATLAB)
ENDIF(WIN32)
IF (UNIX)
	FIND_PROGRAM(Matlab_EXECUTABLE matlab)
ENDIF(UNIX)


IF(UNIX OR WIN32)
	
	IF(UNIX AND NOT Matlab_EXECUTABLE)
		# Alternative way to try to find matlab
		FILE(GLOB_RECURSE Executable_Candidates "/usr/local/matlab*/matlab")
		IF(Executable_Candidates) 
			LIST(GET Executable_Candidates 1 Matlab_EXECUTABLE)
		ENDIF(Executable_Candidates)
	ENDIF(UNIX AND NOT Matlab_EXECUTABLE)
	IF(Matlab_EXECUTABLE)
		GET_FILENAME_COMPONENT(Matlab_ROOT ${Matlab_EXECUTABLE} PATH)
		SET(Matlab_ROOT ${Matlab_ROOT}/..)
		FIND_PATH(Matlab_INCLUDE "mex.h" PATHS ${Matlab_ROOT}/extern/include ${Matlab_ROOT}/extern/include/extern)
		IF(Matlab_INCLUDE)
			IF(UNIX)
				SET(Matlab_LIBRARIES mex mx eng)
				IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
					SET(Matlab_LIB_DIRECTORIES ${Matlab_ROOT}/bin/glnx86)
				ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
					SET(Matlab_LIB_DIRECTORIES ${Matlab_ROOT}/bin/glnxa64)
				ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
			ENDIF(UNIX)
			IF(WIN32)
				SET(Matlab_LIBRARIES libmex libmx libeng) #mclmcrrt
				SET(Matlab_LIB_DIRECTORIES ${Matlab_ROOT}/extern/lib/win32/microsoft)
				# for delayed importation on windows
				TARGET_LINK_LIBRARIES(${PROJECT_NAME} Delayimp )
				SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/DELAYLOAD:libeng.dll /DELAYLOAD:libmx.dll")
				# /DELAYLOAD:libmex.dll /DELAYLOAD:mclmcrrt.dll --> useless, no import
			ENDIF(WIN32)
			SET(Matlab_FOUND TRUE)
		ELSE(Matlab_INCLUDE)
		ENDIF(Matlab_INCLUDE)
	ENDIF(Matlab_EXECUTABLE)
ENDIF(UNIX OR WIN32)

IF(Matlab_FOUND)
	MESSAGE(STATUS "  Found Matlab [${Matlab_EXECUTABLE}]")
	SET(Matlab_LIB_FOUND TRUE)
	INCLUDE_DIRECTORIES(${Matlab_INCLUDE})

	FOREACH(Matlab_LIB ${Matlab_LIBRARIES})
		SET(Matlab_LIB1 "Matlab_LIB1-NOTFOUND")
		FIND_LIBRARY(Matlab_LIB1 NAMES ${Matlab_LIB} PATHS ${Matlab_LIB_DIRECTORIES} NO_DEFAULT_PATH)
		FIND_LIBRARY(Matlab_LIB1 NAMES ${Matlab_LIB})
		IF(Matlab_LIB1)
			MESSAGE(STATUS "	[  OK  ] Third party lib ${Matlab_LIB1}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Matlab_LIB1})
		ELSE(Matlab_LIB1)
			MESSAGE(STATUS "	[FAILED] Third party lib ${Matlab_LIB}")
			SET(Matlab_LIB_FOUND FALSE)
		ENDIF(Matlab_LIB1)
	ENDFOREACH(Matlab_LIB)
	IF(Matlab_LIB_FOUND)
		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyMatlab)
	ELSE(Matlab_LIB_FOUND)
		MESSAGE(STATUS "  FAILED to find Matlab Libs, the plugins won't be built. Please ensure you have a valid MATLAB installation (32 bits only).")
	ENDIF(Matlab_LIB_FOUND)
ELSE(Matlab_FOUND)
	MESSAGE(STATUS "  FAILED to find Matlab...")
ENDIF(Matlab_FOUND)