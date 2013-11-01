#include <QBoxLayout>

#include "matdbaboutdialog.h"

MatDBAboutDialog::MatDBAboutDialog(QWidget *parent) :
    QWizard(parent)
{
    setSizeGripEnabled(false);

    setPage(Page_Intro, new IntroPage);

    setStartId(Page_Intro);

 #ifndef Q_WS_MAC
     setWizardStyle(ModernStyle);
 #endif

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
