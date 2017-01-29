#pragma once

#include "pch.h"

struct Item;

class IconCache : public QObject
{
    Q_OBJECT
public:
    IconCache(QObject* parent = nullptr);
    ~IconCache();

public slots:
    void getIcon(Item* item, const QPersistentModelIndex& parent);

signals:
    void iconReady(Item* item, const QPersistentModelIndex& parent, const QIcon& icon);

private:
    QThread mThread;
    QIcon mFileIcon;

    QHash<QString, QIcon> mIcons;

#ifdef Q_OS_LINUX
    QMimeDatabase mMimeDatabase;
#endif
};
