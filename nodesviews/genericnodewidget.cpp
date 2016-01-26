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

#include "genericnodewidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QFormLayout>


GenericNodeWidget::GenericNodeWidget(QWidget *parent) :
    QWidget(parent),
    _widgets()
{
}

void GenericNodeWidget::setPlugs(const QList<PlugDefinition> &inputs,
                                 const QList<PlugDefinition> &outputs)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    if(outputs.count())
    {
        QVBoxLayout *outputsLayout = new QVBoxLayout(this);

        foreach(const PlugDefinition &output, outputs)
        {
            QLabel *label = new QLabel(this);
            label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            label->setText(output.userReadableName[0].toUpper() + output.userReadableName.mid(1));

            _widgets[output.name] = QPair<QLabel *, QWidget *>(label, NULL);

            outputsLayout->addWidget(label);
        }

        mainLayout->addLayout(outputsLayout);
    }

    if(inputs.count())
    {
        QFormLayout *inputsLayout = new QFormLayout(this);

        foreach(const PlugDefinition &input, inputs)
        {
            QLabel *label = new QLabel(this);
            label->setText(input.userReadableName[0].toUpper() + input.userReadableName.mid(1) + " :");
            label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            QWidget *editWidget = makePlugWidget(input);

            _widgets[input.name] = qMakePair(label, editWidget);

            inputsLayout->addRow(label, editWidget);
        }

        mainLayout->addLayout(inputsLayout);
    }

    setLayout(mainLayout);
}

int GenericNodeWidget::getPlugPosY(const QString &plugName)
{
    auto iterator = _widgets.find(plugName);
    if(iterator != _widgets.end())
    {
        QRect rect = iterator.value().first->geometry();
        if(iterator.value().second)
        {
            rect = rect.united(iterator.value().second->geometry());
        }

        return rect.center().y();
    }
    else
    {
        qCritical() << "No widget for plug" << plugName;
        return 0;
    }
}

void GenericNodeWidget::setInputPlugged(const QString &inputName, bool plugged)
{

}

QWidget *GenericNodeWidget::makePlugWidget(const PlugDefinition &plug)
{
    return NULL;
}

