cmake_minimum_required(VERSION 3.5)
project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(DEBUG_PRINT "0")

if (DEBUG_PRINT)
	message("DEBUG_PRINT is activated!")
	add_definitions(-DDEBUG_PRINT)
endif()

include_directories(dlib)
link_directories(dlib/dlib)

add_executable(rclst
	defines.h
	utils.cpp
	utils.h
	rclst.h
	rclst.cpp
	rclst_main.cpp)

add_executable(rclss
	defines.h
	utils.cpp
	utils.h
	rclss.h
	rclss.cpp
	rclss_main.cpp)

set_target_properties(rclst PROPERTIES
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
	CMAKE_CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED ON
)

set_target_properties(rclss PROPERTIES
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
	CMAKE_CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED ON
)

target_link_libraries(rclst
	${CMAKE_THREAD_LIBS_INIT}
	dlib
	blas
	lapack
)

target_link_libraries(rclss
	${CMAKE_THREAD_LIBS_INIT}
	dlib
	blas
	lapack
)

install(TARGETS rclst DESTINATION bin)
install(TARGETS rclss DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mikhail.gorshkov@gmail.com)

include(CPack)
