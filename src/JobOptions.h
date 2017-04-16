#pragma once
#include "qobject.h"
#include <qexception.h>

class JobOptions :
	public QObject
{
	//Q_OBJECT

public:
	explicit JobOptions(bool isDownload);
	JobOptions();

	~JobOptions();

	enum Operation { UnknownOp, Copy, Move, Sync };
	enum SyncTiming { UnknownTiming, During, After, Before};
	enum CompareOption {UnknownCompare, Checksum, IgnoreSize, SizeOnly, ChecksumIgnoreSize};
	enum JobType {UnknownJobType, Upload, Download};

	//Q_ENUM(Operation)
	//Q_ENUM(SyncTiming)
	//Q_ENUM(CompareOption)
	//Q_ENUM(JobType)

	QString name;
	
	JobType jobType;
	Operation operation;
	bool dryRun;
	bool sync;
	SyncTiming syncTiming;
	bool skipNewer;
	bool skipExisting;
	bool compare;
	CompareOption compareOption;
	bool verbose;
	bool sameFilesystem;
	bool dontUpdateModified;
	QString transfers;
	QString checkers;
	QString bandwidth;
	QString minSize;
	QString minAge;
	QString maxAge;
	int maxDepth;
	QString connectTimeout;
	QString idleTimeout;
	QString retries;
	QString lowLevelRetries;
	bool deleteExcluded;
	QString excluded;
	QString extra;
	QString source;
	QString dest;

	QStringList getOptions() const;
	void setJobType (bool isDownload)
	{
		jobType = (isDownload) ? Download : Upload;
	}





public:


	static const qint32 classVersion;

	QString className() const
	{
		return "JobOptions"; //this->staticQtMetaObject.className();
	}
};

	static QDataStream& operator >> (QDataStream& dataStream, JobOptions& jo);
	static QDataStream& operator << (QDataStream& dataStream, JobOptions& jo);
	static QDataStream& operator >> (QDataStream& in, JobOptions::Operation& e);
	static QDataStream& operator >> (QDataStream& in, JobOptions::SyncTiming& e);
	static QDataStream& operator >> (QDataStream& in, JobOptions::CompareOption& e);
	static QDataStream& operator >> (QDataStream& in, JobOptions::JobType& e);

class SerializationException : public QException
{
public:
	QString Message;
	explicit SerializationException(QString msg);
};

