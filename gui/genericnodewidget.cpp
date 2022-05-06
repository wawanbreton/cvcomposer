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

#include "model/plug.h"
#include "plugwidget/pathwidget.h"
#include "plugwidget/dataviewerwidget.h"
#include "plugwidget/sizewidget.h"
#include "plugwidget/pointwidget.h"
#include "plugwidget/enumerationwidget.h"
#include "plugwidget/kerneldefinitionwidget.h"
#include "plugwidget/doublewidget.h"
#include "plugwidget/stringwidget.h"
#include "plugwidget/colorwidget.h"
#include "plugwidget/imageviewerwidget.h"
#include "plugwidget/booleanwidget.h"


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
            widget.definition = output->getDefinition();

            if(PlugType::isOutputInternal(widget.definition.types))
            {
                widget.label = NULL;

                widget.widget = makePlugWidget(widget.definition);

                outputsLayout->addWidget(widget.widget);
            }
            else
            {
                widget.label = new QLabel(this);
                widget.label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                makeLabelText(widget);

                widget.widget = NULL;

                outputsLayout->addWidget(widget.label);
            }

            _widgets[widget.definition.name] = widget;

        }

        mainLayout->addLayout(outputsLayout);
    }

    if(inputs.count())
    {
        bool hasLabel = false;
        bool hasWidget = false;

        // Iterate once to create the widgets
        foreach(const Plug *input, inputs)
        {
            PlugWidget widget;
            widget.definition = input->getDefinition();

            widget.label = NULL;

            bool labelVisible = PlugType::isLabelVisible(widget.definition.types);
            if(widget.definition.labelVisible != ThreeStateBool::None)
            {
                labelVisible = bool(widget.definition.labelVisible);
            }

            if(labelVisible)
            {
                widget.label = new QLabel(this);
                widget.label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                makeLabelText(widget);
                hasLabel = true;
            }

            widget.widget = NULL;
            if(PlugType::isInputPluggable(widget.definition.types) != PlugType::Mandatory ||
               PlugType::isWidgetAlwaysVisible(widget.definition.types))
            {
                widget.widget = makePlugWidget(widget.definition);
                hasWidget = true;
            }

            _widgets[widget.definition.name] = widget;
        }

        // Now we know whether we have labels and/or widget, so we can create the appropriate layout
        QLayout *inputsLayout = NULL;
        if(hasLabel && hasWidget)
        {
            QFormLayout *formLayout = new QFormLayout();

            foreach(const Plug *input, inputs)
            {
                PlugWidget &widget = _widgets[input->getDefinition().name];
                formLayout->addRow(widget.label, widget.widget);
            }

            inputsLayout = formLayout;
        }
        else if(hasWidget)
        {
            QVBoxLayout *vboxLayout = new QVBoxLayout();

            foreach(const Plug *input, inputs)
            {
                PlugWidget &widget = _widgets[input->getDefinition().name];
                if(widget.widget)
                {
                    vboxLayout->addWidget(widget.widget);
                }
            }

            inputsLayout = vboxLayout;
        }
        else if(hasLabel)
        {
            QVBoxLayout *vboxLayout = new QVBoxLayout();

            foreach(const Plug *input, inputs)
            {
                PlugWidget &widget = _widgets[input->getDefinition().name];
                if(widget.label)
                {
                    vboxLayout->addWidget(widget.label);
                }
            }

            inputsLayout = vboxLayout;
        }

        if(inputsLayout)
        {
            inputsLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->addLayout(inputsLayout);
        }
    }

    setLayout(mainLayout);
}

int GenericNodeWidget::getPlugPosY(const QString &plugName)
{
    auto iterator = _widgets.find(plugName);
    if(iterator != _widgets.end())
    {
        QRect rect;
        if(iterator.value().label == NULL && iterator.value().widget == NULL)
        {
            return height() / 2;
        }

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
        widget.widget->setVisible(PlugType::isWidgetAlwaysVisible(widget.definition.types) ||
                                  not plugged);
    }
}

void GenericNodeWidget::setPlugProperty(const QString &name, const QVariant &value)
{
    const PlugWidget &widget = _widgets[name];
    if(widget.widget)
    {
        widget.widget->setValue(value);
    }
}

const AbstractPlugWidget *GenericNodeWidget::getWidget(const QString &name) const
{
    auto iterator = _widgets.constFind(name);
    if(iterator != _widgets.constEnd())
    {
        return iterator.value().widget;
    }

    return NULL;
}

AbstractPlugWidget *GenericNodeWidget::accessWidget(const QString &name)
{
    return (AbstractPlugWidget *)getWidget(name);
}

void GenericNodeWidget::onProcessDone(const Properties &outputs, const Properties &inputs)
{
    foreach(const PlugWidget &widget, _widgets)
    {
        if(widget.widget)
        {
            widget.widget->onNodeProcessed(inputs, outputs);
        }
    }
}

void GenericNodeWidget::onProcessUnavailable()
{
    foreach(const PlugWidget &widget, _widgets)
    {
        if(widget.widget)
        {
            widget.widget->onNodeProcessed(Properties(), Properties());
        }
    }
}

AbstractPlugWidget *GenericNodeWidget::makePlugWidget(const PlugDefinition &plug)
{
    AbstractPlugWidget *widget = NULL;

    if(PlugType::isSingleType(plug.types))
    {
        switch(PlugType::flagsToEnum(plug.types))
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
            case PlugType::Path:
                widget = new PathWidget(plug.widgetProperties, this);
                break;
            case PlugType::ImagePreview:
                widget = new DataViewerWidget(this);
                break;
            case PlugType::DockableImageViewer:
                widget = new ImageViewerWidget(this);
                break;
            case PlugType::Boolean:
                widget = new BooleanWidget(this);
                break;
            case PlugType::Kernel:
            case PlugType::Image:
            case PlugType::Rectangle:
            case PlugType::Circle:
            case PlugType::Contour:
            case PlugType::Line:
            case PlugType::Ellipse:
            case PlugType::RotatedRectangle:
            case PlugType::Triangle:
                qCritical() << "plug" << plug.name
                            << "is not supposed to be associated to a widget"
                            << "because it is of type" << plug.types;
                break;
        }
    }
    else
    {
        qCritical() << "plug" << plug.name
                    << "is not supposed to be associated to a widget because it is of multiple";
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

        QSizePolicy sizePolicy = widget->sizePolicy();
        sizePolicy.setRetainSizeWhenHidden(true);
        widget->setSizePolicy(sizePolicy);
    }

    return widget;
}

void GenericNodeWidget::makeLabelText(const PlugWidget &widget)
{
    if(widget.label)
    {
        QString text = widget.definition.name;
        text = text[0].toUpper() + text.mid(1);
        widget.label->setText(text);
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
    emit sizeHintChanged();
}
