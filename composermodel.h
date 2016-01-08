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

#include <QMap>
#include <QPair>
#include <QUuid>

#include "connection.h"

class AbstractNode;
class ComposerScheduler;

class ComposerModel : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerModel(QObject *parent = NULL);

        void addNode(AbstractNode *node);

        AbstractNode *findInputPlug(const QUuid &plugId) const;

        AbstractNode *findOutputPlug(const QUuid &plugId) const;

        AbstractNode *findPlug(const QUuid &plugId,
                               bool fromInputs = true,
                               bool fromOutputs = true) const;

        void addConnection(const QUuid &output, const QUuid &input);

        void removeConnection(const QUuid &connectionId);

        Connection getConnection(const QUuid &connectionId) const;

    signals:
        void connectionAdded(const QUuid &connectionId);

        void connectionRemoved(const QUuid &connectionId);

    private:
        void startExecution();

    private:
        QList<AbstractNode *> _nodes;
        QMap<QUuid, Connection> _connections;
        ComposerScheduler *_scheduler;
};

#endif // COMPOSERMODEL_H
