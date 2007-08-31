/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007 Jari-Matti Mäkelä <jmjm@iki.fi>                    *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "dcodeaccessormethod.h"

// qt/kde includes
#include <kdebug.h>

// local includes
#include "../attribute.h"
#include "../codegenerator.h"
#include "../codegenerationpolicy.h"
#include "../classifiercodedocument.h"
#include "../umlobject.h"
#include "../umlrole.h"
#include "../uml.h"

#include "dclassifiercodedocument.h"
#include "dcodegenerationpolicy.h"
#include "dcodeclassfield.h"
#include "dcodedocumentation.h"

// Constructors/Destructors
//

DCodeAccessorMethod::DCodeAccessorMethod ( CodeClassField * field, CodeAccessorMethod::AccessorType type)
        : CodeAccessorMethod ( field )
{
    setType(type);

    // lets use full-blown comment
    DClassifierCodeDocument* jccd = dynamic_cast<DClassifierCodeDocument*>(field->getParentDocument());
    setComment(new DCodeDocumentation(jccd));
}

DCodeAccessorMethod::~DCodeAccessorMethod ( ) { }

// Other methods
//

void DCodeAccessorMethod::setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement)
{

    // set super-class attributes
    CodeAccessorMethod::setAttributesOnNode(doc, blockElement);

    // set local attributes now
}

void DCodeAccessorMethod::setAttributesFromNode( QDomElement & root)
{

    // set attributes from superclass method the XMI
    CodeAccessorMethod::setAttributesFromNode(root);

    // load local stuff

}

void DCodeAccessorMethod::updateContent( )
{

    CodeClassField * parentField = getParentClassField();
    DCodeClassField * dfield = dynamic_cast<DCodeClassField*>(parentField);
    QString fieldName = dfield->getFieldName();

    QString text = "";
    switch(getType()) {
    case CodeAccessorMethod::ADD:
        {
            int maxOccurs = dfield->maximumListOccurances();
            QString fieldType = dfield->getTypeName();
            QString indent = getIndentation();
            QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();
            if(maxOccurs > 0)
                text += "if ("+fieldName+".size() < "+ QString::number(maxOccurs)+") {"+endLine+indent;
            text += fieldName+".add(value);";
            if(maxOccurs > 0)
            {
                text += endLine+"} else {"+endLine;
                text += indent + "System.err.println(\"ERROR: Cant add"+fieldType+" to "+fieldName+", minimum number of items reached.\");"+endLine+'}'+endLine;
            }
            break;
        }
    case CodeAccessorMethod::GET:
        text = "return "+fieldName+';';
        break;
    case CodeAccessorMethod::LIST:
        text = "return (List) "+fieldName+';';
        break;
    case CodeAccessorMethod::REMOVE:
        {
            int minOccurs = dfield->minimumListOccurances();
            QString fieldType = dfield->getTypeName();
            QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();
            QString indent = getIndentation();

            if(minOccurs > 0)
                text += "if ("+fieldName+".size() >= "+ QString::number(minOccurs)+") {"+endLine+indent;
            text += fieldName+".remove(value);";
            if(minOccurs > 0)
            {
                text += endLine+"} else {"+endLine;
                text += indent + "System.err.println(\"ERROR: Cant remove"+fieldType+" from "+fieldName+", minimum number of items reached.\");"+endLine+'}'+endLine;
            }
            break;
        }
    case CodeAccessorMethod::SET:
        text = fieldName+" = value;";
        break;
    default:
        // do nothing
        break;
    }

    setText(text);

}

void DCodeAccessorMethod::updateMethodDeclaration()
{

    DCodeClassField * dfield = dynamic_cast<DCodeClassField*>(getParentClassField());
    DClassifierCodeDocument * ddoc = dynamic_cast<DClassifierCodeDocument*>(dfield->getParentDocument());
    CodeGenerationPolicy *commonpolicy = UMLApp::app()->getCommonPolicy();

    // gather defs
    CodeGenerationPolicy::ScopePolicy scopePolicy = commonpolicy->getAttributeAccessorScope();
    QString strVis = ddoc->scopeToDDecl(dfield->getVisibility());
    QString fieldName = dfield->getFieldName();
    QString fieldType = dfield->getTypeName();
    QString objectType = dfield->getListObjectType();
    if(objectType.isEmpty())
        objectType = fieldName;
    QString endLine = UMLApp::app()->getCommonPolicy()->getNewLineEndingChars();

    // set scope of this accessor appropriately..if its an attribute,
    // we need to be more sophisticated
    if(dfield->parentIsAttribute())
        switch (scopePolicy) {
        case CodeGenerationPolicy::Public:
        case CodeGenerationPolicy::Private:
        case CodeGenerationPolicy::Protected:
              strVis = ddoc->scopeToDDecl((Uml::Visibility::Value) (scopePolicy + 200));
            break;
        default:
        case CodeGenerationPolicy::FromParent:
            // do nothing..already have taken parent value
            break;
        }

    // some variables we will need to populate
    QString headerText = "";
    QString methodReturnType = "";
    QString methodName = "";
    QString methodParams = "";

    switch(getType()) {
    case CodeAccessorMethod::ADD:
        methodName = "add"+ddoc->capitalizeFirstLetter(fieldType);
        methodReturnType = "void";
        methodParams = objectType+" value ";
        headerText = "Add an object of type "+objectType+" to the List "+fieldName+endLine+getParentObject()->getDoc()+endLine+"@return void";
        break;
    case CodeAccessorMethod::GET:
        methodName = "get"+ddoc->capitalizeFirstLetter(fieldName);
        methodReturnType = fieldType;
        headerText = "Get the value of "+fieldName+endLine+getParentObject()->getDoc()+endLine+"@return the value of "+fieldName;
        break;
    case CodeAccessorMethod::LIST:
        methodName = "get"+ddoc->capitalizeFirstLetter(fieldType)+"List";
        methodReturnType = "List";
        headerText = "Get the list of "+fieldName+endLine+getParentObject()->getDoc()+endLine+"@return List of "+fieldName;
        break;
    case CodeAccessorMethod::REMOVE:
        methodName = "remove"+ddoc->capitalizeFirstLetter(fieldType);
        methodReturnType = "void";
        methodParams = objectType+" value ";
        headerText = "Remove an object of type "+objectType+" from the List "+fieldName+endLine+getParentObject()->getDoc();
        break;
    case CodeAccessorMethod::SET:
        methodName = "set"+ddoc->capitalizeFirstLetter(fieldName);
        methodReturnType = "void";
        methodParams = fieldType + " value ";
        headerText = "Set the value of "+fieldName+endLine+getParentObject()->getDoc()+endLine;
        break;
    default:
        // do nothing..no idea what this is
        kWarning()<<"Warning: cant generate DCodeAccessorMethod for type: "<<getType();
        break;
    }

    // set header once.
    if(getComment()->getText().isEmpty())
        getComment()->setText(headerText);

    // set start/end method text
    setStartMethodText(strVis+' '+methodReturnType+' '+methodName+" ( "+methodParams+" ) {");
    setEndMethodText("}");

}

void DCodeAccessorMethod::update()
{
    updateMethodDeclaration();
    updateContent();
}

#include "dcodeaccessormethod.moc"
