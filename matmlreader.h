#ifndef MATMLREADER_H
#define MATMLREADER_H

#include <map>
#include <vector>

#include <QMap>
#include <QObject>
#include <QDomDocument>
#include <QIODevice>

#include <materiallistmodel.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <materialcategorymodel.h>
#include <property.h>

class MATMLReader : public QObject
{
    Q_OBJECT

public:
    explicit MATMLReader(MaterialListModel *model,
                         PropertyModel *propmodel,
                         ParameterModel *paramodel,
                         MaterialCategoryModel* categoryModel,
                         QObject *parent = 0);
    
    void read(QIODevice *source);

signals:
    
public slots:
    
protected:

    MaterialListModel* model_;
    PropertyModel* propmodel_;
    ParameterModel* paramodel_;
    MaterialCategoryModel* categoryModel_;
    QDomDocument document_;

    std::map<QString,PropertyDetail> PropertyDetailMap_;
    std::map<QString,ParameterDetail> ParameterDetailMap_;

    void findMatMLDocElement(QDomElement& parent, QDomElement& element, bool& found);
    QString processUnits(QDomElement units);
    void processPropertyDetail(QDomElement& element);
    void processParameterDetail(QDomElement& element);
    PValue processParameterValue(QDomElement& element);
    PropertyData processPropertyData(QDomElement& element);
    void processMaterial(QDomElement& matElem, Material* mat);
};

#endif // MATMLREADER_H
