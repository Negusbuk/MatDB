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

#ifndef PARAMETERSELECTIONMODEL_H
#define PARAMETERSELECTIONMODEL_H

#include <QObject>

#include <parameter.h>

class ParameterSelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit ParameterSelectionModel(QObject *parent = 0);
    Parameter* getSelection();

signals:
    void selectionChanged(Parameter* selection);
    void parameterModified(Parameter* selection);

public slots:
    void setSelection(Parameter* selection);
    void emitParameterModified();

protected:
    Parameter* Selection_;
};

#endif // PARAMETERSELECTIONMODEL_H
