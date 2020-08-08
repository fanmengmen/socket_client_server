
FILE(GLOB SOURCE_FILES ./*.cpp)

add_executable(client ${SOURCE_FILES})
target_link_libraries(client pthread)
target_link_libraries(client http)