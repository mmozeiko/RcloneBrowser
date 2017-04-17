#include "transfer_dialog.h"
#include "utils.h"

TransferDialog::TransferDialog(bool isDownload, const QString& remote, const QDir& path, bool isFolder, QWidget* parent)
    : QDialog(parent)
    , mIsDownload(isDownload), mJobOptions(nullptr)
{
    ui.setupUi(this);
    resize(0, 0);
    setWindowTitle(isDownload ? "Download" : "Upload");

    QStyle* style = qApp->style();
    ui.buttonSourceFile->setIcon(style->standardIcon(QStyle::SP_FileIcon));
    ui.buttonSourceFolder->setIcon(style->standardIcon(QStyle::SP_DirIcon));
    ui.buttonDest->setIcon(style->standardIcon(QStyle::SP_DirIcon));

    QPushButton* dryRun = ui.buttonBox->addButton("&Dry run", QDialogButtonBox::AcceptRole);
    ui.buttonBox->addButton("&Run", QDialogButtonBox::AcceptRole);

	QPushButton* saveTask = ui.buttonBox->addButton("&Save task", QDialogButtonBox::ButtonRole::ActionRole);

	QPushButton* loadTask = ui.buttonBox->addButton("&Load task", QDialogButtonBox::ButtonRole::ActionRole);

	QObject::connect(ui.buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, [=]()
    {
        ui.cbSyncDelete->setCurrentIndex(0);
        ui.checkSkipNewer->setChecked(false);
        ui.checkSkipNewer->setChecked(false);
        ui.checkCompare->setChecked(true);
        ui.cbCompare->setCurrentIndex(0);
        ui.checkVerbose->setChecked(false);
        ui.checkSameFilesystem->setChecked(false);
        ui.checkDontUpdateModified->setChecked(false);
        ui.spinTransfers->setValue(4);
        ui.spinCheckers->setValue(8);
        ui.textBandwidth->clear();
        ui.textMinSize->clear();
        ui.textMinAge->clear();
        ui.textMaxAge->clear();
        ui.spinMaxDepth->setValue(0);
        ui.spinConnectTimeout->setValue(60);
        ui.spinIdleTimeout->setValue(300);
        ui.spinRetries->setValue(3);
        ui.spinLowLevelRetries->setValue(10);
        ui.checkDeleteExcluded->setChecked(false);
        ui.textExclude->clear();
        ui.textExtra->clear();
    });
    ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)->click();

    QObject::connect(dryRun, &QPushButton::clicked, this, [=]()
    {
        mDryRun = true;
    });

	// temporary 
	QObject::connect(loadTask, &QPushButton::clicked, this, [=]()
	{
		QList<JobOptions> inList;
		bool okIn = JobOptions::RestoreFromUserData(inList);
		if (okIn && inList.size() > 0)
		{
			putJobOptions(inList[0]);
		}
	});

	QObject::connect(saveTask, &QPushButton::clicked, this, [=]()
	{
		// todo 
		JobOptions* jobo = getJobOptions();
		QStringList newWay = jobo->getOptions();

		// first test
		//QStringList oldWay = getOptions();
		//Q_ASSERT(oldWay.size() == newWay.size());
		//foreach (const QString &str, oldWay) {
		//	//qDebug() << QString(" [%1] ").arg(str);
		//	Q_ASSERT(newWay.contains(str));
		//}

		// second test
		//foreach(const QString &str, newWay) {
		//	qDebug() << QString(" [%1] ").arg(str);
		//	//Q_ASSERT(oldWay.contains(str));
		//}

		// third test
		QList<JobOptions> outList;
		outList.append(*jobo);
		bool okOut = JobOptions::PersistToUserData(outList);

		QList<JobOptions> inList;
		bool okIn = JobOptions::RestoreFromUserData(inList);

		qDebug() << QString(" okOut:[%1] okIn:[%2] records in [%3]").arg(okOut).arg(okIn).arg(inList.size());

	});

    QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QObject::connect(ui.buttonSourceFile, &QToolButton::clicked, this, [=]()
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose file to upload");
        if (!file.isEmpty())
        {
            ui.textSource->setText(QDir::toNativeSeparators(file));
        }
    });

    QObject::connect(ui.buttonSourceFolder, &QToolButton::clicked, this, [=]()
    {
        QString folder = QFileDialog::getExistingDirectory(this, "Choose folder to upload");
        if (!folder.isEmpty())
        {
            ui.textSource->setText(QDir::toNativeSeparators(folder));
            ui.textDest->setText(remote + ":" + path.filePath(QFileInfo(folder).fileName()));
        }
    });
    
    QObject::connect(ui.buttonDest, &QToolButton::clicked, this, [=]()
    {
        QString folder = QFileDialog::getExistingDirectory(this, "Choose destination folder");
        if (!folder.isEmpty())
        {
            if (isFolder)
            {
                ui.textDest->setText(QDir::toNativeSeparators(folder + "/" + path.dirName()));
            }
            else
            {
                ui.textDest->setText(QDir::toNativeSeparators(folder));
            }
        }
    });

    auto settings = GetSettings();
    settings->beginGroup("Transfer");
    ReadSettings(settings.get(), this);
    settings->endGroup();

    ui.buttonSourceFile->setVisible(!isDownload);
    ui.buttonSourceFolder->setVisible(!isDownload);
    ui.buttonDest->setVisible(isDownload);

    (isDownload ? ui.textSource : ui.textDest)->setText(remote + ":" + path.path());
}

