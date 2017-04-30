#pragma once
#include <qfile.h>
#include "job_options.h"

class ListOfJobOptions : public QObject
{
    Q_OBJECT

protected:
    ~ListOfJobOptions() = default;
    ListOfJobOptions();
public:
    static ListOfJobOptions* getInstance();
    bool Persist(JobOptions *jo);
    bool Forget(JobOptions *jo);
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

