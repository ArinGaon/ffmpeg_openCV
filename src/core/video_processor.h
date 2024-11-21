#pragma once
#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

class VideoProcessor {
public:
    // ������ �� ��ķ/���� ���� ó��
    VideoProcessor(const std::string& videoFile);
    bool ProcessVideo();

    // Ǯ ���� ��ȭ �޼���
    void StartFullRecording(const std::string& filename, int width, int height);
    void StopFullRecording();

    // �̺�Ʈ ���� �� ��ȭ �޼���
    void StartEventRecording();
    void StopEventRecording();
    void ProcessFrame(const cv::Mat& frame);

private:
    // ���� ���� �Ǵ� ��ķ�� ���� �Է��� �ޱ� ���� ����
    std::string videoFile;

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
