#include "video_processor.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>

#define EVENT_SAVE_DIRECTORY "C:/ArinGaon/Event_Recording"

VideoProcessor::VideoProcessor(const std::string& videoFile)
    : videoFile(videoFile), eventProcessor(EVENT_SAVE_DIRECTORY) {}
VideoProcessor::VideoProcessor()
    : eventProcessor(EVENT_SAVE_DIRECTORY) {}

// 웹캠 또는 비디오 파일에서 영상을 받아오기
bool VideoProcessor::ProcessVideo() {
    cv::VideoCapture cap;

    if (!videoFile.empty()) {
        cap.open(videoFile);  // 비디오 파일 열기
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file: " << videoFile << std::endl;
            return false;
        }
    }
    else {
        cap.open(0);  // 기본 웹캠 열기
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera!" << std::endl;
            return false;
        }
    }

    // VideoCapture의 해상도를 기반으로 VideoWriter 설정
    double frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // full recording 초기화
    StartFullRecording("full_recording.mp4", frameWidth, frameHeight);

    cv::Mat frame;
    while (true) {
        cap >> frame;  // 다음 프레임 가져오기
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty!" << std::endl;
            break;
        }

        if (frame.channels() == 1) {
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // 이벤트 감지 및 녹화 처리
        eventProcessor.processFrame(frame);

        // 프레임을 화면에 표시
        cv::imshow("Video Feed", frame);

        // ESC (27) 또는 'q' (113) 키가 눌리면 종료
        int key = cv::waitKey(30);
        if (key == 27 || key == 'q') {  // 27: ESC 키, 'q': ASCII 값 113
            break;
        }

        // full recording에도 프레임 저장
        fullVideoWriter.write(frame);
    }

    cap.release();  // 카메라 자원 해제
    cv::destroyAllWindows();  // 모든 OpenCV 창 닫기

    StopFullRecording();
    return true;
}


// 전체 비디오 녹화 시작
bool VideoProcessor::StartFullRecording(const std::string& filename, int width, int height) {
    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    fullVideoWriter.open(filename, codec, 30, cv::Size(width, height));

    if (!fullVideoWriter.isOpened()) {
        std::cerr << "Error: Could not open full recording video writer." << std::endl;
        return false;
    }
    else {
        std::cout << "Full recording started: " << filename << std::endl;
        return true;
    }
}

void VideoProcessor::StopFullRecording() {
    if (fullVideoWriter.isOpened()) {
        fullVideoWriter.release();
        std::cout << "Full recording stopped and file saved." << std::endl;
    }
    else {
        std::cerr << "Error: Video writer was not open." << std::endl;
    }
}

// Qt startStreaming 버튼 메서드
bool VideoProcessor::startStreaming(const std::string& url) {
    // 스트리밍 시작
    std::cout << "Starting Streaming to URL : " << url << std::endl;
    return true; // 성공하면 true
}

// Qt stopStreaming 버튼 메서드
void VideoProcessor::stopStreaming() {
    // 스트리밍 중지
    std::cout << "Streaming stopped." << std::endl;
}


