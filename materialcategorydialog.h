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

    void categoryChanged(MaterialCategory*);

public slots:

    void addCategory();
    void removeCategory();
    void categoryDoubleClicked(const QModelIndex& index);

    void storeGeometry();
    void applyGeometry();

protected:

    virtual void closeEvent(QCloseEvent* e);

    MaterialCategoryModel* categoryModel_;
    QListView* categories_;
    QLineEdit* nameEdit_;
    QPushButton* colorButton_;
    QToolButton* addCategoryButton_;
    QToolButton* removeCategoryButton_;

    QPoint positions_;
    QSize size_;
};

#endif // MATERIALCATEGORYDIALOG_H
