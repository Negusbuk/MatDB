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

#include "materialfilterwidget.h"

MaterialFilterWidget::MaterialFilterWidget(MaterialListModel *listmodel,
                                           QWidget *parent) :
    QWidget(parent),
    ListModel_(listmodel)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 2, 2, 2);
    layout->setSpacing(2);
    setLayout(layout);

    lineEdit_ = new QLineEdit(this);
    lineEdit_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout->addWidget(lineEdit_);
    connect(lineEdit_, SIGNAL(textChanged(QString)),
            ListModel_, SLOT(filterChanged(QString)));

    resetButton_ = new QPushButton(QIcon(":/icons/MatDBResetFilter.png"), "", this);
    resetButton_->setFlat(true);
    resetButton_->setIconSize(QSize(22,22));
    resetButton_->setFixedSize(26, 26);
    layout->addWidget(resetButton_);
    connect(resetButton_, SIGNAL(clicked()),
            this, SLOT(resetFilter()));
}

void MaterialFilterWidget::resetFilter()
{
    tokenEdit_->setTokens(QStringList());
    lineEdit_->setText("");
}