TransferDialog::~TransferDialog()
{
    if (result() == QDialog::Accepted)
    {
        auto settings = GetSettings();
        settings->beginGroup("Transfer");
        WriteSettings(settings.get(), this);
        settings->remove("textSource");
        settings->remove("textDest");
        settings->endGroup();
    }
}

void TransferDialog::setSource(const QString& path)
{
    ui.textSource->setText(QDir::toNativeSeparators(path));
}

QString TransferDialog::getMode() const
{
    if (ui.rbCopy->isChecked())
    {
        return "Copy";
    }
    else if (ui.rbMove->isChecked())
    {
        return "Move";
    }
    else if (ui.rbSync->isChecked())
    {
        return "Sync";
    }

    return QString::null;
}

QString TransferDialog::getSource() const
{
    return ui.textSource->text();
}

QString TransferDialog::getDest() const
{
    return ui.textDest->text();
}

QStringList TransferDialog::getOptions()
{
	JobOptions* jobo = getJobOptions();
	QStringList newWay = jobo->getOptions();
	return newWay;
}

void TransferDialog::setJobOptions(JobOptions* job)
{
	mJobOptions = job;
	putJobOptions(*mJobOptions);
}


/*
 * Apply the displayed/edited values on the UI to the 
 * JobOptions object.  
 * 
 * This needs to be edited whenever options are added or changed.
 */
JobOptions *TransferDialog::getJobOptions()
{
	if (mJobOptions == nullptr)
		mJobOptions = new JobOptions(mIsDownload);

	if (ui.rbCopy->isChecked())
	{
		mJobOptions->operation = JobOptions::Copy;
	}
	else if (ui.rbMove->isChecked())
	{
		mJobOptions->operation = JobOptions::Move;
	}
	else if (ui.rbSync->isChecked())
	{
		mJobOptions->operation = JobOptions::Sync;
	}

	mJobOptions->dryRun = mDryRun;;

	if (ui.rbSync->isChecked())
	{
		mJobOptions->sync = true;
		switch (ui.cbSyncDelete->currentIndex())
		{
		case 0:
			mJobOptions->syncTiming = JobOptions::During;
			break;
		case 1:
			mJobOptions->syncTiming = JobOptions::After;
			break;
		case 2:
			mJobOptions->syncTiming = JobOptions::Before;
			break;
		}
	}

	mJobOptions->skipNewer = ui.checkSkipNewer->isChecked();
	mJobOptions->skipExisting = ui.checkSkipExisting->isChecked();

	if (ui.checkCompare->isChecked())
	{
		mJobOptions->compare = true;
		switch (ui.cbCompare->currentIndex())
		{
		case 1:
			mJobOptions->compareOption = JobOptions::Checksum;
			break;
		case 2:
			mJobOptions->compareOption = JobOptions::IgnoreSize;
			break;
		case 3:
			mJobOptions->compareOption = JobOptions::SizeOnly;
			break;
		case 4:
			mJobOptions->compareOption = JobOptions::ChecksumIgnoreSize;
			break;
		}
	}

	mJobOptions->verbose = ui.checkVerbose->isChecked();
	mJobOptions->sameFilesystem = ui.checkSameFilesystem->isChecked();
	mJobOptions->dontUpdateModified = ui.checkDontUpdateModified->isChecked();

	mJobOptions->transfers = ui.spinTransfers->text();
	mJobOptions->checkers = ui.spinCheckers->text();
	mJobOptions->bandwidth = ui.textBandwidth->text();
	mJobOptions->minSize =  ui.textMinSize->text();
	mJobOptions->minAge = ui.textMinAge->text();
	mJobOptions->maxAge = ui.textMaxAge->text();
	mJobOptions->maxDepth = ui.spinMaxDepth->value();

	mJobOptions->connectTimeout = ui.spinConnectTimeout->text();
	mJobOptions->idleTimeout = ui.spinIdleTimeout->text();
	mJobOptions->retries = ui.spinRetries->text();
	mJobOptions->lowLevelRetries = ui.spinLowLevelRetries->text();
	mJobOptions->deleteExcluded = ui.checkDeleteExcluded->isChecked();

	mJobOptions->excluded = ui.textExclude->toPlainText().trimmed();
	mJobOptions->extra = ui.textExtra->text().trimmed();

	mJobOptions->source = ui.textSource->text();
	mJobOptions->dest = ui.textDest->text();

	mJobOptions->description = ui.textDescription->text();
	
	return mJobOptions;
}

