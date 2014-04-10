#ifndef PROJECTRUNNER_H
#define PROJECTRUNNER_H

#include <QProcess>
#include <QThread>

class Project;

class ProjectRunner : public QThread
{
    Q_OBJECT

public:
    ProjectRunner(Project &project);

  // operations
    void run() Q_DECL_OVERRIDE;

public slots:
    void readStandardOutput();
    void readStandardError();

signals:
    void messageReady(const QString& msg);

private:

  // data
    Project& mProject;
    QProcess mGame;
};

#endif // PROJECTRUNNER_H
