#include "event_processor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

// ������
EventProcessor::EventProcessor(const std::string& savePath, int cooldownDuration, int eventDuration)
    : savePath(savePath), cooldownDuration(cooldownDuration), eventDuration(eventDuration), recording(false) {}

// �̺�Ʈ ���� �� ������ ó��
void EventProcessor::processFrame(const cv::Mat& frame) {
    cv::Mat grayFrame, diffFrame, thresholdFrame;

    // ���� �������� �׷��̽����Ϸ� ��ȯ
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    // ���� �������� ������ �ʱ�ȭ
    if (previousFrame.empty()) {
        grayFrame.copyTo(previousFrame);
        return;
    }

    // ������ ���� ���
    cv::absdiff(previousFrame, grayFrame, diffFrame);
    cv::threshold(diffFrame, thresholdFrame, 30, 255, cv::THRESH_BINARY);

    int movementPixels = cv::countNonZero(thresholdFrame);

    // ���� �ð� ���
    auto now = std::chrono::steady_clock::now();

    // �̺�Ʈ ���� �� ��ȭ ó��
    if (movementPixels > 5000 && !recording &&
        std::chrono::duration_cast<std::chrono::seconds>(now - lastRecordingTime).count() >= cooldownDuration) {
        startEventRecording(frame); // ��ȭ ����
        lastRecordingTime = now;
    }
    else if (recording) {
        // ��ȭ ���̶�� ���� �ð� Ȯ��
        if (std::chrono::duration_cast<std::chrono::seconds>(now - recordingStartTime).count() >= eventDuration) {
            stopEventRecording();
        }
        else {
            eventWriter.write(frame); // ��ȭ ����
        }
    }

    grayFrame.copyTo(previousFrame); // ���� ������ ����
}

// ��ȭ ���� Ȯ��
bool EventProcessor::isRecording() const {
    return recording;
}

// �̺�Ʈ ��ȭ ����
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

// �̺�Ʈ ��ȭ ����
void EventProcessor::stopEventRecording() {
    if (recording) {
        recording = false;
        eventWriter.release();
        std::cout << "Event recording stopped." << std::endl;
    }
}

// �̺�Ʈ ���ϸ� ����
std::string EventProcessor::generateFileName() const {
    // �̺�Ʈ ó���� Ÿ�ӽ����� ����
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".mp4";
    return ss.str();
}

// �̺�Ʈ record ��� ������Ʈ
void EventProcessor::setSavePath(const std::string& newSavePath) {
    savePath = newSavePath; // ��� ������Ʈ
    std::cout << "Event save path updated to: " << savePath << std::endl;
}
