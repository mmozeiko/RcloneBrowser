#include "JobOptions.h"
#include <qexception.h>
#include <QDataStream>
#include <qstandardpaths.h>
#include <qdir.h>
#include <qdebug.h>
#include <qlogging.h>
#pragma warning(disable:4505)
JobOptions::JobOptions(bool isDownload) : JobOptions()
{
	setJobType(isDownload);
}

JobOptions::JobOptions():
	jobType(UnknownJobType), operation(UnknownOp), dryRun(false), sync(false), 
    syncTiming(UnknownTiming), skipNewer(false), skipExisting(false), 
    compare(false), compareOption(), verbose(false), sameFilesystem(false),
	dontUpdateModified(false), maxDepth(0), deleteExcluded(false)
{
}

const qint32 JobOptions::classVersion = 1;
const QString JobOptions::persistenceFileName = "tasks.bin";

JobOptions::~JobOptions()
{
}

/*
 * Turn the options held here into a string list for 
 * use in the rclone command.  
 * 
 * This logic was originally in transfer_dialog.cpp.
 * 
 * This needs to change whenever e.g. new options are 
 * added to the dialog.
 */
QStringList JobOptions::getOptions() const
{
	QStringList list;

	if (operation == Copy)
	{
		list << "copy";
	}
	else if (operation == Move)
	{
		list << "move";
	}
	else if (operation == Sync)
	{
		list << "sync";
	}

	if (dryRun)
	{
		list << "--dry-run";
	}

	if (sync)
	{
		switch (syncTiming)
		{
		case During:
			list << "--delete-during";
			break;
		case After:
			list << "--delete-after";
			break;
		case Before:
			list << "--delete-before";
			break;
		}
	}

	if (skipNewer)
	{
		list << "--update";
	}
	if (skipExisting)
	{
		list << "--ignore-existing";
	}

	if (compare)
	{
		switch (compareOption)
		{
		case Checksum:
			list << "--checksum";
			break;
		case IgnoreSize:
			list << "--ignore-size";
			break;
		case SizeOnly:
			list << "--size-only";
			break;
		case ChecksumIgnoreSize:
			list << "--checksum" << "--ignore-size";
			break;
		}
	}

	if (verbose)
	{
		list << "--verbose";
	}
	if (sameFilesystem)
	{
		list << "--one-file-system";
	}
	if (dontUpdateModified)
	{
		list << "--no-update-modtime";
	}

	list << "--transfers" << transfers;
	list << "--checkers" << checkers;

	if (!bandwidth.isEmpty())
	{
		list << "--bwlimit" << bandwidth;
	}
	if (!minSize.isEmpty())
	{
		list << "--min-size" << minSize;
	}
	if (!minAge.isEmpty())
	{
		list << "--min-age" << minAge;
	}
	if (!maxAge.isEmpty())
	{
		list << "--max-age" << maxAge;
	}

	if (maxDepth != 0)
	{
		list << "--max-depth" << QString::number(maxDepth);
	}

	list << "--contimeout" << (connectTimeout + "s");
	list << "--timeout" << (idleTimeout + "s");
	list << "--retries" << retries;
	list << "--low-level-retries" << lowLevelRetries;

	if (deleteExcluded)
	{
		list << "--delete-excluded";
	}

	if (!excluded.isEmpty())
	{
		for (auto line : excluded.split('\n'))
		{
			list << "--exclude" << line;
		}
	}

	if (!extra.isEmpty())
	{
		for (auto arg : extra.split(' '))
		{
			list << arg;
		}
	}

	list << "--stats" << "1s";

	list << source;
	list << dest;

	return list;
}

QFile* JobOptions::GetPersistenceFile(QIODevice::OpenModeFlag mode)
{
	QDir outputDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if (!outputDir.exists())
	{
		outputDir.mkpath(".");
	}
	QString filePath = outputDir.absoluteFilePath(persistenceFileName);
	QFile* file = new QFile(filePath);
	if (!file->open(mode))
	{
		qDebug() << QString("Could not open ") << file->fileName();
		delete file;
		file = nullptr;
	}
	return file;
}

