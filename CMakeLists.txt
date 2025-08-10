cmake_minimum_required(VERSION 3.10)
project(NovaStella VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp src/play.cpp src/vulkan-init.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC Includes/)

find_package(SDL3 REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

find_package(SDL3_image REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_image::SDL3_image)

find_package(fmt REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${fmt_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt-header-only)

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARIES})

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm-header-only)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC 
        BOOST_ALL_NO_LIB
        BOOST_LOG_USE_NATIVE_SYSLOG
    )
endif()

find_package(Boost COMPONENTS log log_setup filesystem system thread date_time regex chrono atomic REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::log
    Boost::log_setup
    Boost::filesystem
    Boost::system
    Boost::thread
    Boost::date_time
    Boost::regex
    Boost::chrono
    Boost::atomic
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/image"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/image")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -O3)
endif()