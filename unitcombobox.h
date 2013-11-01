#ifndef UNITCOMBOBOX_H
#define UNITCOMBOBOX_H

#include <QComboBox>

#include <unit.h>

class UnitComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UnitComboBox(Unit::VUnit *unit = 0,
                          QWidget *parent = 0);
    void setUnit(Unit::VUnit *unit);

signals:
    
public slots:

protected:
    Unit::VUnit *Unit_;
};

#endif // UNITCOMBOBOX_H
