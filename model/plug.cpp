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
#include <QStringList>

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
        case PlugType::Path:
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
        case PlugType::Boolean:
            return value.toBool() ? "true" : "false";
        case PlugType::Image:
        case PlugType::Kernel:
        case PlugType::ImagePreview:
        case PlugType::DockableImageViewer:
        case PlugType::Rectangle:
        case PlugType::Circle:
        case PlugType::Generic:
        case PlugType::Contour:
        case PlugType::Line:
        case PlugType::Ellipse:
            qCritical() << "Plug::save" << "Type" << getDefinition().type << "is not savable";
            break;
    }

    return QString();
}

QVariant Plug::load(const QString &value) const
{
    switch(_definition.type)
    {
        case PlugType::Size:
        {
            QStringList sizeParts = value.split('x', QString::SkipEmptyParts);
            if(sizeParts.count() == 2)
            {
                bool ok1;
                bool ok2;
                cv::Size result(sizeParts[0].toInt(&ok1), sizeParts[1].toInt(&ok2));
                if(ok1 && ok2)
                {
                    return QVariant::fromValue(result);
                }
            }
            break;
        }
        case PlugType::Point:
        {
            QStringList sizeParts = value.split(':', QString::SkipEmptyParts);
            if(sizeParts.count() == 2)
            {
                bool ok1;
                bool ok2;
                cv::Point result(sizeParts[0].toInt(&ok1), sizeParts[1].toInt(&ok2));
                if(ok1 && ok2)
                {
                    return QVariant::fromValue(result);
                }
            }
            break;
        }
        case PlugType::Enumeration:
        {
            bool ok;
            int index = value.toInt(&ok);
            if(ok)
            {
                QList<QPair<QString, QVariant> > values =
                        getDefinition().widgetProperties["values"].value<QList<QPair<QString, QVariant> > >();
                if(index >= 0 && index < values.count())
                {
                    return values[index].second;
                }
                return QString::number(index);
            }
            break;
        }
        case PlugType::Double:
        {
            bool ok;
            double result = loadDouble(value, &ok);
            if(ok)
            {
                return result;
            }

            break;
        }
        case PlugType::String:
        case PlugType::Path:
            return value;
        case PlugType::Color:
        {
            QStringList colorParts = value.split(';', QString::SkipEmptyParts);
            if(colorParts.count() == 4)
            {
                cv::Scalar scalar;
                bool ok = true;
                for(int i = 0 ; i < 4 && ok ; i++)
                {
                    scalar[i] = loadDouble(colorParts[i], &ok);
                }

                if(ok)
                {
                    return QVariant::fromValue(scalar);
                }
            }
            break;
        }
        case PlugType::KernelDefinition:
        {
            QStringList rowsStr = value.split('|', QString::SkipEmptyParts);
            int rows = rowsStr.count();
            int cols = (value.count(';') / rows) + 1;
            bool ok = true;

            cv::Mat mat(rows, cols, CV_64FC1);
            for(int row = 0 ; row < rows && ok ; row++)
            {
                QString rowStr = rowsStr[row];
                QStringList rowParts = rowStr.split(';');
                if(rowParts.count() == cols)
                {
                    for(int col = 0 ; col < cols && ok ; col++)
                    {
                        mat.at<double>(row, col) = loadDouble(rowParts[col], &ok);
                    }
                }
                else
                {
                    ok = false;
                }
            }

            if(ok)
            {
                return QVariant::fromValue(mat);
            }
            break;
        }
        case PlugType::Boolean:
            return QVariant(value).toBool();
        case PlugType::Image:
        case PlugType::Kernel:
        case PlugType::ImagePreview:
        case PlugType::DockableImageViewer:
        case PlugType::Rectangle:
        case PlugType::Circle:
        case PlugType::Contour:
        case PlugType::Line:
        case PlugType::Ellipse:
        case PlugType::Generic:
            break;
    }

    qWarning() << "Unable to parse" << value << "for type" << getDefinition().type;

    return QVariant();
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

double Plug::loadDouble(const QString &valueStr, bool *ok)
{
    if(valueStr == "inf")
    {
        *ok = true;
        return DBL_MAX;
    }
    else
    {
        return valueStr.toDouble(ok);
    }
}
