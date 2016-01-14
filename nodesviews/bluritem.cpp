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

#include "bluritem.h"

#include "blurwidget.h"
#include "nodes/blurnode.h"


BlurItem::BlurItem(BlurNode *node, QGraphicsItem *parent) :
    GenericNodeItem(node, parent)
{
    BlurWidget *widget = new BlurWidget(node->getSize(), node->getAnchor());
    setWidget(widget);
    connect(widget, SIGNAL(sizeChanged(cv::Size)),    node, SLOT(setSize(cv::Size)));
    connect(widget, SIGNAL(anchorChanged(cv::Point)), node, SLOT(setAnchor(cv::Point)));
}
