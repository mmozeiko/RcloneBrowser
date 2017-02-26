#pragma once

#include "pch.h"
#include "ui_preferences_dialog.h"

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget* parent = nullptr);
    ~PreferencesDialog();

    QString getRclone() const;
    QString getRcloneConf() const;
    QString getStream() const;
    QString getMount() const;

    bool getAlwaysShowInTray() const;
    bool getCloseToTray() const;
    bool getNotifyFinishedTransfers() const;

    bool getShowFolderIcons() const;
    bool getShowFileIcons() const;
    bool getRowColors() const;

private:
    Ui::PreferencesDialog ui;
};
