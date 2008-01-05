/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Fri Jun 20 2003
 */
 
#include "codemethodblock.h"

#include "codeclassfield.h"
#include "classifiercodedocument.h"
#include "codegenerationpolicy.h"
#include "uml.h"


CodeMethodBlock::CodeMethodBlock ( ClassifierCodeDocument * doc, UMLObject * parentObj, const QString & body, const QString & comment)
        : CodeBlockWithComments ((CodeDocument*)doc, body, comment), OwnedCodeBlock (parentObj)
{
    m_startMethod = QString();
    m_endMethod = QString();
}

CodeMethodBlock::~CodeMethodBlock ( )
{ }

// we can just call the superclass
CodeDocument * CodeMethodBlock::getParentDocument()
{
    return TextBlock::getParentDocument();
}

QString CodeMethodBlock::getStartMethodText() const
{
    return m_startMethod;
}

QString CodeMethodBlock::getEndMethodText() const
{
    return m_endMethod;
}

void CodeMethodBlock::setStartMethodText (const QString &value)
{
    m_startMethod = value;
}

void CodeMethodBlock::setEndMethodText (const QString &value)
{
    m_endMethod = value;
}

void CodeMethodBlock::release ()
{
    // just call super-class versions
    OwnedCodeBlock::release();
    TextBlock::release();
}

void CodeMethodBlock::setAttributesOnNode ( QDomDocument & doc, QDomElement & elem)
{
    // set super-class attributes
    CodeBlockWithComments::setAttributesOnNode(doc, elem);
    OwnedCodeBlock::setAttributesOnNode(doc, elem);

    // set local class attributes
    if(getContentType() != AutoGenerated)
    {
        QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();
        elem.setAttribute("startMethodText",encodeText(getStartMethodText(),endLine));
        elem.setAttribute("endMethodText",encodeText(getEndMethodText(),endLine));
    }
}

void CodeMethodBlock::setAttributesFromNode ( QDomElement & elem)
{
    // set attributes from the XMI
    CodeBlockWithComments::setAttributesFromNode(elem); // superclass load
    OwnedCodeBlock::setAttributesFromNode(elem); // superclass load

    // now load local attributes
    if(getContentType() != AutoGenerated)
    {
        QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();
        setStartMethodText(decodeText(elem.attribute("startMethodText",""),endLine));
        setEndMethodText(decodeText(elem.attribute("endMethodText",""),endLine));
    }
}

void CodeMethodBlock::setAttributesFromObject(TextBlock * obj)
{
    CodeBlockWithComments::setAttributesFromObject(obj);

    CodeMethodBlock * mb = dynamic_cast<CodeMethodBlock*>(obj);
    if(mb)
    {
        setStartMethodText(mb->getStartMethodText());
        setEndMethodText(mb->getEndMethodText());
    }
}

QString CodeMethodBlock::toString ( ) const
{
    QString string;

    if(getWriteOutText()) {

        QString indent = getIndentationString();
        QString bodyIndent = getIndentationString(getIndentationLevel()+1);
        QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();

        QString startMethod = formatMultiLineText ( getStartMethodText(), indent, endLine);
        QString body = formatMultiLineText (getText(), bodyIndent, endLine);
        QString endMethod = formatMultiLineText( getEndMethodText(), indent, endLine);

        QString comment = getComment()->toString();
        if(!comment.isEmpty() && getComment()->getWriteOutText())
            string.append(comment);

        if(!startMethod.isEmpty())
            string.append(startMethod);

        if(!body.isEmpty())
            string.append(body);

        if(!endMethod.isEmpty())
            string.append(endMethod);

    }
    return string;
}

void CodeMethodBlock::syncToParent ( )
{
    getComment()->setText(getParentObject()->getDoc());

    updateMethodDeclaration();

    // only update IF we are NOT AutoGenerated
    if(getContentType() != AutoGenerated)
        return;

    updateContent();
}

#include "codemethodblock.moc"
