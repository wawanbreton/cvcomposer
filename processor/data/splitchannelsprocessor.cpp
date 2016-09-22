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

#include "splitchannelsprocessor.h"

#include "global/cvutils.h"


SplitChannelsProcessor::SplitChannelsProcessor()
{
    addInput("image", PlugType::Image);

    addOutput("channels", PlugType::Image, true);
}

Properties SplitChannelsProcessor::processImpl(const Properties &inputs)
{
    cv::Mat image = inputs["image"].value<cv::Mat>();
    QList<QVariant> channels;

    for(int channelId = 0 ; channelId < image.channels() ; channelId++)
    {
        cv::Mat channel;
        cv::extractChannel(image, channel, channelId);
        channels << QVariant::fromValue(channel);
    }

    Properties outputs;
    outputs.insert("channels", QVariant::fromValue(channels));
    return outputs;
}

