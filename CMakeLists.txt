cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME HhsArduinoCurcuit)
project(${PROJECT_NAME})

# The target Arduino board type
set(${CMAKE_PROJECT_NAME}_BOARD uno)
# The target port the Arduino is connected to, for uploading
set(${CMAKE_PROJECT_NAME}_PORT COM5)

set(${CMAKE_PROJECT_NAME}_SKETCH HhsArduinoCircuit.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
