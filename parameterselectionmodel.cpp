#include <nqlogger.h>

#include <parameterselectionmodel.h>

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
            NQLog("ParameterSelectionModel", NQLog::Spam) << "parameter selection changed: "
                                                          << selection->getName();
        } else {
            NQLog("ParameterSelectionModel", NQLog::Spam) << "parameter selection changed: "
                                                          << selection;
        }
        emit selectionChanged(Selection_);
    }
}

void ParameterSelectionModel::emitParameterModified()
{
    emit parameterModified(Selection_);
}
