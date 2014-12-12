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

#ifndef MATDBLANGUAGEHANDLER_H
#define MATDBLANGUAGEHANDLER_H

#include <QList>
#include <QTranslator>

class MatDBLanguageHandler
{
public:

    static MatDBLanguageHandler* instance();

    void changeLanguage(const QString &locale);

protected:

    MatDBLanguageHandler();

    static MatDBLanguageHandler *instance_;

    QList<QTranslator*> translators_;
};

#endif // MATDBLANGUAGEHANDLER_H
