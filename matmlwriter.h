#ifndef MATMLWRITER_H
#define MATMLWRTIER_H

#include <map>
#include <vector>

#include <QMap>
#include <QObject>
#include <QIODevice>

#include <materiallistmodel.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <property.h>

class MATMLWriter : public QObject
{
    Q_OBJECT

public:

    enum ExportMode {
        ANSYS           = 1,
        MatML           = 2,
        Unknown
    };

    explicit MATMLWriter(const std::vector<Material*>& materials,
                         PropertyModel *propmodel,
                         ParameterModel *paramodel,
                         QObject *parent = 0);
    
    void write(QIODevice *destination, ExportMode mode);

signals:
    
public slots:
    
protected:

    const std::vector<Material*>& materials_;
    PropertyModel* propmodel_;
    ParameterModel* paramodel_;
};

#endif // MATMLWRITER_H
