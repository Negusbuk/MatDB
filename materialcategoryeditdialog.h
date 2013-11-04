#ifndef MATERIALCATEGORYEDITDIALOG_H
#define MATERIALCATEGORYEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>

class MaterialCategoryEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MaterialCategoryEditDialog(QWidget *parent = 0);

    const QString& getName() const { return nameEdit_->text(); }

signals:

public slots:

    void setName(const QString& name) { nameEdit_->setText(name); }

protected:

    void closeEvent(QCloseEvent* e);

    QLineEdit* nameEdit_;
};

#endif // MATERIALCATEGORYEDITDIALOG_H
