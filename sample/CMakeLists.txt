SET(CMAKE_BUILD_TYPE "Debug")


INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)


if(COMPILER_SUPPORTS_CXX11)
    add_definitions("-std=c++11")
endif()



SET(CMAKE_CXX_FLAGES_DEBUG "${CMAKE_CXX_FLAGES} -O0  -pipe -Wall -fno-strict-aliasing -D_GNU_SOURCE -D_REENTRANT -fPIC -g -ggdb")
SET(CMAKE_CXX_FLAGES_RELEASE "${CMAKE_CXX_FLAGES} -O0  -pipe -Wall -fno-strict-aliasing -D_GNU_SOURCE -D_REENTRANT -fPIC ")

INCLUDE_DIRECTORIES(../inc/)
LINK_DIRECTORIES(../src/)
SET(LIBRARY libcurl.so libCMQ.so)

SET(SRC_TOPIC cmq_sample_topic.cpp)
SET(SRC_QUEUE cmq_sample_queue.cpp)

ADD_EXECUTABLE(cmq_sample_topic ${SRC_TOPIC})
TARGET_LINK_LIBRARIES(cmq_sample_topic ${LIBRARY})


ADD_EXECUTABLE(cmq_sample_queue ${SRC_QUEUE})
TARGET_LINK_LIBRARIES(cmq_sample_queue ${LIBRARY})





