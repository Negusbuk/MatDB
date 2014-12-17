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
