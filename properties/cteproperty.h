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

class IsotropicInstantaneousCoefficientOfThermalExpansion : public Property
{
public:
    IsotropicInstantaneousCoefficientOfThermalExpansion(ParameterModel* model, int id);
    IsotropicInstantaneousCoefficientOfThermalExpansion(const IsotropicInstantaneousCoefficientOfThermalExpansion&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicInstantaneousCoefficientOfThermalExpansion : public Property
{
public:
    OrthotropicInstantaneousCoefficientOfThermalExpansion(ParameterModel* model, int id);
    OrthotropicInstantaneousCoefficientOfThermalExpansion(const OrthotropicInstantaneousCoefficientOfThermalExpansion&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class IsotropicSecantCoefficientOfThermalExpansion : public Property
{
public:
    IsotropicSecantCoefficientOfThermalExpansion(ParameterModel* model, int id);
    IsotropicSecantCoefficientOfThermalExpansion(const IsotropicSecantCoefficientOfThermalExpansion&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicSecantCoefficientOfThermalExpansion : public Property
{
public:
    OrthotropicSecantCoefficientOfThermalExpansion(ParameterModel* model, int id);
    OrthotropicSecantCoefficientOfThermalExpansion(const OrthotropicSecantCoefficientOfThermalExpansion&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // CTEPROPERTY_H
