// Copyright 2017 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "plugtypeshelpdialog.h"
#include "ui_plugtypeshelpdialog.h"

#include <QPainter>

#include "model/plug.h"
#include "model/plugtype.h"
#include "gui/plugitem.h"


PlugTypesHelpDialog::PlugTypesHelpDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::PlugTypesHelpDialog)
{
    _ui->setupUi(this);

    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &PlugTypesHelpDialog::accept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &PlugTypesHelpDialog::reject);

    // Standard plugs
    for(PlugType::Enum type : PlugType::getAllValues())
    {
        if(PlugType::isInputPluggable(type) != PlugType::ManualOnly &&
           !PlugType::isOutputInternal(type))
        {
            PlugDefinition plugDef;
            plugDef.types = type;

            addPlugTypeHelp(plugDef, PlugType::toString(type));
        }
    }

    addSpacer();

    // Plugs with multiple types
    PlugDefinition multiPlugDef;
    multiPlugDef.types = PlugType::Image | PlugType::Contour | PlugType::Ellipse;
    addPlugTypeHelp(multiPlugDef, "Some input plugs can accept multiple data types");

    PlugDefinition allPlugDef;
    allPlugDef.types = PlugType::getAllFlags();
    addPlugTypeHelp(allPlugDef, "Some input plugs can accept all possible data types");

    addSpacer();

    // Plugs with list support
    PlugDefinition listPlugDef;
    listPlugDef.types = PlugType::Image;
    listPlugDef.listSupport = ProcessorListType::Simple;
    addPlugTypeHelp(listPlugDef, "Some plugs can accept or generate a list of elements. All the "
                                 "elements are treated together or independantly, depending on the "
                                 "processor.<br>List and non-list plugs are compatible :"
                                 "<ul>"
                                 " <li>When connecting a list to a non-list plug, only the first element will be treated</li>"
                                 " <li>When connecting a non-list to a list plug, the processor will treat it as a single-element list</li>"
                                 "</ul>");
}

PlugTypesHelpDialog::~PlugTypesHelpDialog()
{
    delete _ui;
}

void PlugTypesHelpDialog::addPlugTypeHelp(const PlugDefinition &plugDef, const QString &text)
{
    QPixmap pixmap(PlugItem::radius * 2, PlugItem::radius * 2);
    pixmap.fill(Qt::transparent);

    Plug plug(plugDef);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    PlugItem item(&plug);
    painter.translate(PlugItem::radius, PlugItem::radius);
    item.paint(&painter, nullptr);

    painter.end();

    QLabel *labelItem = new QLabel(this);
    labelItem->setPixmap(pixmap);
    _ui->gridLayoutPlugs->addWidget(labelItem, _ui->gridLayoutPlugs->rowCount(), 0);

    QLabel *labelDesc = new QLabel(this);
    labelDesc->setText(text);
    labelDesc->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    labelDesc->setWordWrap(true);
    _ui->gridLayoutPlugs->addWidget(labelDesc, _ui->gridLayoutPlugs->rowCount() - 1, 1);
}

void PlugTypesHelpDialog::addSpacer()
{
    _ui->gridLayoutPlugs->addItem(new QSpacerItem(0, 15, QSizePolicy::Fixed, QSizePolicy::Fixed),
                                  _ui->gridLayoutPlugs->rowCount(),
                                  0);
}
