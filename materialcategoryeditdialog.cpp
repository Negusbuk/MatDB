#include <iostream>

#include <QLayout>
#include <QFormLayout>
#include <QPushButton>

#include "materialcategoryeditdialog.h"

MaterialCategoryEditDialog::MaterialCategoryEditDialog(QWidget *parent) :
    QDialog(parent)
{   
//    Qt::WindowFlags flags = 0;
//    flags |= Qt::CustomizeWindowHint;
//    flags |= Qt::WindowTitleHint;
//    flags |= Qt::Tool;
//    setWindowFlags(flags);

    QVBoxLayout * layout = new QVBoxLayout(this);
    setLayout(layout);

    QWidget * widget;

    widget = new QWidget(this);
    layout->addWidget(widget);

    QFormLayout *formLayout = new QFormLayout(widget);
    widget->setLayout(formLayout);

    nameEdit_ = new QLineEdit(widget);
    formLayout->addRow("Name:", nameEdit_);

    colorPicker_ = new NQColorWheel(widget);
    layout->addWidget(colorPicker_);

    widget = new QWidget(this);
    layout->addWidget(widget);

    QHBoxLayout *buttonLayout = new QHBoxLayout(widget);
    widget->setLayout(buttonLayout);

    QPushButton *button;
    button = new QPushButton("Ok", this);
    buttonLayout->addWidget(button);
    connect(button, SIGNAL(clicked()),
            this, SLOT(accept()));

    button = new QPushButton("Cancel", this);
    buttonLayout->addWidget(button);
    connect(button, SIGNAL(clicked()),
            this, SLOT(reject()));

    updateGeometry();
}
