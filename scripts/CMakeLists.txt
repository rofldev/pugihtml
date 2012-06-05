project(pugihtml)

set(SOURCES ../src/pugihtml.cpp ../src/memory.cpp)

add_library(pugihtml STATIC ${SOURCES})
