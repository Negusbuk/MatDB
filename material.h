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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <vector>
#include <string>

#include <QString>
#include <QXmlStreamWriter>

#include <materialcategory.h>
#include <propertymodel.h>
#include <property.h>
#include <parameter.h>

class Material
{
public:
    Material();
    Material(const Material& material);
    virtual ~Material();

    const QString& getName() const { return Name_; }
    void setName(const QString& name) { Name_ = name; }

    void addProperty(Property* property);
    void setProperties(const std::vector<Property*>&);
    Property* getProperty(const QString& name);
    std::map<QString,Property*>& getProperties();
    const std::vector<Property*>& getSortedProperties() const;
    void removeProperty(Property* property);

    void setDescription(const QString& Description) { Description_ = Description; }
    const QString& getDescription() const { return Description_; }

    void setCategory(MaterialCategory* c);
    MaterialCategory* getCategory() const { return Category_; }

    void setTags(const QStringList& t);
    const QStringList& getTags() const { return Tags_; }

    void setNotes(const QString& notes) { Notes_ = notes; }
    const QString& getNotes() const { return Notes_; }

    std::vector<ParameterValue> * getParameterValues(const QString& name);

    virtual void writeXML(QXmlStreamWriter& stream);

    Material* clone();

    static Material* makeDefaultIsotropicMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultOrthotropicMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultLiquidMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultGaseousMaterial(PropertyModel* propertyModel);

protected:

    QString Name_;
    std::map<QString,Property*> Properties_;
    std::map<Property::Type,Property*> PropertiesByType_;
    std::map<QString,std::vector<ParameterValue> > ParameterValues_;
    std::vector<Property*> PropertiesSorted_;

    QString Description_;
    QString Notes_;
    MaterialCategory* Category_;
    QStringList Tags_;
};

#endif // MATERIAL_H
