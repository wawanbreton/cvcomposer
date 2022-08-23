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

#pragma once

#include <QObject>

#include <QPointer>
#include <QMap>
#include <QPair>

#include "model/plug.h"

class Connection;
class Node;
class ComposerScheduler;

class ComposerModel : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerModel(QObject *parent = nullptr);

        void addNode(Node *node);

        QList<const Node *> getNodes() const;

        void removeNode(Node *node);

        Node *findInputPlug(const Plug *plug) const;

        Node *findOutputPlug(const Plug *plug) const;

        Node *findPlug(const Plug *plug,
                       bool fromInputs = true,
                       bool fromOutputs = true) const;

        Node *findNode(const QUuid &uid) const;

        const Connection *findConnectionToInput(const Plug *input) const;

        QSet<const Node *> findDirectDescendantNodes(const Plug *output) const;

        QSet<const Node *> findDescendantNodes(const Node *node, bool includeParent = true) const;

        void addConnection(Plug *output, Plug *input);

        void removeConnection(const Connection *connection);

    signals:
        void nodeAdded(const Node *node);

        void nodeRemoved(const Node *node);

        void connectionAdded(const Connection *connectionId);

        void connectionRemoved(const Connection *connectionId);

    private:
        QList<Node *> _nodes;
        QList<Connection *> _connections;
};
