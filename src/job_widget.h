#pragma once

#include "pch.h"
#include "ui_job_widget.h"

class JobWidget : public QWidget
{
    Q_OBJECT

public:
    JobWidget(QProcess* process, const QString& info, const QStringList& args, const QString& source, const QString& dest, QWidget* parent = nullptr);
    ~JobWidget();

    void showDetails();

public slots:
    void cancel();

signals:
    void finished(const QString& info);
    void closed();

private:
    Ui::JobWidget ui;

    bool mRunning = true;
    QProcess* mProcess;
    int mLines = 0;

    QStringList mArgs;
    QHash<QString, QLabel*> mActive;
    QSet<QLabel*> mUpdated;
};