bool JobOptions::PersistToUserData(QList<JobOptions>& dataOut)
{
	QFile* file = GetPersistenceFile(QIODevice::WriteOnly); // note this mode implies Truncate also
	if (file == nullptr) return false;
	QDataStream outstream(file);
	outstream.setVersion(QDataStream::Qt_5_8);

	for (JobOptions& jo : dataOut)
	{
		outstream << jo;
	}

	file->flush();
	file->close();
	delete file;

	return true;
}

bool JobOptions::RestoreFromUserData(QList<JobOptions>& dataIn)
{
	QFile* file = GetPersistenceFile(QIODevice::ReadOnly);
	if (file == nullptr) return false;
	QDataStream instream(file);
	instream.setVersion(QDataStream::Qt_5_8);

	while (!instream.atEnd())
	{
		try
		{
			JobOptions jo;
			instream >> jo;
			dataIn.append(jo);
		}
		catch (SerializationException ex)
		{
			qDebug() << QString("failed to restore tasks: ") << ex.Message;
			file->close();
			delete file;
			return false;
		}
	}

	file->close();
	delete file;

	return true;
}


QDataStream& operator>>(QDataStream& stream, JobOptions& jo)
{
	QString actualName;
	qint32 actualVersion;
	
	stream >> actualName;
	if (QString::compare(actualName, jo.myName()) != 0)
		throw SerializationException("incorrect class");

	stream >> actualVersion;
	if (actualVersion > JobOptions::classVersion)
		throw SerializationException("stored version is newer");

	stream >> jo.description
		>> jo.jobType >> jo.operation >> jo.dryRun >> jo.sync >> jo.syncTiming
		>> jo.skipNewer >> jo.skipExisting >> jo.compare >> jo.compareOption
		>> jo.verbose >> jo.sameFilesystem >> jo.dontUpdateModified >> jo.transfers
		>> jo.checkers >> jo.bandwidth >> jo.minSize >> jo.minAge >> jo.maxAge
		>> jo.maxDepth >> jo.connectTimeout >> jo.idleTimeout >> jo.retries
		>> jo.lowLevelRetries >> jo.deleteExcluded >> jo.excluded >> jo.extra
		>> jo.source >> jo.dest;

	// if fields are added in later revisions, check actualVersion here and 
	// conditionally extract any new fields iff they are expected based on the stream value

	return stream;
}

QDataStream& operator<<(QDataStream& stream, JobOptions& jo)
{
	stream << jo.myName() << JobOptions::classVersion << jo.description
		<< jo.jobType << jo.operation << jo.dryRun << jo.sync << jo.syncTiming
		<< jo.skipNewer << jo.skipExisting << jo.compare << jo.compareOption
		<< jo.verbose << jo.sameFilesystem << jo.dontUpdateModified << jo.transfers
		<< jo.checkers << jo.bandwidth << jo.minSize << jo.minAge << jo.maxAge
		<< jo.maxDepth << jo.connectTimeout << jo.idleTimeout << jo.retries
		<< jo.lowLevelRetries << jo.deleteExcluded << jo.excluded << jo.extra
		<< jo.source << jo.dest;

	return stream;
}





QDataStream& operator>>(QDataStream& in, JobOptions::Operation& e)
{
	in >> (quint32&)e;
	return in;
}

QDataStream& operator>>(QDataStream& in, JobOptions::SyncTiming& e)
{
	in >> (quint32&)e;
	return in;
}

QDataStream& operator>>(QDataStream& in, JobOptions::CompareOption& e)
{
	in >> (quint32&)e;
	return in;
}

QDataStream& operator>>(QDataStream& in, JobOptions::JobType& e)
{
	in >> (quint32&)e;
	return in;
}

SerializationException::SerializationException(QString msg): QException(), Message(msg)
{
}

//// ostream, << overloading
//QDataStream &JobOptions::operator<<(QDataStream &out)
//{
//out << s.myName() << s.name;
//return out;
//}

//// istream, >> overloading
//QDataStream &JobOptions::operator>>(QDataStream &in, JobOptions &s)
//{
//QString actualName;
//quint32 actualVersion;
//s = JobOptions();
//in >> s.actualName >> s.Name;
//return in;
//}

