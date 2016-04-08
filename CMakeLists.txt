cmake_minimum_required(VERSION 2.8.9)

project(NFsim)




#set(CMAKE_BUILD_TYPE Release)
#set (CMAKE_C_FLAGS "$CMAKE_C_FLAGS -fPIC")
#set (CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS -fPIC")

add_definitions("-fPIC")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")



set(SUB_DIRS
    include/
    include/nauty24 
    include/NFutil 
    include/NFutil/MTrand 
    include/NFtest/transcription 
    include/NFtest/tlbr 
    include/NFtest/simple_system 
    include/NFtest/agentcell/cell 
    include/NFtest/agentcell 
    include/NFproperty
    include/NFscheduler 
    include/NFreactions/transformations 
    include/NFreactions/reactions 
    include/NFreactions/reactantLists 
    include/NFreactions/mappings 
    include/NFreactions 
    include/NFoutput 
    include/NFinput 
    include/NFinput/TinyXML 
    include/NFinput/json 
    include/NFfunction/muParser 
    include/NFfunction 
    include/NFcore 
    include/NFcore/reactionSelector 
    include/NFcore/moleculeLists 
)

add_definitions("-Wno-deprecated-declarations")
add_definitions(-std=c++11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SUB_DIRS})

                                                                                                                        
file(GLOB_RECURSE src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*cpp")
file(GLOB_RECURSE c_src_files  "${CMAKE_CURRENT_SOURCE_DIR}/src/*c")


set(SRC_FILES 
    ${src_files} 
    ${c_src_files} 
)



add_library(objlib OBJECT ${SRC_FILES})
# shared libraries need PIC
set_property(TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
#add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:objlib>)
