/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2020                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H
#define CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H

#include "codeclassfielddeclarationblock.h"

#include <QString>

class CPPHeaderCodeClassFieldDeclarationBlock : public CodeClassFieldDeclarationBlock
{
    Q_OBJECT
public:

    /**
     * Constructor
     */
    explicit CPPHeaderCodeClassFieldDeclarationBlock (CodeClassField * parent);

    /**
     * Empty Destructor
     */
    virtual ~CPPHeaderCodeClassFieldDeclarationBlock ();

    /**
     * This will be called by syncToParent whenever the parent object is "modified"
     */
    void updateContent ();

private:


};

#endif // CPPHEADERCODECLASSFIELDDECLARATIONBLOCK_H
