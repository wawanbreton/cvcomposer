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

#ifndef PLUG_H
#define PLUG_H

#include <QObject>

#include "model/plugdefinition.h"

class Plug : public QObject
{
    Q_OBJECT

    public:
        explicit Plug(const PlugDefinition &definition, QObject *parent = NULL);

        const PlugDefinition &getDefinition() const;

        void signalConnectedTo(const Plug *connectedTo);

        QString save(const QVariant &value) const;

        QVariant load(const QString &value) const;

    signals:
        void connectionChanged(const Plug *connectedTo);

    private:
        static QString saveDouble(double value);

        static double loadDouble(const QString &valueStr, bool *ok);

    private:
        const PlugDefinition _definition;
};


#endif // PLUG_H

