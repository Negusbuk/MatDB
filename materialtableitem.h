#ifndef MATERIALTABLEITEM_H
#define MATERIALTABLEITEM_H

#include <QTableWidgetItem>

#include <material.h>

class MaterialTableItem : public QTableWidgetItem
{
public:
    explicit MaterialTableItem(Material* material,
                               int type);

    Material* getMaterial() { return Material_; }

protected:

    Material* Material_;
};

#endif // MATERIALTABLEITEM_H
