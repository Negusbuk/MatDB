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

#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>

#include <property.h>
#include <parametermodel.h>

class PropertyModel : public QObject
{
    Q_OBJECT
public:
    explicit PropertyModel(ParameterModel* parametermodel,
                           QObject *parent = 0);
    
    const std::vector<QString> getCategories() const { return Categories_; }

    const std::map<QString,Property*>& getProperties() const { return Properties_; }
    const std::map<int,Property*>& getPropertiesById() const { return PropertiesById_; }
    Property* getProperty(const QString& name);
    const std::map<QString,std::vector<Property*> >& getPropertiesByCategory() const {
        return PropertiesByCategory_;
    }
    const std::map<QString,std::vector<Property*> >& getPropertiesByType() const {
        return PropertiesByType_;
    }

    int getPropertySorting(Property* property) const;

signals:
    
public slots:
    
protected:

    ParameterModel* ParameterModel_;
    std::map<QString,Property*> Properties_;
    std::map<int,Property*> PropertiesById_;
    std::map<Property*,int> PropertiesSorting_;
    std::map<QString,std::vector<Property*> > PropertiesByCategory_;
    std::map<QString,std::vector<Property*> > PropertiesByType_;
    std::vector<QString> Categories_;

    void addProperty(Property* property);
    void build();
};

#endif // PROPERTYMODEL_H
