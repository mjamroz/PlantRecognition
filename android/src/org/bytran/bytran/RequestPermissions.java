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
package org.bytran.bytran;

import android.Manifest;
import android.app.Activity;
import android.support.v4.content.ContextCompat;
import android.support.v4.app.ActivityCompat;
import android.content.pm.PackageManager;
import java.lang.Runnable;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Context;
import java.util.ArrayList;
import java.util.List;
import android.content.Intent;
import android.os.Bundle;
import android.app.FragmentManager;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.os.Build;

// Method to request permissions for the read and write from / to an external storage
public class RequestPermissions
{
    // Multiple permissions request code 
    private static final int REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS = 116;

    // Variables used to determine the result of the permission request
    private static String responseStateStr = "Unknown";  //  0 - "Unknown",  1 - "Granted",  2 - "Denied", 3 - "Just Granted"

    // Activity variable passed in from the Qt program
    private final Activity m_MainActivity;

    // Method to request permissions
    public RequestPermissions(Activity MainActivity)
    {
        m_MainActivity = MainActivity;

        MainActivity.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                // Requesting permissions
                askPermissions(m_MainActivity);
            }
        });
    }

    // Method to return the response state string
    static String getResponseState() { return responseStateStr; }

    // Method to determine if the operating system is Android Marshmallow or above
    static String getIsMarshmallowOrAbove()
    {
        if (android.os.Build.VERSION.SDK_INT > 22)
        {
            return "YES";
        }
        else
        {
            return "NO";
        }
    }

    // Method to ask for external storage read and write permissions
    public static void askPermissions(Activity currentActivity)
    {
        final List<String> permissionsList = new ArrayList<String>();

        if (currentActivity.checkCallingOrSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        { permissionsList.add(Manifest.permission.READ_EXTERNAL_STORAGE); }

        if (currentActivity.checkCallingOrSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        { permissionsList.add(Manifest.permission.WRITE_EXTERNAL_STORAGE); }

        if (currentActivity.checkCallingOrSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED)
        { permissionsList.add(Manifest.permission.CAMERA); }
        if (currentActivity.checkCallingOrSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
        { permissionsList.add(Manifest.permission.ACCESS_COARSE_LOCATION); }
        if (currentActivity.checkCallingOrSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)
        { permissionsList.add(Manifest.permission.ACCESS_FINE_LOCATION); }

        final FragmentManager fragmentManager = currentActivity.getFragmentManager();
        final Fragment request = new Fragment() {

            @Override public void onStart()
            {
                super.onStart();

                // Checking the read and write permissions
                if (!permissionsList.isEmpty())
                {
                    requestPermissions(permissionsList.toArray(new String[permissionsList.size()]), REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);
                }
                else
                {
                    // Emitting a signal indicating that the permissions have been granted
                    responseStateStr = "Granted";
                }
            }

            @Override public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
            {
                if (requestCode != REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS) return;

                if ((grantResults.length > 0) && (grantResults[0] == PackageManager.PERMISSION_GRANTED))
                {
                    // Permission was granted

                    AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                    builder.setMessage("Please restart the application for the read / write permissions to take effect (the application may be closed by selecting \"Exit\" at the top right corner of the screen).")
                           .setTitle("Restart required");

                    builder.setPositiveButton("ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id)
                        {
                            // Emitting a signal indicating that the permissions have been granted                            
                            responseStateStr = "Just Granted";
                        }
                    });

                    AlertDialog dialog = builder.create();
                    dialog.show();
                }
                else
                {                    
                    // The check of either read or write permission would be sufficient
                    if (shouldShowRequestPermissionRationale(Manifest.permission.WRITE_EXTERNAL_STORAGE))
                    {
                        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                        builder.setMessage("The functionality of the app will be limited.  Without the external storage read / write permission this app is unable (1) to store and access downloaded HITRAN data, (2) the configuration file is hidden from the user, (3) and the calculation results (images and text files) will not be accessible on local storage which will affect the results sharing capability. \n\nIf you change your mind, you may still grant this permission the next time the application is launched.")
                               .setTitle("Permission denied");

                        builder.setNegativeButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id)
                            {
                                // Emitting a signal indicating that the permission has been denied
                                responseStateStr = "Denied";
                            }
                        });

                        AlertDialog dialog = builder.create();
                        dialog.show();
                    }
                    else
                    {
                        // Never ask again selected, or device policy prohibits the app from having that permission.
                        // So, disable that feature, or fall back to another situation...

                        // Emitting a signal indicating that the permission has been denied permanently this time
                        responseStateStr = "Denied";
                    }
                }

                FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
                fragmentTransaction.remove(this);
                fragmentTransaction.commit();
            }
        };

        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.add(0, request);
        fragmentTransaction.commit();
    }
}
