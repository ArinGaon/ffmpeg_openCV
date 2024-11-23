#pragma once
#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include "event_processor.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

class VideoProcessor {
public:
    VideoProcessor();

    // 생성자 및 웹캠/비디오 파일 처리
    VideoProcessor(const std::string& videoFile);
    bool ProcessVideo();

    // 풀 영상 녹화 메서드
    bool StartFullRecording(const std::string& filename, int width, int height);
    void StopFullRecording();

    // 이벤트 감지 및 녹화 메서드
    void StartEventRecording();
    void StopEventRecording();
    void ProcessFrame(const cv::Mat& frame);

    // Qt UI에 연동할 메서드
    bool startStreaming(const std::string& url);
    void stopStreaming();


private:
    // 비디오 파일 또는 웹캠을 통해 입력을 받기 위한 변수
    std::string videoFile = "";
    EventProcessor eventProcessor; // 이벤트 처리기

    // 녹화 상태 관리 변수
    bool isRecording = false;
    cv::VideoWriter fullVideoWriter;  // 풀 영상용 VideoWriter
    cv::VideoWriter videoWriter;      // 이벤트 영상용 VideoWriter

    // 이벤트 감지용 변수
    cv::Mat previousFrame;
    std::chrono::steady_clock::time_point lastRecordingTime = std::chrono::steady_clock::now(); // 마지막 녹화 시간
    std::chrono::steady_clock::time_point eventRecordingStartTime; // 이벤트 녹화 시작 시간
    int cooldownDuration = 10;

    // 타임스탬프 생성 함수
    std::string getCurrentTimeStamp();
};

#endif // VIDEO_PROCESSOR_H
