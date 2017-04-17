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

#if !defined(Q_OS_WIN32) && !defined(Q_OS_OSX)
    QMimeDatabase mMimeDatabase;
#endif
};
