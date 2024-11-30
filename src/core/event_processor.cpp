#include "event_processor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

// 생성자
EventProcessor::EventProcessor(const std::string& savePath, int cooldownDuration, int eventDuration)
    : savePath(savePath), cooldownDuration(cooldownDuration), eventDuration(eventDuration), recording(false) {}

// 이벤트 감지 및 프레임 처리
void EventProcessor::processFrame(const cv::Mat& frame) {
    cv::Mat grayFrame, diffFrame, thresholdFrame;

    // 현재 프레임을 그레이스케일로 변환
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    // 이전 프레임이 없으면 초기화
    if (previousFrame.empty()) {
        grayFrame.copyTo(previousFrame);
        return;
    }

    // 프레임 차이 계산
    cv::absdiff(previousFrame, grayFrame, diffFrame);
    cv::threshold(diffFrame, thresholdFrame, 30, 255, cv::THRESH_BINARY);

    int movementPixels = cv::countNonZero(thresholdFrame);

    // 현재 시간 계산
    auto now = std::chrono::steady_clock::now();

    // 이벤트 감지 및 녹화 처리
    if (movementPixels > 5000 && !recording &&
        std::chrono::duration_cast<std::chrono::seconds>(now - lastRecordingTime).count() >= cooldownDuration) {
        startEventRecording(frame); // 녹화 시작
        lastRecordingTime = now;
    }
    else if (recording) {
        // 녹화 중이라면 지속 시간 확인
        if (std::chrono::duration_cast<std::chrono::seconds>(now - recordingStartTime).count() >= eventDuration) {
            stopEventRecording();
        }
        else {
            eventWriter.write(frame); // 녹화 유지
        }
    }

    grayFrame.copyTo(previousFrame); // 현재 프레임 저장
}

// 녹화 상태 확인
bool EventProcessor::isRecording() const {
    return recording;
}

// 이벤트 녹화 시작
void EventProcessor::startEventRecording(const cv::Mat& frame) {
    std::string fileName = generateFileName();

    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    eventWriter.open(savePath + "/" + fileName, codec, 30, frame.size());

    if (eventWriter.isOpened()) {
        recording = true;
        recordingStartTime = std::chrono::steady_clock::now();
        std::cout << "Event recording started: " << fileName << std::endl;
    }
    else {
        std::cerr << "Error: Failed to start event recording." << std::endl;
    }
}

// 이벤트 녹화 종료
void EventProcessor::stopEventRecording() {
    if (recording) {
        recording = false;
        eventWriter.release();
        std::cout << "Event recording stopped." << std::endl;
    }
}

// 이벤트 파일명 생성
std::string EventProcessor::generateFileName() const {
    // 이벤트 처리시 타임스탬프 생성
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".mp4";
    return ss.str();
}

// 이벤트 record 경로 업데이트
void EventProcessor::setSavePath(const std::string& newSavePath) {
    savePath = newSavePath; // 경로 업데이트
    std::cout << "Event save path updated to: " << savePath << std::endl;
}
