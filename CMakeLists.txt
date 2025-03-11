cmake_minimum_required (VERSION 3.8)
project(TensorTracker VERSION 0.0.1 DESCRIPTION "TensorTracker" LANGUAGES C CXX)

set(TARGET_NAME "TensorTracker")

MESSAGE(STATUS "CMAKE_VERSION: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${COMPILE_OPTIONS}" )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

macro(copy_json jsonname jsonpath)
    configure_file(${jsonpath}${jsonname} ${CMAKE_BUILD_TYPE}/${jsonname})
endmacro()

copy_json("config.json" "")
copy_json("gtest/test.json" "utils2/")

include_directories( "src" )
include_directories( "include" )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia Network)
find_package(OpenCV REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
add_subdirectory(googletest)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")

add_subdirectory(utils2)

add_subdirectory(image_algorithms)
include_directories( "image_algorithms/src" )
include_directories( "image_algorithms/include" )

add_subdirectory(processing2)
include_directories( "processing2/include" )

add_subdirectory(graph2)
include_directories( "graph2/include" )

set(SRC_MAIN
main.cpp
src/service.cpp
src/camera_capture.cpp
src/camera_thread.cpp
src/frame_processing.cpp
src/frame_median.cpp
src/frame_source.cpp
src/frame_reader.cpp
)


set(HEADERS_MAIN
global.h
include/service.h
include/camera_capture.h
include/camera_thread.h
include/frame_processing.h
include/frame_median.h
include/frame_source.h
include/frame_reader.h
)

add_executable(${PROJECT_NAME} 
    ${SRC_MAIN} ${HEADERS_MAIN})
    target_include_directories(${PROJECT_NAME}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE  
    Qt::Core Qt::Widgets Qt::Multimedia Qt::Network 
    Threads::Threads 
    utils2
    image_algorithms
    graph2
    processing2
    ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${utils2_SOURCE_DIR}/include"
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    "${utils2_SOURCE_DIR}/include"
)

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR" ${QT_BIN_DIR} )
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(PREFIX_BUILD_TYPE "d")
    endif()
    set(OPENCV_VERSION_WINDOWS "4100")

 

    set(QT5_CORE ${QT5_PATH}/bin/Qt5Core${PREFIX_BUILD_TYPE}.dll)
    MESSAGE(STATUS "utils2_DIR ${utils2_DIR}")
    add_custom_command ( TARGET ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${utils2_DIR}utils2.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Network${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_CORE} ${CMAKE_BUILD_TYPE}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll   ${CMAKE_BUILD_TYPE}
    )
endif(WIN32)