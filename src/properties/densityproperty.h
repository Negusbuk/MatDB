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

#ifndef DENSITYPROPERTY_H
#define DENSITYPROPERTY_H

#include <property.h>

class DensityProperty : public Property
{
public:
    DensityProperty(PropertyModel* propmodel, ParameterModel* paramodel, int id);
    DensityProperty(const DensityProperty&);

    virtual Property* clone(PropertyModel* propmodel = 0,
                            ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeXMLData(QXmlStreamWriter& stream);
};

#endif // DENSITYPROPERTY_H
