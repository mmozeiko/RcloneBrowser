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
    QStringList getOptions();

	JobOptions *getJobOptions();
	void setJobOptions(JobOptions *job);
	void putJobOptions(JobOptions& jo);

private:
    Ui::TransferDialog ui;

    bool mIsDownload;
    bool mDryRun = false;
	bool mIsFolder;

	JobOptions *mJobOptions;

    void done(int r) override;

signals:
	void tasksListChanged();
};
