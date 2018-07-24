cmake_minimum_required(VERSION 2.8)

project(ipc)

add_library(ipcposix ipc_posix.c)

add_executable(${PROJECT_NAME} "main.c")
target_link_libraries(${PROJECT_NAME} ipcposix)
