#pragma once
class XAudioPlay
{
protected:
	XAudioPlay();

public:
	int sampleRate = 48000;
	int sampleSize = 16;
	int channel = 2;

	virtual ~XAudioPlay();

	static XAudioPlay *get();
	virtual bool start() = 0;
	virtual void stop() = 0;
	virtual void play(bool isplay) = 0;
	virtual bool write(const char *data, int datasize) = 0;
	virtual int getFree() = 0;
};

