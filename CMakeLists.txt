cmake_minimum_required(VERSION 3.2)

project(pdf_sender)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    src/session.cpp
    src/server.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    boost_system
    pthread
)
