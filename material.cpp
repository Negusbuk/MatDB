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

#include <iostream>
#include <algorithm>

#include <QUuid>
#include <QDomDocument>

#include <nqlogger.h>

#include <material.h>

Material::Material()
    : Description_(""),
      Notes_(""),
      Category_(0),
      modified_(false)
{

}

Material::Material(const Material& material)
    : modified_(false)
{
    setName(material.getName());
    setProperties(material.getSortedProperties());
    setDescription(material.getDescription());
    setNotes(material.getNotes());
    setCategory(material.getCategory());
    setTags(material.getTags());
}

Material::~Material()
{
    NQLog("Material", NQLog::Spam) << "Material::~Material()";
}

Property *Material::addProperty(Property* property)
{
    // NQLog("Material", NQLog::Spam) << "void addProperty(Property* property)";

    std::map<Property::Type,Property*>::iterator it = PropertiesByType_.find(property->getType());
    if (it!=PropertiesByType_.end()) {
        Property * oldproperty = it->second;
        // NQLog("Material", NQLog::Spam) << "remove old property " << oldproperty->getName();
        this->removeProperty(oldproperty);
    }

    Properties_[property->getName()] = property;
    PropertiesByType_[property->getType()] = property;

    NQLog("Material", NQLog::Spam) << "sorted properties for material " << getName();
    PropertiesSorted_.push_back(property);
    std::sort(PropertiesSorted_.begin(), PropertiesSorted_.end(),
              [](Property*lhs, Property*rhs) {

        // NQLog("Material", NQLog::Spam) << "lhs " << lhs->getName() << " " << lhs->getSorting();
        // NQLog("Material", NQLog::Spam) << "rhs " << rhs->getName() << " " << rhs->getSorting();

        return lhs->getSorting()<rhs->getSorting();
    });

    const std::vector<Property*>& v = getSortedProperties();
    for (std::vector<Property*>::const_iterator it = v.begin();
         it!=v.end();
         ++it) {
        NQLog("Material", NQLog::Spam) << "   " << (*it)->getName();
    }

    modified_ = true;

    return property;
}

void Material::setProperties(const std::vector<Property*>& properties)
{
    Properties_.clear();
    PropertiesByType_.clear();
    PropertiesSorted_.clear();

    for (std::vector<Property*>::const_iterator it = properties.begin();
         it!=properties.end();
         ++it) {
        Property * property = *it;
        addProperty(property->clone());
    }

    modified_ = true;
}

Property* Material::getProperty(const QString& name)
{
    std::map<QString,Property*>::iterator it = Properties_.find(name);
    if (it!=Properties_.end()) {
        Property* property = it->second;
        return property;
    }
    return NULL;
}

std::map<QString,Property*>& Material::getProperties()
{
    return Properties_;
}

const std::vector<Property*>& Material::getSortedProperties() const
{
    return PropertiesSorted_;
}

void Material::removeProperty(Property* property)
{
    // NQLog("Material", NQLog::Spam) << "Material::removeProperty(Property* property)";

    for (std::map<QString,Property*>::iterator it = Properties_.begin();
         it!=Properties_.end();
         ++it) {
        if (it->second==property) {
            Properties_.erase(it);
            break;
        }
    }

    for (std::map<Property::Type,Property*>::iterator it = PropertiesByType_.begin();
         it!=PropertiesByType_.end();
         ++it) {
        if (it->second==property) {
            PropertiesByType_.erase(it);
            break;
        }
    }

    for (std::vector<Property*>::iterator it = PropertiesSorted_.begin();
         it!=PropertiesSorted_.end();
         ++it) {
        if ((*it)==property) {
            PropertiesSorted_.erase(it);
            break;
        }
    }

    delete property;

    modified_ = true;
}

void Material::setCategory(MaterialCategory* c)
{
    // NQLog("Material", NQLog::Spam) << "setCategory " << c;

    modified_ = true;
    Category_ = c;
}

void Material::setTags(const QStringList& t)
{
    modified_ = true;
    Tags_ = t;
}

