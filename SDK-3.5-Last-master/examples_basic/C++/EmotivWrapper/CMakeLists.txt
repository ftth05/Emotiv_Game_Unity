cmake_minimum_required(VERSION 3.0.0)

project (EmotivWrapper)

set(WRAPPER_EXE EmotivWrapper)

if (WIN32)
    if(MSVC)
        set_target_properties(${WRAPPER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${WRAPPER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${WRAPPER_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${WRAPPER_EXE}>)
elseif (APPLE)
    set_target_properties(${WRAPPER_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
elseif(UNIX)
    set(PROJECT_LINK_LIBS "${PROJECT_LINK_LIBS} -lncurses")
endif()

add_executable(${WRAPPER_EXE} Emotiv.cpp MyEmotivCallback.cpp main.cpp)

target_include_directories(${WRAPPER_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})

target_link_libraries(${WRAPPER_EXE} ${PROJECT_LINK_LIBS})
