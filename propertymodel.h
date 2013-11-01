#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>

#include <property.h>
#include <parametermodel.h>

class PropertyModel : public QObject
{
    Q_OBJECT
public:
    explicit PropertyModel(ParameterModel* parametermodel,
                           QObject *parent = 0);
    
    const std::vector<QString> getCategories() const { return Categories_; }

    const std::map<QString,Property*>& getProperties() const { return Properties_; }
    const std::map<int,Property*>& getPropertiesById() const { return PropertiesById_; }
    Property* getProperty(const QString& name);
    const std::map<QString,std::vector<Property*> >& getPropertiesByCategory() const {
        return PropertiesByCategory_;
    }
    const std::map<QString,std::vector<Property*> >& getPropertiesByType() const {
        return PropertiesByType_;
    }

    int getPropertySorting(Property* property) const;

signals:
    
public slots:
    
protected:

    ParameterModel* ParameterModel_;
    std::map<QString,Property*> Properties_;
    std::map<int,Property*> PropertiesById_;
    std::map<Property*,int> PropertiesSorting_;
    std::map<QString,std::vector<Property*> > PropertiesByCategory_;
    std::map<QString,std::vector<Property*> > PropertiesByType_;
    std::vector<QString> Categories_;

    void addProperty(Property* property);
    void build();
};

#endif // PROPERTYMODEL_H
