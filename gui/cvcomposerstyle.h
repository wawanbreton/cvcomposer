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

#include <QProxyStyle>

class CvComposerStyle : public QProxyStyle
{
    Q_OBJECT

    public:
        CvComposerStyle();

        virtual void polish(QApplication *application) override;

        virtual void polish(QPalette &palette) override;

        virtual void drawPrimitive(PrimitiveElement element,
                                   const QStyleOption *option,
                                   QPainter *painter,
                                   const QWidget *widget) const override;

        void drawControl(ControlElement element,
                         const QStyleOption *option,
                         QPainter *painter,
                         const QWidget *widget) const override;

        void drawComplexControl(ComplexControl control,
                                const QStyleOptionComplex *option,
                                QPainter *painter,
                                const QWidget *widget) const override;

        virtual QRect subControlRect(ComplexControl control,
                                     const QStyleOptionComplex *option,
                                     SubControl subControl,
                                     const QWidget *widget = NULL) const override;

        virtual int pixelMetric(PixelMetric metric,
                                const QStyleOption *option = NULL,
                                const QWidget * widget = NULL) const override;

    private:
        static void drawBaseControlFrame(const QStyleOption *option, QPainter *painter);

        static void drawArrow(QPainter *painter,
                              const QStyleOption *option,
                              const QRectF &rect,
                              bool enabled,
                              bool active,
                              bool down = true);
};
