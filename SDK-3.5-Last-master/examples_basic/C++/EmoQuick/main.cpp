/****************************************************************************
**
** Copyright 2017 by Emotiv. All rights reserved
** Example: EmoQuick
** How to use Qt C++ Wrapper for SDK with QtQuick/QML
** This application will display data: Gyro/BandPower/Motion/EEG(if available)
** Author: Dinh Duong Ha ha@emotiv.com
****************************************************************************/

#include <QSettings>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickStyle>

#include "Emotiv.hpp"
#include "QEmotivEngine.hpp"

using namespace Emotiv;

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("EmoQuick");
    QApplication::setApplicationVersion("1.0");
    QApplication::setApplicationDisplayName("Emotiv Quick QML Application");
    QApplication::setOrganizationName("Emotiv");
    QApplication::setOrganizationDomain("emotiv.com");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<QEmotivEngine>("com.emotiv.qml", 1, 0, "EmotivEngine");

    QSettings settings;
    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else {
        style = settings.value("style").toString();
        QQuickStyle::setStyle(style);
    }
    QQmlApplicationEngine engine;

    QEmotivEngine emotivConnector;
    engine.rootContext()->setContextProperty("EmotivEngine", &emotivConnector);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Automatic start engine
    emotivConnector.start();
    return app.exec();
}
