#include <iostream>
#include <opencv2/opencv.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "ffmpeg_helper.h"

int main() {
    const char* deviceName = "video=Microsoft® LifeCam HD-3000";  // Windows의 웹캠 장치명

    // FFmpeg 초기화
    avformat_network_init();

    AVFormatContext* formatContext = open_input(deviceName);
    if (!formatContext) {
        std::cerr << "Failed to open camera." << std::endl;
        return -1;
    }

    AVCodecContext* codecContext = get_video_codec_context(formatContext);
    if (!codecContext) {
        avformat_close_input(&formatContext);
        return -1;
    }

    SwsContext* swsCtx = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_BGR24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame* frame = av_frame_alloc();
    AVPacket packet;
    cv::Mat img(codecContext->height, codecContext->width, CV_8UC3);

    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == 0) {
            avcodec_send_packet(codecContext, &packet);
            if (avcodec_receive_frame(codecContext, frame) == 0) {
                uint8_t* data[AV_NUM_DATA_POINTERS] = { img.data };
                int linesize[AV_NUM_DATA_POINTERS] = { img.step };
                sws_scale(swsCtx, frame->data, frame->linesize, 0, codecContext->height, data, linesize);

                cv::imshow("Video", img);
                if (cv::waitKey(25) >= 0) break;
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    sws_freeContext(swsCtx);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_network_deinit();

    return 0;
}
