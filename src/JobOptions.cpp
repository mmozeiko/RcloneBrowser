#include "JobOptions.h"
#include <qexception.h>
#include <QDataStream>
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

JobOptions::~JobOptions()
{
}

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

QDataStream& operator>>(QDataStream& stream, JobOptions& jo)
{
	QString actualName;
	qint32 actualVersion;
	
	stream >> actualName;
	if (QString::compare(actualName, jo.className()) != 0)
		throw SerializationException("incorrect class");

	stream >> actualVersion;
	if (actualVersion > JobOptions::classVersion)
		throw SerializationException("stored version is newer");

	stream >> jo.name
		>> jo.jobType >> jo.operation >> jo.dryRun >> jo.sync >> jo.syncTiming
		>> jo.skipNewer >> jo.skipExisting >> jo.compare >> jo.compareOption
		>> jo.verbose >> jo.sameFilesystem >> jo.dontUpdateModified >> jo.transfers
		>> jo.checkers >> jo.bandwidth >> jo.minSize >> jo.minAge >> jo.maxAge
		>> jo.maxDepth >> jo.connectTimeout >> jo.idleTimeout >> jo.retries
		>> jo.lowLevelRetries >> jo.deleteExcluded >> jo.excluded >> jo.extra
		>> jo.source >> jo.dest;

	return stream;
}

QDataStream& operator<<(QDataStream& stream, JobOptions& jo)
{
	stream << jo.className() << JobOptions::classVersion << jo.name
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
//out << s.className() << s.name;
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

