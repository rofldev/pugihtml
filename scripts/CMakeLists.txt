project(pugihtml)

set(SOURCES ../src/pugihtml.hpp ../src/pugiconfig.hpp ../src/pugihtml.cpp)

add_library(pugihtml STATIC ${SOURCES})
