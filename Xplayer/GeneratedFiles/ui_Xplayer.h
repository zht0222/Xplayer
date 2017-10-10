/********************************************************************************
** Form generated from reading UI file 'Xplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XPLAYER_H
#define UI_XPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "videowidget.h"
#include "xslider.h"

QT_BEGIN_NAMESPACE

class Ui_XplayerClass
{
public:
    QWidget *centralWidget;
    VideoWidget *openGLWidget;
    QPushButton *openFileBtn;
    QPushButton *playAvBtn;
    QLabel *playtime;
    QLabel *sp;
    QLabel *totaltime;
    XSlider *playSlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XplayerClass)
    {
        if (XplayerClass->objectName().isEmpty())
            XplayerClass->setObjectName(QStringLiteral("XplayerClass"));
        XplayerClass->resize(798, 629);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Xplayer/Resources/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XplayerClass->setWindowIcon(icon);
        centralWidget = new QWidget(XplayerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new VideoWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 0, 800, 600));
        openFileBtn = new QPushButton(centralWidget);
        openFileBtn->setObjectName(QStringLiteral("openFileBtn"));
        openFileBtn->setGeometry(QRect(270, 500, 40, 41));
        openFileBtn->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/Xplayer/Resources/open_normal.png);}\n"
"QPushButton:hover{border-image: url(:/Xplayer/Resources/open_hot.png);}\n"
""));
        playAvBtn = new QPushButton(centralWidget);
        playAvBtn->setObjectName(QStringLiteral("playAvBtn"));
        playAvBtn->setGeometry(QRect(500, 500, 40, 41));
        playAvBtn->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/Xplayer/Resources/play_normal.png);}\n"
"QPushButton:hover{border-image: url(:/Xplayer/Resources/play_hot.png);}\n"
""));
        playtime = new QLabel(centralWidget);
        playtime->setObjectName(QStringLiteral("playtime"));
        playtime->setGeometry(QRect(691, 491, 36, 16));
        playtime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"\346\245\267\344\275\223\";"));
        sp = new QLabel(centralWidget);
        sp->setObjectName(QStringLiteral("sp"));
        sp->setGeometry(QRect(733, 491, 16, 16));
        sp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"\346\245\267\344\275\223\";"));
        totaltime = new QLabel(centralWidget);
        totaltime->setObjectName(QStringLiteral("totaltime"));
        totaltime->setGeometry(QRect(745, 491, 36, 16));
        totaltime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"\346\245\267\344\275\223\";"));
        playSlider = new XSlider(centralWidget);
        playSlider->setObjectName(QStringLiteral("playSlider"));
        playSlider->setGeometry(QRect(10, 470, 781, 22));
        playSlider->setMaximum(999);
        playSlider->setOrientation(Qt::Horizontal);
        XplayerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XplayerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 798, 23));
        XplayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XplayerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        XplayerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XplayerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        XplayerClass->setStatusBar(statusBar);

        retranslateUi(XplayerClass);
        QObject::connect(openFileBtn, SIGNAL(clicked()), XplayerClass, SLOT(open()));
        QObject::connect(playSlider, SIGNAL(sliderPressed()), XplayerClass, SLOT(sliderPress()));
        QObject::connect(playSlider, SIGNAL(sliderReleased()), XplayerClass, SLOT(sliderRelease()));
        QObject::connect(playAvBtn, SIGNAL(clicked()), XplayerClass, SLOT(play()));

        QMetaObject::connectSlotsByName(XplayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XplayerClass)
    {
        XplayerClass->setWindowTitle(QApplication::translate("XplayerClass", "Xplayer", Q_NULLPTR));
        openFileBtn->setText(QString());
        playAvBtn->setText(QString());
        playtime->setText(QApplication::translate("XplayerClass", "000:00", Q_NULLPTR));
        sp->setText(QApplication::translate("XplayerClass", "/", Q_NULLPTR));
        totaltime->setText(QApplication::translate("XplayerClass", "000:00", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class XplayerClass: public Ui_XplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLAYER_H
