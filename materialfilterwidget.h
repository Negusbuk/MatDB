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

#ifndef MATERIALFILTERWIDGET_H
#define MATERIALFILTERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

#include <nqtokenedit.h>

#include <materiallistmodel.h>

class MaterialFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialFilterWidget(MaterialListModel *listmodel,
                                  QWidget *parent = 0);

signals:

    void filterChanged(QStringList,bool);

public slots:

protected slots:

    void resetFilter();
    void tokensChanged(const QStringList&);
    void logicChanged(int id);

protected:

    MaterialListModel* ListModel_;
    NQTokenEdit* tokenEdit_;
    QButtonGroup* filterLogicGroup_;
    QPushButton* resetButton_;
};

#endif // MATERIALFILTERWIDGET_H
