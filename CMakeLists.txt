cmake_minimum_required(VERSION 3.30)
project(tweak)
enable_testing()
add_library(tweak INTERFACE)
add_library(tweak::tweak ALIAS tweak)
target_sources(tweak INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/include
	FILES
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/const-math.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/convert.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/math.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/tweak.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/std/amp.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/std/ms.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/std/percentage.hpp
		${CMAKE_CURRENT_LIST_DIR}/include/tweak/std/speed.hpp
	)
target_compile_definitions(tweak INTERFACE
	_USE_MATH_DEFINES
)
if (BUILD_TESTING)
	add_subdirectory(test)
endif()
include(CMakePackageConfigHelpers)
install(TARGETS tweak EXPORT tweak-targets FILE_SET HEADERS)
install(EXPORT tweak-targets FILE tweak-targets.cmake NAMESPACE tweak:: DESTINATION lib/cmake/tweak)
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/tweak-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/tweak-config.cmake"
    INSTALL_DESTINATION lib/cmake/tweak
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/tweak-config.cmake" DESTINATION lib/cmake/tweak)
