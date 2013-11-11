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
#include <QRadioButton>

#include "materialfilterwidget.h"

MaterialFilterWidget::MaterialFilterWidget(MaterialListModel *listmodel,
                                           QWidget *parent) :
    QWidget(parent),
    ListModel_(listmodel)
{
    setMaximumHeight(80);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 2, 2, 2);
    layout->setSpacing(2);
    setLayout(layout);

    tokenEdit_ = new NQTokenEdit(this);
    tokenEdit_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout->addWidget(tokenEdit_);
    connect(tokenEdit_, SIGNAL(tokensChanged(QStringList)),
            this, SLOT(tokensChanged(QStringList)));

    QWidget* control = new QWidget(this);

    filterLogicGroup_ = new QButtonGroup(control);
    connect(filterLogicGroup_, SIGNAL(buttonClicked(int)),
            this, SLOT(logicChanged(int)));

    QHBoxLayout *hbox = new QHBoxLayout(control);
    hbox->setContentsMargins(4, 2, 2, 2);
    control->setLayout(hbox);

    QWidget* stretch = new QWidget(control);
    stretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hbox->addWidget(stretch);

    QRadioButton * rb;
    rb = new QRadioButton("AND", control);
    hbox->addWidget(rb);
    filterLogicGroup_->addButton(rb, 0);
    rb->setChecked(true);
    rb = new QRadioButton("OR", control);
    hbox->addWidget(rb);
    filterLogicGroup_->addButton(rb, 1);

    resetButton_ = new QPushButton(QIcon(":/icons/MatDBResetFilter.png"), "", control);
    resetButton_->setFlat(true);
    resetButton_->setIconSize(QSize(22,22));
    resetButton_->setFixedSize(26, 26);
    hbox->addWidget(resetButton_);
    connect(resetButton_, SIGNAL(clicked()),
            this, SLOT(resetFilter()));

    layout->addWidget(control);

    connect(this, SIGNAL(filterChanged(QStringList,bool)),
            ListModel_, SLOT(filterChanged(QStringList,bool)));


    updateGeometry();
}

void MaterialFilterWidget::tokensChanged(const QStringList& list)
{
    bool logic;
    if (filterLogicGroup_->checkedId()==0) { // AND
        logic = true;
    } else { // OR
        logic = false;
    }

    emit filterChanged(list, logic);
}

void MaterialFilterWidget::logicChanged(int id)
{
    bool logic;
    if (id==0) { // AND
        logic = true;
    } else { // OR
        logic = false;
    }

    emit filterChanged(tokenEdit_->tokens(), logic);
}


void MaterialFilterWidget::resetFilter()
{
    tokenEdit_->setTokens(QStringList());
}
