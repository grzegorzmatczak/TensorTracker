cmake_minimum_required (VERSION 3.8)
project(TensorTracker VERSION 0.0.1 DESCRIPTION "TensorTracker" LANGUAGES C CXX)

set(TARGET_NAME "TensorTracker")

MESSAGE(STATUS "CMAKE_VERSION: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_OPTIONS}")

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${OpenCV_DIR})
find_package(OpenCV REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
find_package(Qt5 REQUIRED COMPONENTS Core)

list(APPEND CMAKE_PREFIX_PATH ${gtest_DIR_CUSTOM})
find_package(GTest REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${utils2_DIR_CUSTOM})
find_package(utils2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${processing2_DIR_CUSTOM})
find_package(processing2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${graph2_DIR_CUSTOM})
find_package(graph2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${image_algorithms_DIR_CUSTOM})
find_package(image_algorithms REQUIRED)

macro(copy_json jsonname jsonpath)
    configure_file(${jsonpath}${jsonname} ${CMAKE_BUILD_TYPE}/${jsonname})
endmacro()

copy_json("config.json" "")

include_directories( "src" )
include_directories( "include" )

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
    include/tracker_global.h
    include/service.h
    include/camera_capture.h
    include/camera_thread.h
    include/frame_processing.h
    include/frame_median.h
    include/frame_source.h
    include/frame_reader.h
)
add_executable(${PROJECT_NAME} 
    ${SRC_MAIN} ${HEADERS_MAIN}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${utils2_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${processing2_DIR_CUSTOM}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${graph2_DIR_CUSTOM}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${image_algorithms_DIR_CUSTOM}/include)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE  
    Qt::Core
    Threads::Threads 
    utils2
    image_algorithms
    graph2
    processing2
    ${OpenCV_LIBS}
)

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR:" ${QT_BIN_DIR})
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(PREFIX_BUILD_TYPE "d")
    endif()
    message(STATUS "OPENCV_VERSION_WINDOWS:" ${OPENCV_VERSION_WINDOWS})
    message(STATUS "PREFIX_BUILD_TYPE:" ${PREFIX_BUILD_TYPE})
    
    set(CUSTOM_BIN_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    add_custom_command ( TARGET ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${utils2_DIR_CUSTOM}lib/utils2.dll                   ${CUSTOM_BIN_DIR}  

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Test${PREFIX_BUILD_TYPE}.dll        ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Gui${PREFIX_BUILD_TYPE}.dll         ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll  ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Network${PREFIX_BUILD_TYPE}.dll     ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Core${PREFIX_BUILD_TYPE}.dll        ${CMAKE_BUILD_TYPE}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll     ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll        ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll              ${CMAKE_BUILD_TYPE}
    )
endif(WIN32)