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

#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <map>

#include <QObject>
#include <QString>

#include "Parameter.h"

class ParameterModel : public QObject
{
    Q_OBJECT
public:
    explicit ParameterModel(QObject *parent = 0);

    Parameter* getParameter(const char* name) const;
    const std::map<QString,Parameter*>& getParameters() const { return Parameters_; }
    const std::map<int,Parameter*>& getParametersById() const { return ParametersById_; }

signals:
    
public slots:

protected:

    std::map<QString,Parameter*> Parameters_;
    std::map<int,Parameter*> ParametersById_;

    void addParameter(Parameter* parameter);
    void build();
};

#endif // PARAMETERMODEL_H
