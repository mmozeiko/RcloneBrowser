#include "remote_widget.h"
#include "transfer_dialog.h"
#include "progress_dialog.h"
#include "icon_cache.h"
#include "item_model.h"
#include "utils.h"

RemoteWidget::RemoteWidget(IconCache* iconCache, const QString& remote, bool isLocal, QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

#ifdef Q_OS_WIN32
    ui.mount->setVisible(false);
    ui.buttonMount->setVisible(false);
#else
    isLocal = false;
#endif
    QSettings settings;
    ui.tree->setAlternatingRowColors(settings.value("Settings/rowColors", false).toBool());

    QStyle* style = QApplication::style();
    ui.refresh->setIcon(style->standardIcon(QStyle::SP_BrowserReload));
    ui.mkdir->setIcon(style->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui.rename->setIcon(style->standardIcon(QStyle::SP_FileIcon));
    ui.purge->setIcon(style->standardIcon(QStyle::SP_TrashIcon));
    ui.mount->setIcon(style->standardIcon(QStyle::SP_DirLinkIcon));
    ui.stream->setIcon(style->standardIcon(QStyle::SP_MediaPlay));
    ui.upload->setIcon(style->standardIcon(QStyle::SP_ArrowUp));
    ui.download->setIcon(style->standardIcon(QStyle::SP_ArrowDown));

    ui.buttonRefresh->setDefaultAction(ui.refresh);
    ui.buttonMkdir->setDefaultAction(ui.mkdir);
    ui.buttonRename->setDefaultAction(ui.rename);
    ui.buttonPurge->setDefaultAction(ui.purge);
    ui.buttonMount->setDefaultAction(ui.mount);
    ui.buttonStream->setDefaultAction(ui.stream);
    ui.buttonUpload->setDefaultAction(ui.upload);
    ui.buttonDownload->setDefaultAction(ui.download);

    ui.tree->sortByColumn(0, Qt::AscendingOrder);
    ui.tree->header()->setSectionsMovable(false);

    ItemModel* model = new ItemModel(iconCache, remote, this);
    ui.tree->setModel(model);
    QTimer::singleShot(0, ui.tree, static_cast<void(QWidget::*)()>(&QWidget::setFocus));

    QObject::connect(model, &QAbstractItemModel::layoutChanged, this, [=]()
    {
        ui.tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui.tree->resizeColumnToContents(1);
        ui.tree->resizeColumnToContents(2);
    });

    QObject::connect(ui.tree->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection& selection)
    {
        for (auto child : findChildren<QAction*>())
        {
            child->setDisabled(selection.isEmpty());
        }

        if (selection.isEmpty())
        {
            ui.path->clear();
            return;
        }

        QModelIndex index = selection.indexes().front();

        bool topLevel = model->isTopLevel(index);
        bool isFolder = model->isFolder(index);

        QDir path;
        if (model->isLoading(index))
        {
            ui.refresh->setDisabled(true);
            ui.rename->setDisabled(true);
            ui.purge->setDisabled(true);
            ui.mount->setDisabled(true);
            ui.stream->setDisabled(true);
            path = model->path(model->parent(index));
        }
        else
        {
            ui.refresh->setDisabled(false);
            ui.rename->setDisabled(topLevel);
            ui.purge->setDisabled(topLevel);
            ui.mount->setDisabled(!isFolder);
            ui.stream->setDisabled(isFolder);
            path = model->path(index);
        }

        ui.path->setText(isLocal ? QDir::toNativeSeparators(path.path()) : path.path());
    });

    QObject::connect(ui.refresh, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();
        model->refresh(index);
    });

    QObject::connect(ui.mkdir, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();
        if (!model->isFolder(index))
        {
            index = index.parent();
        }
        QDir path = model->path(index);
        QString pathMsg = isLocal ? QDir::toNativeSeparators(path.path()) : path.path();

        QString name = QInputDialog::getText(this, "New Folder", QString("Create folder in %1").arg(pathMsg));
        if (!name.isEmpty())
        {
            QString folder = path.filePath(name);
            QString folderMsg = isLocal ? QDir::toNativeSeparators(folder) : folder;

            QProcess process;
            process.setProgram(GetRclone());
            process.setArguments(QStringList() << "mkdir" << remote + ":" + folder);
            process.setReadChannelMode(QProcess::MergedChannels);

            ProgressDialog progress("New Folder", "Creating...", folderMsg, &process, this);
            if (progress.exec() == QDialog::Accepted)
            {
                model->refresh(index);
            }
        }
    });

    QObject::connect(ui.rename, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();

        QString path = model->path(index).path();
        QString pathMsg = isLocal ? QDir::toNativeSeparators(path) : path;

        QString name = model->data(index, Qt::DisplayRole).toString();
        name = QInputDialog::getText(this, "Rename", QString("New name for %1").arg(pathMsg), QLineEdit::Normal, name);
        if (!name.isEmpty())
        {
            QProcess process;
            process.setProgram(GetRclone());
            process.setArguments(QStringList()
                                 << "move"
                                 << remote + ":" + path
                                 << remote + ":" + model->path(index.parent()).filePath(name));
            process.setReadChannelMode(QProcess::MergedChannels);

            ProgressDialog progress("Rename", "Renaming...", pathMsg, &process, this);
            if (progress.exec() == QDialog::Accepted)
            {
                model->rename(index, name);
            }
        }
    });

    QObject::connect(ui.purge, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();

        QString path = model->path(index).path();
        QString pathMsg = isLocal ? QDir::toNativeSeparators(path) : path;

        int button = QMessageBox::question(this, "Delete", QString("Are you sure you want to delete %1 ?").arg(pathMsg), QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            QProcess process;
            process.setProgram(GetRclone());
            process.setArguments(QStringList() << (model->isFolder(index) ? "purge" : "delete") << remote + ":" + path);
            process.setReadChannelMode(QProcess::MergedChannels);

            ProgressDialog progress("Delete", "Deleting...", pathMsg, &process, this);
            if (progress.exec() == QDialog::Accepted)
            {
                QModelIndex parent = index.parent();
                QModelIndex next = parent.child(index.row() + 1, 0);
                ui.tree->selectionModel()->select(next.isValid() ? next : parent, QItemSelectionModel::SelectCurrent);
                model->removeRow(index.row(), parent);
            }
        }
    });

    QObject::connect(ui.mount, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();

        QString path = model->path(index).path();
        QString pathMsg = isLocal ? QDir::toNativeSeparators(path) : path;

        QString folder = QFileDialog::getExistingDirectory(this, QString("Mount %1").arg(pathMsg));
        if (!folder.isEmpty())
        {
            emit addMount(remote + ":" + path, folder);
        }
    });

    QObject::connect(ui.stream, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();
        QString path = model->path(index).path();

        QSettings settings;
        bool streamConfirmed = settings.value("Settings/streamConfirmed", false).toBool();
        QString stream = settings.value("Settings/stream", "mpv -").toString();
        if (!streamConfirmed)
        {
            QString result = QInputDialog::getText(this, "Stream", "Enter stream command (file will be passed in STDIN):", QLineEdit::Normal, stream);
            if (result.isEmpty())
            {
                return;
            }

            stream = result;

            settings.setValue("Settings/stream", stream);
            settings.setValue("Settings/streamConfirmed", true);
        }

        emit addStream(remote + ":" + path, stream);
    });

    QObject::connect(ui.upload, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();
        if (!model->isFolder(index))
        {
            index = index.parent();
        }
        QDir path = model->path(index);

        TransferDialog t(false, remote, path, true, this);
        if (t.exec() == QDialog::Accepted)
        {
            QString src = t.getSource();
            QString dst = t.getDest();

            QStringList args = t.getOptions();
            emit addTransfer(QString("%1 from %2").arg(t.getMode()).arg(src), src, dst, args);
        }
    });

    QObject::connect(ui.download, &QAction::triggered, this, [=]()
    {
        QModelIndex index = ui.tree->selectionModel()->selectedRows().front();
        QDir path = model->path(index);

        TransferDialog t(true, remote, path, model->isFolder(index), this);
        if (t.exec() == QDialog::Accepted)
        {
            QString src = t.getSource();
            QString dst = t.getDest();

            QStringList args = t.getOptions();
            emit addTransfer(QString("%1 %2").arg(t.getMode()).arg(src), src, dst, args);
        }
    });

    QObject::connect(model, &ItemModel::drop, this, [=](const QDir& path, const QModelIndex& parent)
    {
        qApp->setActiveWindow(this);
        QDir destPath = model->path(parent);
        QString dest = QFileInfo(path.path()).isDir() ? destPath.filePath(path.dirName()) : destPath.path();
        TransferDialog t(false, remote, dest, true, this);
        t.setSource(path.path());
        if (t.exec() == QDialog::Accepted)
        {
            QString src = t.getSource();
            QString dst = t.getDest();

            QStringList args = t.getOptions();
            emit addTransfer(QString("%1 from %2").arg(t.getMode()).arg(src), src, dst, args);
        }

    });

    QObject::connect(ui.tree, &QWidget::customContextMenuRequested, this, [=](const QPoint& pos)
    {
        QMenu menu;
        menu.addAction(ui.refresh);
        menu.addSeparator();
        menu.addAction(ui.mkdir);
        menu.addAction(ui.rename);
        menu.addAction(ui.purge);
        menu.addSeparator();
        menu.addAction(ui.mount);
        menu.addAction(ui.stream);
        menu.addAction(ui.upload);
        menu.addAction(ui.download);
        menu.exec(ui.tree->viewport()->mapToGlobal(pos));
    });

    if (isLocal)
    {
        QHash<QString, QPersistentModelIndex> drives;

        // QDir::drives is fast
        for (const auto& drive : QDir::drives())
        {
            QString path = drive.path();
            QModelIndex index = model->addRoot(QDir::toNativeSeparators(path), path);
            drives.insert(path, index);
        }

        QThread* thread = new QThread(this);
        thread->start();

        QObject* worker = new QObject();
        worker->moveToThread(thread);

        QTimer::singleShot(0, worker, [=]()
        {
            QStorageInfo info;
            info.refresh();

            // QStorageInfo::mountedVolumes is slow :(
            for (const auto& volume : info.mountedVolumes())
            {
                QString name = volume.name();
                if (!name.isEmpty())
                {
                    QString path = volume.rootPath();
                    QString item = QString("%1 (%2)").arg(QDir::toNativeSeparators(path)).arg(name);
                    QTimer::singleShot(0, this, [=]() {
                        model->rename(drives[path], item);
                    });
                }
            }

            thread->quit();
            thread->deleteLater();
            worker->deleteLater();
        });

        ui.tree->selectionModel()->selectionChanged(QItemSelection(), QItemSelection());
    }
    else
    {
        QModelIndex index = model->addRoot("/", "/");
        ui.tree->selectionModel()->select(index, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
        ui.tree->expand(index);
    }

    QShortcut* close = new QShortcut(QKeySequence::Close, this);
    QObject::connect(close, &QShortcut::activated, this, [=]()
    {
        auto tabs = qobject_cast<QTabWidget*>(parent);
        tabs->removeTab(tabs->indexOf(this));
    });
}

RemoteWidget::~RemoteWidget()
{
}
