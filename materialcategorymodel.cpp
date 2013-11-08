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

#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QUuid>

#include "materialcategorymodel.h"

MaterialCategoryModel::MaterialCategoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    addCategory("No Category", QColor(255, 255, 255, 0), true);
    addCategory("Structural", QColor(100, 0, 0, 255), true);
    addCategory("Glue", QColor(0, 100, 0, 255), true);
}

void MaterialCategoryModel::addCategory(const QString& name,
                                        const QColor& bgColor,
                                        bool readonly)
{
    MaterialCategory * mc = new MaterialCategory(name, bgColor, readonly);
    mc->setUUID(QUuid::createUuid().toString());

    categoriesUUIDMap_[mc->getUUID()] = mc;
    categoriesMap_[name] = mc;
    categories_.push_back(mc);

    emit dataChanged(QModelIndex(), QModelIndex());
    emit categoriesChanged();
}

void MaterialCategoryModel::addCategory(const QString& uuid,
                                        const QString& name,
                                        const QColor& bgColor,
                                        bool readonly)
{
    MaterialCategory * mc = new MaterialCategory(name, bgColor, readonly);
    mc->setUUID(uuid);
    if (uuid.length()==0) mc->setUUID(QUuid::createUuid().toString());

    categoriesMap_[name] = mc;
    categoriesUUIDMap_[uuid] = mc;
    categories_.push_back(mc);

    emit dataChanged(QModelIndex(), QModelIndex());
    emit categoriesChanged();
}

void MaterialCategoryModel::removeCategory(const QString& name)
{
    MaterialCategory* category;
    QString uuid;

    std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(name);
    if (it!=categoriesMap_.end()) {
        MaterialCategory* category = it->second;
        if (category->isReadOnly()) return;

        categoriesMap_.erase(it);

        uuid = category->getUUID();
        std::map<QString,MaterialCategory*>::iterator ituuid = categoriesUUIDMap_.find(uuid);
        if (it!=categoriesMap_.end()) {
            categoriesUUIDMap_.erase(ituuid);
        }

        for (std::vector<MaterialCategory*>::iterator itv = categories_.begin();
             itv!= categories_.end();
             ++itv) {
            if ((*itv)==category) {
                categories_.erase(itv);
                break;
            }
        }

        delete category;
    }

    emit dataChanged(QModelIndex(), QModelIndex());
    emit categoriesChanged();
}

void MaterialCategoryModel::renameCategory(MaterialCategory* category, const QString& name)
{
    category->setName(name);

    for (std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.begin();
         it != categoriesMap_.end();
         ++it) {
            if (it->second==category) {
                categoriesMap_.erase(it);
                break;
            }
    }

    categoriesMap_[name] = category;
}

void MaterialCategoryModel::changedCategory(MaterialCategory* category)
{
    emit categoryChanged(category);
}

MaterialCategory* MaterialCategoryModel::getCategory(const QString& name)
{
    std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(name);
    if (it!=categoriesMap_.end()) return it->second;
    return 0;
}

MaterialCategory* MaterialCategoryModel::getCategoryByUUID(const QString& uuid)
{
    std::map<QString,MaterialCategory*>::iterator it = categoriesUUIDMap_.find(uuid);
    if (it!=categoriesMap_.end()) return it->second;
    return 0;
}

bool MaterialCategoryModel::isCategoryValid(MaterialCategory* category) const
{
    for (std::vector<MaterialCategory*>::const_iterator it = categories_.begin();
         it!= categories_.end();
         ++it) {
        if ((*it)==category) return true;
    }

    return false;
}

Qt::ItemFlags MaterialCategoryModel::flags(const QModelIndex & index) const
{
    int row = index.row();
    int column = index.column();

    if (row<0 && row>=categories_.size()) return Qt::NoItemFlags;

    MaterialCategory* category = categories_.at(row);

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MaterialCategoryModel::data(const QModelIndex & index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (row<0 && row>=categories_.size()) return QVariant();

    MaterialCategory* category = categories_.at(row);

    if (role==Qt::DisplayRole) {
        if (column==0) return QVariant(category->getName());
    }
    if (role==Qt::DecorationRole) {
        if (column==0) return QVariant(category->getIcon());
    }
    if (role==Qt::EditRole) {
        if (column==0) return QVariant(category->getName());
    }

    return QVariant();
}

//bool MaterialCategoryModel::setData(const QModelIndex & index, const QVariant & value, int role)
//{
//    if (role == Qt::EditRole) {
//        MaterialCategory *category = categories_.at(index.row());
//        //if (index.column()==0) {
//            if (!value.canConvert<QString>()) return false;
//            QString newName = value.toString();
//            if (getCategory(newName)) return false;

//            std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(category->getName());
//            if (it!=categoriesMap_.end()) {
//                std::swap(categoriesMap_[newName], it->second);
//                categoriesMap_.erase(it);
//            }
//            category->setName(newName);

//            emit dataChanged(index, index);
//            emit categoriesChanged();
//        //}
//    }

//    return true;
//}

void MaterialCategoryModel::read(QIODevice *source)
{
    QDomDocument document;
    if (!document.setContent(source)) return;

    QDomElement docElem = document.documentElement();
    QDomNodeList categoryElemList = docElem.elementsByTagName("Category");

    if (categoryElemList.count()==0) return;

    for (int i=0;i<categoryElemList.count();++i) {
        QDomElement catElem = categoryElemList.at(i).toElement();

        QDomElement uuid = catElem.elementsByTagName("UUID").at(0).toElement();
        QDomElement name = catElem.elementsByTagName("Name").at(0).toElement();

        MaterialCategory* category = getCategory(name.text());
        if (category!=NULL) {
            category->setUUID(uuid.text());
            if (category->getUUID().length()==0) category->setUUID(QUuid::createUuid().toString());
            continue;
        }

        QDomElement colorElem = catElem.elementsByTagName("Color").at(0).toElement();
        int red = colorElem.attribute("Red", "255").toInt();
        int green = colorElem.attribute("Green", "255").toInt();
        int blue = colorElem.attribute("Blue", "255").toInt();
        QColor color(red, green, blue);

        addCategory(uuid.text(), name.text(), color, false);
    }
}

void MaterialCategoryModel::write(QIODevice *destination)
{
    QXmlStreamWriter stream(destination);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("Categories");

    for (std::vector<MaterialCategory*>::const_iterator it = categories_.begin();
         it!=categories_.end();
         ++it) {
        MaterialCategory* category = *it;
        //if (category->isReadOnly()) continue;

        stream.writeStartElement("Category");

        stream.writeTextElement("UUID", category->getUUID());
        stream.writeTextElement("Name", category->getName());
        stream.writeStartElement("Color");
        const QColor &color = category->getColor();
        stream.writeAttribute("Red", QString::number(color.red()));
        stream.writeAttribute("Green", QString::number(color.green()));
        stream.writeAttribute("Blue", QString::number(color.blue()));
        stream.writeEndElement();

        stream.writeEndElement();
    }

    stream.writeEndElement();

    stream.writeEndDocument();
}
