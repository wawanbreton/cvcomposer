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

#include <QObject>

#include <QColor>

class PlugType : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        typedef enum
        {
            Image               = 0x00001,
            Size                = 0x00002,
            Point               = 0x00004,
            Enumeration         = 0x00008,
            Double              = 0x00010,
            String              = 0x00020,
            Color               = 0x00040,
            Kernel              = 0x00080,
            KernelDefinition    = 0x00100,
            Path                = 0x00200,
            ImagePreview        = 0x00400,
            DockableImageViewer = 0x00800,
            Rectangle           = 0x01000,
            Circle              = 0x02000,
            Boolean             = 0x04000,
            Contour             = 0x08000,
            Line                = 0x10000,
            Ellipse             = 0x20000,
            RotatedRectangle    = 0x40000,
            Triangle            = 0x80000,
        } Enum;


        typedef enum
        {
            Mandatory, // Plug has to be connected, it can't be configured manually
            Free,      // Plug may be configured manually, or connected
            ManualOnly // Plug can only be manually configured
        } Pluggable;

        static QColor getColor(Enum value);

        static QString toString(Enum value);

        Q_DECLARE_FLAGS(PlugTypes, Enum)

        static Enum flagsToEnum(PlugTypes types);

        static QList<PlugType::Enum> getAllValues();

        static PlugType::PlugTypes getAllFlags();

        static QList<Enum> toList(PlugTypes types);

        static PlugType::PlugTypes fromList(const QList<PlugType::Enum> &types);

        static bool isSingleType(PlugTypes types);

        static Pluggable isInputPluggable(PlugTypes inputTypes);

        static bool isOutputInternal(PlugTypes types);

        static bool isWidgetAlwaysVisible(PlugTypes types);

        static bool isLabelVisible(PlugTypes types);

        static bool isInputSavable(PlugTypes inputTypes);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PlugType::PlugTypes)
