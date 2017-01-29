#pragma once

#include "pch.h"

struct Item
{
    Item()
    {
    }

    ~Item()
    {
        for (auto child : childs)
        {
            if (child->isLoading() || state == LoadingIcon)
            {
                child->isDeleted = true;
            }
            else
            {
                delete child;
            }
        }
    }

    bool isLoading() const
    {
        return state == Loading1 || state == Loading2;
    }

    int num() const
    {
        Q_ASSERT(parent);
        return parent->childs.indexOf(const_cast<Item*>(this));
    }

    Item* parent = nullptr;

    enum State { Unknown, Loading1, Loading2, Ready, Special, LoadingIcon };

    State state = Unknown;
    bool isFolder = false;
    bool isDeleted = false;
    QString name;
    QDir path;
    QString modified;
    quint64 size = 0;

    QVector<Item*> childs;
};

class IconCache;
class ItemSorter;

class ItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ItemModel(IconCache* icons, const QString& remote, QObject* parent);
    ~ItemModel();

    const QDir& path(const QModelIndex& index) const;
    bool isLoading(const QModelIndex& index) const;
    void refresh(const QModelIndex& index);
    void rename(const QModelIndex& index, const QString& name);
    bool isTopLevel(const QModelIndex& index) const;
    bool isFolder(const QModelIndex& index) const;

    QModelIndex addRoot(const QString& name, const QString& path);

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    bool hasChildren(const QModelIndex& parent) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    void sort(int column, Qt::SortOrder order) override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool removeRows(int row, int count, const QModelIndex& parent) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool canDropMimeData(const QMimeData* data, Qt::DropAction action,
        int row, int column, const QModelIndex& parent) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action,
        int row, int column, const QModelIndex& parent) override;

signals:
    void getIcon(Item* item, const QPersistentModelIndex& index);
    void drop(const QDir& path, const QModelIndex& parent);

private:
    Item* mRoot;

    QString mRemote;

    QHash<QString, QIcon> mLoadedIcons;

    bool mFolderIcons;
    bool mFileIcons;

    QIcon mDriveIcon;
    QIcon mFolderIcon;
    QIcon mFileIcon;

    QFont mFixedFont;

    int mSortColumn;
    Qt::SortOrder mSortOrder;

    QRegExp mRegExpFolder;
    QRegExp mRegExpFile;

    Item* get(const QModelIndex& index) const;
    void load(const QPersistentModelIndex& parentIndex, Item* parent);

    void sortRecursive(Item* item, const ItemSorter& sorter);
    void sort(const QModelIndex& parent, Item* item);
};
