#include <iostream>

#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QTextStream>

#include "materialcategorymodel.h"

MaterialCategoryModel::MaterialCategoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    addCategory("Structural", QColor(100, 0, 0, 255), true);
    addCategory("Glue", QColor(0, 100, 0, 255), true);
}

void MaterialCategoryModel::addCategory(const QString& name,
                                        const QColor& bgColor,
                                        bool readonly)
{
    MaterialCategory * mc = new MaterialCategory(name, bgColor, readonly);
    categoriesMap_[name] = mc;
    categories_.push_back(mc);

    emit dataChanged(QModelIndex(), QModelIndex());
    emit categoriesChanged();
}

void MaterialCategoryModel::removeCategory(const QString& name)
{
    MaterialCategory* category;

    std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(name);
    if (it!=categoriesMap_.end()) {
        MaterialCategory* category = it->second;
        if (category->isReadOnly()) return;

        categoriesMap_.erase(it);

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

MaterialCategory* MaterialCategoryModel::getCategory(const QString& name)
{
    std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(name);
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

    if (!category->isReadOnly()) return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

    return Qt::NoItemFlags;
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

bool MaterialCategoryModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole) {
        MaterialCategory *category = categories_.at(index.row());
        if (index.column()==0) {
            if (!value.canConvert<QString>()) return false;
            QString newName = value.toString();
            if (getCategory(newName)) return false;

            std::map<QString,MaterialCategory*>::iterator it = categoriesMap_.find(category->getName());
            if (it!=categoriesMap_.end()) {
                std::swap(categoriesMap_[newName], it->second);
                categoriesMap_.erase(it);
            }
            category->setName(newName);

            emit dataChanged(index, index);
        }
    }

    return true;
}

void MaterialCategoryModel::read(QIODevice *source)
{
    QDomDocument document;
    if (!document.setContent(source)) return;

    QDomElement docElem = document.documentElement();
    QDomNodeList categoryElemList = docElem.elementsByTagName("Category");

    if (categoryElemList.count()==0) return;

    for (int i=0;i<categoryElemList.count();++i) {
        QDomElement catElem = categoryElemList.at(i).toElement();

        QDomElement name = catElem.elementsByTagName("Name").at(0).toElement();

        MaterialCategory* category = getCategory(name.text());
        if (category!=NULL) continue;

        QDomElement colorElem = catElem.elementsByTagName("Color").at(0).toElement();
        int red = colorElem.attribute("Red", "255").toInt();
        int green = colorElem.attribute("Green", "255").toInt();
        int blue = colorElem.attribute("Blue", "255").toInt();
        QColor color(red, green, blue);

        addCategory(name.text(), color, false);
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
        if (category->isReadOnly()) continue;

        stream.writeStartElement("Category");

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
