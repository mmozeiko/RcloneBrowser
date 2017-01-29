#pragma once

#include "pch.h"
#include "ui_stream_widget.h"

class StreamWidget : public QWidget
{
    Q_OBJECT

public:
    StreamWidget(QProcess* rclone, QProcess* player, const QString& remote, const QString& stream, QWidget* parent = nullptr);
    ~StreamWidget();

public slots:
    void cancel();

signals:
    void finished();
    void closed();

private:
    Ui::StreamWidget ui;

    bool mRunning = true;
    QProcess* mRclone;
    QProcess* mPlayer;
};
