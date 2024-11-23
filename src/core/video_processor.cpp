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
        eventProcessor.processFrame(frame);

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

// Qt startStreaming ��ư �޼���
bool VideoProcessor::startStreaming(const std::string& url) {
    // ��Ʈ���� ����
    std::cout << "Starting Streaming to URL : " << url << std::endl;
    return true; // �����ϸ� true
}

// Qt stopStreaming ��ư �޼���
void VideoProcessor::stopStreaming() {
    // ��Ʈ���� ����
    std::cout << "Streaming stopped." << std::endl;
}


