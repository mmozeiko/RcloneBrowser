#include "preferences_dialog.h"
#include "utils.h"

PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    QObject::connect(ui.rcloneBrowse, &QPushButton::clicked, this, [=]()
    {
        QString rclone = QFileDialog::getOpenFileName(this, "Select rclone executable", ui.rclone->text());
        if (rclone.isEmpty())
        {
            return;
        }

        if (!QFileInfo(rclone).isExecutable())
        {
            QMessageBox::critical(this, "Error", QString("File %1 is not executable").arg(rclone));
            return;
        }

        if (QFileInfo(rclone) == QFileInfo(qApp->applicationFilePath()))
        {
            QMessageBox::critical(this, "Error", "You selected RcloneBrowser executable!\nPlease select rclone executable instead.");
            return;
        }

        ui.rclone->setText(rclone);
    });

    QObject::connect(ui.rcloneConfBrowse, &QPushButton::clicked, this, [=]()
    {
        QString rcloneConf = QFileDialog::getOpenFileName(this, "Select .rclone.conf location", ui.rcloneConf->text());
        if (rcloneConf.isEmpty())
        {
            return;
        }

        ui.rcloneConf->setText(rcloneConf);
    });

    QSettings settings;
    ui.rclone->setText(QDir::toNativeSeparators(settings.value("Settings/rclone").toString()));
    ui.rcloneConf->setText(QDir::toNativeSeparators(settings.value("Settings/rcloneConf").toString()));
    ui.stream->setText(settings.value("Settings/stream").toString());
    ui.showFolderIcons->setChecked(settings.value("Settings/showFolderIcons", true).toBool());
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        ui.alwaysShowInTray->setChecked(settings.value("Settings/alwaysShowInTray", false).toBool());
        ui.closeToTray->setChecked(settings.value("Settings/closeToTray", false).toBool());
        ui.notifyFinishedTransfers->setChecked(settings.value("Settings/notifyFinishedTransfers", true).toBool());
    }
    else
    {
        ui.alwaysShowInTray->setChecked(false);
        ui.alwaysShowInTray->setDisabled(true);
        ui.closeToTray->setChecked(false);
        ui.closeToTray->setDisabled(true);
        ui.notifyFinishedTransfers->setChecked(false);
        ui.notifyFinishedTransfers->setDisabled(true);
    }
    ui.showFileIcons->setChecked(settings.value("Settings/showFileIcons", true).toBool());
    ui.rowColors->setChecked(settings.value("Settings/rowColors", false).toBool());

#ifdef Q_OS_WIN32
    ui.mount->hide();
    ui.mountLabel->hide();
#else
    ui.mount->setText(settings.value("Settings/mount").toString());
#endif
}

PreferencesDialog::~PreferencesDialog()
{
}

QString PreferencesDialog::getRclone() const
{
    return QDir::fromNativeSeparators(ui.rclone->text());
}

QString PreferencesDialog::getRcloneConf() const
{
    return QDir::fromNativeSeparators(ui.rcloneConf->text());
}

QString PreferencesDialog::getStream() const
{
    return ui.stream->text();
}

QString PreferencesDialog::getMount() const
{
    return ui.mount->text();
}

bool PreferencesDialog::getAlwaysShowInTray() const
{
    return ui.alwaysShowInTray->isChecked();
}

bool PreferencesDialog::getCloseToTray() const
{
    return ui.closeToTray->isChecked();
}

bool PreferencesDialog::getNotifyFinishedTransfers() const
{
    return ui.notifyFinishedTransfers->isChecked();
}

bool PreferencesDialog::getShowFolderIcons() const
{
    return ui.showFolderIcons->isChecked();
}

bool PreferencesDialog::getShowFileIcons() const
{
    return ui.showFileIcons->isChecked();
}

bool PreferencesDialog::getRowColors() const
{
    return ui.rowColors->isChecked();
}
