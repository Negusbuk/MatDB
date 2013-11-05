#ifndef PROPERTYSPECIALWIDGET_H
#define PROPERTYSPECIALWIDGET_H

#include <QWidget>

class PropertySpecialWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertySpecialWidget(QWidget *parent = 0);

signals:
    void modified();

public slots:

    virtual void updateContents() { }
};

#endif // PROPERTYSPECIALWIDGET_H
