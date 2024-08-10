/*
    SPDX-License-Identifier: GPL-2.0-or-later

    SPDX-FileCopyrightText: 2003 Brian Thomas <thomas@mail630.gsfc.nasa.gov>
    SPDX-FileCopyrightText: 2004-2022 Umbrello UML Modeller Authors <umbrello-devel@kde.org>
*/

// own header
#include "cppcodedocumentation.h"

// app includes
#include "codedocument.h"
#include "codegenerator.h"
#include "codegenerationpolicy.h"
#include "uml.h"

// qt includes
#include <QRegularExpression>

CPPCodeDocumentation::CPPCodeDocumentation(CodeDocument * doc, const QString & text)
  : CodeComment(doc, text)
{
}

CPPCodeDocumentation::~CPPCodeDocumentation()
{
}

/**
 * Save the XMI representation of this object
 */
void CPPCodeDocumentation::saveToXMI(QXmlStreamWriter& writer)
{
    writer.writeStartElement(QStringLiteral("cppcodedocumentation"));
    setAttributesOnNode(writer); // as we added no additional fields to this class we may
    // just use parent TextBlock method
    writer.writeEndElement();
}

/**
 * @return   QString
 */
QString CPPCodeDocumentation::toString() const
{
    QString output;

    // simple output method
    if(getWriteOutText())
    {
        bool useDoubleDashOutput = true;

        // need to figure out output type from cpp policy
        CodeGenerationPolicy * p = UMLApp::app()->commonPolicy();
        if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
            useDoubleDashOutput = false;

        QString indent = getIndentationString();
        QString endLine = getNewLineEndingChars();
        QString body = getText();
        if(useDoubleDashOutput)
        {
            if(!body.isEmpty())
                output.append(formatMultiLineText (body, indent + QStringLiteral("// "), endLine));
        } else {
            output.append(indent + QStringLiteral("/**") + endLine);
            output.append(formatMultiLineText (body, indent + QStringLiteral(" * "), endLine));
            output.append(indent + QStringLiteral(" */") + endLine);
        }
    }

    return output;
}

QString CPPCodeDocumentation::getNewEditorLine(int amount)
{
    CodeGenerationPolicy * p = UMLApp::app()->commonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
        return getIndentationString(amount) + QStringLiteral(" * ");
    else
        return getIndentationString(amount) + QStringLiteral("// ");
}

int CPPCodeDocumentation::firstEditableLine()
{
    CodeGenerationPolicy * p = UMLApp::app()->commonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
        return 1;
    return 0;
}

int CPPCodeDocumentation::lastEditableLine() 
{
    CodeGenerationPolicy * p = UMLApp::app()->commonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
    {
        return -1; // very last line is NOT editable
    }
    return 0;
}

/** UnFormat a long text string. Typically, this means removing
 *  the indentation (linePrefix) and/or newline chars from each line.
 */
QString CPPCodeDocumentation::unformatText(const QString & text, const QString & indent)
{
    QString mytext = TextBlock::unformatText(text, indent);
    CodeGenerationPolicy * p = UMLApp::app()->commonPolicy();
    // remove leading or trailing comment stuff
    mytext.remove(QRegularExpression(QLatin1Char('^') + indent));
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
    {
        mytext.remove(QRegularExpression(QStringLiteral("^\\/\\*\\*\\s*\n?")));
        mytext.remove(QRegularExpression(QStringLiteral("\\s*\\*\\/\\s*\n?$")));
        mytext.remove(QRegularExpression(QStringLiteral("^\\s*\\*\\s*")));
    } else
        mytext.remove(QRegularExpression(QStringLiteral("^\\/\\/\\s*")));

    return mytext;
}
