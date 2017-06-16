CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(CPO C)

set(SUB_DIRS
    src/NFinput/TinyXML 
)


add_definitions(" -Wno-deprecated-declarations  ")
add_definitions(" -DTIXML_USE_STL  ")
 
 
FILE(WRITE ${CMAKE_BINARY_DIR}/f.c "void f(void){}\n")
ADD_LIBRARY(f STATIC f.c)
SET_TARGET_PROPERTIES(f PROPERTIES RULE_LAUNCH_LINK
    "${CMAKE_SOURCE_DIR}/cpo ${CMAKE_BINARY_DIR}   tinyxml.o  --"
)  
  
  
#  message(IF ${CMAKE_SYSTEM_NAME} ${NAME_WE}  RHC--------------------------------------------------)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${XMLRPC_INCLUDE_DIRS} ${SUB_DIRS})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR} include ${SUB_DIRS})


#file(GLOB_RECURSE src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*cpp")
#file(GLOB_RECURSE c_src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*c")


#set(SRC_FILES 
#    ${src_files} 
#    ${c_src_files} 
#)
#if(CMAKE_SYSTEM_NAME  MATCHES "Windows")
#    SET(STATIC_FLAGS "-static -static-libgcc -static-libstdc++")
#endif(CMAKE_SYSTEM_NAME  MATCHES "Windows")

#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_FLAGS}")
#add_executable(${PROJECT_NAME} ${SRC_FILES} )
