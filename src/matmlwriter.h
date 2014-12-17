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

#ifndef MATMLWRITER_H
#define MATMLWRITER_H

#include <map>
#include <vector>

#include <QMap>
#include <QObject>
#include <QIODevice>

#include <materiallistmodel.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <property.h>

class MATMLWriter : public QObject
{
    Q_OBJECT

public:

    enum ExportMode {
        ANSYS           = 1,
        MatML           = 2,
        Unknown
    };

    explicit MATMLWriter(const std::vector<Material*>& materials,
                         PropertyModel *propmodel,
                         ParameterModel *paramodel,
                         QObject *parent = 0);
    
    void write(QIODevice *destination, ExportMode mode);

signals:
    
public slots:
    
protected:

    const std::vector<Material*>& materials_;
    std::map<int,int> properties_;
    std::map<int,int> parameters_;
    PropertyModel* propmodel_;
    ParameterModel* paramodel_;
};

#endif // MATMLWRITER_H
