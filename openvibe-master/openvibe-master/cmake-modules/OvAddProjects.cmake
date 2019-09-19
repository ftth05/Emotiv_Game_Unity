#
# Adds all directories as subdirectories to the CMake build, using the branch specified (if any) in the root CMakeList.txt or
# trunk otherwise.
#
# The branch variable name that is checked is made up from ${CURRENT_BRANCH_PREFIX}_${DIRNAMEUPPER}. 
#
# The script also adds the directory to the global list of projects.
#

FUNCTION(OV_ADD_PROJECTS CURRENT_BRANCH_PREFIX)

	FILE(GLOB FILENAMES "*") 

	FOREACH(FULLPATH ${FILENAMES})
		IF(IS_DIRECTORY ${FULLPATH} AND NOT ${FULLPATH} MATCHES ".*\\.svn")
			GET_FILENAME_COMPONENT(DIRNAME ${FULLPATH} NAME)

			SET(OV_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${DIRNAME}")

			STRING(TOUPPER ${DIRNAME} DIRNAMEUPPER)
			SET(BRANCH_VARIABLE_NAME "${CURRENT_BRANCH_PREFIX}_${DIRNAMEUPPER}")
			SET(SUBDIR "SUBDIR-NOTFOUND")		
			
			# MESSAGE(STATUS "Checking ${CURRENT_BRANCH_PREFIX}_${DIRNAMEUPPER} as branch var ${BRANCH_VARIABLE_NAME}")
			
			IF(${BRANCH_VARIABLE_NAME})	
				SET(BRANCH ${${BRANCH_VARIABLE_NAME}})	
				IF("${BRANCH}" STREQUAL "__SKIPME") 
					MESSAGE(STATUS "Note: ${BRANCH_VARIABLE_NAME} has been disabled by setting it to __SKIPME")
				ELSE("${BRANCH}" STREQUAL "__SKIPME")			
					SET(SUBDIR ${BRANCH})
				ENDIF("${BRANCH}" STREQUAL "__SKIPME")
			ELSE(${BRANCH_VARIABLE_NAME})
				SET(SUBDIR ${OV_TRUNK})
			ENDIF(${BRANCH_VARIABLE_NAME})
			
			IF(SUBDIR)
				#MESSAGE(STATUS "Inserting folder ${SUBDIR}")		
				
				# Add the dir to be parsed for documentation later. We need to do this before adding subdir, in case the subdir is the actual docs dir
				GET_PROPERTY(OV_TMP GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS)
				SET(OV_TMP "${OV_TMP};${FULLPATH}/${SUBDIR}")
				SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS ${OV_TMP})

#				MESSAGE(STATUS "BDIR ${OV_CURRENT_BINARY_DIR}/${SUBDIR}")
				GET_PROPERTY(OV_TMP GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS_BUILD_DIR)
				SET(OV_TMP "${OV_TMP};${OV_CURRENT_BINARY_DIR}/${SUBDIR}")
				SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS_BUILD_DIR ${OV_TMP})


				add_subdirectory(${FULLPATH}/${SUBDIR})
				
			ENDIF(SUBDIR)	
		ENDIF(IS_DIRECTORY ${FULLPATH} AND NOT ${FULLPATH} MATCHES ".*\\.svn")			
	ENDFOREACH(FULLPATH ${FILENAMES})

ENDFUNCTION(OV_ADD_PROJECTS)

