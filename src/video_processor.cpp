#include "video_processor.h"

VideoProcessor::VideoProcessor(const std::string& videoFile) : videoFile(videoFile) {}

bool VideoProcessor::processVideo() {
    cv::VideoCapture cap;

    // 비디오 파일을 처리하는 경우
    if (!videoFile.empty()) {
        cap.open(videoFile);
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file: " << videoFile << std::endl;
            return false;
        }
    }
    // 비디오 파일이 없으면 카메라 입력 처리
    else {
        cap.open(0); // 기본 카메라 장치 열기
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera!" << std::endl;
            return false;
        }
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // 다음 프레임 가져오기
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty!" << std::endl;
            break; // 프레임이 비어 있으면 종료
        }

        // 그레이스케일 입력을 RGB로 변환
        if (frame.channels() == 1) {
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // 영상 분석 및 처리 (예: 프레임을 화면에 표시)
        cv::imshow("Video Feed", frame);

        // ESC 키 또는 아무 키나 입력하면 종료
        if (cv::waitKey(30) >= 0) {
            break;
        }
    }

    return true;
}
