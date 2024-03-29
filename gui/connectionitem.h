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
#include <QGraphicsItemGroup>

#include <QGraphicsPathItem>

#include "model/plugtype.h"

class Connection;

class ConnectionItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    public:
        ConnectionItem(QGraphicsItem *parent = nullptr);

        QPointF getOutput() const;

        void setOutput(const QPointF &output);

        void setInput(const QPointF &input);

        void setConnection(const Connection *connection);

        const Connection *getConnection() const;

        void setCurrentType(PlugType::Enum type, bool immediate = false);

        void copyColorFrom(const ConnectionItem *other);

    private:
        void updateLine();

        void onColorChanged(const QVariant &value);

    private:
        static const int _deltaCenter = 3;

    private:
        const Connection *_connection{nullptr};
        QGraphicsPathItem *const _itemConnectorOutput;
        QGraphicsPathItem *const _itemConnectorInput;
        QGraphicsPathItem *const _itemLine;
};
