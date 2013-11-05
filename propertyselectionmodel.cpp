#include <iostream>

#include "propertyselectionmodel.h"

PropertySelectionModel::PropertySelectionModel(QObject *parent) :
    QObject(parent)
{
    Selection_ = 0;
}

Property * PropertySelectionModel::getSelection()
{
    return Selection_;
}

void PropertySelectionModel::setSelection(Property* selection)
{
    if (Selection_!=selection) {
        Selection_ = selection;
        if (Selection_) {
            std::cout << "property selection changed: " << selection << std::endl;
        } else {
            std::cout << "property selection changed: 0" << std::endl;
        }
        emit selectionChanged(Selection_);
    }
}

void PropertySelectionModel::emitPropertyModified()
{
    emit propertyModified(Selection_);
}

void PropertySelectionModel::emitPropertyModified(Property* property)
{
    emit propertyModified(property);
}
