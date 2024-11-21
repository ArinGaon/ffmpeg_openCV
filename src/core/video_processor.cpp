#include "video_processor.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>

VideoProcessor::VideoProcessor(const std::string& videoFile) : videoFile(videoFile) {}

// ��ķ �Ǵ� ���� ���Ͽ��� ������ �޾ƿ���
bool VideoProcessor::ProcessVideo() {
    cv::VideoCapture cap;

    if (!videoFile.empty()) {
        cap.open(videoFile);  // ���� ���� ����
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file: " << videoFile << std::endl;
            return false;
        }
    }
    else {
        cap.open(0);  // �⺻ ��ķ ����
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera!" << std::endl;
            return false;
        }
    }

    // VideoCapture�� �ػ󵵸� ������� VideoWriter ����
    double frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // full recording �ʱ�ȭ
    StartFullRecording("full_recording.mp4", frameWidth, frameHeight);

    cv::Mat frame;
    while (true) {
        cap >> frame;  // ���� ������ ��������
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty!" << std::endl;
            break;
        }

        if (frame.channels() == 1) {
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // �̺�Ʈ ���� �� ��ȭ ó��
        ProcessFrame(frame);

        // �������� ȭ�鿡 ǥ��
        cv::imshow("Video Feed", frame);

        // ESC (27) �Ǵ� 'q' (113) Ű�� ������ ����
        int key = cv::waitKey(30);
        if (key == 27 || key == 'q') {  // 27: ESC Ű, 'q': ASCII �� 113
            break;
        }

        // full recording���� ������ ����
        fullVideoWriter.write(frame);
    }

    cap.release();  // ī�޶� �ڿ� ����
    cv::destroyAllWindows();  // ��� OpenCV â �ݱ�

    StopFullRecording();
    return true;
}


// ��ü ���� ��ȭ ����
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

// �̺�Ʈ ���� ��ȭ ����
void VideoProcessor::StartEventRecording() {
    std::string filename = "event_record_" + getCurrentTimeStamp() + ".mp4";

    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    videoWriter.open(filename, codec, 30, cv::Size(640, 480));

    if (videoWriter.isOpened()) {
        isRecording = true;
        eventRecordingStartTime = std::chrono::steady_clock::now();  // ��ȭ ���� �ð� ���
    }
    else {
        std::cerr << "Error: Could not open the videowriter with the specified codec" << "\n";
    }
}

// �̺�Ʈ ���� ��ȭ ����
void VideoProcessor::StopEventRecording() {
    isRecording = false;
    videoWriter.release();
}

// ���� Ÿ�ӽ����� ����
std::string VideoProcessor::getCurrentTimeStamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

// �̺�Ʈ ���� �� ������ ó��
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

    // ���� �ð� Ȯ�� �� �̺�Ʈ ����
    auto now = std::chrono::steady_clock::now();
    bool eventDetected = movementPixels > 5000;

    // ��ٿ� ���� �� ��ȭ ���� �ð� ����
    if (eventDetected && !isRecording && std::chrono::duration_cast<std::chrono::seconds>(now - lastRecordingTime).count() >= cooldownDuration) {
        StartEventRecording();  // �̺�Ʈ ��ȭ ����
        lastRecordingTime = now; // ������ ��ȭ �ð� ����
    }
    else if (isRecording) {
        // ��ȭ ���� ��, ��ȭ ���� �ð��� 7�ʸ� ������ ����
        if (std::chrono::duration_cast<std::chrono::seconds>(now - eventRecordingStartTime).count() >= 7) {
            StopEventRecording();
        }
        else {
            videoWriter.write(frame);  // 7�ʰ� ������ �������� ��ȭ ����
        }
    }

    grayFrame.copyTo(previousFrame);
}
