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

#ifndef MATDBABOUTDIALOG_H
#define MATDBABOUTDIALOG_H

#include <QWizard>
#include <QLabel>
#include <QRadioButton>
#include <QTextEdit>

class MatDBAboutDialog : public QWizard
{
    Q_OBJECT
public:

    enum {
        Page_Version,
        Page_License
    };

    explicit MatDBAboutDialog(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

class VersionPage : public QWizardPage
{
    Q_OBJECT

public:
    VersionPage(QWidget *parent = 0);

    int nextId() const;
};

class LicensePage : public QWizardPage
{
    Q_OBJECT

public:
    LicensePage(QWidget *parent = 0);

    int nextId() const;
};

#endif // MATDBABOUTDIALOG_H
