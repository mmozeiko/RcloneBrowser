#include "job_options.h"
#include <qexception.h>
#include <qdebug.h>
#include <qlogging.h>
#ifdef _WIN32
#pragma warning(disable:4505)
#endif
JobOptions::JobOptions(bool isDownload) : JobOptions()
{
    setJobType(isDownload);
    uniqueId = QUuid::createUuid();
}

JobOptions::JobOptions():
    jobType(UnknownJobType), operation(UnknownOp), dryRun(false), sync(false), 
    syncTiming(UnknownTiming), skipNewer(false), skipExisting(false), 
    compare(false), compareOption(), verbose(false), sameFilesystem(false),
    dontUpdateModified(false), maxDepth(0), deleteExcluded(false), isFolder(false)
{
}

const qint32 JobOptions::classVersion = 3;


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
        default: 
            break;;
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
        default: 
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

SerializationException::SerializationException(QString msg) : QException(), Message(msg)
{
}



