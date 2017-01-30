#pragma once

#include "pch.h"

void ReadSettings(QSettings* settings, QObject* widget);
void WriteSettings(QSettings* settings, QObject* widget);

QString GetRclone();
void SetRclone(const QString& rclone);

void UseRclonePassword(QProcess* process);
void SetRclonePassword(const QString& rclonePassword);
