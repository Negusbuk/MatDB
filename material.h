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
#include <QIODevice>
#include <QXmlStreamWriter>

#include <materialcategory.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <materialcategorymodel.h>
#include <property.h>

class Material
{
public:
    Material();
    Material(const Material& material);
    virtual ~Material();

    const QString& getUUID() const { return uuid_; }
    const QString& getName() const { return Name_; }

    Property* addProperty(Property* property);
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

    void setModified() { modified_ = true; }
    bool isModified() const;

    virtual void write(QIODevice* device);
    virtual bool read(QIODevice* device,
                      PropertyModel *propmodel,
                      MaterialCategoryModel *categorymodel);

    virtual void writeXML(QXmlStreamWriter& stream);

    Material* clone();

    static Material* makeDefaultIsotropicMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultOrthotropicMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultLiquidMaterial(PropertyModel* propertyModel);
    static Material* makeDefaultGaseousMaterial(PropertyModel* propertyModel);

public slots:

    void setUUID(const QString& uuid) { uuid_ = uuid; }
    void setName(const QString& name) { Name_ = name; }

protected:

    QString uuid_;
    QString Name_;
    std::map<QString,Property*> Properties_;
    std::map<Property::Type,Property*> PropertiesByType_;
    std::vector<Property*> PropertiesSorted_;

    QString Description_;
    QString Notes_;
    MaterialCategory* Category_;
    QStringList Tags_;
    bool modified_;
};

#endif // MATERIAL_H
