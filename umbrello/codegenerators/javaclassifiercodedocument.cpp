
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
 *      Date   : Mon Jun 23 2003
 */

/**
  We carve the Java document up into sections as follows:

  * header
  * package declaration
  * import statements
  * class declaration
  *   guts of the class (e.g. field decl, accessor methods, operations, dependant classes)
*/

#include <kdebug.h>
#include <kdebug.h>
#include <qregexp.h>

#include "javaclassifiercodedocument.h"
#include "javacodegenerator.h"
#include "javacodecomment.h"
#include "javaclassdeclarationblock.h"
#include "javacodeclassfielddeclarationblock.h"
#include "javagetaccessormethod.h"
#include "javasetaccessormethod.h"
#include "javaaddaccessormethod.h"
#include "javaremoveaccessormethod.h"
#include "javalistaccessormethod.h"
#include "javacodeoperation.h"

// Constructors/Destructors
//  

JavaClassifierCodeDocument::JavaClassifierCodeDocument ( UMLClassifier * concept , JavaCodeGenerator *parent) 
   : ClassifierCodeDocument (concept, (CodeGenerator *) parent) { 
	init();
};

JavaClassifierCodeDocument::~JavaClassifierCodeDocument ( ) { };

//  
// Methods
//  

// Accessor methods
//

// Make it easier on ourselves 
JavaCodeGenerationPolicy * JavaClassifierCodeDocument::getJavaPolicy() {
	CodeGenerator *g = getParentGenerator();
        JavaCodeGenerationPolicy * policy = dynamic_cast<JavaCodeGenerationPolicy*>(g->getPolicy());
        return policy;
}

/**
 * Get the dialog widget which allows user interaction with the object parameters.
 * @return      CodeDocumentDialog
 */
/*
CodeDocumentDialog JavaClassifierCodeDocument::getDialog ( ) {

}
*/

bool JavaClassifierCodeDocument::forceDoc () {
        return getParentGenerator()->forceDoc();
}

// We overwritten by Java language implementation to get lowercase path
QString JavaClassifierCodeDocument::getPath ( )
{

        QString path = getPackage();

        // Replace all white spaces with blanks
        path.simplifyWhiteSpace();

        // Replace all blanks with underscore
        path.replace(QRegExp(" "), "_");

        path.replace(QRegExp("\\."),"/");

        path.lower();

        return path;

}


// Other methods
//  

QString JavaClassifierCodeDocument::capitalizeFirstLetter(QString string)
{
	CodeGenerator *g = getParentGenerator();
	JavaCodeGenerator * gen = dynamic_cast<JavaCodeGenerator *>(g);
	return gen->capitalizeFirstLetter(string);
}

QString JavaClassifierCodeDocument::getJavaClassName (QString name) {
	CodeGenerator *g = getParentGenerator();
	return capitalizeFirstLetter(g->cleanName(name));
}

// we will put the class 'guts' inside a hierarchical code block
JavaClassDeclarationBlock * JavaClassifierCodeDocument::getClassDecl ( )
{

        // So we see if it already exists, IF it *does* then we wont create a 
	// new one.
        JavaClassDeclarationBlock * codeBlock = (JavaClassDeclarationBlock *) findTextBlockByTag("classBlock");
	if(!codeBlock) {
		codeBlock = new JavaClassDeclarationBlock (this);
		codeBlock->setTag("classBlock");
	} 

        return codeBlock;

}

// add declaration blocks for the passed classfields
void JavaClassifierCodeDocument::declareClassFields (QPtrList<CodeClassField> & list ,
       				 HierarchicalCodeBlock * classDeclBlock )
{
     	for (CodeClassField * field = list.first(); field ; field = list.next())
	{
		CodeClassFieldDeclarationBlock * declBlock = field->getDeclarationCodeBlock();
		classDeclBlock->addTextBlock(declBlock); // wont add it IF its already present
        }
}

// Initialize this java classifier code document
void JavaClassifierCodeDocument::init ( ) {

	setFileExtension(".java");

	initCodeClassFields(); // we have to call here as .newCodeClassField is pure virtual in parent class 

	// this will call updateContent() as well as other things that sync our document.
        syncronize();
}

/**
 * @param       op
 */
