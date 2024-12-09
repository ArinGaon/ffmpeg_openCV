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

    // ������ �� ��ķ/���� ���� ó��
    VideoProcessor(const std::string& videoFile);
    bool ProcessVideo();

    // Ǯ ���� ��ȭ �޼���
    bool StartFullRecording(const std::string& filename);
    void StopFullRecording();

    // Qt UI�� ������ �޼���
    bool startStreaming(const std::string& url);
    void stopStreaming();

    // ���� ��� ���� �޼��� �߰�
    void setEventSavePath(const std::string& path); // �̺�Ʈ ��� ����

private:
    // ���� ���� �Ǵ� ��ķ�� ���� �Է��� �ޱ� ���� ����
    std::string videoFile = "";
    EventProcessor eventProcessor; // �̺�Ʈ ó����
    double frameWidth, frameHeight; 

    // ��ȭ ���� ���� ����
    bool isRecording = false;
    cv::VideoWriter fullVideoWriter;  // Ǯ ����� VideoWriter
    cv::VideoWriter videoWriter;      // �̺�Ʈ ����� VideoWriter

    // �̺�Ʈ ������ ����
    cv::Mat previousFrame;
    std::chrono::steady_clock::time_point lastRecordingTime = std::chrono::steady_clock::now(); // ������ ��ȭ �ð�
    std::chrono::steady_clock::time_point eventRecordingStartTime; // �̺�Ʈ ��ȭ ���� �ð�
    int cooldownDuration = 10;

    // Ÿ�ӽ����� ���� �Լ�
    std::string getCurrentTimeStamp();
};

#endif // VIDEO_PROCESSOR_H
