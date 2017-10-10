#include "XFFmpeg.h"
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

//默认构造函数
XFFmpeg::XFFmpeg()
{
	av_register_all();
}

//自定义构造函数
XFFmpeg::XFFmpeg(char * path)
{
	av_register_all();
	open(path);
}

//析构函数
XFFmpeg::~XFFmpeg()
{
}

//获取
XFFmpeg * XFFmpeg::get()
{
	static XFFmpeg ffmpeg;
	return &ffmpeg;
}

//打开视频
int XFFmpeg::open(char *path)
{
	close();
	mutex.lock();
	int re = avformat_open_input(&ic, path, NULL, NULL);
	if (isAvError(re))
	{
		mutex.unlock();
		cout << "打开视频失败" << endl;
		return 0;
	}
	duration = ic->duration / AV_TIME_BASE * 1000;
	if (!openDecoder() || !videoCtx || !audioCtx)
	{
		mutex.unlock();
		cout << "打开解码器失败" << endl;
		return 0;
	}
	mutex.unlock();
	return 1;
}

//打开视频解码器
int XFFmpeg::openDecoder()
{
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVCodecContext * avctx = ic->streams[i]->codec;
		if (avctx->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			videoCtx = avctx;
			AVCodec * codec = avcodec_find_decoder(avctx ->codec_id);
			if (!codec)
			{
				cout << "video codec is not finded!" << endl;
				return 0;
			}
			int re = avcodec_open2(avctx, codec, NULL);
			if (isAvError(re)) {
				return 0;
			}
			fps = r2d(ic->streams[i]->avg_frame_rate);
		}
		else if(avctx->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			audioCtx = avctx;
			AVCodec * codec = avcodec_find_decoder(avctx->codec_id);
			if (!codec)
			{
				cout << "audio codec is not finded!" << endl;
				return 0;
			}
			int re = avcodec_open2(avctx, codec, NULL);
			if (isAvError(re)) {
				return 0;
			}
			sampleRate = avctx->sample_rate;
			channel = avctx->channels;
			switch (avctx ->sample_fmt)
			{
			case AV_SAMPLE_FMT_S16:
				sampleSize = 16;
				break;
			case AV_SAMPLE_FMT_S32:
				sampleSize = 32;
				break;
			default:
				break;
			}
		}
	 }
	return 1;
}

//读帧
AVPacket XFFmpeg::read()
{
	AVPacket pkt;
	memset(&pkt, 0, sizeof(AVPacket));//清理空间
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return pkt;
	}
	int re = av_read_frame(ic, &pkt);
	isAvError(re);
	mutex.unlock();
	return pkt;
}

//解码
int XFFmpeg::decode(const AVPacket * pkt)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return 0;
	}
	if (!yuv)
	{
		yuv = av_frame_alloc();
	}
	if (!pcm)
	{
		pcm = av_frame_alloc();
	}
	AVFrame * fram = yuv;
	AVCodecContext * avctx = videoCtx;
	if (pkt->stream_index == audioStream)
	{
		fram = pcm;
		avctx = audioCtx;
	}
	int re = avcodec_send_packet(avctx, pkt);
	if (isAvError(re))
	{
		mutex.unlock();
		return 0;
	}
	re = avcodec_receive_frame(avctx, fram);
	if (isAvError(re))
	{
		mutex.unlock();
		return 0;
	}
	mutex.unlock();
	int p = (fram->pts * r2d(ic ->streams[pkt ->stream_index]->time_base)) * 1000;
	if (pkt->stream_index == audioStream)
	{
		this->pts = p;
	}
	return p;
}

//转码
int XFFmpeg::toRGB(char * out, int outwidth, int outheight)
{
	mutex.lock();
	if (!ic || !yuv)
	{
		mutex.unlock();
		return 0;
	}
	cCtx = sws_getCachedContext(
		cCtx, videoCtx->width,
		videoCtx->height,
		videoCtx->pix_fmt,
		outwidth, outheight,
		AV_PIX_FMT_BGRA,
		SWS_BICUBIC,
		NULL, NULL, NULL
	);
	if (!cCtx)
	{
		mutex.unlock();
		cout << "open sws_getCachedContext failed!" << endl;
		return 0;
	}
	//输出空间
	uint8_t * data[AV_NUM_DATA_POINTERS] = { 0 };
	data[0] = (uint8_t *)out;
	int linesize[AV_NUM_DATA_POINTERS] = { 0 };
	linesize[0] = outwidth * 4;
	int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
		data, linesize);
	if (h <= 0)
	{
		mutex.unlock();
		cout << "sws_scale failed!" << endl;
		return 0;
	}
	mutex.unlock();
	return 1;
}

//音频的重采样
int XFFmpeg::toPcm(char * out)
{
	mutex.lock();
	if (!ic || !pcm || !out)
	{
		mutex.unlock();
		return 0;
	}
	if (!aCtx)
	{
		aCtx = swr_alloc();
		swr_alloc_set_opts(
			aCtx, audioCtx->channel_layout,
			AV_SAMPLE_FMT_S16,
			audioCtx->sample_rate, audioCtx->channels,
			audioCtx->sample_fmt,audioCtx->sample_rate,
			0, 0
		);
		swr_init(aCtx);
	}
	uint8_t * data[1];
	data[0] = (uint8_t *)out;
	int len = swr_convert(aCtx, data, 10000,
		(const uint8_t **)pcm->data,
		pcm->nb_samples
	);
	if (len <= 0)
	{
		mutex.unlock();
		return 0;
	}
	int outsize = av_samples_get_buffer_size(NULL, audioCtx->channels,
		pcm->nb_samples, AV_SAMPLE_FMT_S16, 0);
	mutex.unlock();
	return outsize;
}
//根据包获取pts
int XFFmpeg::getPts(AVPacket &pkt)
{
	mutex.lock();
	if (pkt.size <= 0 || !ic)
	{
		mutex.unlock();
		return 0;
	}
	int pts = (pkt.pts *r2d(ic->streams[pkt.stream_index]->time_base)) * 1000;
	mutex.unlock();
	return pts;
}
//变更位置
bool XFFmpeg::seek(float pos)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return false;
	}
	int64_t stamp = 0;
	stamp = pos * duration;
	int re = av_seek_frame(ic, videoStream, stamp, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
	avcodec_flush_buffers(videoCtx);
	if (isAvError(re))
	{
		mutex.unlock();
		return false;
	}
	pts = stamp;
	mutex.unlock();
	return true;
}

//释放内存
void XFFmpeg::close()
{
	mutex.lock();
	if (ic) avformat_close_input(&ic);
	if (yuv) av_frame_free(&yuv);
	if (pcm) av_frame_free(&pcm);
	if (cCtx) { sws_freeContext(cCtx); cCtx = NULL; }
	if (aCtx) swr_free(&aCtx);
	mutex.unlock();
}

void XFFmpeg::logerror(int line)
{
	mutex.lock();
	cout << "Error line : " << line << endl
		<< "Error Type : " << errInfo.errType << endl
		<< "Error Info : " << errInfo.errInfo << endl;
	mutex.unlock();
}

int XFFmpeg::isAvError(int re)
{
	if (re != 0)
	{
		errInfo.errType = re;
		av_strerror(re, errInfo.errInfo, buffSize);
		return 1;
	}
	return 0;
}

double XFFmpeg::r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ?
		0. : (double)r.num / (double)r.den;
}