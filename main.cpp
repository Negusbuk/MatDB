#include <iostream>
#include <unistd.h>

#include <QApplication>
#include <QFile>
#include <QPixmap>
#include <QSplashScreen>
#include <QSettings>
#include <QDesktopServices>

#include <nqlogger.h>

#include "matdbmainwindow.h"

//#define SHOWSPLASHSCREEN

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);
    QFile * logfile = new QFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/Library/Logs/MatDB.log");
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

    QCoreApplication::setOrganizationName("Negusbuk");
    QCoreApplication::setOrganizationDomain("mussgiller.de");
    QCoreApplication::setApplicationName("MatDB");

#ifdef SHOWSPLASHSCREEN
    QPixmap pixmap(":/pics/MatDBSplashScreen.png");
    QSplashScreen splash(pixmap);
    splash.show();

    qApp->processEvents();
#endif

    QSettings settings;
    QString defaultDBPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString dbPath = settings.value("dbpath").toString();

    NQLog("Main") << "dbPath: " << dbPath;

    if (dbPath.length()==0) {
        settings.setValue("dbpath", defaultDBPath);
    }

    QFile file(":/qss/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet(file.readAll());
    a.setStyleSheet(stylesheet);
    a.setStyle("macintosh");

    MatDBMainWindow w;

#ifdef SHOWSPLASHSCREEN
    usleep(2000*1000);
#endif

    w.show();

#ifdef SHOWSPLASHSCREEN
    splash.finish(&w);
#endif

    return a.exec();
}
