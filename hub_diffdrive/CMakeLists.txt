cmake_minimum_required(VERSION 3.16)
project(hub_diffdrive LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
  add_compile_options(-Wall -Wextra)
endif()

# find dependencies
set(THIS_PACKAGE_INCLUDE_DEPENDS
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
)

# find dependencies
find_package(ament_cmake REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()


## COMPILE
add_library(
  hub_diffdrive
  SHARED
  hardware/diffbot_system.cpp
)
target_compile_features(hub_diffdrive PUBLIC cxx_std_17)
target_include_directories(hub_diffdrive PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/hardware/include>
$<INSTALL_INTERFACE:include/hub_diffdrive>
)
ament_target_dependencies(
  hub_diffdrive PUBLIC
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)

target_link_libraries(hub_diffdrive PUBLIC serial)

# Causes the visibility targ to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(${PROJECT_NAME} PRIVATE "HUB_DIFFDRIVE_BUILDING_DLL")

# Export hardware plugins
pluginlib_export_plugin_description_file(hardware_interface hub_diffdrive.xml)

# INSTALL
install(
  DIRECTORY hardware/include/
  DESTINATION include/hub_diffdrive
)
install(
  DIRECTORY description/launch description/ros2_control description/urdf
  DESTINATION share/hub_diffdrive
)
install(
  DIRECTORY bringup/launch bringup/config
  DESTINATION share/hub_diffdrive
)
install(TARGETS hub_diffdrive
  EXPORT export_hub_diffdrive
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
endif()

## EXPORTS
ament_export_targets(export_hub_diffdrive HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
