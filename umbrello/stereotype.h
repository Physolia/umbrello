/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "classifierlistitem.h"

/**
 *	This class is used to set up information for an stereotype.
 *	Stereotypes are used essentially as comments to class together
 *	attributes and operations etc.
 *
 *	@short Sets up stereotype information.
 *	@author Jonathan Riddell
 * 	Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */

class UMLStereotype : public UMLClassifierListItem {
public:
	/**
	 *	Sets up an stereotype.
	 *
	 *	@param	parent		The parent of this UMLStereotype.
	 *	@param	name		The name of this UMLStereotype.
	 *	@param	id		The unique id given to this UMLStereotype.
	 *	@param listType		The list which this stereotype is part of (attribute, operation etc)
	 */
	UMLStereotype(QObject* parent, QString name, int id, UMLObject_Type listType);

	/**
	 *	Sets up an stereotype.
	 *
	 *	@param	parent		The parent of this UMLStereotype.
	 */
	UMLStereotype(QObject* parent);

	/**
	 * 	Overloaded '==' operator
	 */
	bool operator==(UMLStereotype &rhs);

	/**
	 *	Standard deconstructor.
	 */
	~UMLStereotype();

	/**
	 *	Returns a string representation of the UMLStereotype.
	 *	@param	sig	If true will show the stereotype type and inital value.
	 *
	 *	@return	Returns a string representation of the UMLStereotype.
	 */
	QString toString(Signature_Type sig);

	/**
	 * Saves a <stereotype> tag to the dom tree
	 */
	bool saveToXMI(QDomDocument& qDoc, QDomElement& qElement);

	/**
	 * Loads a <stereotype> tag from the document tree
	 */
	bool loadFromXMI(QDomElement& element);

	/**
	 * Display the properties configuration dialogue for the stereotype (just a line edit).
	 */
	bool showPropertiesDialogue(QWidget* parent);

	/**
	 * Return the steretype name surrounded by double angle brackets
	 */
	virtual QString getShortName();
private:
	/**
	 * The list this stereotype is part of (operation, attribute etc)
	 */
	UMLObject_Type m_listType;
};

#endif
