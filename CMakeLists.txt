if($ENV{ROS_VERSION} EQUAL 1)
  cmake_minimum_required(VERSION 2.8.3)
  project(serial_parser)

  add_compile_options(-std=c++11 -O3)

  find_package(catkin REQUIRED COMPONENTS
    roscpp
    serial
  )

  catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ${PROJECT_NAME}
   CATKIN_DEPENDS roscpp serial
  )

  include_directories(
    ${catkin_INCLUDE_DIRS}
  )

  add_library(${PROJECT_NAME}
    src/${PROJECT_NAME}.cpp
  )

  add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

  target_link_libraries(${PROJECT_NAME}
    ${catkin_LIBRARIES}
  )

  ## Example
  add_executable(parser_example examples/parser_example.cpp)
  add_dependencies(parser_example ${PROJECT_NAME})
  target_link_libraries(parser_example ${PROJECT_NAME})

  include_directories(
    include
  )

  install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  )

  install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
  )

else()
  cmake_minimum_required(VERSION 3.5)
  project(serial_parser)

  set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")

  add_compile_options(-std=c++14)

  find_package(ament_cmake REQUIRED)
  find_package(rclcpp REQUIRED)
  find_package(serial REQUIRED)

  include_directories(${serial_INCLUDE_DIRS})

  add_library(${PROJECT_NAME}
    src/${PROJECT_NAME}.cpp
  )

  ament_target_dependencies(${PROJECT_NAME}
    "rclcpp"
    "serial"
  )

  target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )

  install(
    DIRECTORY include/
    DESTINATION include
  )

  install(
    TARGETS ${PROJECT_NAME}
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
  )

  add_executable(parser_example include/${PROJECT_NAME}/serial_parser.h examples/parser_example.cpp)

  ament_export_libraries(${PROJECT_NAME} serial)
  target_link_libraries(parser_example ${PROJECT_NAME})


  install(TARGETS parser_example
    EXPORT export_${PROJECT_NAME}
    DESTINATION lib/${PROJECT_NAME}
  )

  ament_export_include_directories(
    include
  )

  ament_export_interfaces(
    export_${PROJECT_NAME}
  )

  ament_export_libraries(
    ${PROJECT_NAME}
  )

  ament_package()

endif()
