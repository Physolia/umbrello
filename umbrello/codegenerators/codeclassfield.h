/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CODECLASSFIELD_H
#define CODECLASSFIELD_H

#include "codeaccessormethodlist.h"
#include "codeclassfielddeclarationblock.h"
#include "codeparameter.h"
#include "codeaccessormethod.h"

class ClassifierCodeDocument;
class UMLAttribute;
class UMLObject;
class UMLRole;

/**
 * class CodeClassField
 * a special type of parameter.. occurs on class declarations.
 */
class CodeClassField : public CodeParameter
{
    Q_OBJECT
public:

    enum ClassFieldType { Attribute, PlainAssociation, Self, Aggregation, Composition, Unknown_Assoc };

    /**
     * Constructors
     */
    CodeClassField ( ClassifierCodeDocument * parentDoc , UMLAttribute * attrib );
    CodeClassField ( ClassifierCodeDocument * parentDoc , UMLRole * role);

    /**
     * Finish off initializations of the object.
     * This is necessary as a separate method because we cannot call
     * virtual methods that are reimplemented in a language specific class
     * during our own construction (the own object is not finished being
     * constructed and therefore the C++ dispatch mechanism does not yet
     * work as expected.)
     */
    void finishInitialization();

    // CodeClassField ( ClassifierCodeDocument * doc , UMLRole role);

    /**
     * Empty Destructor
     */
    virtual ~CodeClassField ( );

//    /**
//     * Get the value of m_dialog
//     * @return the value of m_dialog
//     */
//    CodeClassFieldDialog getDialog ( );

    // Determine if this cf is attribute or some type of association
    QString getTypeName ( );

    /**
     * @return  CodeClassFieldDeclarationBlock representing the declaration statement of this class field
     */
    CodeClassFieldDeclarationBlock * getDeclarationCodeBlock( );

    /**
     * Get the list of Method objects held by m_methodVector
     * @return CodeAccessorMethodList list of Method objects held by
     * m_methodVector
     */
    CodeAccessorMethodList getMethodList() const;

    /**
     * Utility method to allow finding particular accessor method of this
     * code class field by its type identifier.
     */
    CodeAccessorMethod * findMethodByType(CodeAccessorMethod::AccessorType type, int role_id = -1);

    /**
     * Determine whether the parent object in this classfield indicates that it is
     * a single variable or a List (Vector). One day this will be done correctly with special
     * multiplicity object.
     */
    bool fieldIsSingleValue ( );

    /**
     * Get the type of classfield this is.
     */
    ClassFieldType getClassFieldType() const;

    // quick utility call to figure out if parent is an attribute or not
    bool parentIsAttribute ( ) const;

    // get the type of object that will be added/removed from lists
    // of objects (as per specification of associations)
    QString getListObjectType();

    /**
     * Determine if we will *allow* methods to be viewable.
     * this flag is often used to toggle autogeneration of accessor
     * methods in the code class field.
     */
    bool getWriteOutMethods () const;

    /**
     * Determine if we will *allow* methods to be viewable.
     * this flag is often used to toggle autogeneration of accessor
     * methods in the code class field.
     */
    void setWriteOutMethods( bool val);

    /**
     * Find the minimum number of things that can occur in an association
     * If mistakenly called on attribute CF's the default value of is "0"
     * is returned. Similarly, if the association (role) CF doesn't have a multiplicty
     * 0 is returned.
     */
    int minimumListOccurances( );

    /**
     * Find the maximum number of things that can occur in an association
     * If mistakenly called on attribute CF's the default value of is "1"
     * is returned. If the association (role) CF doesn't have a multiplicty
     * or has a "*" specified then '-1' (unbounded) is returned.
     */
    int maximumListOccurances( );

    /**
     * Save the XMI representation of this object
     */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    /**
     * load params from the appropriate XMI element node.
     */
    virtual void loadFromXMI ( QDomElement & root );

    /**
     * Force the synchronization of the content (methods and declarations)
     * of this class field.
     */
    virtual void synchronize ();

    /**
     * Updates the status of the accessor methods
     * as to whether or not they should be written out.
     */
    void updateContent();

protected:

    /**
     * Set the parent UMLobject appropriately.
     */
    void setParentUMLObject (UMLObject * obj);

    //  CodeClassFieldDialog * m_dialog;

    /**
     * A little utility method to make life easier for code document programmers
     */
    QString cleanName(const QString &name);

    /**
     * Another utility method to make life easier for code document programmers
     * this one fixes the initial declared value of string attributes so that if
     * its empty or lacking quotations, it comes out as ""
     */
    QString fixInitialStringDeclValue(const QString& val, const QString &type);

    // set the list class name
    void setListClassName ( const QString &className );

    /**
     * Add a Method object to the m_methodVector List
     */
    bool addMethod ( CodeAccessorMethod * add );

    QString getUMLObjectName(UMLObject *obj);

    /**
     * Remove a Method object from m_methodVector List
     */
    bool removeMethod ( CodeAccessorMethod * remove);

private:

    QString m_listClassName;
    ClassFieldType m_classFieldType;
    CodeClassFieldDeclarationBlock * m_declCodeBlock;
    CodeAccessorMethodList m_methodVector; // the list of methods related to this codeclassfield
    bool m_parentIsAttribute;

    /**
     * This flag tells if we want the methods to have the possibility
     * of being written out. IF the value is false, then all methods
     * are never written out.
     */
    bool m_writeOutMethods;

    /**
     * Set attributes of the node that represents this class
     * in the XMI document.
     */
    virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

    /**
     * Set the class attributes of this object from
     * the passed element node.
     */
    virtual void setAttributesFromNode ( QDomElement & element);

    /**
     * Init class fields.
     */
    void initFields(bool inConstructor = false);

    // initialize the accessor methods for this field
    void initAccessorMethods();

signals:

    void modified ();

};

#endif // CODECLASSFIELD_H
