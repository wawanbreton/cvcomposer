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

#include "global/properties.h"
#include "model/plugdefinition.h"
#include "processor/helpmessage.h"

#include <opencv2/core/core.hpp>

class Plug;

class Node : public QObject
{
    Q_OBJECT

    public:
        explicit Node(const QString &name,
                      const QString &userReadableName,
                      QObject *parent = NULL);

        const QString &getName() const;

        const QString &getUserReadableName() const;

        const QList<Plug *> &getInputs() const;

        bool hasInput(const Plug *input) const;

        Plug *findInput(const QString &name) const;

        const QList<Plug *> &getOutputs() const;

        bool hasOutput(const Plug *output) const;

        Plug *findOutput(const QString &name) const;

        const Properties &getProperties() const;

        const QList<HelpMessage> &getHelpMessages() const { return _helpMessages; }

    public slots:
        void setProperty(const QString &name, const QVariant &value);

    signals:
        void propertyChanged(const QString &name, const QVariant &value);

    private:
        const QString _name;
        const QString _userReadableName;
        QList<Plug *> _inputs;
        QList<Plug *> _outputs;
        Properties _properties;
        QList<HelpMessage> _helpMessages;
};
