#pragma once

#include "pch.h"
#include "ui_remote_widget.h"

class IconCache;

class RemoteWidget : public QWidget
{
    Q_OBJECT

public:
    RemoteWidget(IconCache* icons, const QString& remote, bool isLocal, QWidget* parent = nullptr);
    ~RemoteWidget();

signals:
    void addTransfer(const QString& message, const QString& source, const QString& remote, const QStringList& args);
    void addMount(const QString& remote, const QString& folder);
    void addStream(const QString& remote, const QString& stream);

private:
    Ui::RemoteWidget ui;
};