void Material::write(QIODevice* device)
{
    QXmlStreamWriter stream(device);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Material");

    stream.writeTextElement("Name", getName());
    stream.writeTextElement("UUID", getUUID());
    stream.writeTextElement("Description", getDescription());
    stream.writeTextElement("Notes", getNotes());

    if (getCategory())
        stream.writeTextElement("Category", getCategory()->getUUID());

    for (QStringList::ConstIterator itT = getTags().begin();
         itT!=getTags().end();
         ++itT) {
         stream.writeTextElement("Tag", *itT);
    }

    for (std::vector<Property*>::iterator it = PropertiesSorted_.begin();
         it!=PropertiesSorted_.end();
         ++it) {
        Property* property = *it;
        stream.writeStartElement("Property");
        property->write(stream);
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    modified_ = false;
}

bool Material::read(QIODevice* device,
                    PropertyModel *propmodel,
                    MaterialCategoryModel *categorymodel)
{
    QDomDocument document;
    if (!document.setContent(device)) return false;

    QDomElement docElem = document.documentElement();
    QDomNodeList matElemList = docElem.elementsByTagName("Material");
    QDomElement matElem = matElemList.at(0).toElement();

    QDomNodeList descriptionElemList = docElem.elementsByTagName("Description");
    if (descriptionElemList.count()==1) {
        setDescription(descriptionElemList.at(0).toElement().text());
    }

    QDomNodeList notesElemList = docElem.elementsByTagName("Notes");
    if (notesElemList.count()==1) {
        setNotes(notesElemList.at(0).toElement().text());
    }

    QDomNodeList categoryElemList = docElem.elementsByTagName("Category");
    if (categoryElemList.count()==1) {
        NQLog("Material", NQLog::Spam) << "Cat: " << categoryElemList.at(0).toElement().text();
        MaterialCategory* category = categorymodel->getCategoryByUUID(categoryElemList.at(0).toElement().text());
        NQLog("Material", NQLog::Spam) << "Cat: " << category;
        NQLog("Material", NQLog::Spam) << "Cat: " << category->getName();
        setCategory(category);
    }

    QDomNodeList tagElemList = docElem.elementsByTagName("Tag");
    QStringList tags;
    for (int i=0;i<tagElemList.size();++i) {
        tags << tagElemList.at(i).toElement().text();
    }
    setTags(tags);

    QDomNodeList propElemList = docElem.elementsByTagName("Property");
    for (int i=0;i<propElemList.size();++i) {
        QDomElement propElem = propElemList.at(i).toElement();

        QDomElement name = propElem.elementsByTagName("Name").at(0).toElement();

        NQLog("Material", NQLog::Spam) << "Prop: " << name.text();

        Property* property = propmodel->getProperty(name.text());
        if (!property) continue;
        property = property->clone();

        // NQLog("Material", NQLog::Spam) << "Prop: " << property;
        // NQLog("Material", NQLog::Spam) << "Prop: " << property->getName();

        property->read(propElem);

        addProperty(property);
    }

    return true;
}

void Material::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("Material");

    stream.writeStartElement("BulkDetails");

    stream.writeTextElement("Name", getName());

    if (getCategory()) {
        stream.writeTextElement("Category", getCategory()->getName());
    }

    for (QStringList::ConstIterator it = Tags_.begin();
         it!=Tags_.end();
         ++it) {
        QString c = *it;
        stream.writeTextElement("Tag", c);
    }

    if (Notes_.length()!=0) {
        QStringList notes = Notes_.split('\n');
        for (QStringList::ConstIterator it = notes.begin();
             it!=notes.end();
             ++it) {
            QString n = *it;
            if (n.length()==0) continue;
            stream.writeTextElement("Note", n);
        }
    }

    for (std::vector<Property*>::const_iterator it = PropertiesSorted_.begin();
         it!=PropertiesSorted_.end();
         ++it) {
        Property *property = *it;

        NQLog("Material", NQLog::Message) << "blah";
        property->writeXMLData(stream);
        NQLog("Material", NQLog::Message) << "blah";
    }

    stream.writeEndElement();
    stream.writeEndElement();
}

Material* Material::clone()
{
    Material* mat = new Material(*this);
    mat->setUUID(QUuid::createUuid().toString());
    return mat;
}

Material* Material::makeDefaultIsotropicMaterial(PropertyModel* propertyModel)
{
    Material * mat = new Material();
    mat->setUUID(QUuid::createUuid().toString());
    mat->setName("Structural Steel");

    Property * prop;

    prop = propertyModel->getProperty("Density")->clone();
    prop->getParameter("Density")->addValue(7.85);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Elasticity")->clone();
    prop->getParameter("Young's Modulus")->addValue(200);
    prop->getParameter("Poisson's Ratio")->addValue(0.3);
    prop->recalculate();
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Secant Coefficient of Thermal Expansion")->clone();
    prop->getParameter("Coefficient of Thermal Expansion")->addValue(12);
    prop->getParameter("Reference Temperature")->addValue(22);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Thermal Conductivity")->clone();
    prop->getParameter("Thermal Conductivity")->addValue(60.5);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Specific Heat")->clone();
    prop->getParameter("Specific Heat")->addValue(434);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Resistivity")->clone();
    prop->getParameter("Resistivity")->addValue(1.7e-7);
    mat->addProperty(prop);

    return mat;
}

