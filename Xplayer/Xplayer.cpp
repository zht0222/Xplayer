#include "Xplayer.h"
#include <QFileDialog>
#include <QMessageBox>
#include "XFFmpeg.h"
#include "XAudioPlay.h"
#define PLAY "QPushButton:!hover{border-image:url(:/Xplayer/Resources/play_normal.png);}\
QPushButton:hover{ border-image:url(:/Xplayer/Resources/play_hot.png);}"

#define PAUSE "QPushButton:!hover{border-image:url(:/Xplayer/Resources/pause_normal.png);}\
QPushButton:hover{ border-image:url(:/Xplayer/Resources/pause_hot.png);}"

bool Xplayer::isPressSlider = true;
bool Xplayer::isPlay = true;

Xplayer::Xplayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Xplayer::open()
{
	QString name;
	name = QFileDialog::getOpenFileName(
		this, QString::fromLocal8Bit("选择视频文件"));
	if (name.isEmpty())
	{
		return;
	}
	this ->setWindowTitle(name);
	QByteArray ba = name.toLatin1();
	char *cstr = ba.data();
	if (!XFFmpeg::get()->open(cstr))
	{
		QMessageBox::information(this, "Info", "file open failed!");
		return;
	}

	XAudioPlay::get()->sampleRate = XFFmpeg::get()->sampleRate;
	XAudioPlay::get()->channel = XFFmpeg::get()->channel;
	XAudioPlay::get()->sampleSize = XFFmpeg::get()->sampleSize;
	XAudioPlay::get()->start();

	int totaltime = XFFmpeg::get()->duration / 1000;
	int m = totaltime / 60;
	int s = totaltime % 60;
	char buff[1024];
	sprintf(buff, "%03d:%02d", m, s);
	ui.totaltime->setText(buff);
	startTimer(40);
}

void Xplayer::timerEvent(QTimerEvent *e)
{
	int playtime = XFFmpeg::get()->pts / 1000;
	int m = playtime / 60;
	int s = playtime % 60;
	char buff[1024];
	sprintf(buff, "%03d:%02d", m, s);
	ui.playtime->setText(buff);
	
	if (XFFmpeg::get()->duration > 0 && 
		isPressSlider &&
		XFFmpeg::get()->isplay)
	{
		float rate = (float)XFFmpeg::get()->pts / (float)XFFmpeg::get()->duration;
		ui.playSlider ->setValue(rate * 1000);
	}
}

void Xplayer::play()
{
	if (XFFmpeg::get()->duration <= 0)
	{
		return;
	}
	isPlay = !isPlay;
	XFFmpeg::get()->isplay = isPlay;
	if (isPlay)
	{
		
		ui.playAvBtn ->setStyleSheet(PLAY);
	}
	else
	{
		ui.playAvBtn->setStyleSheet(PAUSE);
	}
}

void Xplayer::sliderPress()
{
	isPressSlider = false;
}

void Xplayer::sliderRelease()
{
	isPressSlider = true;
	float pos = 0.;
	pos = (float)ui.playSlider->value() / (float)(ui.playSlider->maximum() + 1);
	XFFmpeg::get()->seek(pos);
}

void Xplayer::resizeEvent(QResizeEvent * e)
{
	ui.openGLWidget ->resize(size());
}