#include "scriptbuilder.h"

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QString>

#include "scriptfile.h"

#include "../project.h"


// - Statics

ScriptFile* ScriptBuilder::build(Project& project, const QString& name, const QString& baseclass)
{
    QStringList parts = name.split('.');
    if ( parts.size() > 1 )
    {
        // ensure that the package exists
        QDir path(project.getBasePath());
        path.cd("scripts");
        for ( int index = 0; index < parts.size() - 1; ++index )
        {
            const QString& part = parts[index];
            if ( !path.exists(part) && !path.mkdir(part) )
            {
                QMessageBox::critical(nullptr, "Crafter Workshop", "Could not create folder for package '" + part + "'.", QMessageBox::Ok);
                return nullptr;
            }
            path.cd(part);
        }
    }

    QStringList arguments;
    arguments.append("class");
    arguments.append("name=" + name);
    arguments.append("path=" + project.getBasePath());
    if ( !baseclass.isEmpty() )
    {
        arguments.append("base=" + baseclass);
    }

    if ( name.indexOf('.') == -1 )
    {
        arguments.append("package=" + project.getName());
    }

    QProcess gen;
    gen.start("gend.exe", arguments);
    if ( gen.waitForFinished() )
    {
        if ( gen.exitStatus() == QProcess::NormalExit )
        {
            int exitCode = gen.exitCode();
            if ( exitCode == 0 )
            {
                QString filename = name;
                filename.replace('.', QDir::separator());
                QString resourcepath = QString("scripts") + QDir::separator() + project.getName() + QDir::separator() + filename + ".as";
                QString filepath = project.getBasePath() + QDir::separator() + resourcepath;
                ScriptFile* pscript = new ScriptFile(filepath);
                pscript->setResourceName(resourcepath);
                return pscript;
            }
        }
    }

    return nullptr;
}
