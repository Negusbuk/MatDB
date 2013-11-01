#ifndef MATERIALCATEGORYDIALOG_H
#define MATERIALCATEGORYDIALOG_H

#include <QDialog>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

#include <materialcategorymodel.h>

class MaterialCategoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MaterialCategoryDialog(MaterialCategoryModel* categoryModel,
                                    QWidget *parent = 0);

signals:

public slots:

    void selectionChanged();
    void addCategory();
    void removeCategory();

protected:

    MaterialCategoryModel* categoryModel_;
    QListView* categories_;
    QLineEdit* nameEdit_;
    QPushButton* colorButton_;
    QToolButton* addCategoryButton_;
    QToolButton* removeCategoryButton_;
};

#endif // MATERIALCATEGORYDIALOG_H
