#ifndef PROPERTYSELECTIONMODEL_H
#define PROPERTYSELECTIONMODEL_H

#include <QObject>

#include <property.h>

class PropertySelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit PropertySelectionModel(QObject *parent = 0);
    Property* getSelection();

signals:
    void selectionChanged(Property* selection);
    void propertyModified(Property* selection);

public slots:
    void setSelection(Property* selection);
    void emitPropertyModified();
    void emitPropertyModified(Property* property);

protected:
    Property* Selection_;
};

#endif // PROPERTYSELECTIONMODEL_H
