cmake_minimum_required(VERSION 3.5)

project(tmc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES /
    main.cpp /
    simple.cpp /
    tmc.cpp /
    column.cpp /
    )

add_executable(tmc ${SOURCES})

#target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
target_link_libraries(${PROJECT_NAME} PRIVATE mysqlclient)
