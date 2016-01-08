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

#ifndef COMPOSEREXECUTOR_H
#define COMPOSEREXECUTOR_H

#include <QThread>

#include <QQueue>
#include <QMutex>

#include <opencv2/core/core.hpp>

class AbstractNode;

class ComposerExecutor : public QThread
{
    Q_OBJECT

    public:
        explicit ComposerExecutor(QObject *parent = NULL);

        void processNode(AbstractNode *node, const QList<cv::Mat> &inputs);

    signals:
        void nodeProcessed(const QList<cv::Mat> &outputs);

    protected:
        virtual void run();

    private slots:
        void onFinished();

    private:
        typedef struct
        {
            AbstractNode *node;
            QList<cv::Mat> inputs;
            QList<cv::Mat> outputs;
        } Job;

    private:
        Job _job;
};

#endif // COMPOSEREXECUTOR_H
