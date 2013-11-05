#include <vector>

#include "unitcombobox.h"

UnitComboBox::UnitComboBox(Unit::VUnit *unit,
                           QWidget *parent) :
    QComboBox(parent)
{
    if (unit) setUnit(unit);
}

void UnitComboBox::setUnit(Unit::VUnit *unit)
{
    clear();
    if (!unit) return;

    const std::vector<QString>& units = unit->getUnits();
    for (std::vector<QString>::const_iterator it = units.begin();
         it!=units.end();
         ++it) {
        addItem(*it);
    }

    setCurrentIndex(unit->currentUnit());
}
