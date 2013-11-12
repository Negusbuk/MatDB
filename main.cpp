/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <iostream>
#include <unistd.h>

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QSplashScreen>
#include <QSettings>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>

#include "matdbmainwindow.h"

//#define SHOWSPLASHSCREEN

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    QDir dir(logdir);
    if (!dir.exists()) dir.mkpath(".");
    QString logfilename = logdir + "/MatDB.log";

    NQLog("Main") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
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
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString defaultDBPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    QString defaultDBPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif
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
