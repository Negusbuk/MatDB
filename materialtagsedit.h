#ifndef MATERIALTAGSEDIT_H
#define MATERIALTAGSEDIT_H

#include <nqtokenedit.h>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>

class MaterialTagsEdit : public NQTokenEdit
{
    Q_OBJECT
public:
    explicit MaterialTagsEdit(MaterialListModel* listModel,
                              MaterialSelectionModel* selectionModel,
                              QWidget *parent = 0);

signals:

    void materialMetadataChanged(Material*);

public slots:

    void materialChanged(Material* material);
    void tagsChanged(const QStringList&);

protected:

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
};

#endif // MATERIALTAGSEDIT_H
