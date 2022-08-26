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

#include <QGraphicsScene>

#include <QDomDocument>
#include <QGraphicsSceneDragDropEvent>
#include <QMainWindow>
#include <QUndoStack>

#include "model/plug.h"

class Node;
class Connection;
class ComposerModel;
class ConnectionItem;
class GenericNodeItem;
class ComposerScheduler;

class ComposerScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        explicit ComposerScene(QUndoStack *commandsStack, QObject *parent = nullptr);

        explicit ComposerScene(const QDomDocument &doc,
                               QMainWindow *mainWindow,
                               QUndoStack *commandsStack,
                               QObject *parent = nullptr);

        const QList<GenericNodeItem *> &getNodes() const;

        GenericNodeItem *findNode(const QUuid &uid) const;

        const QList<ConnectionItem *> &getConnections() const;

        const ComposerScheduler *getScheduler() const;

        ComposerScheduler *accessScheduler();

        GenericNodeItem *addNode(const QString &nodeName, const QUuid &uid);

        const ComposerModel *getModel() const { return _model; }

        ComposerModel *accessModel() { return _model; }

        void save(QDomDocument &doc, QMainWindow *mainWindow) const;

        void end();

        void loadNode(const QDomElement &node);

        static void saveNode(QDomDocument &doc, QDomElement &domNode, const GenericNodeItem *nodeItem);

    signals:
        void ended();

    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;

        virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void keyPressEvent(QKeyEvent *keyEvent) override;

    private:
        void onNodeRemoved(const Node *node);

        void onConnectionAdded(const Connection *connection);

        void onConnectionRemoved(const Connection *connection);

        void onPlugItemPositionChanged();

        void onExecutionStarted(const Node *node);

        void onExecutionProgress(const Node *node, qreal progress);

        void onExecutionEnded(const Node *node,
                              const Properties &outputs,
                              const Properties &inputs,
                              qint64 duration,
                              const QString &error);

        void onNodeInvalid(const Node *node);

        void init();

        void load(const QDomDocument &doc, QMainWindow *mainWindow);

        GenericNodeItem *findItem(const Node *node);

        QUuid loadUid(const QDomElement &node, const QString &attributePrefix = "");

    private:
        struct EditedConnection
        {
            ConnectionItem *item;
            ConnectionItem *initialItem;
            bool fromOutput;
            Plug *plugInput;
            Plug *plugOutput;
            Node *baseNode;
            const Connection *initialConnection;
        };

        struct EditedNode
        {
            GenericNodeItem *item;
            QPointF initClickPos;
            QPointF initNodePose;
        };

    private:
        QUndoStack *const _commandsStack;
        ComposerModel *const _model;
        ComposerScheduler *const _scheduler;
        EditedConnection _editedConnection;
        EditedNode _editedNode;
        QList<ConnectionItem *> _connections;
        QList<GenericNodeItem *> _nodes;
};
