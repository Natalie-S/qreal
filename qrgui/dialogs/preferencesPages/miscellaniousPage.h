#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"
#include "models/models.h"

namespace Ui {
    class PreferencesMiscellaniousPage;
}

class PreferencesMiscellaniousPage : public PreferencesPage
{
    Q_OBJECT

public:
    explicit PreferencesMiscellaniousPage(QWidget *parent = 0);
    ~PreferencesMiscellaniousPage();

    void save();
    virtual void restoreSettings();

signals:
    void iconsetChanged();
    void newRole(int exRole, QString addr);

protected:
    void changeEvent(QEvent *e);

private slots:
    void browseImagesPath();
    void enableIpAddressEdit();
    void disableIpAddressEdit();

private:
    Ui::PreferencesMiscellaniousPage *mUi;

    QString mLastIconsetPath;

    int mExRole;
};
