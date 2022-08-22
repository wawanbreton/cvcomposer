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

#include "plugtype.h"

#include <QDebug>
#include <QMetaEnum>

#include "global/cvutils.h"


PlugType::Pluggable PlugType::isInputPluggable(PlugType::PlugTypes inputTypes)
{
    if(isSingleType(inputTypes))
    {
        switch(flagsToEnum(inputTypes))
        {
            case PlugType::Double:
                return Free;
            case PlugType::Size:
            case PlugType::Point:
            case PlugType::Enumeration:
            case PlugType::Path:
            case PlugType::KernelDefinition:
            case PlugType::String:
            case PlugType::Color:
            case PlugType::DockableImageViewer:
            case PlugType::Boolean:
                return ManualOnly;
            case PlugType::Image:
            case PlugType::Kernel:
            case PlugType::ImagePreview:
            case PlugType::Rectangle:
            case PlugType::Circle:
            case PlugType::Contour:
            case PlugType::Segment:
            case PlugType::Ellipse:
            case PlugType::RotatedRectangle:
            case PlugType::Triangle:
            case PlugType::Line:
                return Mandatory;
        }
    }

    return Mandatory;
}

bool PlugType::isWidgetAlwaysVisible(PlugType::PlugTypes types)
{
    return isSingleType(types) &&
           (flagsToEnum(types) == PlugType::ImagePreview ||
            flagsToEnum(types) == PlugType::DockableImageViewer);
}

bool PlugType::isLabelVisible(PlugType::PlugTypes types)
{
    return !isSingleType(types) ||
           (flagsToEnum(types) != PlugType::ImagePreview &&
            flagsToEnum(types) != PlugType::KernelDefinition);
}

bool PlugType::isInputSavable(PlugType::PlugTypes inputTypes)
{
    if(isSingleType(inputTypes))
    {
        if(flagsToEnum(inputTypes) == PlugType::DockableImageViewer)
        {
            return false;
        }
        else
        {
            return PlugType::isInputPluggable(inputTypes) != PlugType::Mandatory;
        }
    }
    else
    {
        return false;
    }
}

bool PlugType::isOutputInternal(PlugType::PlugTypes types)
{
    if(isSingleType(types))
    {
        Enum type = flagsToEnum(types);
        return type == ImagePreview || type == DockableImageViewer;
    }
    return false;
}

QColor PlugType::getColor(PlugType::Enum value)
{
    switch(value)
    {
        case PlugType::Image:
            return QColor(46, 204, 113);
        case PlugType::Kernel:
            return QColor(52, 152, 219);
        case PlugType::Rectangle:
            return QColor(142, 68, 173);
        case PlugType::Double:
            return QColor(230, 126, 34);
        case PlugType::Circle:
            return QColor(85, 110, 134);
        case PlugType::Contour:
            return QColor(241, 196, 15);
        case PlugType::Segment:
            return QColor(156, 80, 6);
        case PlugType::Ellipse:
            return QColor(255, 79, 70);
        case PlugType::RotatedRectangle:
            return QColor(69, 33, 85);
        case PlugType::Triangle:
            return QColor(255, 170, 255);
        case PlugType::Line:
            return QColor(49, 191, 182);
        case PlugType::Size:
        case PlugType::Point:
        case PlugType::Enumeration:
        case PlugType::String:
        case PlugType::Color:
        case PlugType::KernelDefinition:
        case PlugType::Path:
        case PlugType::ImagePreview:
        case PlugType::DockableImageViewer:
        case PlugType::Boolean:
            qCritical() << "Plug type" << value << "has no defined color";
            break;
    }

    return Qt::white;
}

QString PlugType::toString(PlugType::Enum value)
{
    switch(value)
    {
        case PlugType::Size:
            return "Size";
        case PlugType::Point:
            return "Point";
        case PlugType::Enumeration:
            return "Enumeration";
        case PlugType::Path:
            return "Path";
        case PlugType::KernelDefinition:
            return "Size";
        case PlugType::Double:
            return "Numeric value";
        case PlugType::String:
            return "Text";
        case PlugType::Color:
            return "Color";
        case PlugType::DockableImageViewer:
            return "Dockable image viewer";
        case PlugType::Boolean:
            return "Yes/No";
        case PlugType::Image:
            return "Image";
        case PlugType::Kernel:
            return "Kernel";
        case PlugType::ImagePreview:
            return "Image preview";
        case PlugType::Rectangle:
            return "Rectangle";
        case PlugType::Circle:
            return "Circle";
        case PlugType::Contour:
            return "Contour";
        case PlugType::Segment:
            return "Segment";
        case PlugType::Ellipse:
            return "Ellipse";
        case PlugType::RotatedRectangle:
            return "Rotated rectangle";
        case PlugType::Triangle:
            return "Triangle";
        case PlugType::Line:
            return "Line";
    }

    return "";
}

bool PlugType::isSingleType(PlugTypes types)
{
    return (types & (static_cast<int>(types) - 1)) == 0;
}


PlugType::Enum PlugType::flagsToEnum(PlugTypes types)
{
    return Enum(int(types));
}

PlugType::PlugTypes PlugType::getAllFlags()
{
    return fromList(getAllValues());
}

QList<PlugType::Enum> PlugType::getAllValues()
{
    QList<PlugType::Enum> result;
    QMetaEnum me = PlugType::staticMetaObject.enumerator(0);
    for(int i = 0 ; i < me.keyCount() ; i++)
    {
        result << (PlugType::Enum)me.value(i);
    }
    return result;
}

QList<PlugType::Enum> PlugType::toList(PlugTypes types)
{
    QList<PlugType::Enum> result;
    foreach(PlugType::Enum type, PlugType::getAllValues())
    {
        if(types.testFlag(type))
        {
            result << type;
        }
    }
    return result;
}

PlugType::PlugTypes PlugType::fromList(const QList<PlugType::Enum> &types)
{
    PlugType::PlugTypes result;
    foreach(const PlugType::Enum &type, types)
    {
        result |= type;
    }
    return result;
}
