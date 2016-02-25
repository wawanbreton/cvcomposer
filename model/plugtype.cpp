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


PlugType::Pluggable PlugType::isInputPluggable(PlugType::Enum value)
{
    switch(value)
    {
        case PlugType::Image:
        case PlugType::Kernel:
            return Mandatory;
        case PlugType::Size:
        case PlugType::Point:
        case PlugType::Enumeration:
        case PlugType::ImagePath:
        case PlugType::KernelDefinition:
        case PlugType::Double:
        case PlugType::String:
        case PlugType::Color:
        case PlugType::DockableImageViewer:
            return ManualOnly;
        case PlugType::ImagePreview:
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

PlugType::Enum PlugType::getCompatibility(Enum value)
{
    switch(value)
    {
        case PlugType::ImagePreview:
            return Image;

        default:
            return value;
    }

    return Image;
}
