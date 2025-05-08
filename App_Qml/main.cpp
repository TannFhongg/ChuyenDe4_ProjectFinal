#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serialhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SerialHandler serialHandler;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("SerialHandler", &serialHandler);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
