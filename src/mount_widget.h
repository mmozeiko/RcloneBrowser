#pragma once

#include "pch.h"
#include "ui_mount_widget.h"

class MountWidget : public QWidget
{
    Q_OBJECT

public:
    MountWidget(QProcess* process, const QString& remote, const QString& folder, QWidget* parent = nullptr);
    ~MountWidget();

public slots:
    void cancel();

signals:
    void finished();
    void closed();

private:
    Ui::MountWidget ui;

    bool mRunning = true;
    QProcess* mProcess;
};
