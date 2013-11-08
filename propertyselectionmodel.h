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

#ifndef PROPERTYSELECTIONMODEL_H
#define PROPERTYSELECTIONMODEL_H

#include <QObject>

#include <property.h>

class PropertySelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit PropertySelectionModel(QObject *parent = 0);
    Property* getSelection();

signals:
    void selectionChanged(Property* selection);
    void propertyModified(Property* selection);

public slots:
    void setSelection(Property* selection);
    void emitPropertyModified();
    void emitPropertyModified(Property* property);

protected:
    Property* Selection_;
};

#endif // PROPERTYSELECTIONMODEL_H
