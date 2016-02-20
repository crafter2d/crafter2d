#include "projectbuilder.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>

#include "../project.h"

ProjectBuilder::ProjectBuilder(Project& project):
    QThread(nullptr),
    mProject(project)
{
}

// - Operations

struct BuildInfo
{
    QString path;
    QString filter;
} sContentFolders[] = {
    { "objects", "*.xml" },
    { "shaders", "*.fx" },
    { "sounds", "*.ogg" },
    { "tileatlas", "*.png" },
    { "tileatlas", "*.xml" },
    { "tilesets", "*.xml" },
    { "worlds", "*.world" },
};

void ProjectBuilder::run()
{
    const QString name = "build";

    QDir path(mProject.getBasePath());

    if ( setupDestination(path, name) )
    {
        buildContent(path, name);
        buildScripts(path, name);
    }
}

bool ProjectBuilder::setupDestination(QDir& path, const QString& name)
{
    if ( !path.exists(name) )
    {
        if ( !path.mkdir(name) )
        {
            // could not create the folder where the compiled file must be placed
            emit messageAvailable("Could not create folder " + name + " in " + path.absolutePath());
        }
    }

    path.cd(name);
    for ( BuildInfo& info : sContentFolders )
    {
        if ( !path.exists(info.path) )
        {
            path.mkdir(info.path);
        }
        else
        {
            path.cd(info.path);
            QFileInfoList infos = path.entryInfoList();
            for ( const QFileInfo& info : infos )
            {
                path.remove(info.fileName());
            }
            path.cdUp();
        }
    }

    path.cdUp();

    return true;
}

void ProjectBuilder::buildScripts(QDir& path, const QString& name)
{
    QString sourcepath = path.absolutePath() + QDir::separator() + "scripts";
    QString destpath = path.absolutePath() + QDir::separator() + name + QDir::separator() + "scripts";

    emit messageAvailable("Compiling scripts in " + sourcepath);

    QDir destdir(path);
    destdir.cd(name);
    destdir.mkdir("scripts");

    QString command;
#ifdef _DEBUG
    command = "yascd.exe -p ../scripts;";
#else
    command = "yasc.exe -p ../scripts;";
#endif
    command += sourcepath + " -r -o " + destpath + " " + sourcepath + QDir::separator() + "*.as";

    QProcess yasc;
    yasc.setWorkingDirectory(QDir::currentPath());
    yasc.start(command);
    if ( yasc.waitForFinished(60000) )
    {
        QString msg;

        switch ( yasc.exitStatus() )
        {
        case QProcess::CrashExit:
            msg = "An unknown error occured while compiling the scripts.";
            break;
        case QProcess::NormalExit:
            {
                QString errors(yasc.readAllStandardOutput());
                if ( !errors.isEmpty() )
                {
                    reportErrors(errors);
                }

                int exitcode = yasc.exitCode();
                if ( exitcode == 0 )
                {
                    msg = "Compilation successfull";
                }
            }
            break;
        }

        if ( !msg.isEmpty() )
        {
            emit messageAvailable(msg);
        }
    }
}

void ProjectBuilder::buildContent(QDir& path, const QString& name)
{
    QString destpath = path.absolutePath() + QDir::separator() + name + QDir::separator();

    for ( BuildInfo& info : sContentFolders )
    {
        QDir folder(path);
        folder.cd(info.path);

        QString folderpath = destpath + info.path + QDir::separator();

        QStringList filters;
        filters << info.filter;

        QFileInfoList infos = folder.entryInfoList(filters);
        for ( const QFileInfo& info : infos )
        {
            QString source = info.absoluteFilePath();
            QString dest   = folderpath + info.baseName() + ".c2d";

            compile(source, dest);
        }
    }
}

void ProjectBuilder::compile(const QString& source, const QString& dest)
{
    QProcess compiler;
    compiler.setWorkingDirectory(QDir::currentPath());

    QStringList arguments;
    arguments.append(source);
    arguments.append(dest);

#ifdef _DEBUG
    compiler.start("compilerd.exe", arguments);
#else
    compiler.start("compiler.exe", arguments);
#endif

    if ( compiler.waitForFinished(60000) )
    {
        QString message;
        QFileInfo info(source);

        switch ( compiler.exitStatus() )
        {
        case QProcess::CrashExit:
            message = tr("Crash while compiling asset '%0'").arg(info.baseName());
            break;
        case QProcess::NormalExit:
            {
                int exitcode = compiler.exitCode();
                if ( exitcode == 0 )
                {
                    return;
                }
                else
                {
                    message = QString("Failed to convert asset '%0'").arg(info.baseName());
                    switch ( exitcode )
                    {
                    case -1:
                        break;
                    case -2:
                        break;
                    }
                }
                break;
            }
        }

        if ( !message.isEmpty() )
        {
            emit messageAvailable(message);
        }
    }
}

// - Error reporting

void ProjectBuilder::reportErrors(const QString& errors)
{
    QString error;
    QStringList errorlist = errors.split('\n');
    errorlist.removeAt(0); // remove info
    foreach (error, errorlist)
    {
        emit messageAvailable(error);
    }
}