// in the vannilla version, we just tack all operations on the end
// of the document
bool JavaClassifierCodeDocument::addCodeOperation (CodeOperation * op ) {

	if(!op->getParentOperation()->isConstructorOperation())
        	return operationsBlock->addTextBlock(op);
	else
        	return constructorBlock->addTextBlock(op);
}

CodeClassFieldDeclarationBlock * JavaClassifierCodeDocument::newDeclarationCodeBlock (CodeClassField * cf ) {
        return new JavaCodeClassFieldDeclarationBlock(cf);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return      CodeBlockWithComments
 */
CodeComment * JavaClassifierCodeDocument::newCodeComment ( ) {
        return new JavaCodeComment(this);
}

/**
 * create a new CodeAccesorMethod object belonging to this CodeDocument.
 * @return      CodeAccessorMethod
 */
CodeAccessorMethod * JavaClassifierCodeDocument::newCodeAccessorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type ) {
        CodeAccessorMethod * method = (CodeAccessorMethod*) NULL;
        switch (type) {
                case CodeAccessorMethod::GET:
                        method = new JavaGetAccessorMethod((JavaCodeClassField*)cf);
                        method->setOverallIndentationLevel(1);
                        break;
                case CodeAccessorMethod::SET:
                        method = new JavaSetAccessorMethod((JavaCodeClassField*)cf);
                        method->setOverallIndentationLevel(1);
                        break;
                case CodeAccessorMethod::LIST:
                        method = new JavaListAccessorMethod((JavaCodeClassField*)cf);
                        method->setOverallIndentationLevel(1);
                        break;
                case CodeAccessorMethod::REMOVE:
                        method = new JavaRemoveAccessorMethod((JavaCodeClassField*)cf);
                        method->setOverallIndentationLevel(1);
                        break;
                case CodeAccessorMethod::ADD:
                        method = new JavaAddAccessorMethod((JavaCodeClassField*)cf);
                        method->setOverallIndentationLevel(1);
                        break;
                default:
                        // perhaps this is a fatal condition??
                        kdWarning()<<"Error: cannot make accessor method with type:"<<type<<endl;
                        break;
        }
        return method;
}

CodeClassField * JavaClassifierCodeDocument::newCodeClassField ( UMLAttribute * at) {
        return new JavaCodeClassField(this,at);
}

CodeClassField * JavaClassifierCodeDocument::newCodeClassField ( UMLRole * role) {
        return new JavaCodeClassField(this,role);
}

/**
 * create a new CodeOperation object belonging to this CodeDocument.
 * @return      CodeOperation
 */
CodeOperation * JavaClassifierCodeDocument::newCodeOperation( UMLOperation * op) {
	return new JavaCodeOperation(this, op); 
}

/** set attributes of the node that represents this class
 * in the XMI document.
 */
void JavaClassifierCodeDocument::setAttributesOnNode ( QDomDocument & doc, QDomElement & docElement)
{

        // superclass call
        ClassifierCodeDocument::setAttributesOnNode(doc,docElement);

        // now set local attributes/fields -- none to set 

}

/** set the class attributes of this object from
 * the passed element node.
 */
void JavaClassifierCodeDocument::setAttributesFromNode ( QDomElement & root) 
{

	// superclass save
        ClassifierCodeDocument::setAttributesFromNode(root);

        // now set local attributes/fields -- none to set 

}

