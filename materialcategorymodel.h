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

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const { return 1; }
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const { return categories_.size(); }

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

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

    std::map<QString,MaterialCategory*> categoriesUUIDMap_;
    std::map<QString,MaterialCategory*> categoriesMap_;
    std::vector<MaterialCategory*> categories_;
};

#endif // MATERIALCATEGORYMODEL_H
