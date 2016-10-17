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

#include <QLineEdit>
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

    palette.setColor(QPalette::Button, QColor("#cccccc"));

    palette.setColor(QPalette::Base,   QColor("#cccccc"));

    palette.setColor(QPalette::Text,       Qt::black);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);

    palette.setColor(QPalette::BrightText, Qt::white);
    palette.setColor(QPalette::Inactive, QPalette::BrightText, Qt::gray);

    palette.setColor(QPalette::Highlight,       Qt::white);
    palette.setColor(QPalette::HighlightedText, Qt::black);

    palette.setColor(QPalette::Light, Qt::green);
    palette.setColor(QPalette::Midlight, Qt::red);
    palette.setColor(QPalette::Dark, Qt::blue);
    palette.setColor(QPalette::Mid, QColor("#808a95"));
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

    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void CvComposerStyle::drawControl(QStyle::ControlElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    if(element == CE_PushButtonBevel)
    {
        QPen pen = Qt::NoPen;
        if(option->state & QStyle::State_Sunken)
        {
            pen = QPen(option->palette.alternateBase(), 4);
        }
        else if(option->state & QStyle::State_MouseOver)
        {
            pen = QPen(option->palette.highlight(), 4);
        }
        painter->setPen(pen);
        painter->setBrush(option->palette.color(QPalette::Button));

        painter->drawRect(option->rect);
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

        drawArrow(painter,
                  option,
                  rectUp,
                  optionSB->stepEnabled & QAbstractSpinBox::StepUpEnabled,
                  option->activeSubControls & SC_SpinBoxUp,
                  false);

        drawArrow(painter,
                  option,
                  rectDown,
                  optionSB->stepEnabled & QAbstractSpinBox::StepDownEnabled,
                  option->activeSubControls & SC_SpinBoxDown,
                  true);

        return;
    }
    else if(control == CC_ComboBox)
    {
        const QStyleOptionComboBox *optionCB = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        QStyleOptionComboBox optionCopy = *optionCB;
        optionCopy.subControls &= ~SC_ComboBoxFrame;
        optionCopy.frame = false;

        drawBaseControlFrame(option, painter);

        QBrush arrowColor = option->palette.alternateBase();
        QRectF rectArrow = subControlRect(control, &optionCopy, SC_ComboBoxArrow, widget);
        painter->fillRect(rectArrow, arrowColor);

        drawArrow(painter,
                  option,
                  rectArrow,
                  true,
                  true);

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
        if(qobject_cast<const QLineEdit *>(widget))
        {
            return 6;
        }

        return 0;
    }
    else if(metric == PM_ButtonShiftHorizontal || metric == PM_ButtonShiftVertical)
    {
        return 0;
    }

    return QCommonStyle::pixelMetric(metric, option, widget);
}

void CvComposerStyle::drawArrow(QPainter *painter,
                                const QStyleOption *option,
                                const QRectF &rect,
                                bool enabled,
                                bool active,
                                bool down)
{
    bool mouseOver = (option->state & QStyle::State_MouseOver) && !(option->state & QStyle::State_Sunken);
    qreal bigScale = 1.3;

    painter->save();
    painter->translate(rect.center());
    if(!down)
    {
        painter->scale(1, -1);
    }
    if(mouseOver && active && enabled)
    {
        painter->scale(bigScale, bigScale);
    }
    QBrush color = option->palette.color(enabled ? QPalette::Active : QPalette::Inactive,
                                         QPalette::BrightText);

    qreal deltaX = rect.width() / 3;
    qreal deltaY = deltaX * 0.7;
    QPainterPath arrowPath(QPointF(0, deltaY));
    arrowPath.lineTo(deltaX, -deltaY);
    arrowPath.lineTo(-deltaX, -deltaY);
    arrowPath.closeSubpath();

    painter->fillPath(arrowPath, color);
    painter->restore();
}

void CvComposerStyle::drawBaseControlFrame(const QStyleOption *option, QPainter *painter)
{
    painter->fillRect(option->rect, option->palette.base());
}
