cmake_minimum_required(VERSION 3.1.3)
project(agv_moveit_config)


find_package(catkin REQUIRED
REQUIRED COMPONENTS
  actionlib
  roscpp
  sensor_msgs
  control_msgs
  trajectory_msgs
        moveit_ros_planning_interface
        tf2
        tf2_ros
)
find_package(Eigen3 REQUIRED)


add_service_files(
  FILES
  variable.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
        LIBRARIES simple_trajectory_action_client
  CATKIN_DEPENDS actionlib roscpp sensor_msgs control_msgs trajectory_msgs moveit_ros_planning_interface geometric_shapes
)



include_directories(${catkin_INCLUDE_DIRS})
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})
include_directories(include)



add_executable(add_object src/add_object.cpp)
target_link_libraries(add_object
        ${catkin_LIBRARIES}

        )


add_executable(tf_broadcaster src/tf_broadcaster.cpp  )
target_link_libraries(tf_broadcaster
        ${catkin_LIBRARIES}
        )

add_executable(tt src/tt.cpp )
target_link_libraries(tt
        ${catkin_LIBRARIES}
        )


add_executable(move_camera src/move_camera.cpp )
target_link_libraries(move_camera
        ${catkin_LIBRARIES}
        )
        
add_executable(finite_state_machine  src/finite_state_machine.cpp )
target_link_libraries(finite_state_machine
        ${catkin_LIBRARIES}
        )
add_executable(move_to_tag src/move_to_tag.cpp  )
target_link_libraries(move_to_tag
        ${catkin_LIBRARIES}
        )

add_executable(move_to_chuck src/move_to_chuck.cpp)
target_link_libraries(move_to_chuck
        ${catkin_LIBRARIES}
        )
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN "setup_assistant.launch" EXCLUDE)
install(DIRECTORY config DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
