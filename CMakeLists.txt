cmake_minimum_required(VERSION 3.10)
project(ffmpeg_openCV_Project) 

# OpenCV 라이브러리 경로 설정
set(OPENCV_DIR "C:/opencv/build")
# FFmpeg 라이브러리 경로 설정
set(FFMPEG_LIB_DIR "C:/ffmpeg/lib")
set(FFMPEG_INCLUDE_DIR "C:/ffmpeg/include")

# 소스파일 추가
set(SOURCE_FILES src/main.cpp src/video_processor.cpp src/video_processor.h)

find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# FFmpeg 라이브러리 찾기
find_library(AVCODEC_LIB avcodec HINTS ${FFMPEG_LIB_DIR})
find_library(AVFORMAT_LIB avformat HINTS ${FFMPEG_LIB_DIR})
find_library(AVUTIL_LIB avutil HINTS ${FFMPEG_LIB_DIR})
find_library(SWSCALE_LIB swscale HINTS ${FFMPEG_LIB_DIR})

# Include 디렉토리 추가
include_directories(${FFMPEG_INCLUDE_DIR})

# Executable 추가
add_executable(ffmpeg_openCV_Project ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(ffmpeg_openCV_Project ${OpenCV_LIBS} ${AVCODEC_LIB} ${AVFORMAT_LIB} ${AVUTIL_LIB} ${SWSCALE_LIB})