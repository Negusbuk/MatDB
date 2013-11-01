#ifndef MATERIALMETADATAWIDGET_H
#define MATERIALMETADATAWIDGET_H

#include <QScrollArea>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QFormLayout>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <materialcategorymodel.h>

class MaterialMetadataWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit MaterialMetadataWidget(MaterialListModel* listModel,
                                    MaterialSelectionModel* selectionModel,
                                    MaterialCategoryModel* categoryModel,
                                    QWidget *parent = 0);
    
signals:
    
public slots:
    
    void materialChanged(Material* material);

protected:

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
    MaterialCategoryModel* categoryModel_;

    QWidget* content_;
    QFormLayout* layout_;

    QLineEdit* descriptionEdit_;
    QPlainTextEdit* notesEdit_;
};

#endif // MATERIALMETADATAWIDGET_H
