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
#include "permissions.h" 
#include <QMessageBox>
#include <QApplication>

Permissions::Permissions(QObject *parent) : QObject(parent)
{
    // Creating the timer
    timer = new QTimer(this);

    // Creating a connection for the timer to trigger the check of the permission state
    connect(this->timer, SIGNAL(timeout()), this, SLOT(verifyPermissionState()));
}

// Method to request permissions
void Permissions::requestPermissions()
{
    #if defined(Q_OS_ANDROID)

        // Only requesting permissions if the operating system is Android Marshmallow 6.0 or above
        if (getIsMarshmallowOrAbove())
        {
            RequestPermissions = QAndroidJniObject("org/bytran/bytran/RequestPermissions",
                                                   "(Landroid/app/Activity;)V",
                                                   QtAndroid::androidActivity().object<jobject>()
                                                   );

            // Checking for errors in the JNI
            QAndroidJniEnvironment env;
            if (env->ExceptionCheck()) {
                // Handle exception here.
                env->ExceptionClear();
            }

            // Starting the timer
            this->permissionWasGranted = 0;  // Unclear if the permission has been granted
            timer->start(10);
        }

    #else

        emit permissionGranted(true);

    #endif
}

// Method to verify the permission state
void Permissions::verifyPermissionState()
{
    #if defined(Q_OS_ANDROID)

        QString auxStr;  // Auxiliary string

        QAndroidJniObject stringResult = QAndroidJniObject::callStaticObjectMethod("org/bytran/bytran/RequestPermissions",
                                                                                   "getResponseState",
                                                                                   "()Ljava/lang/String;");

        QAndroidJniEnvironment env;
        if (env->ExceptionCheck()) {
            // Handle exception here.
            env->ExceptionClear();
        }

        // Returning a result value depending on the state of the returned string variable
        auxStr = stringResult.toString();
        if (auxStr == "Granted")
        {
            timer->stop();
            this->permissionWasGranted = 1;
            emit permissionGranted(true);
            QApplication::processEvents();
        }
        else if ((auxStr == "Denied") || (auxStr == "Just Granted"))
        {
            timer->stop();
            this->permissionWasGranted = 2;
            emit permissionGranted(false);
            QApplication::processEvents();
        }

    #else

        this->permissionWasGranted = 1;
        emit permissionGranted(true);
        QApplication::processEvents();

    #endif
}


// Method to determnie if the Android operating system is Marshmallow or above
bool Permissions::getIsMarshmallowOrAbove()
{
    #if defined(Q_OS_ANDROID)

        QAndroidJniObject stringResult = QAndroidJniObject::callStaticObjectMethod("org/bytran/bytran/RequestPermissions",
                                                                                   "getIsMarshmallowOrAbove",
                                                                                   "()Ljava/lang/String;");

        QAndroidJniEnvironment env;
        if (env->ExceptionCheck()) {
            // Handle exception here.
            env->ExceptionClear();
        }

        // If the operating system is 6.0 or above (after marshmallow, then true is returned)
        if (stringResult.toString() == "YES")
        {
            return true;
        }
        else
        {
            return false;
        }

        return false;

    #else

        return false;

    #endif
}

// Method to get the permission granted state
int Permissions::getPermissionGranted()
{
    return permissionWasGranted;
}
