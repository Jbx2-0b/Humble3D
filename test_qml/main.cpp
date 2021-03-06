﻿#include <QGuiApplication>
#include <QQuickStyle>

#include "TestQml.h"
#include "CQmlWidget3D.h"

// Qml
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<CQmlWidget3D>("Humble3D", 1, 0, "Widget3D");

    TestQml humbleApp;

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));
    return app.exec();
}
