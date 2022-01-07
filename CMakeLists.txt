message(STATUS "Build: lcd5110_lib")

include_directories("src/inc")
file(GLOB_RECURSE LCD5110_SOURCES "src/*.*")

ADD_LIBRARY(lcd5110_lib STATIC ${LCD5110_SOURCES})