// Sigh. NOT optimal. The only reason that we need to have this
// is so we can create the JavaClassDeclarationBlock.
// would be better if we could create a handler interface that each
// codeblock used so all we have to do here is add the handler
// for "javaclassdeclarationblock"
void JavaClassifierCodeDocument::loadChildTextBlocksFromNode ( QDomElement & root)
{

        QDomNode tnode = root.firstChild();
        QDomElement telement = tnode.toElement();
        bool gotChildren = false;
        while( !telement.isNull() ) {
                QString nodeName = telement.tagName();

                if( nodeName == "textblocks" ) {

                        QDomNode node = telement.firstChild();
                        QDomElement element = node.toElement();

                        while( !element.isNull() ) {
                                QString name = element.tagName();

                                if( name == "javaclassdeclarationblock" ) {
					JavaClassDeclarationBlock * classDeclBlock = new JavaClassDeclarationBlock (this);
 					classDeclBlock->loadFromXMI(element);
					if(!addTextBlock(classDeclBlock))
                                                kdError()<<"Unable to add javaclassdeclaration block to :"<<this<<endl;
					else
                                                gotChildren= true;
                                } else
                                if( name == "codecomment" ) {
                                        CodeComment * block = newCodeComment();
                                        block->loadFromXMI(element);
                                        if(!addTextBlock(block))
                                                kdError()<<"Unable to add codeComment to :"<<this<<endl;
                                        else
                                                gotChildren= true;
                                } else
                                if( name == "codeaccessormethod" ||
                                    name == "declarationcodeblock"
                                  ) {
                                        QString acctag = element.attribute("tag","");
                                        // search for our method in the
                                        TextBlock * tb = findCodeClassFieldTextBlockByTag(acctag);
                                        if(!tb || !addTextBlock(tb, true))
                                                kdWarning()<<"Unable to add codeaccessormethod to:"<<this<<endl;
                                        else
                                                gotChildren= true;

                                } else
                                if( name == "codeblock" ) {
                                        CodeBlock * block = newCodeBlock();
                                        block->loadFromXMI(element);
                                        if(!addTextBlock(block))
                                                kdError()<<"Unable to add codeBlock to :"<<this<<endl;
                                        else
                                                gotChildren= true;
                                } else
                                if( name == "codeblockwithcomments" ) {
                                        CodeBlockWithComments * block = newCodeBlockWithComments();
                                        block->loadFromXMI(element);
                                        if(!addTextBlock(block))
                                                kdError()<<"Unable to add codeBlockwithcomments to:"<<this<<endl;
                                        else
                                                gotChildren= true;
                                } else
                                if( name == "header" ) {
                                       // do nothing.. this is treated elsewhere
                                } else
                                if( name == "hierarchicalcodeblock" ) {
                                        HierarchicalCodeBlock * block = newHierarchicalCodeBlock();
                                        block->loadFromXMI(element);
                                        if(!addTextBlock(block))
                                                kdError()<<"Unable to add hierarchicalcodeBlock to:"<<this<<endl;
                                        else
                                                gotChildren= true;
                                } else
                                if( name == "codeoperation" ) {
                                       // find the code operation by id
                                        QString id = element.attribute("parent_op","-1");
                                        UMLObject * obj = getParentGenerator()->getDocument()->findUMLObject(id.toInt());
                                        UMLOperation * op = dynamic_cast<UMLOperation*>(obj);
                                        if(op) {
                                                CodeOperation * block = newCodeOperation(op);
                                                block->loadFromXMI(element);
                                                if(addTextBlock(block))
                                                        gotChildren= true;
                                        } else
                                              kdWarning()<<"Unable to add codeoperation to:"<<this<<endl;
                                } else
                                        kdError()<<" LoadFromXMI: Got strange tag in text block stack:"<<name<<endl;

                                node = element.nextSibling();
                                element = node.toElement();
                        }
                        break;
                }

                tnode = telement.nextSibling();
                telement = tnode.toElement();
        }

        if(!gotChildren)
                kdWarning()<<" loadFromXMI : Warning: unable to initialize any child blocks in:"<<this<<endl;

}

QString JavaClassifierCodeDocument::scopeToJavaDecl(Uml::Scope scope)
{
        QString scopeString;
        switch(scope)
        {
                case Uml::Public:
                        scopeString = "public";
                        break;
                case Uml::Protected:
                        scopeString = "protected";
                        break;
                case Uml::Private:
                default:
                        scopeString = "private";
                        break;
        }
        return scopeString;
}

