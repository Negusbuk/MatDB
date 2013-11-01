#ifndef PARAMETERSELECTIONMODEL_H
#define PARAMETERSELECTIONMODEL_H

#include <QObject>

#include <parameter.h>

class ParameterSelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit ParameterSelectionModel(QObject *parent = 0);
    Parameter* getSelection();

signals:
    void selectionChanged(Parameter* selection);
    void parameterModified(Parameter* selection);

public slots:
    void setSelection(Parameter* selection);
    void emitParameterModified();

protected:
    Parameter* Selection_;
};

#endif // PARAMETERSELECTIONMODEL_H
