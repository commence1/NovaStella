cmake_minimum_required(VERSION 3.10)
project(NovaStella VERSION 1.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/play.cpp)

if(UNIX AND NOT WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME novastella-bin)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC Includes/)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image fmt::fmt-header-only)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

if(WIN32)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
	install(DIRECTORY image/ DESTINATION image)
	install(FILES LICENSE README.md DESTINATION .)
else()
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(PROGRAMS ${CMAKE_BINARY_DIR}/novastella DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(DIRECTORY image/ DESTINATION ${CMAKE_INSTALL_DATADIR}/novastella/image)
	install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/novastella/audio)
	install(FILES LICENSE README.md DESTINATION ${CMAKE_INSTALL_DATADIR}/novastella)
endif()

find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIMP3_INCLUDE_DIRS})

if(UNIX AND NOT WIN32)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/novastella.sh.in ${CMAKE_BINARY_DIR}/novastella @ONLY)

	set(CPACK_GENERATOR "DEB")
	set(CPACK_PACKAGE_NAME "novastella")
	set(CPACK_PACKAGE_VENDOR "NovaStella")
	set(CPACK_PACKAGE_CONTACT "ahuaoih@outlook.com")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ahuaoih@outlook.com")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NovaStella music player")
	set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
	set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
	set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

	include(CPack)
endif()
