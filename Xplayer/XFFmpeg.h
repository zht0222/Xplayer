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
	int duration = 0;//视频长度
	int pts = 0;//视频当前进度
	int videoStream = 0;
	int audioStream = 1;
	int fps = 0;
	bool isplay = true;
	int sampleRate = 48000;
	int sampleSize = 16;
	int channel = 2;

	virtual ~XFFmpeg();

	static XFFmpeg * get();//获取唯一性
	int open(char *);//打开视频
	int openDecoder();//打开解码器
	AVPacket read();//读帧
	int decode(const AVPacket *);// 解码
	int toRGB(char *, int, int);
	int toPcm(char *);
	bool seek(float);
	int getPts(AVPacket &);
	void close();//关闭文件
	void logerror(int line);
};
#endif


