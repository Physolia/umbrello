
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Sep 26 2003
 */



#ifndef XMLCODECOMMENT_H
#define XMLCODECOMMENT_H

#include <qstring.h>
#include "../codecomment.h"

class CodeDocument;

/**
  * class XMLCodeDocumentation
  * A XML (code) comment.
  */

class XMLCodeComment: virtual public CodeComment
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //


    /**
     * Constructors
     */
    explicit XMLCodeComment ( CodeDocument * doc, const QString & text = "" );

    /**
     * Empty Destructor
     */
    virtual ~XMLCodeComment( );

    // Public attributes
    //

    // Other
    //

    /**
     * @return  QString
     */
    QString toString ( ) const;


protected:

private:

};

#endif // XMLCODECOMMENT_H
