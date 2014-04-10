#ifndef PROJECTBUILDER_H
#define PROJECTBUILDER_H

#include <QThread>

class Project;
class QDir;
class QString;

class ProjectBuilder : public QThread
{
    Q_OBJECT

public:
    explicit ProjectBuilder(Project& project);

  // overrides
    virtual void run() override;

signals:
    void messageAvailable(const QString& msg);

private:

  // operations
    void buildContent(QDir& path, const QString& name);
    void buildScripts(QDir& path, const QString& name);

    bool setupDestination(QDir &path, const QString& name);

    void compile(const QString& source, const QString& dest);

  // data
    Project& mProject;
};

#endif // PROJECTBUILDER_H
