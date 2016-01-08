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

#include "connection.h"

class AbstractNode;
class ComposerExecutor;

class ComposerScheduler : public QObject
{
    Q_OBJECT

    public:
        explicit ComposerScheduler(QObject *parent = NULL);

        void execute(const QList<AbstractNode *> &nodes,
                     const QList<Connection> &connections);

    private slots:
        void onNodeProcessed(const QList<cv::Mat> &outputs);

    private:
        ComposerExecutor *_executor;
        QQueue<QPair<AbstractNode *, QList<AbstractNode *> > > _executionList;
        QMap<AbstractNode *, QList<cv::Mat> > _processedNodes;
};

#endif // COMPOSERSCHEDULER_H
