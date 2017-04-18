#pragma once
#include <qfile.h>
#include "JobOptions.h"

class ListOfJobOptions : public QObject
{
	Q_OBJECT

protected:
	~ListOfJobOptions() = default;
	ListOfJobOptions();
public:
	static ListOfJobOptions* getInstance();
	bool Persist(JobOptions *jo);
	QList<JobOptions*> &getTasks() { return tasks; }

signals: 
	void tasksListUpdated();

private:
	static ListOfJobOptions* SavedJobOptions;
	static const QString persistenceFileName;
	static bool RestoreFromUserData(ListOfJobOptions& dataIn);
	static QFile* GetPersistenceFile(QIODevice::OpenModeFlag mode);


	QList<JobOptions*> tasks;
	bool PersistToUserData();

};

static QDataStream& operator >> (QDataStream& dataStream, JobOptions& jo);
static QDataStream& operator << (QDataStream& dataStream, JobOptions& jo);
static QDataStream& operator >> (QDataStream& in, JobOptions::Operation& e);
static QDataStream& operator >> (QDataStream& in, JobOptions::SyncTiming& e);
static QDataStream& operator >> (QDataStream& in, JobOptions::CompareOption& e);
static QDataStream& operator >> (QDataStream& in, JobOptions::JobType& e);
