#include "export_dialog.h"
#include "utils.h"

ExportDialog::ExportDialog(const QString& remote, const QDir& path, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    resize(0, 0);

    mTarget = remote + ":" + path.path();

    QObject::connect(ui.buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, [=]()
    {
        ui.rbText->setChecked(true);
        ui.checkVerbose->setChecked(false);
        ui.checkSameFilesystem->setChecked(false);
        ui.textMinSize->clear();
        ui.textMinAge->clear();
        ui.textMaxAge->clear();
        ui.spinMaxDepth->setValue(0);
        ui.textExclude->clear();
        ui.textExtra->clear();
    });
    ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)->click();

    QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QObject::connect(ui.fileBrowse, &QToolButton::clicked, this, [=]()
    {
        QString file = QFileDialog::getSaveFileName(this, "Choose destination file");
        if (!file.isEmpty())
        {
            ui.textFile->setText(QDir::toNativeSeparators(file));
        }
    });

    QSettings settings;
    settings.beginGroup("Export");
    ReadSettings(&settings, this);
    settings.endGroup();
}

ExportDialog::~ExportDialog()
{
    if (result() == QDialog::Accepted)
    {
        QSettings settings;
        settings.beginGroup("Export");
        WriteSettings(&settings, this);
        settings.remove("textFile");
        settings.endGroup();
    }
}

QString ExportDialog::getDestination() const
{
    return ui.textFile->text();
}

bool ExportDialog::onlyFilenames() const
{
    return ui.rbText->isChecked();
}

QStringList ExportDialog::getOptions() const
{
    QStringList list;
    list << "lsl";
    if (ui.checkVerbose->isChecked())
    {
        list << "--verbose";
    }
    if (ui.checkSameFilesystem->isChecked())
    {
        list << "--one-file-system";
    }
    if (!ui.textMinSize->text().isEmpty())
    {
        list << "--min-size" << ui.textMinSize->text();
    }
    if (!ui.textMinAge->text().isEmpty())
    {
        list << "--min-age" << ui.textMinAge->text();
    }
    if (!ui.textMaxAge->text().isEmpty())
    {
        list << "--max-age" << ui.textMaxAge->text();
    }
    if (ui.spinMaxDepth->value() != 0)
    {
        list << "--max-depth" << ui.spinMaxDepth->text();
    }

    QString excluded = ui.textExclude->toPlainText().trimmed();
    if (!excluded.isEmpty())
    {
        for (auto line : excluded.split('\n'))
        {
            list << "--exclude" << line;
        }
    }

    QString extra = ui.textExtra->text().trimmed();
    if (!extra.isEmpty())
    {
        for (auto arg : extra.split(' '))
        {
            list << arg;
        }
    }

    list << mTarget;

    return list;
}

void ExportDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        if (ui.textFile->text().isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Please enter destination filename!");
            return;
        }
    }
    QDialog::done(r);
}
