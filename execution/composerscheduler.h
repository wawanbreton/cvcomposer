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

#ifndef COMPOSERSCHEDULER_H
#define COMPOSERSCHEDULER_H

#include <QObject>

#include <opencv2/core/core.hpp>

#include <QMap>
#include <QQueue>

#include "global/properties.h"

class Connection;
class Node;
class ComposerExecutor;
class AbstractProcessor;
class Plug;

class ComposerScheduler : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerScheduler(QObject *parent = NULL);

        void prepareExecution(const QList<Node *> &nodes,
                              const QList<Connection *> &connections);

        void cancel();

    public slots:
        void execute();

    private slots:
        void onNodeProcessed(bool success, const Properties &outputs, bool keepProcessing);

    private:
        bool makeInputs(Node *node, Properties &inputs);

        void processNextIfPossible();

        const Connection *findConnectionToInput(const Plug *input);

    private:
        ComposerExecutor * _executor;
        QQueue<Node *> _executionList;
        QQueue<Node *> _initialExecutionList;
        QList<Connection *> _connections;
        QList<Node *> _unreachableNodes;
        QMap<Node *, Properties> _processedNodes;
        bool _cancelled;
        bool _keepProcessing;
};

#endif // COMPOSERSCHEDULER_H
