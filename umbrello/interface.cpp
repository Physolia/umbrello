/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "interface.h"
#include "association.h"
#include "operation.h"
#include "clipboard/idchangelog.h"
#include <kdebug.h>
#include <klocale.h>

UMLInterface::UMLInterface(QObject* parent, QString name, int id) : UMLCanvasObject(parent, name, id) {
	init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
UMLInterface::UMLInterface(QObject* parent) : UMLCanvasObject(parent) {
	init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
UMLInterface::~UMLInterface() {
  	m_OpsList.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
UMLObject* UMLInterface::addOperation(QString name, int id) {
	UMLOperation* operation = new UMLOperation(this, name, id);
	m_OpsList.append(operation);
	return operation;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMLInterface::addOperation(UMLOperation* Op) {
	m_OpsList.append( Op );
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMLInterface::addOperation(UMLOperation* Op, IDChangeLog* Log) {
	QString name = (QString)Op->getName();
	if( findChildObject( Uml::ot_Operation, name).count() == 0 ) {
		Op->parent()->removeChild( Op );
		this->insertChild( Op );
		m_OpsList.append( Op );
		return true;
	} else if( Log ) {
		Log->removeChangeByNewID( Op->getID() );
		delete Op;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int UMLInterface::removeOperation(UMLObject *o) {
	if(!m_OpsList.remove((UMLOperation *)o)) {
		kdWarning() << "can't find opp given in list" << endl;
		return -1;
	}
	return m_OpsList.count();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
QPtrList<UMLObject> UMLInterface::findChildObject(UMLObject_Type t , QString n) {
  	QPtrList<UMLObject> list;
 	if (t == ot_Association) {
		return UMLCanvasObject::findChildObject(t, n);
	} else if (t == ot_Operation) {
		UMLOperation * obj=0;
		for(obj=m_OpsList.first();obj != 0;obj=m_OpsList.next()) {
			if(obj->getBaseType() == t && obj->getName() == n)
				list.append( obj );
		}
	} else {
		kdWarning() << "finding child object which isn't association or op for interface" << endl;
	}
	return list;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
UMLObject* UMLInterface::findChildObject(int id) {
	UMLOperation * o=0;
	for(o=m_OpsList.first();o != 0;o=m_OpsList.next()) {
		if(o->getID() == id)
			return o;
	}
	return UMLCanvasObject::findChildObject(id);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMLInterface::serialize(QDataStream *s, bool archive, int fileversion) {
	bool status = UMLObject::serialize(s, archive, fileversion);
	if(!status)
		return status;
	// Note: The m_AssocsList is filled by UMLDoc::serialize().
	// UMLDoc::serialize() serializes the UMLInterfaces before the
	// UMLAssociations.
	if(archive) {
		*s << m_OpsList.count();
		//save operations
		UMLOperation * o=0;
		for(o=m_OpsList.first();o != 0;o=m_OpsList.next()) {
			o->serialize(s, archive, fileversion);
		}
	} else {
		int opCount, type;
		//load operations
		*s >> opCount;
		//load operations
		for(int i=0;i<opCount;i++) {
			*s >> type;
			UMLOperation *o = new UMLOperation(this);
			o->serialize(s, archive, fileversion);
			m_OpsList.append(o);
		}
	}//end else
	return status;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void UMLInterface::init() {
	m_BaseType = ot_Interface;
	m_OpsList.clear();
	m_OpsList.setAutoDelete(true);
	setStereotype( i18n("interface") );
}

long UMLInterface::getClipSizeOf() {
	long l_size = UMLObject::getClipSizeOf();
	//  Q_UINT32 tmp; //tmp is used to calculate the size of each serialized null string
	l_size += sizeof(m_OpsList.count());

	UMLOperation * o=0;
	for(o=m_OpsList.first();o != 0;o=m_OpsList.next()) {
		l_size += o->getClipSizeOf();
	}

	return l_size;
}

bool UMLInterface::operator==( UMLInterface & rhs ) {
	if ( m_OpsList.count() != rhs.m_OpsList.count() ) {
		return false;
	}
	return UMLCanvasObject::operator==(rhs);
}

bool UMLInterface::saveToXMI( QDomDocument & qDoc, QDomElement & qElement ) {
	QDomElement classElement = qDoc.createElement("UML:Interface");
	bool status = UMLObject::saveToXMI( qDoc, classElement );
	//save operations
	UMLOperation* pOp = 0;
	for ( pOp = m_OpsList.first(); pOp != 0; pOp = m_OpsList.next() ) {
		pOp->saveToXMI(qDoc, classElement);
	}
	qElement.appendChild( classElement );
	return status;
}

bool UMLInterface::loadFromXMI( QDomElement & element ) {
	if( !UMLObject::loadFromXMI(element) ) {
		return false;
	}

	QDomNode node = element.firstChild();
	QDomElement tempElement = node.toElement();
	while( !tempElement.isNull() ) {
		QString tag = tempElement.tagName();
		if (tag == "UML:Operation") {
			UMLOperation* pOp = new UMLOperation(this);
			if( !pOp->loadFromXMI(tempElement) ) {
				return false;
			}
			m_OpsList.append(pOp);
		}
		node = node.nextSibling();
		tempElement = node.toElement();
	}//end while
	return true;
}

int UMLInterface::operations() {
	return m_OpsList.count();
}

QPtrList<UMLOperation>* UMLInterface::getOpList() {
	return &m_OpsList;
}
