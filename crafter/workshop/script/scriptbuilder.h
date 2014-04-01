#ifndef SCRIPTBUILDER_H
#define SCRIPTBUILDER_H

class Project;
class ScriptFile;
class QString;

class ScriptBuilder
{
public:
    static ScriptFile* build(Project& project, const QString& name, const QString& baseclass);
};

#endif // SCRIPTBUILDER_H
