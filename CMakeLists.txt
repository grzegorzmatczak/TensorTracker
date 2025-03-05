cmake_minimum_required (VERSION 3.8)
project(TensorTracker VERSION 0.0.1 DESCRIPTION "TensorTracker" LANGUAGES C CXX)

set(TARGET_NAME "TensorTracker")

MESSAGE(STATUS "CMAKE_VERSION: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

macro(copy_json jsonname jsonpath)
    configure_file(${jsonpath}${jsonname} ${CMAKE_CONFIGURATION_TYPES}/${jsonname})
endmacro()

copy_json("config.json" "")
copy_json("gtest/test.json" "utils2/")

if (WIN32)
    set(QT5_PATH  "D:\\Qt5\\5.15.2\\msvc2019_64")
    list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH})
    set(OpenCV_DIR "D:\\repo\\opencv-4.10.0-build")
    set(QMAKE_EXE  "D:\\Qt5\\5.15.2\\msvc2019_64\\bin\\qmake.exe")
endif (WIN32)

include_directories( "src" )
include_directories( "include" )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia Network)
find_package(OpenCV REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")
include_directories(${OpenCV_INCLUDE_DIRS})

MESSAGE(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
add_subdirectory(googletest)
if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "Release")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(utils2)
include_directories( "utils2/include" )

add_subdirectory(image_algorithms)
include_directories( "image_algorithms/src" )
include_directories( "image_algorithms/include" )

add_subdirectory(graph2)
include_directories( "graph2/include" )

add_subdirectory(ProcessingModules)
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

add_executable(${PROJECT_NAME} ${SRC_MAIN} ${HEADERS_MAIN})
target_include_directories(${PROJECT_NAME}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE  
Qt::Core Qt::Widgets Qt::Multimedia Qt::Network 
Threads::Threads 
utils2_lib
image_algorithms_lib
graph2_lib
processing_module_lib
${OpenCV_LIBS})

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR" ${QT_BIN_DIR} )
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "")
    if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "Debug")
		set(PREFIX_BUILD_TYPE "d")
    endif()
    set(OPENCV_VERSION_WINDOWS "4100")

    MESSAGE(STATUS "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
    MESSAGE(STATUS "CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")

    set(QT5_CORE ${QT5_PATH}/bin/Qt5Core${PREFIX_BUILD_TYPE}.dll)
    MESSAGE(STATUS "QT5_CORE ${QT5_CORE}")
    add_custom_command ( TARGET ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Network${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_CORE} ${CMAKE_CONFIGURATION_TYPES}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_tracking${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_videoio${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_video${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_core${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_features2d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_flann${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_dnn${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_highgui${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_gapi${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_CONFIGURATION_TYPES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll   ${CMAKE_CONFIGURATION_TYPES}
    )
endif(WIN32)