#ifndef PROJECTBUILDER_H
#define PROJECTBUILDER_H

class QDir;
class QString;

class ProjectBuilder
{
public:
    ProjectBuilder();

  // operations
    bool build(QDir& path, const QString& name);

private:

  // operations
    bool setupDestination(QDir &path, const QString& name);

    void buildContent(QDir& path, const QString& name);
    void buildScripts(QDir& path, const QString& name);

    void compile(const QString& source, const QString& dest);

  // data
};

#endif // PROJECTBUILDER_H
