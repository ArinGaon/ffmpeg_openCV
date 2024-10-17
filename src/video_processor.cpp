#include "video_processor.h"

VideoProcessor::VideoProcessor(const std::string& videoFile) : videoFile(videoFile) {}

bool VideoProcessor::processVideo() {
    cv::VideoCapture cap;

    // ���� ������ ó���ϴ� ���
    if (!videoFile.empty()) {
        cap.open(videoFile);
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file: " << videoFile << std::endl;
            return false;
        }
    }
    // ���� ������ ������ ī�޶� �Է� ó��
    else {
        cap.open(0); // �⺻ ī�޶� ��ġ ����
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera!" << std::endl;
            return false;
        }
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // ���� ������ ��������
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty!" << std::endl;
            break; // �������� ��� ������ ����
        }

        // �׷��̽����� �Է��� RGB�� ��ȯ
        if (frame.channels() == 1) {
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // ���� �м� �� ó�� (��: �������� ȭ�鿡 ǥ��)
        cv::imshow("Video Feed", frame);

        // ESC Ű �Ǵ� �ƹ� Ű�� �Է��ϸ� ����
        if (cv::waitKey(30) >= 0) {
            break;
        }
    }

    return true;
}
