cmake_minimum_required(VERSION 2.8.9)

project(NFsim)

#find_package(XMLRPC REQUIRED abyss-server c++2)


#if (NOT XMLRPC_FOUND) 
#  message (FATAL_ERROR "XMLRPC-C not found") 
#endif (NOT XMLRPC_FOUND) 

set(CMAKE_BUILD_TYPE Release)

#set(SUB_DIRS
#    src/nauty24 
#    src/NFutil 
#    src/NFutil/MTrand 
#    src/NFtest/transcription 
#    src/NFtest/tlbr 
#    src/NFtest/simple_system 
#    src/NFtest/agentcell/cell 
#    src/NFtest/agentcell 
#    src/NFscheduler 
#    src/NFreactions/transformations 
#    src/NFreactions/reactions 
#    src/NFreactions/reactantLists 
#    src/NFreactions/mappings 
#    src/NFreactions 
#    src/NFoutput 
#    src/NFinput 
#    src/NFinput/json 
#    src/NFfunction/muParser 
#    src/NFfunction 
#    src/NFcore 
#    src/NFcore/reactionSelector 
#    src/NFcore/moleculeLists 
#)


set(SUB_DIRS
    src/NFinput/TinyXML 
)


add_definitions(" -Wno-deprecated-declarations  ")


  add_definitions(" -DTIXML_USE_STL  ")
#if (NAME_WE STREQUAL tinyxml)
#  message(IF ${CMAKE_SYSTEM_NAME} ${NAME_WE}  RHC--------------------------------------------------)
#else() 
#  add_definitions( -std=c++0x )
#  message(ELSE ${CMAKE_SYSTEM_NAME} ${NAME_WE}  RHC--------------------------------------------------)
#endif()

#include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${XMLRPC_INCLUDE_DIRS} ${SUB_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} include ${SUB_DIRS})


file(GLOB_RECURSE src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*cpp")
file(GLOB_RECURSE c_src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*c")


set(SRC_FILES 
    ${src_files} 
    ${c_src_files} 
)
if(CMAKE_SYSTEM_NAME  MATCHES "Windows")
    SET(STATIC_FLAGS "-static -static-libgcc -static-libstdc++")
endif(CMAKE_SYSTEM_NAME  MATCHES "Windows")

SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_FLAGS}")
#add_executable(${PROJECT_NAME} ${SRC_FILES} )
