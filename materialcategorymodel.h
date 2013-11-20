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

#ifndef MATERIALCATEGORYMODEL_H
#define MATERIALCATEGORYMODEL_H

#include <map>
#include <vector>

#include <QAbstractTableModel>
#include <QIODevice>

#include <materialcategory.h>

class MaterialCategoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MaterialCategoryModel(QObject *parent = 0);
    
    MaterialCategory* getCategory(const QString& name);
    MaterialCategory* getCategoryByUUID(const QString& uuid);
    bool isCategoryValid(MaterialCategory*) const;
    const std::vector<MaterialCategory*>& getCategories() const { return categories_; }

    virtual int	columnCount(const QModelIndex & /* parent = QModelIndex() */) const { return 1; }
    virtual int	rowCount(const QModelIndex & /* parent = QModelIndex() */) const { return categories_.size(); }

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    //virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    void read(QIODevice *source);
    void write(QIODevice *destination);

signals:

    void categoriesChanged();
    void categoryChanged(MaterialCategory* category);

public slots:

    void addCategory(const QString& name,
                     const QColor& bgColor,
                     bool readonly);
    void addCategory(const QString& uuid,
                     const QString& name,
                     const QColor& bgColor,
                     bool readonly);
    void removeCategory(const QString& name);
    void renameCategory(MaterialCategory* category, const QString& name);
    void changedCategory(MaterialCategory* category);

protected:

    void changedCategoryUUID(MaterialCategory* category, const QString& uuid);

    std::map<QString,MaterialCategory*> categoriesUUIDMap_;
    std::map<QString,MaterialCategory*> categoriesMap_;
    std::vector<MaterialCategory*> categories_;
};

#endif // MATERIALCATEGORYMODEL_H
