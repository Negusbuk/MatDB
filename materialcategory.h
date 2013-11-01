#ifndef MATERIALCATEGORY_H
#define MATERIALCATEGORY_H

#include <QString>
#include <QColor>
#include <QIcon>

class MaterialCategory
{
public:
    explicit MaterialCategory(const QString& name, const QColor& bgColor,
                              bool readonly);

    bool isReadOnly() const { return isReadOnly_; }
    const QString& getName() const { return name_; }
    const QColor& getColor() const { return color_; }
    const QIcon& getIcon() const { return icon_; }
    static QIcon& getEmptyIcon() { return *emptyIcon_; }

public slots:

    void setName(const QString& name) { name_ = name; }
    void setColor(const QColor& color) { color_ = color; }

protected:

    bool isReadOnly_;
    QString name_;
    QColor color_;
    QIcon icon_;
    static QIcon* emptyIcon_;
};

#endif // MATERIALCATEGORY_H
