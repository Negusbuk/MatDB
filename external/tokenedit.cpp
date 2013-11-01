/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <QLineEdit>

#include "flowlayout.h"
#include "tokenedit.h"
#include "tokenedititem.h"

TokenEdit::TokenEdit(QWidget *parent) :
    QScrollArea(parent),
    itemColor_(QColor(Qt::lightGray)),
    horizontalMargin_(15),
    verticalMargin_(2)
{
    setWidgetResizable(true);

    QWidget * widget = new QWidget(this);

    layout_ = new FlowLayout(widget, 4, 5, 2);

    widget->setLayout(layout_);

    setWidget(widget);

    clear();
}

void TokenEdit::clear()
{
    tokens_.clear();

    TokenEditItem *item;
    while (!itemList_.isEmpty()) {
        item = itemList_.takeFirst();
        layout_->removeWidget(item);
        delete item;
    }
}

void TokenEdit::setTokens(const QStringList& l)
{
    clear();
    appendTokens(l);
}

void TokenEdit::appendTokens(const QStringList& l)
{
    bool tokensDidChange = false;

    for (QStringList::ConstIterator it = l.begin();
         it!=l.end();
         ++it) {
        tokens_ << *it;
        TokenEditItem * item = new TokenEditItem(*it, itemList_.size(),
                                                 itemColor_,
                                                 horizontalMargin_, verticalMargin_,
                                                 this);
        itemList_.append(item);
        layout_->addWidget(item);
        connect(item, SIGNAL(itemChanged(TokenEditItem*)),
                this, SLOT(itemChanged(TokenEditItem*)));
        tokensDidChange = true;
    }

    if (tokensDidChange) emit tokensChanged(tokens_);
}

void TokenEdit::mousePressEvent(QMouseEvent * /* ev */)
{
    tokens_ << "";
    TokenEditItem * item = new TokenEditItem(itemList_.size(),
                                             itemColor_,
                                             horizontalMargin_, verticalMargin_,
                                             this);
    itemList_.append(item);
    layout_->addWidget(item);
    connect(item, SIGNAL(itemChanged(TokenEditItem*)),
            this, SLOT(itemChanged(TokenEditItem*)));
}

void TokenEdit::itemChanged(TokenEditItem* item)
{
    if (!item) return;

    QString newEntry = item->tokenText();
    if (newEntry.length()==0) {
        tokens_.takeAt(item->index());
        itemList_.takeAt(item->index());
        layout_->removeWidget(item);
        delete item;
        emit tokensChanged(tokens_);
    } else {
        QString& entry = tokens_[item->index()];
        if (entry!=newEntry) {
            entry = item->tokenText();
            emit tokensChanged(tokens_);
        }
    }
}

void TokenEdit::setItemColor(const QColor& color)
{
    itemColor_ = color;

    for (QList<TokenEditItem*>::Iterator it = itemList_.begin();
         it!=itemList_.end();
         ++it) {
        (*it)->setItemColor(color);
    }
}

void TokenEdit::setHorizontalMargin(int margin)
{
    if (margin<15) return;

    horizontalMargin_ = margin;

    for (QList<TokenEditItem*>::Iterator it = itemList_.begin();
         it!=itemList_.end();
         ++it) {
        (*it)->setHorizontalMargin(margin);
    }
}

void TokenEdit::setVerticalMargin(int margin)
{
    if (margin<2) return;

    verticalMargin_ = margin;

    for (QList<TokenEditItem*>::Iterator it = itemList_.begin();
         it!=itemList_.end();
         ++it) {
        (*it)->setVerticalMargin(margin);
    }
}

void TokenEdit::setHorizontalSpacing(int spacing)
{
    if (spacing<-1) return;
    layout_->setHorizontalSpacing(spacing);
}

void TokenEdit::setVerticalSpacing(int spacing)
{
    if (spacing<-1) return;
    layout_->setVerticalSpacing(spacing);
}
