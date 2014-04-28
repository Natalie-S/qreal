#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealFileDialog.h>

using namespace qReal;

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent)
        : PreferencesPage(parent)
        , mUi(new Ui::PreferencesMiscellaniousPage)
{
    mIcon = QIcon(":/icons/preferences/miscellaneous.png");
    mUi->setupUi(this);

    connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

    mUi->colorComboBox->addItems(QColor::colorNames());

    mExRole = SettingsManager::value("role").toInt();
    if(mExRole != 1) {
        mUi->ipAddressEdit->setDisabled(1);
    } else {
        mUi->ipAddressEdit->setText(SettingsManager::value("lastServerAddress").toString());
    }
    connect(mUi->clientRole, SIGNAL(clicked()), this, SLOT(enableIpAddressEdit()));
    connect(mUi->serverRole, SIGNAL(clicked()), this, SLOT(disableIpAddressEdit()));
    connect(mUi->standAloneRole, SIGNAL(clicked()), this, SLOT(disableIpAddressEdit()));
    //SettingsManager::setValue("role", 0);
    restoreSettings();
}

PreferencesMiscellaniousPage::~PreferencesMiscellaniousPage()
{
    delete mUi;
}

void PreferencesMiscellaniousPage::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        mUi->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PreferencesMiscellaniousPage::browseImagesPath()
{
    QString path = utils::QRealFileDialog::getExistingDirectory("OpenImagesOnMiscellaniousPage", this, "Open Directory");
    if (!path.isEmpty()) {
        mUi->imagesPathEdit->setText(path.replace("\\", "/"));
    }
}

void PreferencesMiscellaniousPage::enableIpAddressEdit()
{

    QString addr = SettingsManager::value("lastServerAddress").toString();
    //qDebug() << "enable " << addr;
    mUi->ipAddressEdit->setEnabled(1);
    if(addr != "") {
        mUi->ipAddressEdit->setText(addr);
    }

}

void PreferencesMiscellaniousPage::disableIpAddressEdit()
{
    if(mUi->ipAddressEdit->isEnabled()) {
        mUi->ipAddressEdit->clear();
        mUi->ipAddressEdit->setDisabled(1);
    }
}

void PreferencesMiscellaniousPage::save()
{
    SettingsManager::setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
    SettingsManager::setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());

    SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());
    SettingsManager::setValue("recentProjectsLimit", mUi->recentProjectsLimitSpinBox->value());
    SettingsManager::setValue("PaintOldEdgeMode", mUi->paintOldLineCheckBox->isChecked());
    SettingsManager::setValue("oldLineColor", mUi->colorComboBox->currentText());

    int curRole = mExRole;
    QString addr = "";
    if(mUi->clientRole->isChecked()) {
        curRole = 1;
        addr = mUi->ipAddressEdit->text();
    }
    else if (mUi->serverRole->isChecked()) {
        curRole = 2;
    }
    else if(mUi->standAloneRole->isChecked()) {
        curRole = 0;
    }
    if(curRole != mExRole) {
        SettingsManager::setValue("role",  curRole);
        if (curRole == 1) {
            SettingsManager::setValue("lastServerAddress", addr);
        }
//        qDebug << "miscel role changed";
        emit newRole(mExRole, addr);
        mExRole = curRole;
    }

    if (mLastIconsetPath != mUi->imagesPathEdit->text()) {
        emit iconsetChanged();
    }
}

void PreferencesMiscellaniousPage::restoreSettings()
{
    mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
    mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());

    mUi->paintOldLineCheckBox->setChecked(SettingsManager::value("PaintOldEdgeMode").toBool());

    QString curColor = SettingsManager::value("oldLineColor").toString();
    int curColorIndex = mUi->colorComboBox->findText(curColor);
    mUi->colorComboBox->setCurrentIndex(curColorIndex);

    mLastIconsetPath = SettingsManager::value("pathToImages").toString();

    int curRole = SettingsManager::value("role").toInt();
    switch (curRole) {
    case 0:
        mUi->standAloneRole->setChecked(true);
        break;
    case 1:
    {
        mUi->clientRole->setChecked(true);
        mUi->ipAddressEdit->setText(SettingsManager::value("lastServerAddress").toString());
    }
        break;
    case 2:
        mUi->serverRole->setChecked(true);
        break;
    default:
        break;
    }
}
