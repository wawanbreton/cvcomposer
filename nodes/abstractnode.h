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

#include <opencv2/core/core.hpp>

class AbstractNode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractNode(quint8 nbInputs, quint8 nbOutputs, const QString &userReadableName, QObject *parent = NULL);

        quint8 getNbInputs() const;

        quint8 getNbOutputs() const;

        const QString &getUserReadableName() const;

        QList<cv::Mat> process(const QList<cv::Mat> &inputs);

    protected:
        virtual QList<cv::Mat> processImpl(const QList<cv::Mat> &inputs) = 0;

    signals:
        void processDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

    private:
        const quint8 _nbInputs;
        QList<AbstractNode *> _outputs;
        QString _userReadableName;
};

#endif // ABSTRACTNODE_H
