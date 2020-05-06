#ifndef PROJECTDOCUMENT_H
#define PROJECTDOCUMENT_H

#include "../project.h"

class QFile;
class QJsonObject;

class ProjectDocument
{
public:
    static Project* fromJson(QFile& file);

    ProjectDocument(Project& project);

    QByteArray toJson() const;

private:

    void writeSettings(QJsonObject& json) const;
    void writeResources(QJsonObject& json) const;

    void load(const QJsonObject &json);
    void loadSettings(const QJsonObject& json);
    void loadResources(const QJsonObject& json);

    void loadScript(const QString& fileName);
    void loadWorld(const QString& fileName);
    void loadTileset(const QString& filename);

    void loadOldFormat(QByteArray &contents);

    Project& mProject;
};

#endif // PROJECTDOCUMENT_H
