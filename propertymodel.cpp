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

#include "densityproperty.h"
#include "elasticityproperty.h"
#include "thermalconductivityproperty.h"
#include "cteproperty.h"
#include "resistivityproperty.h"
#include "specificheatproperty.h"
#include "viscosityproperty.h"

#include "propertymodel.h"

PropertyModel::PropertyModel(ParameterModel* parametermodel,
                             QObject *parent) :
    QObject(parent),
    ParameterModel_(parametermodel)
{
    build();
}

Property* PropertyModel::getProperty(const QString& name)
{
    std::map<QString,Property*>::iterator it = Properties_.find(name);
    if (it!=Properties_.end()) return it->second->clone(ParameterModel_);
    return NULL;
}

void PropertyModel::addProperty(Property* property)
{
    std::map<QString,Property*>::iterator it = Properties_.find(property->getName());
    Property * prop = NULL;
    if (it!=Properties_.end()) prop = it->second;

    Properties_[property->getName()] = property;
    PropertiesById_[property->getId()] = property;
    PropertiesByCategory_[property->getCategoryName()].push_back(property);
    PropertiesByType_[property->getTypeName()].push_back(property);

    property->setSorting((int)PropertiesSorting_.size());
    PropertiesSorting_[property] = (int)PropertiesSorting_.size();

    if (prop) delete prop;
}

void PropertyModel::build()
{
    int id = 0;

    addProperty(new DensityProperty(ParameterModel_, id++));

    addProperty(new IsotropicCoefficientOfThermalExpansionProperty(ParameterModel_, id));
    addProperty(new OrthotropicCoefficientOfThermalExpansionProperty(ParameterModel_, id++));

    addProperty(new IsotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel_, id));
    addProperty(new OrthotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel_, id++));

    addProperty(new IsotropicElasticityProperty(ParameterModel_, id));
    addProperty(new OrthotropicElasticityProperty(ParameterModel_, id++));

    addProperty(new IsotropicThermalConductivityProperty(ParameterModel_, id));
    addProperty(new OrthotropicThermalConductivityProperty(ParameterModel_, id++));

    addProperty(new IsotropicResistivityProperty(ParameterModel_, id));
    addProperty(new OrthotropicResistivityProperty(ParameterModel_, id++));

    addProperty(new SpecificHeatProperty(ParameterModel_, id++));

    addProperty(new ViscosityProperty(ParameterModel_, id++));

    Categories_.push_back("Physical Properties");
    Categories_.push_back("Linear Elastic Properties");
    Categories_.push_back("Thermal Properties");
    Categories_.push_back("Electrical Properties");
    Categories_.push_back("Fluid Properties");
}

int PropertyModel::getPropertySorting(Property* property) const
{
    std::map<Property*,int>::const_iterator it = PropertiesSorting_.find(property);
    if (it!=PropertiesSorting_.end()) return it->second;
    return -1;
}
