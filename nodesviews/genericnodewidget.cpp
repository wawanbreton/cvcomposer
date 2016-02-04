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
#include <QFileDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

#include "plug.h"
#include "plugwidgets/imagepathwidget.h"
#include "plugwidgets/imagepreviewwidget.h"


GenericNodeWidget::GenericNodeWidget(QWidget *parent) :
    QWidget(parent),
    _widgets(),
    _widgetsMapper(new QSignalMapper(this))
{
    connect(_widgetsMapper, SIGNAL(mapped(QString)), SLOT(onWidgetValueChanged(QString)));
}

void GenericNodeWidget::setPlugs(const QList<Plug*> &inputs,
                                 const QList<Plug*> &outputs)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    if(outputs.count())
    {
        QVBoxLayout *outputsLayout = new QVBoxLayout();

        foreach(const Plug *output, outputs)
        {
            PlugWidget widget;
            widget.input = false;
            widget.definition = output->getDefinition();

            widget.label = new QLabel(this);
            widget.label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            makeLabelText(widget, true);

            widget.widget = NULL;

            _widgets[widget.definition.name] = widget;

            outputsLayout->addWidget(widget.label);
        }

        mainLayout->addLayout(outputsLayout);
    }

    if(inputs.count())
    {
        QFormLayout *inputsLayout = new QFormLayout();

        foreach(const Plug *input, inputs)
        {
            PlugWidget widget;
            widget.input = true;
            widget.definition = input->getDefinition();

            widget.label = new QLabel(this);
            widget.label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            makeLabelText(widget, false);

            widget.widget = NULL;
            if(PlugType::isInputPluggable(widget.definition.type) != PlugType::Mandatory ||
               PlugType::isWidgetAlwaysVisible(widget.definition.type))
            {
                widget.widget = makePlugWidget(widget.definition);
            }

            _widgets[widget.definition.name] = widget;
            inputsLayout->addRow(widget.label, widget.widget);
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
        QRect rect = iterator.value().label->geometry();
        if(iterator.value().widget)
        {
            rect = rect.united(iterator.value().widget->geometry());
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
    const PlugWidget &widget = _widgets[inputName];
    if(widget.widget)
    {
        widget.widget->setVisible(PlugType::isWidgetAlwaysVisible(widget.definition.type) ||
                                  not plugged);
    }
    makeLabelText(widget, plugged);
}

void GenericNodeWidget::onProcessDone(const Properties &outputs, const Properties &inputs)
{
    foreach(const PlugWidget &widget, _widgets)
    {
        if(widget.input)
        {
            widget.widget->onConnectedInputProcessed(inputs[widget.definition.name]);
        }
    }
}

AbstractPlugWidget *GenericNodeWidget::makePlugWidget(const PlugDefinition &plug)
{
    AbstractPlugWidget *widget = NULL;

    switch(plug.type)
    {
        /*case PlugType::Size:
        {
            widget = new QWidget(this);

            QSpinBox *spinBoxWidth = new QSpinBox(widget);
            QSpinBox *spinBoxHeight = new QSpinBox(widget);
            QLabel *label = new QLabel(widget);
            label->setText("x");

            QHBoxLayout *layout = new QHBoxLayout(widget);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(spinBoxWidth);
            layout->addWidget(label);
            layout->addWidget(spinBoxHeight);

            break;
        }*/
        case PlugType::ImagePath:
            widget = new ImagePathWidget(this);
            break;
        case PlugType::ImagePreview:
            widget = new ImagePreviewWidget(this);
            break;
    }

    if(widget)
    {
        _widgetsMapper->setMapping(widget, plug.name);
        connect(widget, SIGNAL(valueChanged()), _widgetsMapper, SLOT(map()));
    }

    return widget;
}

void GenericNodeWidget::makeLabelText(const PlugWidget &widget, bool plugged)
{
    QString text = widget.definition.userReadableName;
    text = text[0].toUpper() + text.mid(1);

    if(plugged || PlugType::isInputPluggable(widget.definition.type) == PlugType::Mandatory)
    {
        widget.label->setText(text);
    }
    else
    {
        widget.label->setText(text + " :");
    }
}

void GenericNodeWidget::onWidgetValueChanged(const QString &propertyName)
{
    QObject *widget = _widgetsMapper->mapping(propertyName);
    AbstractPlugWidget *plugWidget = qobject_cast<AbstractPlugWidget *>(widget);
    if(plugWidget)
    {
        emit propertyChanged(propertyName, plugWidget->getValue());
    }
    else
    {
        qCritical() << "GenericNodeWidget::onWidgetValueChanged"
                    << "Unable to find the AbstractPlugWidget for property" << propertyName;
    }
}
