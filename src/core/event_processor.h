#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

class EventProcessor{
public:
	// �⺻ ������
	// ������
	EventProcessor(const std::string& savePath, int cooldownDuration = 5, int eventDuration = 7);

	// �̺�Ʈ ������ ó��
	void processFrame(const cv::Mat& frame);

	// ��ȭ���� Ȯ��
	bool isRecording() const;

	// �̺�Ʈ ��ȭ ����
	void startEventRecording(const cv::Mat& frame);

	// �̺�Ʈ ��ȭ ����
	void stopEventRecording();

	// ���� ��� ���� �޼��� �߰�
	void setSavePath(const std::string& newSavePath); // ��� ����

private:
	std::string savePath;
	cv::VideoWriter eventWriter;
	cv::Mat previousFrame;
	bool recording;		
	int cooldownDuration;
	int eventDuration;

	std::chrono::steady_clock::time_point lastRecordingTime;
	std::chrono::steady_clock::time_point recordingStartTime;

	// ���� �޼���
	std::string generateFileName() const;
};

#endif // EVENT_H