/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ACTOR_H
#define ACTOR_H

#include "umlcanvasobject.h"

/**
 *	This class contains the non-graphical information required for a UML Actor.
 *	This class inherits from @ref UMLCanvasObject which contains most of the information.
 *	The @ref UMLDoc class creates instances of this type.  All Actors will need a unique
 *	id.  This will be given by the @ref UMLDoc class.  If you don't leave it up to the @ref UMLDoc
 *	class then call the method @ref UMLDoc::getUniqueID to get a unique id.
 *
 *	@short Information for a non-graphical UML Actor.
 *	@author Paul Hensgen <phensgen@techie.com>
 *	@see	UMLCanvasObject
 */
class UMLActor : public UMLCanvasObject {
public:
	/**
	 * Constructs an Actor.
	 * @param parent		The parent of this Actor.
	 * @param name		The name of the Actor.
	 * @param id				The unique id to assign to this Actor.
	 */
	UMLActor(QObject* parent, QString Name, int id);

	/**
	 * Constructs an Actor.
	 * @param parent	The parent of this Actor.
	 */
	UMLActor(QObject* parent);

	/**
	 *	Standard deconstructor.
	 */
	~UMLActor();

	/**
	 *	Initializes key variables of the class.
	 */
	virtual void init();

	/**
	 * Creates the <UML:Actor> element
	 */
	bool saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

	/**
	 * Loads the <UML:Actor> element
	 */
	bool loadFromXMI( QDomElement & element );
};

#endif
