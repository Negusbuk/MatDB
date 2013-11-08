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

#ifndef CTEPROPERTY_H
#define CTEPROPERTY_H

#include <property.h>

class IsotropicCoefficientOfThermalExpansionProperty : public Property
{
public:
    IsotropicCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    IsotropicCoefficientOfThermalExpansionProperty(const IsotropicCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicCoefficientOfThermalExpansionProperty : public Property
{
public:
    OrthotropicCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    OrthotropicCoefficientOfThermalExpansionProperty(const OrthotropicCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class IsotropicSecantCoefficientOfThermalExpansionProperty : public Property
{
public:
    IsotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    IsotropicSecantCoefficientOfThermalExpansionProperty(const IsotropicSecantCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicSecantCoefficientOfThermalExpansionProperty : public Property
{
public:
    OrthotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    OrthotropicSecantCoefficientOfThermalExpansionProperty(const OrthotropicSecantCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // CTEPROPERTY_H
