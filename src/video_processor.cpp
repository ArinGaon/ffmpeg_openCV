#include "video_processor.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>

VideoProcessor::VideoProcessor(const std::string& videoFile) : videoFile(videoFile) {}

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
        ProcessFrame(frame);

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
void VideoProcessor::StartFullRecording(const std::string& filename, int width, int height) {
    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    fullVideoWriter.open(filename, codec, 30, cv::Size(width, height));

    if (!fullVideoWriter.isOpened()) {
        std::cerr << "Error: Could not open full recording video writer." << std::endl;
    }
    else {
        std::cout << "Full recording started: " << filename << std::endl;
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

// 이벤트 비디오 녹화 시작
void VideoProcessor::StartEventRecording() {
    std::string filename = "event_record_" + getCurrentTimeStamp() + ".mp4";

    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    videoWriter.open(filename, codec, 30, cv::Size(640, 480));

    if (videoWriter.isOpened()) {
        isRecording = true;
        eventRecordingStartTime = std::chrono::steady_clock::now();  // 녹화 시작 시간 기록
    }
    else {
        std::cerr << "Error: Could not open the videowriter with the specified codec" << "\n";
    }
}

// 이벤트 비디오 녹화 종료
void VideoProcessor::StopEventRecording() {
    isRecording = false;
    videoWriter.release();
}

// 현재 타임스탬프 생성
std::string VideoProcessor::getCurrentTimeStamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

// 이벤트 감지 및 프레임 처리
void VideoProcessor::ProcessFrame(const cv::Mat& frame) {
    cv::Mat grayFrame, diffFrame, thresholdFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    if (previousFrame.empty()) {
        grayFrame.copyTo(previousFrame);
        return;
    }

    cv::absdiff(previousFrame, grayFrame, diffFrame);
    cv::threshold(diffFrame, thresholdFrame, 30, 255, cv::THRESH_BINARY);
    int movementPixels = cv::countNonZero(thresholdFrame);

    // 현재 시간 확인 및 이벤트 감지
    auto now = std::chrono::steady_clock::now();
    bool eventDetected = movementPixels > 5000;

    // 쿨다운 적용 및 녹화 지속 시간 제한
    if (eventDetected && !isRecording && std::chrono::duration_cast<std::chrono::seconds>(now - lastRecordingTime).count() >= cooldownDuration) {
        StartEventRecording();  // 이벤트 녹화 시작
        lastRecordingTime = now; // 마지막 녹화 시간 갱신
    }
    else if (isRecording) {
        // 녹화 중일 때, 녹화 지속 시간이 7초를 넘으면 종료
        if (std::chrono::duration_cast<std::chrono::seconds>(now - eventRecordingStartTime).count() >= 7) {
            StopEventRecording();
        }
        else {
            videoWriter.write(frame);  // 7초가 지나기 전까지는 녹화 유지
        }
    }

    grayFrame.copyTo(previousFrame);
}
