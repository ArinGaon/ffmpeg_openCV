cmake_minimum_required(VERSION 3.10)
project(ffmpeg_openCV_Project VERSION 0.1 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 자동 기능 설정
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS src/gui)

# 라이브러리 경로 설정
set(OPENCV_DIR "C:/opencv/build")
set(FFMPEG_LIB_DIR "C:/ffmpeg/lib")
set(FFMPEG_INCLUDE_DIR "C:/ffmpeg/include")

# 소스 파일 및 헤더 파일 추가
set(SOURCE_FILES 
    # 영상 소스 로직
    # src/core/main.cpp # logic
    src/core/video_processor.cpp  
    src/core/video_processor.h
    # 이벤트 처리 로직
    src/core/event_processor.cpp
    src/core/event_processor.h

    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui    
    src/gui/main.cpp # GUI
)

# 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/src/gui)
include_directories(${CMAKE_SOURCE_DIR}/src/core)

# CMake path 설정
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2022_64" "C:/opencv/build/x64/vc16/lib")

# OpenCV 설정
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# FFmpeg 라이브러리 찾기
find_library(AVCODEC_LIB avcodec HINTS ${FFMPEG_LIB_DIR})
find_library(AVFORMAT_LIB avformat HINTS ${FFMPEG_LIB_DIR})
find_library(AVUTIL_LIB avutil HINTS ${FFMPEG_LIB_DIR})
find_library(SWSCALE_LIB swscale HINTS ${FFMPEG_LIB_DIR})

# FFmpeg Include 디렉토리 추가
include_directories(${FFMPEG_INCLUDE_DIR})

# Qt 라이브러리 설정
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 실행 파일 생성
qt_add_executable(ffmpeg_openCV_Project
    ${SOURCE_FILES}
)

# 라이브러리 링크 설정
target_link_libraries(ffmpeg_openCV_Project 
    PRIVATE 
    Qt6::Widgets 
    ${OpenCV_LIBS} 
    ${AVCODEC_LIB} 
    ${AVFORMAT_LIB} 
    ${AVUTIL_LIB} 
    ${SWSCALE_LIB}
)

# 설치 설정
install(TARGETS ffmpeg_openCV_Project
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6용 최종 실행 파일 설정
# qt_finalize_executable(ffmpeg_openCV_Project)

# 문자 인코딩 명시
add_compile_options(/utf-8)
