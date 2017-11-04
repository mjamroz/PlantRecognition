QT += qml quick widgets sql
CONFIG += c++11
android {
    QT += androidextras
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    bufferfile.cpp \
    downloader.cpp \
    mxpredict.cpp \
    permissions.cpp


RESOURCES += qml.qrc
include(vendor/android/native/pri/androidnative.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bufferfile.h \
    DB.h \
    downloader.h \
    mxpredict.h \
    permissions.h


INCLUDEPATH += $$PWD/mxnet/include
DEPENDPATH += $$PWD/mxnet/include

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/libs/android-support-v4.jar \
    android/src/org/bytran/bytran/RequestPermissions.java \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = $$PWD/lib/libmxnet.so \ # compile using Dockerfile from mxnet package
        $$PWD/lib/libcrypto.so \ # compiled from openssl-1.1.0e.tar.gz
        $$PWD/lib/libssl.so # compiled from openssl-1.1.0e.tar.gz
        LIBS += -L$$PWD/lib/ -lmxnet
}


linux:!android {
        LIBS += -L$$PWD/mxnet/lib/ -lmxnet
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
