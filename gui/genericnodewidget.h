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

#ifndef GenericNodeWidget_H
#define GenericNodeWidget_H

#include <QWidget>

#include <QLabel>
#include <QSignalMapper>
#include <QDomDocument>
#include <QDomNode>

#include "model/plugdefinition.h"

class AbstractPlugWidget;
class Plug;

class GenericNodeWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit GenericNodeWidget(QWidget *parent = NULL);

        void setPlugs(const QList<Plug *> &inputs, const QList<Plug *> &outputs);

        int getPlugPosY(const QString &plugName);

        void setInputPlugged(const QString &inputName, bool plugged);

        void setPlugProperty(const QString &name, const QVariant &value);

        const AbstractPlugWidget *getWidget(const QString &name) const;

        AbstractPlugWidget *accessWidget(const QString &name);

    public slots:
        void onProcessDone(const Properties &outputs, const Properties &inputs);

        void onProcessUnavailable();

    signals:
        void propertyChanged(const QString &name, const QVariant &value);

        void sizeHintChanged();

    private:
        typedef struct
        {
            QLabel *label;
            AbstractPlugWidget *widget;
            PlugDefinition definition;
        } PlugWidget;

    private:
        AbstractPlugWidget *makePlugWidget(const PlugDefinition &plug);

        void makeLabelText(const PlugWidget &widget, bool plugged);

    private slots:
        void onWidgetValueChanged(const QString &propertyName);

        void onPlugSizeHintChanged();

    private:
        QMap<QString, PlugWidget> _widgets;
        QSignalMapper *_widgetsMapper;
};

#endif // GenericNodeWidget_H