// This method will cause the class to rebuild its text representation.
// based on the parent classifier object.
// For any situation in which this is called, we are either building the code
// document up, or replacing/regenerating the existing auto-generated parts. As
// such, we will want to insert everything we resonablely will want
// during creation. We can set various parts of the document (esp. the
// comments) to appear or not, as needed.
void JavaClassifierCodeDocument::updateContent( ) 
{
      	// Gather info on the various fields and parent objects of this class...
	UMLClassifier * c = getParentClassifier();
	CodeGenerator * g = getParentGenerator();
	// JavaCodeGenerator * gen = dynamic_cast<JavaCodeGenerator*>(g);
	JavaCodeGenerator * gen = (JavaCodeGenerator*)g;

	// first, set the global flag on whether or not to show classfield info 
	QPtrList<CodeClassField> * cfList = getCodeClassFieldList();
	for(CodeClassField * field = cfList->first(); field; field = cfList->next())
		field->setWriteOutMethods(gen->getAutoGenerateAccessors());

      	// attribute-based ClassFields
      	// we do it this way to have the static fields sorted out from regular ones
        QPtrList<CodeClassField> staticAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, true);
        QPtrList<CodeClassField> attribClassFields = getSpecificClassFields (CodeClassField::Attribute, false);
      	// association-based ClassFields 
      	// dont care if they are static or not..all are lumped together
        QPtrList<CodeClassField> plainAssocClassFields = getSpecificClassFields ( CodeClassField::PlainAssociation );
        QPtrList<CodeClassField> aggregationClassFields = getSpecificClassFields ( CodeClassField::Aggregation );
        QPtrList<CodeClassField> compositionClassFields = getSpecificClassFields ( CodeClassField::Composition );

        bool hasOperationMethods = c->getFilteredOperationsList()->last() ? true : false;
        QString endLine = gen->getNewLineEndingChars(); // a shortcut..so we dont have to call this all the time

	//
	// START GENERATING CODE/TEXT BLOCKS and COMMENTS FOR THE DOCUMENT
	//

	//
        // PACKAGE CODE BLOCK
        //
	QString packageText = getPackage().isEmpty() ? "" : "package "+getPackage()+";";
	addOrUpdateTaggedCodeBlockWithComments("packages", packageText, "", 0, false);

       	// IMPORT CODEBLOCK 
       	//
       	// Q: Why all utils? Isnt just List and Vector the only classes we are using?
       	// A: doesn't matter at all; its more readable to just include '*' and java compilers
       	//    don't slow down or anything. (TZ)
       	QString importStatement = "";
       	if ( hasObjectVectorClassFields() )
       		importStatement.append("import java.util.*;"+endLine);

   	//only import classes in a different package from this class
       	UMLClassifierList imports;
       	QMap<UMLClassifier *,QString> *packageMap = new QMap<UMLClassifier*,QString>; // so we dont repeat packages

	gen->findObjectsRelated(c,imports);
       	for(UMLClassifier *con = imports.first(); con ; con = imports.next())
       	if ((con->getPackage() != c->getPackage())
       	        && !(packageMap->contains(con)))
	{
       		packageMap->insert(con,con->getPackage());
               	importStatement.append("import "+con->getPackage()+"."+gen->cleanName(con->getName())+";"+endLine);
	}
        // now, add/update the imports codeblock
	addOrUpdateTaggedCodeBlockWithComments("imports", importStatement, "", 0, false);

	// CLASS DECLARATION BLOCK
	//

	// get the declaration block. If its not already present, add it too
        JavaClassDeclarationBlock * classDeclBlock = getClassDecl ( );
	addTextBlock(classDeclBlock); // note: wont add if already present

	// NOW create document in sections..
	// now we want to populate the body of our class
	// our layout is the following general groupings of code blocks:

	// start java classifier document

	// header comment

	// package code block

	// import code block

	// class declaration 

	//   section:
	//   - class field declaration section comment
	//   - class field declarations (0+ codeblocks)
 
	//   section:
	//   - methods section comment

	//     sub-section: constructor ops
	//     - constructor method section comment
	//     - constructor methods (0+ codeblocks)

	//     sub-section: accessors
	//     - accessor method section comment
	//     - static accessor methods (0+ codeblocks)
	//     - non-static accessor methods (0+ codeblocks)

	//     sub-section: non-constructor ops 
	//     - operation method section comment
	//     - operations (0+ codeblocks)

	// end class declaration 

	// end java classifier document


	// Q: Why use the more complicated scheme of arranging code blocks within codeblocks?
	// A: This will allow us later to preserve the format of our document so that if 
	//    codeblocks are added, they may be easily added in the correct place, rather than at
	//    the end of the document, or by using a difficult algorithm to find the location of
	//    the last appropriate code block sibling (which may not exist.. for example user adds
	//    a constructor operation, but there currently are no constructor code blocks 
	//    within the document). 

	//
	// * CLASS FIELD declaration section
	//

	// get/create the field declaration code block
        HierarchicalCodeBlock * fieldDeclBlock = classDeclBlock->getHierarchicalCodeBlock("fieldsDecl", "Fields", 1);

        // Update the comment: we only set comment to appear under the following conditions
        CodeComment * fcomment = fieldDeclBlock->getComment();
        if (!forceDoc() && !hasClassFields() )
		fcomment->setWriteOutText(false);
	else
		fcomment->setWriteOutText(true);

	// now actually declare the fields within the appropriate HCodeBlock
	declareClassFields(staticAttribClassFields, fieldDeclBlock);
	declareClassFields(attribClassFields, fieldDeclBlock);
	declareClassFields(plainAssocClassFields, fieldDeclBlock);
	declareClassFields(aggregationClassFields, fieldDeclBlock);
	declareClassFields(compositionClassFields, fieldDeclBlock);

	//
        // METHODS section
        //

        // get/create the method codeblock
        HierarchicalCodeBlock * methodsBlock = classDeclBlock->getHierarchicalCodeBlock("methodsBlock", "Methods", 1);

        // Update the section comment
        CodeComment * methodsComment = methodsBlock->getComment();
        // set conditions for showing this comment
        if (!forceDoc() && !hasClassFields() && !hasOperationMethods)
                methodsComment->setWriteOutText(false);
        else
                methodsComment->setWriteOutText(true);

        // METHODS sub-section : constructor methods
        //

	// get/create the constructor codeblock
        HierarchicalCodeBlock * constBlock = methodsBlock->getHierarchicalCodeBlock("constructorMethods", "Constructors", 1);
	constructorBlock = constBlock; // record this codeblock for later, when operations are updated

	// special condiions for showing comment: only when autogenerateding empty constructors
	// Although, we *should* check for other constructor methods too
	CodeComment * constComment = constBlock->getComment();
	if (!forceDoc() && (parentIsInterface() || !gen->getAutoGenerateConstructors()))
		constComment->setWriteOutText(false);
	else
		constComment->setWriteOutText(true);

	// add/get the empty constructor 
	QString JavaClassName = getJavaClassName(c->getName()); 
	QString emptyConstStatement = "public "+JavaClassName+" ( ) { }";
	CodeBlockWithComments * emptyConstBlock = 
		constBlock->addOrUpdateTaggedCodeBlockWithComments("emptyconstructor", emptyConstStatement, "Empty Constructor", 1, false);
	// Now, as an additional condition we only show the empty constructor block 
	// IF it was desired to be shown
	if(!parentIsInterface() && gen->getAutoGenerateConstructors())
		emptyConstBlock->setWriteOutText(true);
	else
		emptyConstBlock->setWriteOutText(false);

        // METHODS subsection : ACCESSOR METHODS 
	//

        // get/create the accessor codeblock
        HierarchicalCodeBlock * accessorBlock = methodsBlock->getHierarchicalCodeBlock("accessorMethods", "Accessor Methods", 1);

	// set conditions for showing section comment
        CodeComment * accessComment = accessorBlock->getComment();
        if (!forceDoc() && !hasClassFields())
		accessComment->setWriteOutText(false);
	else
		accessComment->setWriteOutText(true);

	// now, 2 sub-sub sections in accessor block
        // add/update accessor methods for attributes
        HierarchicalCodeBlock * staticAccessors = accessorBlock->getHierarchicalCodeBlock("staticAccessorMethods", "", 1);
	staticAccessors->getComment()->setWriteOutText(false); // never write block comment
	staticAccessors->addCodeClassFieldMethods(staticAttribClassFields); 
	staticAccessors->addCodeClassFieldMethods(attribClassFields);

        // add/update accessor methods for associations
        HierarchicalCodeBlock * regularAccessors = accessorBlock->getHierarchicalCodeBlock("regularAccessorMethods", "", 1);
	regularAccessors->getComment()->setWriteOutText(false); // never write block comment
	regularAccessors->addCodeClassFieldMethods(plainAssocClassFields);
	regularAccessors->addCodeClassFieldMethods(aggregationClassFields);
	regularAccessors->addCodeClassFieldMethods(compositionClassFields);

        // METHODS subsection : Operation methods (which arent constructors) 
	//

        // get/create the operations codeblock
        HierarchicalCodeBlock * opsBlock = methodsBlock->getHierarchicalCodeBlock("operationMethods", "Operations", 1);
	operationsBlock = opsBlock; // record this so that later operations go in right place

	// set conditions for showing section comment
	CodeComment * ocomment = opsBlock->getComment();
        if (!forceDoc() && !hasOperationMethods )
		ocomment->setWriteOutText(false);
	else
		ocomment->setWriteOutText(true);

}

