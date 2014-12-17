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

#include <parameterselectionmodel.h>

ParameterSelectionModel::ParameterSelectionModel(QObject *parent) :
    QObject(parent)
{
    Selection_ = 0;
}

Parameter * ParameterSelectionModel::getSelection()
{
    return Selection_;
}

void ParameterSelectionModel::setSelection(Parameter* selection)
{
    if (Selection_ != selection) {
        Selection_ = selection;
        if (Selection_) {
            NQLog("ParameterSelectionModel", NQLog::Spam) << "parameter selection changed: "
                                                          << selection->getName();
        } else {
            NQLog("ParameterSelectionModel", NQLog::Spam) << "parameter selection changed: "
                                                          << selection;
        }
        emit selectionChanged(Selection_);
    }
}

void ParameterSelectionModel::emitParameterModified()
{
    emit parameterModified(Selection_);
}
