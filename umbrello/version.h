/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2011                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef UMBRELLO_VERSION_H
#define UMBRELLO_VERSION_H

#include "revision.h"

#include <kdeversion.h>

inline QByteArray umbrelloVersion()
{
    return QString("%1.%2.%3-r%4")
        .arg(KDE::versionMajor()-2)
        .arg(KDE::versionMinor())
        .arg(KDE::versionRelease())
        .arg(REVISION).toLatin1();
}

#endif
