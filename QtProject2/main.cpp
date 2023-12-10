#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "WindowController.h"
#include "ImageSource.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
        QPointer<WindowController> winController = new WindowController();
        QPointer<ImageSource> source = new ImageSource(winController->image);

        QQmlApplicationEngine engine;

        engine.rootContext()->setContextProperty("windowController", winController);
    engine.addImageProvider(QString("ImageSource"), source);

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objectUrl) {
          if (!obj && url == objectUrl) {
            QCoreApplication::exit(-1);
          }
        },
        Qt::QueuedConnection);

    
    engine.load(url);
    
    return app.exec();
}
