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

#ifndef PROPERTYTOOLBOX_H
#define PROPERTYTOOLBOX_H

#include <vector>
#include <map>

#include <QLabel>
#include <QToolBox>
#include <QEvent>
#include <QMouseEvent>

#include <property.h>
#include <propertymodel.h>

class PropertyToolBoxItem : public QLabel
{
    Q_OBJECT
public:
    explicit PropertyToolBoxItem(const QString &name,
                                 Property *property,
                                 QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);

    Property* Property_;
};

class PropertyToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit PropertyToolBox(PropertyModel *model,
                             QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:

    void changeEvent(QEvent *event);

    std::map<QString,QWidget*> categoryWidgetMap_;
    std::map<QLabel*,QString> itemLabelMap_;
    PropertyModel* PropertyModel_;
    void build();
};

#endif // PROPERTYTOOLBOX_H
