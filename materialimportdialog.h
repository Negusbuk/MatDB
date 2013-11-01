#ifndef MATERIALIMPORTDIALOG_H
#define MATERIALIMPORTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QRadioButton>

#include <materiallistmodel.h>

class MaterialImportDialog : public QDialog
{
    Q_OBJECT
public:

    explicit MaterialImportDialog(MaterialListModel* model,
                                  QWidget *parent = 0);

    const std::vector<Material*>& getSelectedMaterials() const { return selectedMaterials_; }

signals:
    
public slots:
    
    void buildSelectedMaterials();
    void headerViewDoubleClicked(int logicalIndex);

protected:

    MaterialListModel* model_;
    QTableWidget* materialView_;
    std::vector<Material*> selectedMaterials_;
    bool allItemsChecked_;
};

#endif // MATERIALIMPORTDIALOG_H
