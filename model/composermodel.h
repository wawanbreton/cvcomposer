// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#ifndef COMPOSERMODEL_H
#define COMPOSERMODEL_H

#include <QObject>

#include <QPointer>
#include <QMap>
#include <QPair>

#include "model/plug.h"
#include "execution/executorsettings.h"

class Connection;
class Node;
class ComposerScheduler;

class ComposerModel : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerModel(QObject *parent = NULL);

        void addNode(Node *node);

        Node *findInputPlug(const Plug *plug) const;

        Node *findOutputPlug(const Plug *plug) const;

        Node *findPlug(const Plug *plug,
                       bool fromInputs = true,
                       bool fromOutputs = true) const;

        void addConnection(Plug *output, Plug *input);

        void removeConnection(Connection *connection);

        const ExecutorSettings &getExecutorSettings() const;

        void setExecutorSettings(const ExecutorSettings &settings);

    signals:
        void connectionAdded(Connection *connectionId);

        void connectionRemoved(Connection *connectionId);

    private slots:
        void startExecution();

    private:
        QList<Node *> _nodes;
        QList<Connection *> _connections;
        ExecutorSettings _executorSettings;
        QPointer<ComposerScheduler> _scheduler;
};

#endif // COMPOSERMODEL_H
