#include "utils.h"

static QString gRclone;
static QString gRcloneConf;
static QString gRclonePassword;

static QString GetIniFilename()
{
    QFileInfo info = qApp->applicationFilePath();
    return info.dir().filePath(info.baseName() + ".ini");
}

static bool IsPortableMode()
{
    QString ini = GetIniFilename();
    return QFileInfo(ini).exists();
}

std::unique_ptr<QSettings> GetSettings()
{
    if (IsPortableMode())
    {
        return std::unique_ptr<QSettings>(new QSettings(GetIniFilename(), QSettings::IniFormat));
    }
    return std::unique_ptr<QSettings>(new QSettings);
}

void ReadSettings(QSettings* settings, QObject* widget)
{
    QString name = widget->objectName();
    if (!name.isEmpty() && settings->contains(name))
    {
        if (QRadioButton* obj = qobject_cast<QRadioButton*>(widget))
        {
            obj->setChecked(settings->value(name).toBool());
            return;
        }
        if (QCheckBox* obj = qobject_cast<QCheckBox*>(widget))
        {
            obj->setChecked(settings->value(name).toBool());
            return;
        }
        if (QComboBox* obj = qobject_cast<QComboBox*>(widget))
        {
            obj->setCurrentIndex(settings->value(name).toInt());
            return;
        }
        if (QSpinBox* obj = qobject_cast<QSpinBox*>(widget))
        {
            obj->setValue(settings->value(name).toInt());
            return;
        }
        if (QLineEdit* obj = qobject_cast<QLineEdit*>(widget))
        {
            obj->setText(settings->value(name).toString());
            return;
        }
        if (QPlainTextEdit* obj = qobject_cast<QPlainTextEdit*>(widget))
        {
            int count = settings->beginReadArray(name);
            QStringList lines;
            lines.reserve(count);
            for (int i=0; i<count; i++)
            {
                settings->setArrayIndex(i);
                lines.append(settings->value("value").toString());
            }
            settings->endArray();

            obj->setPlainText(lines.join('\n'));
            return;
        }
    }

    for (auto child : widget->children())
    {
        ReadSettings(settings, child);
    }
}

void WriteSettings(QSettings* settings, QObject* widget)
{
    QString name = widget->objectName();
    if (QCheckBox* obj = qobject_cast<QCheckBox*>(widget))
    {
        settings->setValue(name, obj->isChecked());
        return;
    }
    if (QComboBox* obj = qobject_cast<QComboBox*>(widget))
    {
        settings->setValue(name, obj->currentIndex());
        return;
    }
    if (QSpinBox* obj = qobject_cast<QSpinBox*>(widget))
    {
        settings->setValue(name, obj->value());
        return;
    }
    if (QLineEdit* obj = qobject_cast<QLineEdit*>(widget))
    {
        if (obj->text().isEmpty())
        {
            settings->remove(name);
        }
        else
        {
            settings->setValue(name, obj->text());
        }
        return;
    }
    if (QPlainTextEdit* obj = qobject_cast<QPlainTextEdit*>(widget))
    {
        QString text = obj->toPlainText().trimmed();
        if (!text.isEmpty())
        {
            QStringList lines = text.split('\n');
            settings->beginWriteArray(name, lines.size());
            for (int i=0; i<lines.count(); i++)
            {
                settings->setArrayIndex(i);
                settings->setValue("value", lines[i]);
            }
            settings->endArray();
        }
        return;
    }

    for (auto child : widget->children())
    {
        WriteSettings(settings, child);
    }
}

QStringList GetRcloneConf()
{
    if (gRcloneConf.isEmpty())
    {
        return QStringList();
    }

    QString conf = gRcloneConf;
    if (IsPortableMode() && QFileInfo(conf).isRelative())
    {
        conf = QDir(qApp->applicationDirPath()).filePath(conf);
    }
    return QStringList() << " --config" << conf;
}

void SetRcloneConf(const QString& rcloneConf)
{
    gRcloneConf = rcloneConf;
}

QString GetRclone()
{
    QString rclone = gRclone;
    if (IsPortableMode() && QFileInfo(rclone).isRelative())
    {
        rclone = QDir(qApp->applicationDirPath()).filePath(rclone);
    }

    return rclone;
}

void SetRclone(const QString& rclone)
{
    gRclone = rclone.trimmed();
}

void UseRclonePassword(QProcess* process)
{
    if (!gRclonePassword.isEmpty())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("RCLONE_CONFIG_PASS", gRclonePassword);
        process->setProcessEnvironment(env);
    }
}

void SetRclonePassword(const QString& rclonePassword)
{
    gRclonePassword = rclonePassword;
}
