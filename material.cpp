#include <iostream>
#include <algorithm>

#include "material.h"

Material::Material()
    : Description_(""),
      Notes_(""),
      Category_(0)
{

}

Material::Material(const Material& material)
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
    std::cout << "Material::~Material()" << std::endl;
}

void Material::addProperty(Property* property)
{
    std::map<Property::Type,Property*>::iterator it = PropertiesByType_.find(property->getType());
    if (it!=PropertiesByType_.end()) {
        Property * oldproperty = it->second;
        Properties_.erase(oldproperty->getName());
        delete oldproperty;
    }

    Properties_[property->getName()] = property;
    PropertiesByType_[property->getType()] = property;

    PropertiesSorted_.push_back(property);
    std::sort(PropertiesSorted_.begin(), PropertiesSorted_.end(),
              [](Property*lhs, Property*rhs) {
        //std::cout << "#### lhs " << lhs->getSorting() << " " << lhs->getName().toStdString() << std::endl;
        //std::cout << "#### rhs " << rhs->getSorting() << " " << rhs->getName().toStdString() << std::endl;
        return lhs->getSorting()<rhs->getSorting();
    });

    std::map<QString,Parameter*>& pmap = property->getParameters();
    for (std::map<QString,Parameter*>::iterator itP = pmap.begin();
         itP!=pmap.end();
         ++itP) {
        ParameterValues_[QString(itP->second->getName())] = std::vector<ParameterValue>();
    }
}

void Material::setProperties(const std::vector<Property*>& properties)
{
    Properties_.clear();
    PropertiesByType_.clear();
    ParameterValues_.clear();
    PropertiesSorted_.clear();

    for (std::vector<Property*>::const_iterator it = properties.begin();
         it!=properties.end();
         ++it) {
        Property * property = *it;
        addProperty(property->clone());
    }
}

Property* Material::getProperty(const QString& name)
{
    std::map<QString,Property*>::iterator it = Properties_.find(name);
    if (it!=Properties_.end()) {
        Property* property = it->second;

        std::map<QString,Parameter*>& pmap = property->getParameters();
        for (std::map<QString,Parameter*>::iterator itP = pmap.begin();
             itP!=pmap.end();
             ++itP) {
            std::vector<ParameterValue>* pvalues = getParameterValues(itP->second->getName());
            itP->second->setParameterValues(pvalues);
        }
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
    std::cout << "Material::removeProperty(Property* property)" << std::endl;

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

    std::map<QString,std::vector<ParameterValue> >::iterator it = ParameterValues_.find(property->getName());
    if (it!=ParameterValues_.end()) {
        ParameterValues_.erase(it);
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
}

std::vector<ParameterValue> * Material::getParameterValues(const QString& name)
{
    std::map<QString,std::vector<ParameterValue> >::iterator it = ParameterValues_.find(name);
    if (it!=ParameterValues_.end()) return &(it->second);
    //std::cout << name.toStdString() << " not found" << std::endl;
    return NULL;
}

void Material::setCategory(MaterialCategory* c)
{
    Category_ = c;
}

void Material::setTags(const QStringList& t)
{
    Tags_ = t;
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
        Property * property = *it;
        property->writeXMLData(stream);
    }

    stream.writeEndElement();
    stream.writeEndElement();
}

Material* Material::clone()
{
    Material* mat = new Material(*this);
    return mat;
}

Material* Material::makeDefaultMaterial()
{

}
