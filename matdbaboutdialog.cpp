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

#include "matdbaboutdialog.h"

MatDBAboutDialog::MatDBAboutDialog(QWidget *parent) :
    QWizard(parent)
{
#ifdef Q_WS_MAC
    this->setParent(qApp->focusWidget());
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::Sheet);
    setWizardStyle(ModernStyle);
#endif

    setSizeGripEnabled(false);

    setPage(Page_Intro, new IntroPage);

    setStartId(Page_Intro);

     //setOption(HaveHelpButton, false);
     setPixmap(QWizard::BackgroundPixmap, QPixmap(":/pics/MatDBBG.png"));

     setWindowTitle(tr("License Wizard"));
}

IntroPage::IntroPage(QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Introduction"));
     setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

     topLabel = new QLabel(tr("This wizard will help you register your copy of "
                              "<i>Super Product One</i>&trade; or start "
                              "evaluating the product."));
     topLabel->setWordWrap(true);

     registerRadioButton = new QRadioButton(tr("&Register your copy"));
     evaluateRadioButton = new QRadioButton(tr("&Evaluate the product for 30 "
                                               "days"));
     registerRadioButton->setChecked(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(topLabel);
     layout->addWidget(registerRadioButton);
     layout->addWidget(evaluateRadioButton);
     setLayout(layout);
 }

 int IntroPage::nextId() const
 {
    return MatDBAboutDialog::Page_Intro;
 }
