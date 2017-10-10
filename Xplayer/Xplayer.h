#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Xplayer.h"

class Xplayer : public QMainWindow
{
	Q_OBJECT
private:
	Ui::XplayerClass ui;

	static bool isPressSlider;
	static bool isPlay;

public:
	Xplayer(QWidget *parent = Q_NULLPTR);

	void timerEvent(QTimerEvent *);
	void resizeEvent(QResizeEvent *);

public slots:
	void open();
	void play();
	void sliderPress();
	void sliderRelease();
};
