/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Creator.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAEMODEBUGSUPPORT_H
#define MAEMODEBUGSUPPORT_H

#include "maemorunconfiguration.h"

#include <coreplugin/ssh/sftpdefs.h>
#include <utils/environment.h>

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>

namespace Core { class SftpChannel; }

namespace Debugger {
class DebuggerEngine;
}

namespace ProjectExplorer { class RunControl; }

namespace Qt4ProjectManager {
namespace Internal {

class MaemoRunConfiguration;
class MaemoSshRunner;

class MaemoDebugSupport : public QObject
{
    Q_OBJECT
public:
    static ProjectExplorer::RunControl *createDebugRunControl(MaemoRunConfiguration *runConfig);

    MaemoDebugSupport(MaemoRunConfiguration *runConfig,
        Debugger::DebuggerEngine *engine);
    ~MaemoDebugSupport();

    static QString uploadDir(const MaemoDeviceConfig &devConf);

private slots:
    void handleAdapterSetupRequested();
    void handleSshError(const QString &error);
    void startExecution();
    void handleSftpChannelInitialized();
    void handleSftpChannelInitializationFailed(const QString &error);
    void handleSftpJobFinished(Core::SftpJobId job, const QString &error);
    void handleDebuggingFinished();
    void handleRemoteOutput(const QByteArray &output);
    void handleRemoteErrorOutput(const QByteArray &output);
    void handleProgressReport(const QString &progressOutput);

private:
    enum State {
        Inactive, StartingRunner, InitializingUploader, UploadingDumpers,
        DumpersUploaded, StartingRemoteProcess, Debugging
    };

    static QString environment(MaemoRunConfiguration::DebuggingType debuggingType,
        const QList<Utils::EnvironmentItem> &userEnvChanges);

    void handleAdapterSetupFailed(const QString &error);
    void handleAdapterSetupDone();
    void startDebugging();
    bool useGdb() const;
    void setState(State newState);
    bool setPort(int &port);

    void showMessage(const QString &msg, int channel);
    const QPointer<Debugger::DebuggerEngine> m_engine;
    const QPointer<MaemoRunConfiguration> m_runConfig;
    MaemoSshRunner * const m_runner;
    const MaemoRunConfiguration::DebuggingType m_debuggingType;
    const QString m_dumperLib;

    QSharedPointer<Core::SftpChannel> m_uploader;
    Core::SftpJobId m_uploadJob;
    QByteArray m_gdbserverOutput;
    State m_state;
    int m_gdbServerPort;
    int m_qmlPort;
};

} // namespace Internal
} // namespace Qt4ProjectManager

#endif // MAEMODEBUGSUPPORT_H
