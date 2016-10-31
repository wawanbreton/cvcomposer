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
#include "execution/executorsettings.h"

class Connection;
class Node;
class ComposerExecutor;
class AbstractProcessor;
class Plug;
class ComposerModel;

class ComposerScheduler : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerScheduler(const ComposerModel *model, QObject *parent = NULL);

        void start();

        void end();

        void setSettings(const ExecutorSettings &settings);

        const ExecutorSettings &getSettings() const;

    signals:
        void ended();

        void executorStarted(const Node *node);

        void executorProgress(const Node *node, qreal progress);

        void executorEnded(const Node *node,
                           const Properties &outputs,
                           const Properties &inputs,
                           qint64 duration,
                           const QString &error);

    private slots:
        void onNodeAdded(const Node *node);

        void onNodeRemoved(const Node *node);

        void onNodePropertyChanged();

        void onConnectionRemoved(const Connection *connection);

        void onConnectionAdded(const Connection *connection);

        void onNodeProcessed();

        void onProgress(qreal progress);

    private:
        bool allInputsProcessed(const Node *node);

        void makeInputs(const Node *node, Properties &inputs);

        quint16 maxThreads() const;

        void processNexts();

        void reProcessFromNode(const Node *node);

        void reProcessAll();

        void invalidateFromNode(const Node *node);

        void invalidateExecutors(const Node *node);

        void clearUnusedCache();

        void executorAborted(const Node *node, const QString &error = "");

    private:
        ExecutorSettings _settings;
        QList<ComposerExecutor *> _currentExecutors;
        QList<ComposerExecutor *> _oldExecutors;
        QList<const Node *> _keepProcessingNodes;
        const ComposerModel *_model;
        QMap<const Node *, Properties> _processedNodes;
        bool _end;
};

#endif // COMPOSERSCHEDULER_H
