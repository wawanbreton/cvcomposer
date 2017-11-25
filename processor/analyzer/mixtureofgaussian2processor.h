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

#include "processor/abstractprocessor.h"

#include <opencv2/video/background_segm.hpp>

class MixtureOfGaussian2Processor : public AbstractProcessor
{
    public:
        MixtureOfGaussian2Processor();

    protected:
        virtual Properties processImpl(const Properties &inputs) override;

    private:
        cv::Ptr<cv::BackgroundSubtractorMOG2> _mog;
};
