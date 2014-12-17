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

#include <vector>

#include "unitcombobox.h"

UnitComboBox::UnitComboBox(Unit::VUnit *unit,
                           QWidget *parent) :
    QComboBox(parent)
{
    if (unit) setUnit(unit);
}

void UnitComboBox::setUnit(Unit::VUnit *unit)
{
    clear();
    if (!unit) return;

    const std::vector<QString>& units = unit->getUnits();
    for (std::vector<QString>::const_iterator it = units.begin();
         it!=units.end();
         ++it) {
        addItem(*it);
    }

    setCurrentIndex(unit->currentUnit());
}
