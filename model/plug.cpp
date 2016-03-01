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

#include "model/plug.h"

#include <QDebug>

#include "global/cvutils.h"


Plug::Plug(const PlugDefinition &definition, QObject *parent) :
    QObject(parent),
    _definition(definition)
{
}

const PlugDefinition &Plug::getDefinition() const
{
    return _definition;
}

void Plug::signalConnectedTo(const Plug *connectedTo)
{
    emit connectionChanged(connectedTo);
}

QString Plug::save(const QVariant &value) const
{
    switch(_definition.type)
    {
        case PlugType::Size:
        {
            cv::Size size = value.value<cv::Size>();
            return QString("%1x%2").arg(size.width).arg(size.height);
        }
        case PlugType::Point:
        {
            cv::Point point = value.value<cv::Point>();
            return QString("%1:%2").arg(point.x).arg(point.y);
        }
        case PlugType::Enumeration:
        {
            QList<QPair<QString, QVariant> > values =
                    getDefinition().widgetProperties["values"].value<QList<QPair<QString, QVariant> > >();
            int index = -1;
            for(int i = 0 ; i < values.count() ; i++)
            {
                const QPair<QString, QVariant> &pair = values[i];
                if(pair.second == value)
                {
                    index = i;
                    break;
                }
            }

            return QString::number(index);
        }
        case PlugType::Double:
            return saveDouble(value.toDouble());
        case PlugType::String:
            return value.toString();
        case PlugType::ImagePath:
            return value.toString();
        case PlugType::Color:
        {
            cv::Scalar color = value.value<cv::Scalar>();
            QStringList colorParts;
            for(int i = 0 ; i < 4 ; i++)
            {
                colorParts << saveDouble(color[i]);
            }
            return colorParts.join(";");
        }
        case PlugType::KernelDefinition:
        {
            cv::Mat mat = value.value<cv::Mat>();
            cv::MatIterator_<double> it;
            cv::MatIterator_<double> end = mat.end<double>();
            QList<QStringList> rows;
            for(int i = 0 ; i < mat.rows ; i++)
            {
                rows << QStringList();
            }

            int count = 0;
            for(it = mat.begin<double>() ; it != end; it++, count++)
            {
                rows[count / mat.cols] << saveDouble(*it);
            }

            QStringList rowsStr;
            for(int i = 0 ; i < mat.rows ; i++)
            {
                rowsStr << rows[i].join(";");
            }
            return rowsStr.join("|");
        }
        case PlugType::Image:
        case PlugType::Kernel:
        case PlugType::ImagePreview:
        case PlugType::DockableImageViewer:
            qCritical() << "Plug::save" << "Type" << getDefinition().type << "is not savable";
            break;
    }

    return QString();
}

QString Plug::saveDouble(double value)
{
    if(value == DBL_MAX)
    {
        return "inf";
    }
    else
    {
        return QString::number(value, 'f');
    }
}
