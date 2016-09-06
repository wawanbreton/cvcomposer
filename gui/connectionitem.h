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

#include <QGraphicsItemGroup>

#include <QGraphicsPathItem>

class Connection;

class ConnectionItem : public QGraphicsItemGroup
{
    public:
        ConnectionItem(QGraphicsItem *parent = NULL);

        QPointF getOutput() const;

        void setOutput(const QPointF &output);

        void setInput(const QPointF &input);

        void setConnection(const Connection *connection);

        const Connection *getConnection() const;

    private:
        void updateLine();

    private:
        static const int _deltaCenter = 3;

    private:
        const Connection *_connection;
        QGraphicsPathItem *_itemConnectorOutput;
        QGraphicsPathItem *_itemConnectorInput;
        QGraphicsPathItem *_itemLine;
};
