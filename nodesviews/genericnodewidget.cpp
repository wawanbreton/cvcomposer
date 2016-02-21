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
#include "plugwidgets/sizewidget.h"
#include "plugwidgets/pointwidget.h"
#include "plugwidgets/enumerationwidget.h"
#include "plugwidgets/kerneldefinitionwidget.h"
#include "plugwidgets/doublewidget.h"
#include "plugwidgets/stringwidget.h"
#include "plugwidgets/colorwidget.h"
#include "plugwidgets/dockableimageviewerwidget.h"


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
        outputsLayout->setContentsMargins(0, 0, 0, 0);

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
        inputsLayout->setContentsMargins(0, 0, 0, 0);

        foreach(const Plug *input, inputs)
        {
            PlugWidget widget;
            widget.input = true;
            widget.definition = input->getDefinition();

            widget.label = NULL;
            if(PlugType::isLabelVisible(widget.definition.type))
            {
                widget.label = new QLabel(this);
                widget.label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                makeLabelText(widget, false);
            }

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
        QRect rect;
        if(iterator.value().label)
        {
            rect = rect.united(iterator.value().label->geometry());
        }
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
        if(widget.input && widget.widget)
        {
            widget.widget->onNodeProcessed(inputs, outputs);
        }
    }
}

void GenericNodeWidget::onProcessUnavailable()
{
    foreach(const PlugWidget &widget, _widgets)
    {
        if(widget.input && widget.widget)
        {
            widget.widget->onNodeProcessed(Properties(), Properties());
        }
    }
}

AbstractPlugWidget *GenericNodeWidget::makePlugWidget(const PlugDefinition &plug)
{
    AbstractPlugWidget *widget = NULL;

    switch(plug.type)
    {
        case PlugType::Size:
            widget = new SizeWidget(plug.widgetProperties, this);
            break;
        case PlugType::Point:
            widget = new PointWidget(plug.widgetProperties, this);
            break;
        case PlugType::Enumeration:
            widget = new EnumerationWidget(plug.widgetProperties, this);
            break;
        case PlugType::Double:
            widget = new DoubleWidget(plug.widgetProperties, this);
            break;
        case PlugType::String:
            widget = new StringWidget(plug.widgetProperties, this);
            break;
        case PlugType::Color:
            widget = new ColorWidget(this);
            break;
        case PlugType::KernelDefinition:
            widget = new KernelDefinitionWidget(plug.widgetProperties, this);
            break;
        case PlugType::ImagePath:
            widget = new ImagePathWidget(this);
            break;
        case PlugType::ImagePreview:
            widget = new ImagePreviewWidget(this);
            break;
        case PlugType::DockableImageViewer:
            widget = new DockableImageViewerWidget(this);
            break;
        case PlugType::Kernel:
        case PlugType::Image:
            qCritical() << "GenericNodeWidget::makePlugWidget"
                        << "plug type" << plug.type
                        << "is not supposed to be associated to a widget";
            break;
    }

    if(widget)
    {
        _widgetsMapper->setMapping(widget, plug.name);
        connect(widget, SIGNAL(valueChanged()), _widgetsMapper, SLOT(map()));
        connect(widget, SIGNAL(sizeHintChanged()), SLOT(onPlugSizeHintChanged()));
        if(not plug.defaultValue.isNull())
        {
            widget->setValue(plug.defaultValue);
        }
    }

    return widget;
}

void GenericNodeWidget::makeLabelText(const PlugWidget &widget, bool plugged)
{
    if(widget.label)
    {
        QString text = widget.definition.name;
        text = text[0].toUpper() + text.mid(1);

        if((plugged || PlugType::isInputPluggable(widget.definition.type) == PlugType::Mandatory) &&
           not PlugType::isWidgetAlwaysVisible(widget.definition.type))
        {
            widget.label->setText(text);
        }
        else
        {
            widget.label->setText(text + " :");
        }
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

void GenericNodeWidget::onPlugSizeHintChanged()
{
    layout()->activate();
    resize(sizeHint());

    emit sizeHintChanged();
}
