#include <iostream>

#include "parameterselectionmodel.h"

ParameterSelectionModel::ParameterSelectionModel(QObject *parent) :
    QObject(parent)
{
    Selection_ = 0;
}

Parameter * ParameterSelectionModel::getSelection()
{
    return Selection_;
}

void ParameterSelectionModel::setSelection(Parameter* selection)
{
    if (Selection_ != selection) {
        Selection_ = selection;
        if (Selection_) {
            std::cout << "parameter selection changed: " << selection->getName().toStdString() << std::endl;
        } else {
            std::cout << "parameter selection changed: 0" << std::endl;
        }
        emit selectionChanged(Selection_);
    }
}

void ParameterSelectionModel::emitParameterModified()
{
    emit parameterModified(Selection_);
}
