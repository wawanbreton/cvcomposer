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

#pragma once

#include <QColor>

namespace PlugType
{
    typedef enum
    {
        Generic,
        Image,
        Size,
        Point,
        Enumeration,
        Double,
        String,
        Color,
        Kernel,
        KernelDefinition,
        Path,
        ImagePreview,
        DockableImageViewer,
        Rectangle,
        Circle,
        Boolean,
        Contour,
        Line,
        Ellipse
    } Enum;

    typedef enum
    {
        Mandatory, // Plug has to be connected, it can't be configured manually
        Free,      // Plug may be configured manually, or connected
        ManualOnly // Plug can only be manually configured
    } Pluggable;

    Pluggable isInputPluggable(Enum value);

    bool isOutputInternal(Enum value);

    bool isWidgetAlwaysVisible(Enum value);

    bool isLabelVisible(Enum value);

    bool isCompatible(Enum output, Enum input);

    bool isInputSavable(Enum value);

    QColor getColor(Enum value);
}
