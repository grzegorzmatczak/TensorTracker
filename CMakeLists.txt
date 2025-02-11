cmake_minimum_required (VERSION 3.8)
project(TensorTracker VERSION 0.0.1 DESCRIPTION "TensorTracker" LANGUAGES C CXX)

set(TARGET_NAME "TensorTracker")
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(utils2)
include_directories( "utils2/include" )

macro(copy_json jsonname)
    configure_file(${jsonname} ${CMAKE_BINARY_DIR}/${jsonname})
    configure_file(${jsonname} ${CMAKE_BUILD_TYPE}/${jsonname})
    configure_file(${jsonname} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${jsonname})
endmacro()

copy_json("config.json")

if (WIN32)
    set(QT5_PATH  "D:\\Qt5\\5.15.2\\msvc2019_64")
    list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH})
    set(OpenCV_DIR "D:\\repo\\opencv-4.10.0-build")
    set(QMAKE_EXE  "D:\\Qt5\\5.15.2\\msvc2019_64\\bin\\qmake.exe")
endif (WIN32)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia Network)
find_package(OpenCV REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")
include_directories(${OpenCV_INCLUDE_DIRS})

set(SRC_MAIN main.cpp "src/service.cpp" "include/service.h" "src/camera_capture.cpp" "include/camera_capture.h" "global.h" "src/camera_thread.cpp" "include/camera_thread.h")
set(HEADERS_MAIN)


add_executable(${PROJECT_NAME} ${SRC_MAIN})
target_include_directories(${PROJECT_NAME}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE  Qt::Core Qt::Widgets Qt::Multimedia Qt::Network Threads::Threads utils2_lib ${OpenCV_LIBS})

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR" ${QT_BIN_DIR} )
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "d")
    set(OPENCV_PREFIX_BUILD_TYPE "d")
    message("Copy OpenCV file:" )
    
    set(OPENCV_VERSION_WINDOWS "4100")

    MESSAGE(STATUS "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
    MESSAGE(STATUS "CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
    MESSAGE(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")

    set(QT5_CORE ${QT5_PATH}/bin/Qt5Core${PREFIX_BUILD_TYPE}.dll)
    MESSAGE(STATUS "QT5_CORE ${QT5_CORE}")
    add_custom_command ( TARGET ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Network${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_CORE} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
    )
endif(WIN32)