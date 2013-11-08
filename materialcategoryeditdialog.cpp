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

#include <QLayout>
#include <QFormLayout>
#include <QPushButton>

#include "materialcategoryeditdialog.h"

MaterialCategoryEditDialog::MaterialCategoryEditDialog(QWidget *parent) :
    QDialog(parent)
{   
    QVBoxLayout * layout = new QVBoxLayout(this);
    setLayout(layout);

    QWidget * widget;

    widget = new QWidget(this);
    layout->addWidget(widget);

    QFormLayout *formLayout = new QFormLayout(widget);
    widget->setLayout(formLayout);

    nameEdit_ = new QLineEdit(widget);
    formLayout->addRow("Name:", nameEdit_);

    colorPicker_ = new NQColorWheel(widget);
    layout->addWidget(colorPicker_);

    widget = new QWidget(this);
    layout->addWidget(widget);

    QHBoxLayout *buttonLayout = new QHBoxLayout(widget);
    widget->setLayout(buttonLayout);

    QPushButton *button;
    button = new QPushButton("Ok", this);
    buttonLayout->addWidget(button);
    connect(button, SIGNAL(clicked()),
            this, SLOT(accept()));

    button = new QPushButton("Cancel", this);
    buttonLayout->addWidget(button);
    connect(button, SIGNAL(clicked()),
            this, SLOT(reject()));

    updateGeometry();
}
