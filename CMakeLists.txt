# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(tcp_ip_server_project)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Declare the executable target built from your sources
add_executable(tcp_ip_server_project main.cpp
     videoCapture.cpp 
     stereoCam.cpp
     tcp_ip_server.cpp
     motor.cpp
     electricDrive.cpp
     speed_sensor.cpp
     fdrm_stbc_agm01.cpp
     )


# Link your application with OpenCV libraries
# Include headers


# Link against libraries
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(tcp_ip_server_project PRIVATE ${WIRINGPI_LIBRARIES})
target_link_libraries(tcp_ip_server_project PRIVATE ${OpenCV_LIBS})
target_link_libraries(tcp_ip_server_project PRIVATE Threads::Threads)
