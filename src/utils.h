#pragma once

#include "pch.h"

void ReadSettings(QSettings* settings, QObject* widget);
void WriteSettings(QSettings* settings, QObject* widget);

QString GetRclone();
void SetRclone(const QString& rclone);

QStringList GetRcloneConf();
void SetRcloneConf(const QString& rcloneConf);

void UseRclonePassword(QProcess* process);
void SetRclonePassword(const QString& rclonePassword);
