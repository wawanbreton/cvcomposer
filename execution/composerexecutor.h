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

#include <QThread>

#include <QQueue>
#include <QMutex>
#include <QSharedPointer>

#include <opencv2/core/core.hpp>

#include "global/properties.h"

class Node;
class AbstractProcessor;

class ComposerExecutor : public QThread
{
    Q_OBJECT

    public:
        explicit ComposerExecutor(const Node *node,
                                  const Properties &inputs,
                                  const QSharedPointer<AbstractProcessor> &processor,
                                  QObject *parent = NULL);

        void process();

        const Node *getNode() { return _node; }

        const Properties &getInputs() const { return _inputs; }

        const Properties &getOutputs() const { return _outputs; }

        qint64 getDuration() const { return _duration; }

        const QString &getError() const { return _error; }

    signals:
        void executionProgress(qreal progress);

        void nodeProcessed();

    protected:
        virtual void run();

    private:
        void onFinished();

    private:
        const Node *_node{Q_NULLPTR};
        const QSharedPointer<AbstractProcessor> _processor;
        const Properties _inputs;
        Properties _outputs;
        qint64 _duration{0};
        QString _error;
};
