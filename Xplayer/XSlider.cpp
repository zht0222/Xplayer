#include "XSlider.h"
#include <QMouseEvent>

XSlider::XSlider(QWidget * p):QSlider(p)
{
}


XSlider::~XSlider()
{
}

void XSlider::mousePressEvent(QMouseEvent * e)
{
	float val = ((float)e->pos().x() / (float)this->width()) * (this ->maximum() + 1);
	this ->setValue(val);
	QSlider::mousePressEvent(e);
}