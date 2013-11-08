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

#ifndef MATERIALCATEGORYEDITDIALOG_H
#define MATERIALCATEGORYEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include <nqcolorwheel.h>

class MaterialCategoryEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MaterialCategoryEditDialog(QWidget *parent = 0);

    const QString& getName() const { return nameEdit_->text(); }
    const QColor& getColor() const { return colorPicker_->color(); }

signals:

public slots:

    void setName(const QString& name) { nameEdit_->setText(name); }
    void setColor(const QColor& color) { colorPicker_->setColor(color); }

protected:

    QLineEdit* nameEdit_;
    NQColorWheel* colorPicker_;
};

#endif // MATERIALCATEGORYEDITDIALOG_H
