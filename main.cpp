#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <QVariantList>

#include "mxpredict.h"
#include "downloader.h"
#include "permissions.h"


#ifdef Q_OS_ANDROID
#include <AndroidNative/systemdispatcher.h>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QtAndroid>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
        Q_UNUSED(vm);
        AndroidNative::SystemDispatcher::registerNatives();
        return JNI_VERSION_1_6;
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    app.setOrganizationName("Atlas roslin");
    app.setOrganizationDomain("atlas.roslin.pl");
    app.setApplicationName("Plant image recognition");

    QQmlApplicationEngine engine;
    DownloadManager downloader;
#ifdef Q_OS_ANDROID
    Permissions permissions;
    permissions.requestPermissions();
#endif

    MXPredict p;
    QVariantList list;
    p.setData(list);

    engine.rootContext()->setContextProperty("MXNet", &p);
    engine.rootContext()->setContextProperty("downloader", &downloader);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));


    return app.exec();
}

