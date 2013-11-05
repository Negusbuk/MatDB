#ifndef MATERIALCATEGORYBOX_H
#define MATERIALCATEGORYBOX_H

#include <QComboBox>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <materialcategorymodel.h>

class MaterialCategoryBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MaterialCategoryBox(MaterialListModel* listModel,
                                 MaterialSelectionModel* selectionModel,
                                 MaterialCategoryModel* categoryModel,
                                 QWidget *parent = 0);

signals:

    void materialMetadataChanged(Material*);

public slots:

    void materialChanged(Material* material);
    void selectedCategoryChanged(QString);

protected:

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
    MaterialCategoryModel* categoryModel_;
};

#endif // MATERIALCATEGORYBOX_H
