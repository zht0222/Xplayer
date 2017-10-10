#include "XAudioPlay.h"
#include <QAudioOutput>
#include <QMutex>
class CXAudioPlay : public XAudioPlay
{
private:
public:
	QMutex mutex;
	QAudioOutput * output = NULL;
	QIODevice * io = NULL;

	bool start()
	{
		stop();
		mutex.lock();
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setSampleSize(sampleSize);
		fmt.setChannelCount(channel);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		output = new QAudioOutput(fmt);
		io = output->start();
		mutex.unlock();
		return true;
	}

	void stop()
	{
		mutex.lock();
		if (output)
		{
			output->stop();
			delete output;
			output = NULL;
			io = NULL;
		}
		mutex.unlock();
	}

	void play(bool isplay)
	{
		mutex.lock();
		if (!output)
		{
			mutex.unlock();
			return;
		}
		if (isplay)
		{
			output->resume();
		}
		else
		{
			output->suspend();
		}
		mutex.unlock();
	}

	bool write(const char *data, int datasize)
	{
		mutex.lock();
		if (io && data && datasize>0)
		{
			io->write(data, datasize);
			mutex.unlock();
			return true;
		}
		mutex.unlock();
		return false;
	}

	int getFree()
	{
		mutex.lock();
		if (output)
		{
			int free = output->bytesFree();
			mutex.unlock();
			return free;
		}
		mutex.unlock();
		return 0;
	}
};

XAudioPlay::XAudioPlay()
{
}

XAudioPlay::~XAudioPlay()
{
}

XAudioPlay * XAudioPlay::get()
{
	static CXAudioPlay ap;
	return &ap;
}




