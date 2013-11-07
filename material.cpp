#include <iostream>
#include <algorithm>

#include <nqlogger.h>

#include <material.h>

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
    NQLog("Material", NQLog::Spam) << "Material::~Material()";
}

void Material::addProperty(Property* property)
{
    NQLog("Material", NQLog::Spam) << "void addProperty(Property* property)";

    std::map<Property::Type,Property*>::iterator it = PropertiesByType_.find(property->getType());
    if (it!=PropertiesByType_.end()) {
        Property * oldproperty = it->second;
        NQLog("Material", NQLog::Spam) << "remove old property " << oldproperty->getName();
        this->removeProperty(oldproperty);
    }

    Properties_[property->getName()] = property;
    PropertiesByType_[property->getType()] = property;

    NQLog("Material", NQLog::Spam) << "sorted properties for material " << getName();
    PropertiesSorted_.push_back(property);
    std::sort(PropertiesSorted_.begin(), PropertiesSorted_.end(),
              [](Property*lhs, Property*rhs) {
        NQLog("Material", NQLog::Spam) << "lhs " << lhs->getName() << " " << lhs->getSorting();
        NQLog("Material", NQLog::Spam) << "rhs " << rhs->getName() << " " << rhs->getSorting();
        return lhs->getSorting()<rhs->getSorting();
    });

    const std::vector<Property*>& v = getSortedProperties();
    for (std::vector<Property*>::const_iterator it = v.begin();
         it!=v.end();
         ++it) {
        NQLog("Material", NQLog::Spam) << "   " << (*it)->getName();
    }

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
    NQLog("Material", NQLog::Spam) << "Material::removeProperty(Property* property)";

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

Material* Material::makeDefaultMaterial(PropertyModel* propertyModel)
{
    Material * mat = new Material();
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
