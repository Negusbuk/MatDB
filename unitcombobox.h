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

#ifndef UNITCOMBOBOX_H
#define UNITCOMBOBOX_H

#include <QComboBox>

#include <unit.h>

class UnitComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UnitComboBox(Unit::VUnit *unit = 0,
                          QWidget *parent = 0);
    void setUnit(Unit::VUnit *unit);

signals:
    
public slots:

protected:
    Unit::VUnit *Unit_;
};

#endif // UNITCOMBOBOX_H
