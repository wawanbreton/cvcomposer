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

#include <opencv2/core/core.hpp>

#include <QMap>
#include <QQueue>
#include <QSharedPointer>

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
        explicit ComposerScheduler(const ComposerModel *model, QObject *parent = nullptr);

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

        void nodeInvalid(const Node *node);

    private:
        void onNodeAdded(const Node *node, bool processNow);

        void onNodeAdded(const Node *node)
        { onNodeAdded(node, true); }

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

        void invalidateFromNode(const Node *node, bool includeCurrent = true);

        void cancelExecutors(const Node *node);

        void cancelExecutor(ComposerExecutor *executor);

        void clearUnusedCache();

    private:
        ExecutorSettings _settings;
        QList<ComposerExecutor *> _currentExecutors;
        QList<ComposerExecutor *> _oldExecutors;
        const ComposerModel *const _model;
        QMap<const Node *, QSharedPointer<AbstractProcessor>> _processors;
        QMap<const Node *, Properties> _processedNodes;
        bool _end;
};
