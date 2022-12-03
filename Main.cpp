#include "MainWindow.hpp"

#include <QApplication>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroidExtras>

    bool requestAndroidPermissions()
    {
        // Request requiered permissions at runtime
        const QVector<QString> permissions({
            "android.permission.WRITE_EXTERNAL_STORAGE",
            "android.permission.READ_EXTERNAL_STORAGE"
        });

        for (const QString& permission : permissions)
        {
            auto result = QtAndroid::checkPermission(permission);
            if (result == QtAndroid::PermissionResult::Denied)
            {
                auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
                if (resultHash[permission] == QtAndroid::PermissionResult::Denied)
                {
                    return false;
                }
            }
        }

        return true;
    }
#endif

int main(int argc, char** argv) {
    QApplication a(argc, argv);
    MainWindow w;

    #ifdef Q_OS_ANDROID
        requestAndroidPermissions();
    #endif

    w.show();
    return QApplication::exec();
}
