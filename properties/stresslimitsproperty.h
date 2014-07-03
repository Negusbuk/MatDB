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

#ifndef STRESSLIMITSPROPERTY_H
#define STRESSLIMITSPROPERTY_H

#include <QString>

#include <property.h>

class IsotropicStressLimitsProperty : public Property
{
public:

    IsotropicStressLimitsProperty(PropertyModel* propmodel,
                                ParameterModel* paramodel, int id);
    IsotropicStressLimitsProperty(const IsotropicStressLimitsProperty&);

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
};

class OrthotropicStressLimitsProperty : public Property
{
public:
    OrthotropicStressLimitsProperty(PropertyModel* propmodel,
                                  ParameterModel* paramodel, int id);
    OrthotropicStressLimitsProperty(const OrthotropicStressLimitsProperty&);

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
};

#endif // STRESSLIMITSPROPERTY_H
