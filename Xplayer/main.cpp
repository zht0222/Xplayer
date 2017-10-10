#include <iostream>
using namespace std;
#include "Xplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Xplayer w;
	w.show();
	return a.exec();
}
