/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2020                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include "multipagedialogbase.h"

class NoteWidget;

/**
 * @author Ralf Habacker
 * Bugs and comments to umbrello-devel@kde.org or https://bugs.kde.org
 */
class NoteDialog : public MultiPageDialogBase
{
   Q_OBJECT
public:
    NoteDialog(QWidget *parent, NoteWidget *widget);

    bool apply();

public slots:
    void slotOk();
    void slotApply();

protected:
    void setupPages();
    NoteWidget *m_widget;
};

#endif
