//
// Copyright 2014 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SendAreaToolBar.h"

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtGui/QIcon>

#include "GuiAppMgr.h"

namespace comms_champion
{

namespace
{

const QIcon& startIcon()
{
    static const QIcon Icon(":/image/start.png");
    return Icon;
}

const QIcon& startAllIcon()
{
    static const QIcon Icon(":/image/start_all.png");
    return Icon;
}

const QIcon& stopIcon()
{
    static const QIcon Icon(":/image/stop.png");
    return Icon;
}

const QIcon& addIcon()
{
    static const QIcon Icon(":/image/add.png");
    return Icon;
}

const QIcon& editIcon()
{
    static const QIcon Icon(":/image/edit.png");
    return Icon;
}

const QIcon& deleteIcon()
{
    static const QIcon Icon(":/image/delete.png");
    return Icon;
}

void createStartButton(QToolBar& bar)
{
    auto* action = bar.addAction(startIcon(), "Send Selected");
    QObject::connect(action, SIGNAL(triggered()),
                     GuiAppMgr::instance(), SLOT(sendStartClicked()));

}

void createStartAllButton(QToolBar& bar)
{
    auto* action = bar.addAction(startAllIcon(), "Send All");
    QObject::connect(action, SIGNAL(triggered()),
                     GuiAppMgr::instance(), SLOT(sendStartAllClicked()));

}

void createAddButton(QToolBar& bar)
{
    auto* action = bar.addAction(addIcon(), "Add New Message");
    QObject::connect(action, SIGNAL(triggered()),
                     GuiAppMgr::instance(), SLOT(sendAddClicked()));
}

void createEditButton(QToolBar& bar)
{
    auto* action = bar.addAction(editIcon(), "Edit Selected Message");
    QObject::connect(action, SIGNAL(triggered()),
                     GuiAppMgr::instance(), SLOT(sendEditClicked()));
}

void createDeleteButton(QToolBar& bar)
{
    auto* action = bar.addAction(deleteIcon(), "Delete Selected Message");
    QObject::connect(action, SIGNAL(triggered()),
                     GuiAppMgr::instance(), SLOT(sendDeleteClicked()));
}

}  // namespace

SendAreaToolBar::SendAreaToolBar(QWidget* parent)
  : Base(parent)
{
    createStartButton(*this);
    createStartAllButton(*this);
    createAddButton(*this);
    createEditButton(*this);
    createDeleteButton(*this);
}

}  // namespace comms_champion

