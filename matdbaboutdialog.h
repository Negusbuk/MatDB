#ifndef MATDBABOUTDIALOG_H
#define MATDBABOUTDIALOG_H

#include <QWizard>
#include <QLabel>
#include <QRadioButton>

class MatDBAboutDialog : public QWizard
{
    Q_OBJECT
public:
     enum { Page_Intro };

    explicit MatDBAboutDialog(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

class IntroPage : public QWizardPage
 {
     Q_OBJECT

 public:
     IntroPage(QWidget *parent = 0);

     int nextId() const;

 private:
     QLabel *topLabel;
     QRadioButton *registerRadioButton;
     QRadioButton *evaluateRadioButton;
 };

#endif // MATDBABOUTDIALOG_H
