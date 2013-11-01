#ifndef MATERIALTABLEVIEW_H
#define MATERIALTABLEVIEW_H

#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <materialtableitem.h>

class MaterialTableView : public QTableWidget
{
    Q_OBJECT
public:
    explicit MaterialTableView(MaterialListModel *listmodel,
                               MaterialSelectionModel * selectionmodel,
                               PropertyModel *propertymodel,
                               ParameterModel *parametermodel,
                               QWidget *parent = 0);

signals:
    
public slots:

protected slots:
    void fillTable(int count);
    void setSelection(Material* selection);
    void metadataChanged(Material* material);
    void selectionChanged();
    void itemEdited(QTableWidgetItem * item);
    void displayContextMenu(const QPoint& point);

protected:
    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertyModel* PropertyModel_;
    ParameterModel* ParameterModel_;

    typedef std::map<const Material*,MaterialTableItem*> MaterialMap;
    MaterialMap indexMap_;

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QMenu* ContextMenu_;
};

#endif // MATERIALTABLEVIEW_H
