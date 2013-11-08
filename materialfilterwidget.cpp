#include <QLayout>

#include "materialfilterwidget.h"

MaterialFilterWidget::MaterialFilterWidget(MaterialListModel *listmodel,
                                           QWidget *parent) :
    QWidget(parent),
    ListModel_(listmodel)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 2, 2, 2);
    layout->setSpacing(2);
    setLayout(layout);

    lineEdit_ = new QLineEdit(this);
    lineEdit_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout->addWidget(lineEdit_);
    connect(lineEdit_, SIGNAL(textChanged(QString)),
            ListModel_, SLOT(filterChanged(QString)));

    resetButton_ = new QPushButton(QIcon(":/icons/MatDBResetFilter.png"), "", this);
    resetButton_->setFlat(true);
    resetButton_->setIconSize(QSize(24,24));
    resetButton_->setFixedSize(26, 26);
    layout->addWidget(resetButton_);
}
