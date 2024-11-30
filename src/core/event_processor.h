#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

class EventProcessor{
public:
	// 기본 생성자
	// 생성자
	EventProcessor(const std::string& savePath, int cooldownDuration = 5, int eventDuration = 7);

	// 이벤트 프레임 처리
	void processFrame(const cv::Mat& frame);

	// 녹화상태 확인
	bool isRecording() const;

	// 이벤트 녹화 시작
	void startEventRecording(const cv::Mat& frame);

	// 이벤트 녹화 종료
	void stopEventRecording();

	// 저장 경로 설정 메서드 추가
	void setSavePath(const std::string& newSavePath); // 경로 변경

private:
	std::string savePath;
	cv::VideoWriter eventWriter;
	cv::Mat previousFrame;
	bool recording;		
	int cooldownDuration;
	int eventDuration;

	std::chrono::steady_clock::time_point lastRecordingTime;
	std::chrono::steady_clock::time_point recordingStartTime;

	// 내부 메서드
	std::string generateFileName() const;
};

#endif // EVENT_H