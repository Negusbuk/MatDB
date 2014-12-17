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

#include <nqlogger.h>

#include <propertyselectionmodel.h>

PropertySelectionModel::PropertySelectionModel(QObject *parent) :
    QObject(parent)
{
    Selection_ = 0;
}

Property * PropertySelectionModel::getSelection()
{
    return Selection_;
}

void PropertySelectionModel::setSelection(Property* selection)
{
    NQLog("PropertySelectionModel", NQLog::Spam) << "void setSelection(Property* selection)"
                                                 << selection;

    Selection_ = selection;
    emit selectionChanged(Selection_);
}

void PropertySelectionModel::emitPropertyModified()
{
    emit propertyModified(Selection_);
}

void PropertySelectionModel::emitPropertyModified(Property* property)
{
    emit propertyModified(property);
}
