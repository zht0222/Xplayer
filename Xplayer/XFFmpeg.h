#pragma once
#ifndef XFFMPEG_H
#define XFFMPEG_H
#include <iostream>
using namespace std;
extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#include <QMutex>

class XFFmpeg
{
private:
	QMutex mutex;
	enum{ buffSize = 1024 };
	struct ErrInfo
	{
		int errType = 0;
		char errInfo[buffSize] = "No error";
	} errInfo;

	AVFormatContext *ic = NULL;
	AVCodecContext * videoCtx = NULL;
	AVCodecContext * audioCtx = NULL;
	AVFrame *  yuv = NULL;
	AVFrame *  pcm = NULL;
	SwsContext * cCtx = NULL;
	SwrContext * aCtx = NULL;

	XFFmpeg();
	XFFmpeg(char *);
	int isAvError(int);
	static double r2d(AVRational);
public:
	int duration = 0;//��Ƶ����
	int pts = 0;//��Ƶ��ǰ����
	int videoStream = 0;
	int audioStream = 1;
	int fps = 0;
	bool isplay = true;
	int sampleRate = 48000;
	int sampleSize = 16;
	int channel = 2;

	virtual ~XFFmpeg();

	static XFFmpeg * get();//��ȡΨһ��
	int open(char *);//����Ƶ
	int openDecoder();//�򿪽�����
	AVPacket read();//��֡
	int decode(const AVPacket *);// ����
	int toRGB(char *, int, int);
	int toPcm(char *);
	bool seek(float);
	int getPts(AVPacket &);
	void close();//�ر��ļ�
	void logerror(int line);
};
#endif


