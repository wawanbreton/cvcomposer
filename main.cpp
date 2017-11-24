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

#include <QApplication>
#include <QFontDatabase>

#include "gui/mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/Ubuntu-R.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont ubuntuFont(fontFamily);
    ubuntuFont.setPixelSize(14);
    a.setFont(ubuntuFont);

    QCoreApplication::setApplicationName("CvComposer");
    QCoreApplication::setOrganizationName("CvComposer");

    MainWidget w;
    w.show();

    int result = a.exec();

    return result;
}
