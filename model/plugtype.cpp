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

#include "global/cvutils.h"


PlugType::Pluggable PlugType::isInputPluggable(PlugType::Enum value)
{
    switch(value)
    {
        case PlugType::Size:
        case PlugType::Point:
        case PlugType::Enumeration:
        case PlugType::Path:
        case PlugType::KernelDefinition:
        case PlugType::Double:
        case PlugType::String:
        case PlugType::Color:
        case PlugType::DockableImageViewer:
        case PlugType::Boolean:
            return ManualOnly;
        case PlugType::Generic:
        case PlugType::Image:
        case PlugType::Kernel:
        case PlugType::ImagePreview:
        case PlugType::Rectangle:
        case PlugType::Circle:
        case PlugType::Contour:
        case PlugType::Line:
            return Mandatory;
    }

    return Mandatory;
}

bool PlugType::isWidgetAlwaysVisible(PlugType::Enum value)
{
    return value == PlugType::ImagePreview || value == PlugType::DockableImageViewer;
}

bool PlugType::isLabelVisible(PlugType::Enum value)
{
    return value != PlugType::ImagePreview && value != PlugType::KernelDefinition;
}

bool PlugType::isInputSavable(PlugType::Enum value)
{
    if(value == PlugType::DockableImageViewer)
    {
        return false;
    }
    else
    {
        return PlugType::isInputPluggable(value) != PlugType::Mandatory;
    }
}

bool PlugType::isOutputInternal(PlugType::Enum value)
{
    return value == PlugType::ImagePreview;
}

bool PlugType::isCompatible(PlugType::Enum output, PlugType::Enum input)
{
    if(input == Generic || output == Generic)
    {
        return true;
    }
    else
    {
        return output == input;
    }
}
