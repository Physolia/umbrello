/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "classifierlistitem.h"

// local includes
#include "classifier.h"
#include "model_utils.h"
#include "object_factory.h"
#include "uml.h"
#include "umldoc.h"

// qt/kde includes
#include <kdebug.h>
#include <klocale.h>


UMLClassifierListItem::UMLClassifierListItem(UMLObject *parent,
                                             const QString& name, Uml::IDType id)
        : UMLObject(parent, name, id)
{
    UMLObject *parentObj = const_cast<UMLObject*>(parent);
    UMLClassifier *pc = dynamic_cast<UMLClassifier*>(parentObj);
    if (pc)
        UMLObject::setUMLPackage(pc);
}

UMLClassifierListItem::UMLClassifierListItem(UMLObject *parent)
        : UMLObject(parent)
{
    UMLObject *parentObj = const_cast<UMLObject*>(parent);
    UMLClassifier *pc = dynamic_cast<UMLClassifier*>(parentObj);
    if (pc)
        UMLObject::setUMLPackage(pc);
}

UMLClassifierListItem::~UMLClassifierListItem()
{
}

/**
 * Copy the internal presentation of this object into the new
 * object.
 */
void UMLClassifierListItem::copyInto(UMLObject *lhs) const
{
    // Call the parent.
    UMLObject::copyInto(lhs);
}

/**
 * Returns a string representation of the list item.
 *
 * @param sig               What type of operation string to show.
 * @return  The string representation of the operation.
 */
QString UMLClassifierListItem::toString(Uml::Signature_Type sig)
{
    Q_UNUSED(sig);
    return getName();
}

/**
 * Returns the type of the UMLClassifierListItem.
 *
 * @return  The type of the UMLClassifierListItem.
 */
UMLClassifier * UMLClassifierListItem::getType() const
{
    return static_cast<UMLClassifier*>(m_pSecondary);
}

/**
 * Returns the type name of the UMLClassifierListItem.
 *
 * @return  The type name of the UMLClassifierListItem.
 */
QString UMLClassifierListItem::getTypeName() const
{
    if (m_pSecondary == NULL)
        return m_SecondaryId;
    const UMLPackage *typePkg = m_pSecondary->getUMLPackage();
    if (typePkg != NULL && typePkg != m_pUMLPackage)
        return m_pSecondary->getFullyQualifiedName();
    return m_pSecondary->getName();
}

/**
 * Sets the type of the UMLAttribute.
 *
 * @param type      Pointer to the UMLObject of the type.
 */
void UMLClassifierListItem::setType(UMLObject *type)
{
    if (m_pSecondary != type) {
        m_pSecondary = type;
        UMLObject::emitModified();
    }
}

/**
 * Sets the type name of the UMLClassifierListItem.
 * DEPRECATED - use setType() instead.
 *
 * @param type      The type name of the UMLClassifierListItem.
 */
void UMLClassifierListItem::setTypeName(const QString &type)
{
    if (type.isEmpty() || type == "void") {
        m_pSecondary = NULL;
        m_SecondaryId.clear();
        return;
    }
    UMLDoc *pDoc = UMLApp::app()->getDocument();
    m_pSecondary = pDoc->findUMLObject(type);
    if (m_pSecondary == NULL) {
        // Make data type for easily identified cases
        if (Model_Utils::isCommonDataType(type) || type.contains('*')) {
            m_pSecondary = Object_Factory::createUMLObject(Uml::ot_Datatype, type);
            uDebug() << "created datatype for " << type;
        } else {
            m_SecondaryId = type;
        }
    }
    UMLObject::emitModified();
}


#include "classifierlistitem.moc"
