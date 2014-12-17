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

#ifndef STRAINLIMITSPROPERTY_H
#define STRAINLIMITSPROPERTY_H

#include <QString>

#include <property.h>

class IsotropicStrainLimitsProperty : public Property
{
public:

    IsotropicStrainLimitsProperty(PropertyModel* propmodel,
                                ParameterModel* paramodel, int id);
    IsotropicStrainLimitsProperty(const IsotropicStrainLimitsProperty&);

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
};

class OrthotropicStrainLimitsProperty : public Property
{
public:
    OrthotropicStrainLimitsProperty(PropertyModel* propmodel,
                                  ParameterModel* paramodel, int id);
    OrthotropicStrainLimitsProperty(const OrthotropicStrainLimitsProperty&);

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
};

#endif // STRAINLIMITSPROPERTY_H
