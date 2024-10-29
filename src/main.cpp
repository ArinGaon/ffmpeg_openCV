#include <opencv2/opencv.hpp>
#include "video_processor.h"
#include <iostream>

int main() {
    std::string videoFile = "";  // ���� ���� ��� (��ķ ��� �� ��� �μ���)
    VideoProcessor videoProcessor(videoFile);

    // ��ü ���� ��ȭ �� �̺�Ʈ ���� ����
    if (!videoProcessor.processVideo()) {
        std::cerr << "Failed to process video." << std::endl;
        return -1;
    }

    std::cout << "Program exited and full recording saved." << std::endl;
    return 0;
}
