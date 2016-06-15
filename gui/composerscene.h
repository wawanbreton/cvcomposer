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
        explicit ComposerScene(QObject *parent = NULL);

        explicit ComposerScene(const QDomDocument &doc,
                               QMainWindow *mainWindow,
                               QObject *parent = NULL);

        const QList<GenericNodeItem *> &getNodes() const;

        const QList<ConnectionItem *> &getConnections() const;

        const ComposerScheduler *getScheduler() const;

        ComposerScheduler *accessScheduler();

        GenericNodeItem *addNode(const QString &nodeName);

        void save(QDomDocument &doc, QMainWindow *mainWindow) const;

    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private slots:
        void onConnectionAdded(const Connection *connection);

        void onConnectionRemoved(const Connection *connection);

        void onPlugItemPositionChanged();

    private:
        void init();

        void load(const QDomDocument &doc, QMainWindow *mainWindow);

    private:
        typedef struct
        {
            ConnectionItem *item;
            bool fromOutput;
            Plug *plugInput;
            Plug *plugOutput;
            Node *baseNode;
        } EditedConnection;

        typedef struct
        {
            GenericNodeItem *item;
            QPointF initClickPos;
            QPointF initNodePose;
        } EditedNode;

    private:
        ComposerModel *_model;
        ComposerScheduler *_scheduler;
        EditedConnection _editedConnection;
        EditedNode _editedNode;
        QList<ConnectionItem *> _connections;
        QList<GenericNodeItem *> _nodes;
};
