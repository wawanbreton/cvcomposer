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

#include "cvcomposerstyle.h"

#include <QDebug>
#include <QApplication>
#include <QFontDatabase>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>


CvComposerStyle::CvComposerStyle()
{

}

void CvComposerStyle::polish(QApplication *application)
{
    QProxyStyle::polish(application);

    int fontId = QFontDatabase::addApplicationFont(":/Ubuntu-R.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont ubuntuFont(fontFamily);
    ubuntuFont.setPixelSize(14);
    application->setFont(ubuntuFont);
}

void CvComposerStyle::polish(QPalette &palette)
{
    QProxyStyle::polish(palette);

    palette.setColor(QPalette::Window,        QColor("#273035"));
    palette.setColor(QPalette::AlternateBase, QColor("#34495e"));

    palette.setColor(QPalette::Inactive, QPalette::Button, QColor("#cccccc"));
    palette.setColor(QPalette::Active,   QPalette::Button, QColor("#2ecc71"));

    palette.setColor(QPalette::Base,          QColor("#cccccc"));

    palette.setColor(QPalette::Text,       Qt::black);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::white);
    palette.setColor(QPalette::Inactive, QPalette::BrightText, Qt::gray);

    palette.setColor(QPalette::Light, Qt::green);
    palette.setColor(QPalette::Midlight, Qt::red);
    palette.setColor(QPalette::Dark, Qt::blue);
    palette.setColor(QPalette::Mid, Qt::yellow);
    palette.setColor(QPalette::Shadow, Qt::magenta);
}

void CvComposerStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                    const QStyleOption *option,
                                    QPainter *painter,
                                    const QWidget *widget) const
{
    if(element == PE_PanelLineEdit)
    {
        drawBaseControlFrame(option, painter);
        return;
    }
    else if(element == PE_IndicatorSpinUp || element == PE_IndicatorSpinDown)
    {
        //painter->fillRect(option->rect, option->palette.color(QPalette::AlternateBase));
        painter->fillRect(option->rect, Qt::red);
        return;
    }

    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void CvComposerStyle::drawControl(QStyle::ControlElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    if(element == CE_PushButtonBevel)
    {
        QPalette::ColorGroup group = (option->state & QStyle::State_MouseOver) ? QPalette::Active : QPalette::Inactive;
        painter->fillRect(option->rect, option->palette.color(group, QPalette::Button));
        return;
    }
    else if(element == CE_PushButtonLabel)
    {
        painter->setPen(option->palette.color(QPalette::ButtonText));
        painter->drawText(option->rect, Qt::AlignCenter, ((QStyleOptionButton *)option)->text);
        return;
    }

    return QProxyStyle::drawControl(element, option, painter, widget);
}

void CvComposerStyle::drawComplexControl(QStyle::ComplexControl control,
                                         const QStyleOptionComplex *option,
                                         QPainter *painter,
                                         const QWidget *widget) const
{
    if(control == CC_SpinBox)
    {
        const QStyleOptionSpinBox *optionSB = qstyleoption_cast<const QStyleOptionSpinBox *>(option);
        QStyleOptionSpinBox optionCopy = *optionSB;
        optionCopy.subControls &= ~SC_SpinBoxFrame;

        drawBaseControlFrame(option, painter);

        QBrush spinFrameColor = option->palette.alternateBase();
        QRectF rectUp = subControlRect(CC_SpinBox, option, SC_SpinBoxUp, widget);
        QRectF rectDown = subControlRect(CC_SpinBox, option, SC_SpinBoxDown, widget);
        painter->fillRect(rectUp | rectDown, spinFrameColor);

        qreal deltaHeight = rectUp.height() / 6;
        qreal deltaWidth = rectUp.width() / 3;
        QPainterPath upDownSymbolPath(QPointF(0, deltaHeight));
        upDownSymbolPath.lineTo(deltaWidth, -deltaHeight);
        upDownSymbolPath.lineTo(-deltaWidth, -deltaHeight);
        upDownSymbolPath.closeSubpath();

        bool mouseOver = (option->state & QStyle::State_MouseOver) && !(option->state & QStyle::State_Sunken);
        qreal bigScale = 1.3;

        bool enabledUp = optionSB->stepEnabled & QAbstractSpinBox::StepUpEnabled;
        bool bigUp = mouseOver && (option->activeSubControls & SC_SpinBoxUp) && enabledUp;
        painter->save();
        painter->translate(rectUp.center());
        painter->scale(1, -1);
        if(bigUp)
        {
            painter->scale(bigScale, bigScale);
        }
        QBrush upColor = option->palette.color(enabledUp ? QPalette::Active : QPalette::Inactive,
                                               QPalette::BrightText);
        painter->fillPath(upDownSymbolPath, upColor);
        painter->restore();

        bool enabledDown = optionSB->stepEnabled & QAbstractSpinBox::StepDownEnabled;
        bool bigDown = mouseOver && (option->activeSubControls & SC_SpinBoxDown) && enabledDown;
        painter->save();
        painter->translate(rectDown.center());
        if(bigDown)
        {
            painter->scale(bigScale, bigScale);
        }
        QBrush downColor = option->palette.color(enabledDown ? QPalette::Active : QPalette::Inactive,
                                                 QPalette::BrightText);
        painter->fillPath(upDownSymbolPath, downColor);
        painter->restore();

        return;
    }

    return QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect CvComposerStyle::subControlRect(QStyle::ComplexControl control,
                                      const QStyleOptionComplex *option,
                                      QStyle::SubControl subControl,
                                      const QWidget *widget) const
{
    if(control == CC_SpinBox)
    {
        if(subControl == SC_SpinBoxDown)
        {
            // Make default rectangle higher to remove eventual line at bottom
            QRect rect = QCommonStyle::subControlRect(control, option, subControl, widget);
            rect.setHeight(widget->height() - rect.top());
            return rect;
        }
    }

    return QCommonStyle::subControlRect(control, option, subControl, widget);
}

int CvComposerStyle::pixelMetric(QStyle::PixelMetric metric,
                                 const QStyleOption *option,
                                 const QWidget *widget) const
{
    if(metric == PM_DefaultFrameWidth)
    {
        return 0;
    }

    return QCommonStyle::pixelMetric(metric, option, widget);
}

void CvComposerStyle::drawBaseControlFrame(const QStyleOption *option, QPainter *painter) const
{
    painter->fillRect(option->rect, option->palette.base());
}
