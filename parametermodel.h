#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <map>

#include <QObject>
#include <QString>

#include "Parameter.h"

class ParameterModel : public QObject
{
    Q_OBJECT
public:
    explicit ParameterModel(QObject *parent = 0);

    Parameter* getParameter(const char* name) const;
    const std::map<QString,Parameter*>& getParameters() const { return Parameters_; }
    const std::map<int,Parameter*>& getParametersById() const { return ParametersById_; }

signals:
    
public slots:

protected:

    std::map<QString,Parameter*> Parameters_;
    std::map<int,Parameter*> ParametersById_;

    void addParameter(Parameter* parameter);
    void build();
};

#endif // PARAMETERMODEL_H
