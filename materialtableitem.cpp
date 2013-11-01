#include "materialtableitem.h"

MaterialTableItem::MaterialTableItem(Material* material,
                                     int type):
    QTableWidgetItem(type),
    Material_(material)
{

}
