#ifndef MATERIALCATEGORYEDITDIALOG_H
#define MATERIALCATEGORYEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include <nqcolorwheel.h>

class MaterialCategoryEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MaterialCategoryEditDialog(QWidget *parent = 0);

    const QString& getName() const { return nameEdit_->text(); }
    const QColor& getColor() const { return colorPicker_->color(); }

signals:

public slots:

    void setName(const QString& name) { nameEdit_->setText(name); }
    void setColor(const QColor& color) { colorPicker_->setColor(color); }

protected:

    QLineEdit* nameEdit_;
    NQColorWheel* colorPicker_;
};

#endif // MATERIALCATEGORYEDITDIALOG_H
