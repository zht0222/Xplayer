#include "XVideoThread.h"
#include "XFFmpeg.h"
#include "XAudioPlay.h"
#include <list>
static bool isexit = true;
static list<AVPacket> videos;
XVideoThread::XVideoThread():QThread()
{
}

XVideoThread::~XVideoThread()
{
	isexit = false;
	msleep(100);
	wait();
}

XVideoThread * XVideoThread::get()
{
	static XVideoThread vt;
	return &vt;
}

void XVideoThread::run()
{
	char out[10000] = { 0 };
	while (isexit)
	{
		if (!XFFmpeg::get()->isplay)
		{
			msleep(10);
			continue;
		}
		while (videos.size() > 0)
		{
			AVPacket pack = videos.front();
			int pts = XFFmpeg::get() ->getPts(pack);
			if (pts > XFFmpeg::get()->pts)
			{
				break;
			}
			XFFmpeg::get()->decode(&pack);
			av_packet_unref(&pack);
			videos.pop_front();
		}

		int free = XAudioPlay::get()->getFree();
		if (free < 4096)
		{
			msleep(1);
			continue;
		}
		AVPacket pkt = XFFmpeg::get()->read();
		if (pkt.size <= 0)
		{
			msleep(10);
			continue;
		}
		if (pkt.stream_index == XFFmpeg::get()->audioStream)
		{
			XFFmpeg::get()->decode(&pkt);
			av_packet_unref(&pkt);
			int datasize = XFFmpeg::get()->toPcm(out);
			XAudioPlay::get()->write(out, datasize);
			continue;
		}
		if (pkt.stream_index != XFFmpeg::get()->videoStream)
		{
			av_packet_unref(&pkt);
			continue;
		}
		videos.push_back(pkt);
	}
}