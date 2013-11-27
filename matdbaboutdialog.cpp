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
#include <QBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QFile>

#include "matdbversion.h"
#include "matdbaboutdialog.h"

MatDBAboutDialog::MatDBAboutDialog(QWidget *parent) :
    QWizard(parent)
{
#ifdef Q_WS_MAC
    this->setParent(qApp->focusWidget());
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::Sheet);
    setWizardStyle(ModernStyle);
    setFixedWidth(700);
#else
    setFixedWidth(500);
#endif
    setFixedHeight(400);

    setSizeGripEnabled(false);

    setPage(Page_Version, new VersionPage);
    setPage(Page_License, new LicensePage);

    setStartId(Page_Version);

    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::FinishButton << QWizard::NextButton;
    setButtonLayout(layout);

    QPixmap pix(":/artwork/MatDBBG.png");
    pix = pix.scaled(pix.width()/1.5, pix.height()/1.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(QWizard::WatermarkPixmap, pix);
    setPixmap(QWizard::LogoPixmap, pix);
    setPixmap(QWizard::BannerPixmap, pix);
    setPixmap(QWizard::BackgroundPixmap, pix);
}

VersionPage::VersionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Version"));

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QGridLayout *grid = new QGridLayout(this);
    layout->addLayout(grid);

    QString release = MATDBRELEASESTR;
    release += " (";
    release += QString::number(MATDBRELEASE);
    release += ")";
    grid->addWidget(new QLabel(tr("MatDB Version")), 1, 0);
    grid->addWidget(new QLabel(release), 1, 1);
    grid->addItem(new QSpacerItem(10, 10, QSizePolicy::Maximum),
                  1, 2);

    grid->addWidget(new QLabel(tr("Qt Version")), 2, 0);
    grid->addWidget(new QLabel(qVersion()), 2, 1);
    grid->addItem(new QSpacerItem(10, 10, QSizePolicy::Maximum),
                  2, 2);
}

int VersionPage::nextId() const
{
    return MatDBAboutDialog::Page_License;
}

LicensePage::LicensePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("License"));

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    licenseView_ = new QTextEdit(this);
    layout->addWidget(licenseView_);
    licenseView_->setReadOnly(true);

    QFile file(":/LICENSE.html");
    file.open(QFile::ReadOnly);
    licenseView_->setHtml(file.readAll());
    file.close();
}

int LicensePage::nextId() const
{
    return -1;
}
