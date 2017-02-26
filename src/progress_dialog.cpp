#include "progress_dialog.h"

ProgressDialog::ProgressDialog(const QString& title, const QString& operation, const QString& message, QProcess* process, QWidget* parent, bool close)
    : QDialog(parent)
{
    ui.setupUi(this);
    resize(width(), 0);

    setWindowTitle(title);
    ui.labelOperation->setText(operation);
    ui.labelInfo->setText(message);

    ui.output->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui.output->setVisible(false);

    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QObject::connect(ui.buttonShowOutput, &QPushButton::toggled, this, [=](bool checked)
    {
        ui.output->setVisible(checked);
        ui.buttonShowOutput->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
        if (!checked)
        {
            adjustSize();
        }
    });

    QObject::connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
        this, [=](int code, QProcess::ExitStatus status)
    {
        if (status == QProcess::NormalExit && code == 0)
        {
            if (close)
            {
                emit accept();
            }
        }
        else
        {
            ui.buttonShowOutput->setChecked(true);
            ui.buttonBox->setEnabled(true);
        }
    });

    QObject::connect(process, &QProcess::readyRead, this, [=]()
    {
        ui.output->appendPlainText(process->readAll());
    });

    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(QIODevice::ReadOnly);
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::expand()
{
    ui.buttonShowOutput->setChecked(true);
}

void ProgressDialog::allowToClose()
{
    ui.buttonBox->setEnabled(true);
}
