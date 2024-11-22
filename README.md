# ffmpeg_openCV Project

-----

## 프로젝트 개요

    영상을 실시간으로 분석하여 사람이나 객체를 감지하고, 특정 행동을 탐지한 경우, 상황에 맞는 대처를 하는 C++기반 프로젝트

    기술 스택

    * FFmpeg : 영상을 받아오는 역할, 여러 형식의 비디오를 디코딩하고 재생 및 저장함
    * OpenCV : 객체감지, 움직임 분석, 얼굴인식, 특정 패턴 분석 등을 처리
    * Cmake : 다양한 파일들을 하나의 빌드파일로 빌드
    * Nginx : RTMP 프로토콜을 사용해 서버로 웹캠영상을 전송
    * Qt : C++ GUI 구성을 위한 라이브러리

## 주요 기능

    1. 실시간 영상 스트리밍
        * FFmpeg을 이용해 카메라에서 실시간으로 영상 스트림을 받아옴
        * 다양한 비디오 포맷을 지원하도록 구현

    2. 객체 및 행동 감지
        * OpenCV를 활용하여 프레임 단위로 객체를 감지
        * 특정한 움직임을 분석해 이상행동 탐지
    
    3. 알림
        * 객체가 감지되거나 이상행동이 탐지되면 즉시 사용자에게 알림 전송
        * 소리로 경고음을 울리거나 시스템 로그로 기록

    4. 저장 및 로그 관리
        * 특정 이벤트가 발생했을 때, 해당 영상의 클립을 FFmpeg으로 추출하여 저장
        * 날짜와 시간별로 로그 파일을 생성하여 나중에 분석
        * 클라우드에 저장하거나 로컬 서버에 백업

    5. 사용자 인터페이스 
        * 웹 UI or 어플리케이션 or 데스크탑 GUI를 통해 영상을 확인
        * 감지된 객체, 경고 내역, 알림이 온 내역을 UI를 통해 보여주고 사용자가 관리

    ### 확장 기능
        * 추적 기능 : 감지된 객체를 추적하며, 이동경로를 시각적으로 표시 
        * 딥러닝 모델을 활용하여 행동패턴을 학습하고 정확한 이상행동을 감지
        * 한 대의 카메라가 아닌 다중 카메라를 지원하여 여러 영상 스트림을 동시에 분석

### 가이드라인 (CmakeList.txt에 따른 권장사항 & 서버구동 필수사항) 

    * CMake 설치 후 bash의 경로를 이 레포지토리가 저장된 경로로 지정하고 Cmake로 빌드
    * Ngnix 설치 후, 
        ** config 폴더 내의 nginx.conf 파일을 ngnix 폴더의 conf 폴더 내로 복사
        ** html 폴더 내의 live_stream.html파일을 ngnix 폴더의 html 폴더 내로 복사
    * Cmake, openCV, ffmpeg은 모두 C:\ 경로로 저장하는 것을 권장함
    * 서버를 구동하기 위한 ffmpeg 명령어는 ffmpeg -re (-f dshow -i video="Microsoft® LifeCam HD-3000") -c:v libx264 -f flv rtmp://localhost:1935/hls/stream 의 형식을 가짐
        ** 괄호 부분은 웹캠을 끌어올 때 사용, 일반 영상을 송출하고 싶으면 -i "파일명"의 형식으로 지정하면 됨
        ** libx264는 코덱부분, 위 예제에서는 H264코덱으로 송출하겠다는 의미로 본인의 상황에 맞게 변경가능
        ** 서버 구동 시, nginx.exe를 실행하는것보다 terminal을 열어서 명령어로 구동하는 것이 진행상황을 확실하게 확인할 수 있음
    * VS2022에서 Qt VS Tools 확장프로그램 설치 요망

###### 특이사항

    * src 내의 코드는 모두 visual studio 2022 에서 작업하였고 이에 openCV 환경변수 경로는 x64/vc16으로 설정해주어야함
    * C++을 제외한 코드는 vs code에서 작성
    * CMakeLists.txt에 명시된 라이브러리 경로는 로컬 컴퓨터에 따라 설정해주어야 함 (C: 권장)
    * openCV 버전 4.10.0, ffmpeg 7.1full-build-shared 사용
    * Qt 라이브러리 버전 6.8.0 버전 사용 -> installer에서 6.8.0버전 사용 
        ** 5버전 이하의 Qt를 사용하여 편집할 경우 CMakeLists 편집필요(if구문 추가해서 버전을 맞춰줘야함)

##### 다운로드 링크

    * Ngnix-RTMP Windows : https://github.com/illuspas/nginx-rtmp-win32
    * openCV (ver 4.10.0) : https://opencv.org/releases/
    * ffmpeg : https://github.com/GyanD/codexffmpeg/releases/tag/2024-10-31-git-87068b9600 -> full_build
    * Cmake : https://cmake.org/download/
    * QT (ver 6.8.0) : https://www.qt.io/download-dev#eval-form
