#include "projectrunner.h"

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTime>

#include "../project.h"
#include "projectbuilder.h"

ProjectRunner::ProjectRunner(Project& project):
    QThread(),
    mProject(project),
    mGame()
{
}

// - Operations

void ProjectRunner::run()
{
    emit messageReady("Building content...");

    QTime timer;
    timer.start();
    ProjectBuilder* pbuilder = new ProjectBuilder(mProject);
    pbuilder->start();
    pbuilder->wait();
    delete pbuilder;

    QString msg = QString("Build in %0 seconds").arg(timer.elapsed());
    emit messageReady(msg);

    QString args = " -path " + mProject.getBasePath() + QDir::separator() + "build";

    connect(&mGame, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(&mGame, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));

    mGame.setWorkingDirectory(QDir::currentPath());
    mGame.start("gamed.exe" + args);

    if ( !mGame.waitForStarted() )
    {
        msg = "Could not launch the game!";
    }
    else
    {
        msg = "Launched game.";
    }

    emit messageReady(msg);

    if ( mGame.waitForFinished(-1) )
    {
        switch ( mGame.exitStatus() )
        {
        case QProcess::NormalExit:
            msg = "Game stopped.";
            break;

        case QProcess::CrashExit:
            msg = "Game crashed.";
            break;
        }
    }

    emit messageReady(msg);
}

// - Slots

void ProjectRunner::readStandardOutput()
{
    QString msg(mGame.readAllStandardOutput());
    if ( !msg.isEmpty() )
    {
        emit messageReady(msg);
    }
}

void ProjectRunner::readStandardError()
{
    QString msg(mGame.readAllStandardError());
    if ( !msg.isEmpty() )
    {
        emit messageReady(msg);
    }
}
