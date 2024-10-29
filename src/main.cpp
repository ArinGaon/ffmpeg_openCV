#include <opencv2/opencv.hpp>
#include "video_processor.h"
#include <iostream>

int main() {
    std::string videoFile = "";  // 비디오 파일 경로 (웹캠 사용 시 비워 두세요)
    VideoProcessor videoProcessor(videoFile);

    // 전체 비디오 녹화 및 이벤트 감지 시작
    if (!videoProcessor.processVideo()) {
        std::cerr << "Failed to process video." << std::endl;
        return -1;
    }

    std::cout << "Program exited and full recording saved." << std::endl;
    return 0;
}