Material* Material::makeDefaultOrthotropicMaterial(PropertyModel* propertyModel)
{
    Material * mat = new Material();
    mat->setUUID(QUuid::createUuid().toString());
    mat->setName("Orthotropic Structural Steel");

    Property * prop;

    prop = propertyModel->getProperty("Density")->clone();
    prop->getParameter("Density")->addValue(7.85);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Orthotropic Elasticity")->clone();
    prop->getParameter("Young's Modulus X direction")->addValue(200);
    prop->getParameter("Young's Modulus Y direction")->addValue(200);
    prop->getParameter("Young's Modulus Z direction")->addValue(200);
    prop->getParameter("Poisson's Ratio XY")->addValue(0.3);
    prop->getParameter("Poisson's Ratio YZ")->addValue(0.3);
    prop->getParameter("Poisson's Ratio XZ")->addValue(0.3);
    prop->getParameter("Shear Modulus XY")->addValue(76.923);
    prop->getParameter("Shear Modulus YZ")->addValue(76.923);
    prop->getParameter("Shear Modulus XZ")->addValue(76.923);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Orthotropic Secant Coefficient of Thermal Expansion")->clone();
    prop->getParameter("Coefficient of Thermal Expansion X direction")->addValue(12);
    prop->getParameter("Coefficient of Thermal Expansion Y direction")->addValue(12);
    prop->getParameter("Coefficient of Thermal Expansion Z direction")->addValue(12);
    prop->getParameter("Reference Temperature")->addValue(22);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Orthotropic Thermal Conductivity")->clone();
    prop->getParameter("Thermal Conductivity X direction")->addValue(60.5);
    prop->getParameter("Thermal Conductivity Y direction")->addValue(60.5);
    prop->getParameter("Thermal Conductivity Z direction")->addValue(60.5);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Specific Heat")->clone();
    prop->getParameter("Specific Heat")->addValue(434);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Orthotropic Resistivity")->clone();
    prop->getParameter("Resistivity X direction")->addValue(1.7e-7);
    prop->getParameter("Resistivity Y direction")->addValue(1.7e-7);
    prop->getParameter("Resistivity Z direction")->addValue(1.7e-7);
    mat->addProperty(prop);

    return mat;
}

Material* Material::makeDefaultLiquidMaterial(PropertyModel* propertyModel)
{
    Material * mat = new Material();
    mat->setUUID(QUuid::createUuid().toString());
    mat->setName("Water");

    Property * prop;

    prop = propertyModel->getProperty("Density")->clone();
    prop->getParameter("Density")->addValue(0.9982);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Specific Heat")->clone();
    prop->getParameter("Specific Heat")->addValue(4182);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Viscosity")->clone();
    prop->getParameter("Viscosity")->addValue(0.001003);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Boiling Point")->clone();
    prop->getParameter("Boiling Point")->addValue(373, "K");
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Thermal Conductivity")->clone();
    prop->getParameter("Thermal Conductivity")->addValue(0.6);
    mat->addProperty(prop);

    return mat;
}

Material* Material::makeDefaultGaseousMaterial(PropertyModel* propertyModel)
{
    Material * mat = new Material();
    mat->setUUID(QUuid::createUuid().toString());
    mat->setName("Air");

    Property * prop;

    prop = propertyModel->getProperty("Density")->clone();
    prop->getParameter("Density")->addValue(0.001225);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Specific Heat")->clone();
    prop->getParameter("Specific Heat")->addValue(1006.4);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Critical Temperature")->clone();
    prop->getParameter("Critical Temperature")->addValue(132.3, "K");
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Critical Pressure")->clone();
    prop->getParameter("Critical Pressure")->addValue(3.758, "MPa");
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Viscosity")->clone();
    prop->getParameter("Viscosity")->addValue(1.7894e-5);
    mat->addProperty(prop);

    prop = propertyModel->getProperty("Isotropic Thermal Conductivity")->clone();
    prop->getParameter("Thermal Conductivity")->addValue(0.0242);
    mat->addProperty(prop);

    return mat;
}

bool Material::isModified() const
{
    if (modified_) return modified_;

    return false;
}
