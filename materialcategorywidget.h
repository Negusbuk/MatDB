#ifndef MATERIALCATEGORYWIDGET_H
#define MATERIALCATEGORYWIDGET_H

#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

#include <materialcategorymodel.h>

class MaterialCategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialCategoryWidget(MaterialCategoryModel* categoryModel,
                                    QWidget *parent = 0);

signals:

    void categoryChanged(MaterialCategory*);

public slots:

    void addCategory();
    void removeCategory();
    void categoryDoubleClicked(const QModelIndex& index);

protected:

    MaterialCategoryModel* categoryModel_;
    QListView* categories_;
    QLineEdit* nameEdit_;
    QPushButton* colorButton_;
    QToolButton* addCategoryButton_;
    QToolButton* removeCategoryButton_;
};

#endif // MATERIALCATEGORYWIDGET_H
