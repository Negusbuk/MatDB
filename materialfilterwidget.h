#ifndef MATERIALFILTERWIDGET_H
#define MATERIALFILTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include <materiallistmodel.h>

class MaterialFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialFilterWidget(MaterialListModel *listmodel,
                                  QWidget *parent = 0);

signals:

public slots:

protected:

    MaterialListModel* ListModel_;
    QLineEdit* lineEdit_;
    QPushButton* resetButton_;
};

#endif // MATERIALFILTERWIDGET_H
