#include <nqlogger.h>

#include <propertyselectionmodel.h>

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
    NQLog("PropertySelectionModel", NQLog::Spam) << "void setSelection(Property* selection)"
                                                 << selection;

    Selection_ = selection;
    emit selectionChanged(Selection_);
}

void PropertySelectionModel::emitPropertyModified()
{
    emit propertyModified(Selection_);
}

void PropertySelectionModel::emitPropertyModified(Property* property)
{
    emit propertyModified(property);
}
