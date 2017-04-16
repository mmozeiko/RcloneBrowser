#pragma once

#include "pch.h"
#include "ui_transfer_dialog.h"
#include "JobOptions.h"

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    TransferDialog(bool isDownload, const QString& remote, const QDir& path, bool isFolder, QWidget* parent = nullptr);
    ~TransferDialog();

    void setSource(const QString& path);

    QString getMode() const;
    QString getSource() const;
    QString getDest() const;
    QStringList getOptions() const;
	JobOptions *getJobOptions() const;

private:
    Ui::TransferDialog ui;

    bool mIsDownload;
    bool mDryRun = false;

    void done(int r) override;
};
