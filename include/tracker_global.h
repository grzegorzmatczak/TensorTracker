#pragma once

#if defined(UTILS2_SHARED)
#  define UTILS2_SHARED Q_DECL_EXPORT
#else
#  define UTILS2_SHARED Q_DECL_IMPORT
#endif

#if defined(PROCESSING2_SHARED)
#  define PROCESSING2_SHARED Q_DECL_EXPORT
#else
#  define PROCESSING2_SHARED Q_DECL_IMPORT
#endif

#include <opencv2/opencv.hpp>
#include "qglobal.h"
#include "graph_global.h"

constexpr auto SERVICE{ "Service" };
constexpr auto CAMERA_FPS{ "CameraFPS" };
constexpr auto CONFIG{ "config.json" };
constexpr auto CAMERA_CAPTURE_CONFIG{ "CameraCapture" };
constexpr auto DEVICE_ID{ "deviceID" };
constexpr auto CAMERA_THREAD_CONFIG{ "CameraThread" };
constexpr auto RESIZE_WIDTH{ "ResizeWidth" };
constexpr auto RESIZE_HEIGHT{ "ResizeHeight" };
constexpr auto MEDIAN_SIZE{ "MedianSize" };
constexpr auto FRAME_PROCESSING_CONFIG{ "FrameProcessing" };
constexpr auto FRAME_MEDIAN_CONFIG{ "FrameMedian" };
constexpr auto FRAME_READER_CONFIG{ "FrameReader" };
constexpr auto DATASET_WIN32{ "DatasetWin32" };
constexpr auto PATH_TO_DATASET{ "PathToDataset" };
constexpr auto CONFIG_NAME{ "ConfigName" };
constexpr auto INPUT_TYPE{ "InputType" };
constexpr auto ALL_FRAMES{ "AllFrames" };
constexpr auto DIR_CLEAN{ "DirectoryClean" };

struct ProcessingStruct
{
    cv::Mat frame;
    qint64 timestamp;
};