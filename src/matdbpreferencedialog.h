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

#ifndef MATDBPREFERENCEDIALOG_H
#define MATDBPREFERENCEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QComboBox>

class LanguagePage;

class MatDBPreferenceDialog : public QDialog
{
    Q_OBJECT
public:

    explicit MatDBPreferenceDialog(QWidget *parent = 0);

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

protected slots:

    void applyChanges();

private:

    void createIcons();

    QListWidget *contentsWidget_;
    QStackedWidget *pagesWidget_;
    LanguagePage *languagePage_;
};

class LanguagePage : public QWidget
{
    Q_OBJECT
public:
    LanguagePage(QWidget *parent = 0);

    QVariant selectedLanguage();

protected:

    QComboBox *languageCombo_;
};

#endif // MATDBPREFERENCEDIALOG_H
