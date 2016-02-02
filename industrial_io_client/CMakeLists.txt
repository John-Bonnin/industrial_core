cmake_minimum_required(VERSION 2.8.3)

project(industrial_io_client)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sensor_msgs 
  control_msgs trajectory_msgs simple_message actionlib_msgs actionlib 
  urdf industrial_msgs industrial_utils)

find_package(Boost REQUIRED COMPONENTS system thread chrono)

# The definition is copied from the CMakeList for the simple_message package.
add_definitions(-DROS=1)           #build using ROS libraries
add_definitions(-DLINUXSOCKETS=1)  #build using LINUX SOCKETS libraries

set(SRC_FILES src/io_input_handler.cpp src/io_read_handler.cpp)

# NOTE: The libraries generated this package are not included in the catkin_package
# macro because libraries must be explicitly linked in projects that depend on this
# package.  If this is not done (and these libraries were exported), then multiple
# library definitions (normal - industrial_io_client and byteswapped.
# industrial_io_client_bswap) are both included (this is bad).
catkin_package(
    CATKIN_DEPENDS roscpp std_msgs sensor_msgs control_msgs trajectory_msgs
      simple_message actionlib_msgs actionlib urdf industrial_msgs
      industrial_utils
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}_dummy
    CFG_EXTRAS issue46_workaround.cmake
)



include_directories(include
  ${catkin_INCLUDE_DIRS}
)


# generate dummy library (we export it in catkin_package(..)), to force catkin
# to set up LIBRARY_DIRS properly.
# TODO: find out if LIBRARY_DIRS can be exported without dummy library target
add_custom_command(
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_dummy.cpp
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_dummy.cpp)
add_library(${PROJECT_NAME}_dummy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_dummy.cpp)
# unfortunately this will have to be installed, but the linker will remove it
# from the library dependencies of dependent targets, as it contains no symbols
# that can be imported from it.
install(TARGETS ${PROJECT_NAME}_dummy DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})


# This library depends on the simple_message library, which is available
# in two formats to support endian differences between robot & PC.
#
# As in simple_message, two libraries are created: one that does direct
# passthrough of binary numeric data to the robot controller, and one that
# uses byte-swapping to account for endian mis-matches.
#
# Due to catkin dependency limitations, higher-level code must
# explicitly link to the desired industrial_io_client library:
#    target_link_libraries(my_node industrial_io_client)
#      or
#    target_link_libraries(my_node industrial_io_client_bswap)
#

add_library(industrial_io_client ${SRC_FILES})
target_link_libraries(industrial_io_client simple_message)

add_library(industrial_io_client_bswap ${SRC_FILES})
target_link_libraries(industrial_io_client_bswap simple_message_bswap)

# The following executables(nodes) are for applications where the robot
# controller and pc have the same byte order (i.e. byte swapping NOT
# required)

add_executable(industrial_io_node
  src/industrial_io_node.cpp)
target_link_libraries(industrial_io_node 
  industrial_io_client
  simple_message
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES})

# The following executables(nodes) are for applications where the robot
# controller and pc have different same byte order (i.e. byte swapping IS
# required)

add_executable(industrial_io_node_bswap
  src/industrial_io_node.cpp)
target_link_libraries(industrial_io_node_bswap 
  industrial_io_client_bswap 
  simple_message_bswap
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES})
  
install(
    TARGETS industrial_io_client industrial_io_client_bswap
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

install(TARGETS 
        industrial_io_node 
        industrial_io_node_bswap  
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(
    DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

foreach(dir config launch)
   install(DIRECTORY ${dir}/
      DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

