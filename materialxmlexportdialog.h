#ifndef MATERIALXMLEXPORTDIALOG_H
#define MATERIALXMLEXPORTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QRadioButton>

#include <materiallistmodel.h>

class MaterialXMLExportDialog : public QDialog
{
    Q_OBJECT
public:

    enum ExportMode {
        ANSYS           = 1,
        MatML           = 2,
        Unknown
    };

    explicit MaterialXMLExportDialog(MaterialListModel* model,
                                     QWidget *parent = 0);

    const std::vector<Material*>& getSelectedMaterials() const { return selectedMaterials_; }

    ExportMode getExportMode() const { return exportMode_; }

signals:
    
public slots:
    
    void exportMaterials();
    void headerViewDoubleClicked(int logicalIndex);
    void modeChanged(QAbstractButton*);

protected:

    MaterialListModel* model_;
    QTableWidget* materialView_;
    std::vector<Material*> selectedMaterials_;
    bool allItemsChecked_;
    QRadioButton* modeMATMLbutton_;
    QRadioButton* modeANSYSbutton_;
    ExportMode exportMode_;
};

#endif // MATERIALXMLEXPORTDIALOG_H
