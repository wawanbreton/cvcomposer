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

#ifndef GENERICNODE_H
#define GENERICNODE_H

#include <QObject>

#include "plug.h"

class GenericNode : public QObject
{
    Q_OBJECT

    public:
        explicit GenericNode(const QString &name,
                             const QString &userReadableName,
                             quint8 nbInputs,
                             quint8 nbOutputs,
                             QObject *parent = NULL);

        const QString &getName() const;

        const QString &getUserReadableName() const;

        quint8 getNbInputs() const;

        const QList<Plug *> &getInputs() const;

        bool hasInput(Plug *input) const;

        quint8 getNbOutputs() const;

        const QList<Plug *> &getOutputs() const;

        bool hasOutput(Plug *output) const;

    signals:
        void propertyChanged(const QString &propertyName, const QVariant &value);

    protected:
        virtual bool event(QEvent *event);

    private:
        const QString _name;
        const QString _userReadableName;
        QList<Plug *> _inputs;
        QList<Plug *> _outputs;
};

#endif // GENERICNODE_H
