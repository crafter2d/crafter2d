#include "projectbuilder.h"

#include <QDir>
#include <QProcess>

ProjectBuilder::ProjectBuilder()
{
}

// - Operations

int sFolderCount = 6;
struct BuildInfo
{
    QString path;
    QString filter;
};

BuildInfo sContentFolders[] = {
    { "images", "*.dds" },
    { "objects", "*.xml" },
    { "shaders", "*.fx" },
    { "sounds", "*.ogg" },
    { "tilesets", "*.xml" },
    { "worlds", "*.world" },
};

bool ProjectBuilder::build(QDir &path, const QString& name)
{
    if ( !setupDestination(path, name) )
    {
        return false;
    }

    QString destpath = path.absolutePath() + QDir::separator() + name + QDir::separator();

    for ( int index = 0; index < sFolderCount; ++index )
    {
        BuildInfo& info = sContentFolders[index];

        QDir folder(path);
        folder.cd(info.path);

        QString folderpath = destpath + info.path + QDir::separator();

        QStringList filters;
        filters << info.filter;

        QFileInfoList infos = folder.entryInfoList(filters);
        for ( int index = 0; index < infos.size(); ++index )
        {
            const QFileInfo& info = infos.at(index);
            QString source = info.absoluteFilePath();
            QString dest   = folderpath + info.baseName() + ".c2d";

            compile(source, dest);
        }
    }

    return true;
}

bool ProjectBuilder::setupDestination(QDir& path, const QString& name)
{
    if ( path.exists(name) )
    {
        path.cd(name);
        for ( int index = 0; index < sFolderCount; ++index )
        {
            BuildInfo& info = sContentFolders[index];
            path.cd(info.path);

            QFileInfoList infos = path.entryInfoList();
            for ( int index = 0; index < infos.size(); ++index )
            {
                const QFileInfo& info = infos.at(index);
                path.remove(info.fileName());
            }

            path.cdUp();
        }
        path.cdUp();
    }
    else
    {
        if ( !path.mkdir(name) )
        {
            // could not create the folder where the compiled file must be placed
            return false;
        }
        else
        {
            path.cd(name);

            for ( int index = 0; index < sFolderCount; ++index )
            {
                BuildInfo& info = sContentFolders[index];
                path.mkdir(info.path);
            }

            path.cdUp();
        }
    }

    return true;
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

    if ( compiler.waitForFinished() )
    {
        QString message;

        switch ( compiler.exitStatus() )
        {
        case QProcess::CrashExit:
            message = "";
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
                    message = "Failed to convert asset.";
                    switch ( exitcode )
                    {

                    }
                }
                break;
            }
        }
    }
}
