#include <QPixmap>
#include <QPainter>

#include "materialcategory.h"

QIcon* MaterialCategory::emptyIcon_ = 0;

MaterialCategory::MaterialCategory(const QString& name,
                                   const QColor& color,
                                   bool readonly):
    name_(name),
    color_(color),
    isReadOnly_(readonly)
{
    if (emptyIcon_==0) {
        QPixmap e(32, 32);
        e.fill(QColor(242, 142, 0, 0));
        emptyIcon_ = new QIcon(e);
    }

    QPixmap pm(32, 32);
    pm.fill(color_);
    icon_ = QIcon(pm);
}
