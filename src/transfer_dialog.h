#pragma once

#include "pch.h"
#include "ui_transfer_dialog.h"
#include "job_options.h"

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    TransferDialog(bool isDownload, const QString& remote, const QDir& path, bool isFolder, QWidget* parent = nullptr, JobOptions *task = nullptr, bool editMode = false);
    ~TransferDialog();

    void setSource(const QString& path);

    QString getMode() const;
    QString getSource() const;
    QString getDest() const;
    QStringList getOptions();

    JobOptions *getJobOptions();

private:
    Ui::TransferDialog ui;

    bool mIsDownload;
    bool mDryRun = false;
    bool mIsFolder;
    bool mIsEditMode;

    JobOptions *mJobOptions;

    void putJobOptions();

    void done(int r) override;

signals:
    void tasksListChanged();
};
