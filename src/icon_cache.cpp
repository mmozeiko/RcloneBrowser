#include "icon_cache.h"
#include "item_model.h"
#if defined(Q_OS_OSX)
#include "osx_helper.h"
#endif

IconCache::IconCache(QObject* parent)
    : QObject(parent)
{
    mFileIcon = QFileIconProvider().icon(QFileIconProvider::File);

#ifdef Q_OS_WIN32
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif

    mThread.start();
    moveToThread(&mThread);
}

IconCache::~IconCache()
{
    mThread.quit();
    mThread.wait();

#ifdef Q_OS_WIN32
    CoUninitialize();
#endif
}

void IconCache::getIcon(Item* item, const QPersistentModelIndex& parent)
{
    QString ext = QFileInfo(item->name).suffix();
    QIcon icon;
    auto it = mIcons.find(ext);
    if (it == mIcons.end())
    {
#if defined(Q_OS_WIN32)
        SHFILEINFOW info;
        if (SHGetFileInfoW(reinterpret_cast<LPCWSTR>(("dummy." + ext).utf16()),
            FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES) && info.hIcon)
        {
            icon = QtWin::fromHICON(info.hIcon);
            DestroyIcon(info.hIcon);
        }
#elif defined(Q_OS_OSX)
        icon = osxGetIcon(ext.toUtf8().constData());
#else
        QMimeType mime = mMimeDatabase.mimeTypeForFile(item->name, QMimeDatabase::MatchExtension);
        if (mime.isValid())
        {
            icon = QIcon::fromTheme(mime.iconName());
        }
#endif
        if (icon.isNull())
        {
            icon = mFileIcon;
        }
        mIcons.insert(ext, icon);
    }
    else
    {
        icon = it.value();
    }

    emit iconReady(item, parent, icon);
}
