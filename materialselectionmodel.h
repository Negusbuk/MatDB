#ifndef MATERIALSELECTIONMODEL_H
#define MATERIALSELECTIONMODEL_H

#include <QObject>

#include <material.h>

class MaterialSelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit MaterialSelectionModel(QObject *parent = 0);
    Material* getSelection();

signals:
    void selectionChanged(Material* selection);
    void showMaterialDetails(Material* selection);
    void duplicateMaterial(Material* selection);
    void deleteMaterial(Material* selection);

public slots:
    void setSelection(Material* selection);
    void materialChanged(Material* selection);
    void materialDetailsRequested(Material* selection);
    void duplicateMaterial();
    void deleteMaterial();

protected:
    Material* Selection_;
};

#endif // MATERIALSELECTIONMODEL_H
