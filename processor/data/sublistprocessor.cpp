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

#include "sublistprocessor.h"

#include "global/cvutils.h"


SubListProcessor::SubListProcessor()
{
    addInput("input list", PlugType::getAllFlags(), QVariant(), Properties(), ThreeStateBool::None, true);

    Properties sizeProperties;
    sizeProperties.insert("decimals", 0);
    addInput("first element", PlugType::Double, 0, sizeProperties);
    addInput("amount", PlugType::Double, 1, sizeProperties);

    addOutput("output list", PlugType::getAllFlags(), true);
}

Properties SubListProcessor::processImpl(const Properties &inputs)
{
    QList<QVariant> list = inputs["input list"].value<QList<QVariant>>();
    list = list.mid(inputs["first element"].toInt(), inputs["amount"].toInt());

    Properties outputs;
    outputs.insert("output list", QVariant::fromValue(list));
    return outputs;
}

