//
// The following resources where most helpful in implementing this Java code insert
// http://doc.qt.io/qt-5/android3rdpartylibs.html
// http://falsinsoft.blogspot.com.by/2015/03/qt-android-interface-c-and-java-through.html
// http://www.kdab.com/qt-android-episode-5/
// http://www.kdab.com/qt-android-episode-6/
// https://www.kdab.com/qt-android-episode-7/
// http://stackoverflow.com/questions/35027043/implementing-android-6-0-permissions-in-unity3d
// http://stackoverflow.com/questions/32347532/android-m-permissions-confused-on-the-usage-of-shouldshowrequestpermissionrati
// http://stackoverflow.com/questions/34342816/android-6-0-multiple-permissions
// http://stackoverflow.com/questions/21417419/how-to-convert-aar-to-jar
// https://developer.android.com/training/permissions/requesting.html
// https://inthecheesefactory.com/blog/things-you-need-to-know-about-android-m-permission-developer-edition/en
// http://stackoverflow.com/questions/30128718/general-share-button-on-android-in-qt-quick
// https://gist.github.com/patrickhammond/47cff299bc34e976e18b
// https://github.com/xtext/xtend-gradle-plugin/issues/3
// https://github.com/codepath/android_guides/wiki/Understanding-App-Permissions
// http://doc.qt.io/qt-5/qandroidjniobject.html



// Java code to request external storage read/write runtime permissions from the Android Marshmallow (6.0) operating system
#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <QObject>
#ifdef Q_OS_ANDROID
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
    #include <QtAndroid>
#endif

#include <QTimer>

class Permissions : public QObject
{
    Q_OBJECT
public:
    explicit Permissions(QObject *parent = 0);

    // Method to request permissions
    void requestPermissions();

    // Method to get the permission granted state
    int getPermissionGranted();

    // Method to determnie if the Android operating system is Marshmallow or above
    bool getIsMarshmallowOrAbove();

signals:

    // Signal indicating if a permission to read / write to external storage has been granted
    void permissionGranted(bool granted);

private slots:

    // Method to verify the permission state
    void verifyPermissionState();

public slots:

private:

    #if defined(Q_OS_ANDROID)

        // Object used to obtain permissions on Android Marshmallow
        QAndroidJniObject RequestPermissions;

    #endif

    // Object to monitor when the user has allowed or denied read/write permissions to access external storage
    QTimer *timer;

    // Variable indicating if the permission to read / write has been granted
    int permissionWasGranted;  //  0 - "Unknown",  1 - "Granted",  2 - "Denied"

};

#endif // PERMISSIONS_H
