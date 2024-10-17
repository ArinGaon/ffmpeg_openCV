#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>

class VideoProcessor {
public:
    VideoProcessor(const std::string& videoFile);
    bool processVideo();

private:
    std::string videoFile;
};

#endif // VIDEO_PROCESSOR_H