/*
 * Apply the JobOptions object to the displayed widget values.
 * 
 * It could be "better" to use a two-way binding mechanism, but 
 * if used that should be global to the app; and anyway doing 
 * it this old primitive way makes it easier when the user wants 
 * to not save changes...
 */
void TransferDialog::putJobOptions(JobOptions& jobo)
{
	ui.rbCopy->setChecked(jobo.operation == JobOptions::Copy);
	ui.rbMove->setChecked(jobo.operation == JobOptions::Move);
	ui.rbSync->setChecked(jobo.operation == JobOptions::Sync);

	mDryRun = jobo.dryRun;
	ui.rbSync->setChecked(jobo.sync);
	ui.cbSyncDelete->setCurrentIndex((int)jobo.syncTiming);


	ui.checkSkipNewer->setChecked(jobo.skipNewer);
	ui.checkSkipExisting->setChecked(jobo.skipExisting);

	ui.checkCompare->setChecked(jobo.compare);
	ui.cbCompare->setCurrentIndex(jobo.compareOption);

	ui.checkVerbose->setChecked(jobo.verbose);
	ui.checkSameFilesystem->setChecked(jobo.sameFilesystem);
	ui.checkDontUpdateModified->setChecked(jobo.dontUpdateModified);

	ui.spinTransfers->setValue(jobo.transfers.toInt());
	ui.spinCheckers->setValue(jobo.checkers.toInt());
	ui.textBandwidth->setText(jobo.bandwidth);
	ui.textMinSize->setText(jobo.minSize);
	ui.textMinAge->setText(jobo.minAge);
	ui.textMaxAge->setText(jobo.maxAge);
	ui.spinMaxDepth->setValue(jobo.maxDepth);

	ui.spinConnectTimeout->setValue(jobo.connectTimeout.toInt());
	ui.spinIdleTimeout->setValue(jobo.idleTimeout.toInt());
	ui.spinRetries->setValue(jobo.retries.toInt());
	ui.spinLowLevelRetries->setValue(jobo.lowLevelRetries.toInt());
	ui.checkDeleteExcluded->setChecked(jobo.deleteExcluded);

	ui.textExclude->setPlainText(jobo.excluded);
	ui.textExtra->setText(jobo.extra);

	ui.textSource->setText(jobo.source);
	ui.textDest->setText(jobo.dest);

	ui.textDescription->setText(jobo.description);
}

void TransferDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        if (mIsDownload)
        {
            if (ui.textDest->text().isEmpty())
            {
                QMessageBox::warning(this, "Warning", "Please enter destination!");
                return;
            }
        }
        else
        {
            if (ui.textSource->text().isEmpty())
            {
                QMessageBox::warning(this, "Warning", "Please enter source!");
                return;
            }
        }
    }
    QDialog::done(r);
}
