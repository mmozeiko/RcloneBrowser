#pragma once

#include "pch.h"
#include "ui_progress_dialog.h"

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    ProgressDialog(const QString& title, const QString& operation, const QString& message, QProcess* process, QWidget* parent = nullptr);
    ~ProgressDialog();

private:
    Ui::ProgressDialog ui;
};
