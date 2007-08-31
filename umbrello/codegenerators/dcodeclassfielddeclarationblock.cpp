
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

#include "dcodeclassfielddeclarationblock.h"

#include "dcodeclassfield.h"
#include "dclassifiercodedocument.h"
#include "dcodegenerationpolicy.h"
#include "../codegenerator.h"
#include "../classifier.h"
#include "../umlrole.h"
#include "../uml.h"

// Constructors/Destructors
//

DCodeClassFieldDeclarationBlock::DCodeClassFieldDeclarationBlock ( CodeClassField * parent )
        : CodeClassFieldDeclarationBlock ( parent )
{
    setOverallIndentationLevel(1);
    updateContent();
}

DCodeClassFieldDeclarationBlock::~DCodeClassFieldDeclarationBlock ( ) { }

//
// Methods
//



// Other methods
//

/**
 */
void DCodeClassFieldDeclarationBlock::updateContent( )
{

    CodeClassField * cf = getParentClassField();
    ClassifierCodeDocument * doc = cf->getParentDocument();
    DCodeClassField * jcf = dynamic_cast<DCodeClassField*>(cf);
    DClassifierCodeDocument* jdoc = dynamic_cast<DClassifierCodeDocument*>(doc);
    CodeGenerationPolicy * commonpolicy = UMLApp::app()->getCommonPolicy();

    CodeGenerationPolicy::ScopePolicy scopePolicy = commonpolicy->getAssociationFieldScope();

    // Set the comment
    QString notes = getParentObject()->getDoc();
    getComment()->setText(notes);

    // Set the body
    QString staticValue = getParentObject()->getStatic() ? "static " : "";
    QString scopeStr = jdoc->scopeToDDecl(getParentObject()->getVisibility());

    // IF this is from an association, then scope taken as appropriate to policy
    if(!jcf->parentIsAttribute())
    {
        switch (scopePolicy) {
        case CodeGenerationPolicy::Public:
        case CodeGenerationPolicy::Private:
        case CodeGenerationPolicy::Protected:
              scopeStr = jdoc->scopeToDDecl((Uml::Visibility::Value) (scopePolicy + 200));
            break;
        default:
        case CodeGenerationPolicy::FromParent:
            // do nothing here... will leave as from parent object
            break;
        }
    }

    QString typeName = jcf->getTypeName();
    QString fieldName = jcf->getFieldName();
    QString initialV = jcf->getInitialValue();

    if (!cf->parentIsAttribute() && !cf->fieldIsSingleValue())
        typeName = "List";

    QString body = staticValue+scopeStr+' '+typeName+' '+fieldName;
    if (!initialV.isEmpty())
        body.append(" = " + initialV);
    else if (!cf->parentIsAttribute())
    {
        UMLRole * role = dynamic_cast<UMLRole*>(cf->getParentObject());
        if (role->getObject()->getBaseType() == Uml::ot_Interface)
        {
            // do nothing.. can't instanciate an interface
        } else {

            // FIX?: IF a constructor method exists in the classifiercodedoc
            // of the parent Object, then we can use that instead (if its empty).
            if(cf->fieldIsSingleValue())
            {
                if(!typeName.isEmpty())
                    body.append(" = new " + typeName + " ( )");
            } else
                body.append(" = new Vector ( )");
        }
    }

    setText(body+';');

}

#include "dcodeclassfielddeclarationblock.moc"
