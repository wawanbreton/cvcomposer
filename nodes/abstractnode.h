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

#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QObject>

#include <QMap>

#include <opencv2/core/core.hpp>

#include "plug.h"

class AbstractNode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractNode(quint8 nbInputs,
                              quint8 nbOutputs,
                              const QString &userReadableName,
                              QObject *parent = NULL);

        quint8 getNbInputs() const;

        const QList<Plug *> &getInputs() const;

        bool hasInput(Plug *input) const;

        quint8 getNbOutputs() const;

        const QList<Plug *> &getOutputs() const;

        bool hasOutput(Plug *output) const;

        const QString &getUserReadableName() const;

        QList<cv::Mat> process(const QList<cv::Mat> &inputs);

        void signalProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void signalProcessUnavailable();

    protected:
        virtual QList<cv::Mat> processImpl(const QList<cv::Mat> &inputs) = 0;

    signals:
        void processDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void processUnavailable();

    private:
        QList<Plug *> _inputs;
        QList<Plug *> _outputs;
        QString _userReadableName;
};

#endif // ABSTRACTNODE_H
