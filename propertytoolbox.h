#ifndef PROPERTYTOOLBOX_H
#define PROPERTYTOOLBOX_H

#include <vector>
#include <map>

#include <QLabel>
#include <QToolBox>
#include <QMouseEvent>

#include <property.h>
#include <propertymodel.h>

class PropertyToolBoxItem : public QLabel
{
    Q_OBJECT
public:
    explicit PropertyToolBoxItem(const QString &name,
                                 Property *property,
                                 QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);

    Property* Property_;
};

class PropertyToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit PropertyToolBox(PropertyModel *model,
                             QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:

    PropertyModel* PropertyModel_;
    void build();
};

#endif // PROPERTYTOOLBOX_H
