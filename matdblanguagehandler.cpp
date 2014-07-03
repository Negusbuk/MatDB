/****************************************************************************
 **
 **  Copyright (C) 2014 Andreas Mussgiller
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

#include <QtCore>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>

#include "matdblanguagehandler.h"

MatDBLanguageHandler *MatDBLanguageHandler::instance_ = NULL;

MatDBLanguageHandler::MatDBLanguageHandler()
{

}

MatDBLanguageHandler* MatDBLanguageHandler::instance()
{
    if (instance_==NULL) {
        instance_ = new MatDBLanguageHandler();
    }
    return instance_;
}

void MatDBLanguageHandler::changeLanguage(const QString &locale)
{
    while (!translators_.isEmpty()) {
        QTranslator * t = translators_.takeFirst();
        QCoreApplication::instance()->removeTranslator(t);
        delete t;
    }

    QString resourceDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    QTranslator *qttranslator = new QTranslator(0);
    if (qttranslator->load(QLatin1String("qt_") + locale, resourceDir)) {
        QCoreApplication::instance()->installTranslator(qttranslator);
    } else if (qttranslator->load(QLatin1String("qt_") + locale,
                                  QCoreApplication::applicationDirPath())) {
        QCoreApplication::instance()->installTranslator(qttranslator);
    }
    translators_.push_back(qttranslator);

    QTranslator *translator = new QTranslator(0);
    QString transName = ":/translations/" + QLatin1String("MatDB_") + locale + ".qm";
    if (translator->load(transName)) {
        NQLog("Main") << "using file '" + transName + "' for translations.";
        QCoreApplication::instance()->installTranslator(translator);
    }
    translators_.push_back(translator);
}
