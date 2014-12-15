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

#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSettings>

#include <nqlogger.h>

#include "matdbversion.h"
#include "matdbpreferencedialog.h"
#include "matdblanguagehandler.h"

MatDBPreferenceDialog::MatDBPreferenceDialog(QWidget *parent) :
    QDialog(parent)
{
#ifdef Q_WS_MAC
    this->setParent(qApp->focusWidget());
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::Sheet);
    setFixedWidth(700);
#else
    setFixedWidth(500);
#endif
    setFixedHeight(300);

    contentsWidget_ = new QListWidget;
    contentsWidget_->setViewMode(QListView::IconMode);
    contentsWidget_->setIconSize(QSize(96, 84));
    contentsWidget_->setMovement(QListView::Static);
    contentsWidget_->setMaximumWidth(128);
    contentsWidget_->setSpacing(12);

    pagesWidget_ = new QStackedWidget();
    languagePage_ = new LanguagePage();
    pagesWidget_->addWidget(languagePage_);

    createIcons();
    contentsWidget_->setCurrentRow(0);

    QPushButton *okButton = new QPushButton(tr("Ok"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(applyChanges()));

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(pagesWidget_, 1);
    verticalLayout->addStretch(1);
    verticalLayout->addSpacing(12);
    verticalLayout->addLayout(buttonsLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(contentsWidget_);
    mainLayout->addLayout(verticalLayout);

    setLayout(mainLayout);
}

void MatDBPreferenceDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget_);
    configButton->setIcon(QIcon(":/icons/MatDBPrefLanguage.png"));
    configButton->setText(tr("Language"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget_, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void MatDBPreferenceDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget_->setCurrentIndex(contentsWidget_->row(current));
}

void MatDBPreferenceDialog::applyChanges()
{
    NQLog("MatDBPreferenceDialog", NQLog::Spam) << "void applyChanges()";

    QSettings settings;
    QVariant currentLanguage = settings.value("language");
    QVariant newLanguage = languagePage_->selectedLanguage();
    if (currentLanguage!=newLanguage) {
        settings.setValue("language", newLanguage);

        QString locale = QLocale::system().name();
        if (settings.value("language")!="system") locale = settings.value("language").toString();

        MatDBLanguageHandler::instance()->changeLanguage(locale);
    }

    NQLog("MatDBPreferenceDialog", NQLog::Spam) << newLanguage.toString();

    this->close();
}

LanguagePage::LanguagePage(QWidget *parent)
    : QWidget(parent)
{
    QLabel *languageLabel = new QLabel(tr("Language:"));
    languageCombo_ = new QComboBox;
    languageCombo_->addItem(tr("System default"), "system");
    languageCombo_->addItem("English", "en");
    languageCombo_->addItem("Deutsch", "de");

    QSettings settings;
    if (settings.contains("language")) {
        NQLog("Language", NQLog::Spam) << settings.value("language").toString();
        int idx = languageCombo_->findData(settings.value("language"));
        languageCombo_->setCurrentIndex(idx);
    }

    QHBoxLayout *languageLayout = new QHBoxLayout;
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageCombo_);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(languageLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

QVariant LanguagePage::selectedLanguage()
{
    return languageCombo_->itemData(languageCombo_->currentIndex());
}
