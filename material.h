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

    static Material* makeDefaultMaterial(PropertyModel* propertyModel);

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
