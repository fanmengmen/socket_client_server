
add_executable(server server.cpp)
target_link_libraries(server pthread)
target_link_libraries(server http)