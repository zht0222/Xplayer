#pragma once
#ifndef XVIDEOTHREAD_H
#define XVIDEOTHREAD_H
#include <QThread>
class XVideoThread : public QThread
{
private:
	XVideoThread();

public:
	~XVideoThread();

	static XVideoThread * get();
	void run();
};
#endif


