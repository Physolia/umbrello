/***************************************************************************
 *   cpptree2uml.cpp - based on kdevelop/languages/cpp/store_walker.cpp,   *
 *		     Copyright (C) 2003 by Roberto Raggi		 *
 *   Copyright (C) 2004 by Oliver Kellogg <okellogg@users.sourceforge.net> *
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#include "cpptree2uml.h"
#include "ast_utils.h"
#include "../classimport.h"
#include "../attribute.h"
#include "../operation.h"
#include "../association.h"
#include "../enum.h"
#include "../umldoc.h"
#include "../uml.h"
#include "urlutil.h"

#include <kdebug.h>
#include <qfileinfo.h>
#include <qdir.h>

CppTree2Uml::CppTree2Uml( const QString& fileName, ClassImport* store )
    : m_importer( store ), m_anon( 0 )
{
    m_fileName = URLUtil::canonicalPath(fileName);
}

CppTree2Uml::~CppTree2Uml()
{
}

void CppTree2Uml::parseTranslationUnit( TranslationUnitAST* ast )
{
    m_currentScope.clear();
    m_currentNamespace.clear();
    m_currentClass.clear();

    m_currentAccess = Uml::Public;
    m_inSlots = false;
    m_inSignals = false;
    m_inStorageSpec = false;
    m_inTypedef = false;
    m_currentDeclarator = 0;
    m_anon = 0;

    TreeParser::parseTranslationUnit( ast );
}

void CppTree2Uml::parseNamespace( NamespaceAST* ast )
{
    if( !m_currentClass.isEmpty() ){
	kdDebug() << "CppTree2Uml::parseNamespace: **error** !!" << endl;
	return;
    }

    QString nsName;
    if( !ast->namespaceName() || ast->namespaceName()->text().isEmpty() ){
	QFileInfo fileInfo( m_fileName );
	QString shortFileName = fileInfo.baseName();

	nsName.sprintf( "(%s_%d)", shortFileName.local8Bit().data(), m_anon++ );
    } else {
	nsName = ast->namespaceName()->text();
    }

    kdDebug() << "CppTree2Uml::parseNamespace: " << nsName << endl;
    UMLObject * o = m_importer->createUMLObject( Uml::ot_Package, nsName, "",
						 m_currentNamespace.top() );
    UMLPackage *ns = (UMLPackage *)o;
    m_currentScope.push_back( nsName );
    m_currentNamespace.push( ns );

    TreeParser::parseNamespace( ast );

    m_currentNamespace.pop();
    m_currentScope.pop_back();
}

void CppTree2Uml::parseTypedef( TypedefAST* ast )
{
#if 0
    DeclaratorAST* oldDeclarator = m_currentDeclarator;

    if( ast && ast->initDeclaratorList() && ast->initDeclaratorList()->initDeclaratorList().count() > 0 ) {
	    QPtrList<InitDeclaratorAST> lst( ast->initDeclaratorList()->initDeclaratorList() );
	    m_currentDeclarator = lst.at( 0 )->declarator();
    }

    m_inTypedef = true;

    TreeParser::parseTypedef( ast );

    m_inTypedef = false;
    m_currentDeclarator = oldDeclarator;
#else
    TypeSpecifierAST* typeSpec = ast->typeSpec();
    InitDeclaratorListAST* declarators = ast->initDeclaratorList();

    if( typeSpec && declarators ){
	QString typeId;

	if( typeSpec->name() )
	    typeId = typeSpec->name()->text();

	QPtrList<InitDeclaratorAST> l( declarators->initDeclaratorList() );
	QPtrListIterator<InitDeclaratorAST> it( l );

	InitDeclaratorAST* initDecl = 0;
	while( 0 != (initDecl = it.current()) ){

	    QString type, id;
	    if( initDecl->declarator() ){
	       type = typeOfDeclaration( typeSpec, initDecl->declarator() );

	       DeclaratorAST* d = initDecl->declarator();
	       while( d->subDeclarator() ){
		   d = d->subDeclarator();
	       }

	       if( d->declaratorId() )
		  id = d->declaratorId()->text();
	    }
	    kdDebug() << "CppTree2Uml::parseTypedef: name=" << id << ", type=" << type << endl;

	    UMLObject * o = m_importer->createUMLObject( Uml::ot_Class, id, "",
							 m_currentNamespace.top() );
	    o->setStereotype( "typedef" );

	    ++it;
	}

    }
#endif
}

void CppTree2Uml::parseTemplateDeclaration( TemplateDeclarationAST* ast )
{
    if( ast->declaration() )
	TreeParser::parseDeclaration( ast->declaration() );

    TreeParser::parseTemplateDeclaration( ast );
}

void CppTree2Uml::parseSimpleDeclaration( SimpleDeclarationAST* ast )
{
    TypeSpecifierAST* typeSpec = ast->typeSpec();
    InitDeclaratorListAST* declarators = ast->initDeclaratorList();

    if( typeSpec )
	parseTypeSpecifier( typeSpec );

    if( declarators ){
	QPtrList<InitDeclaratorAST> l = declarators->initDeclaratorList();

	QPtrListIterator<InitDeclaratorAST> it( l );
	while( it.current() ){
	    parseDeclaration(  ast->functionSpecifier(), ast->storageSpecifier(), typeSpec, it.current() );
	    ++it;
	}
    }
}

void CppTree2Uml::parseFunctionDefinition( FunctionDefinitionAST* ast )
{
    TypeSpecifierAST* typeSpec = ast->typeSpec();
    GroupAST* funSpec = ast->functionSpecifier();
    GroupAST* storageSpec = ast->storageSpecifier();

    if( !ast->initDeclarator() )
	return;

    DeclaratorAST* d = ast->initDeclarator()->declarator();

    if( !d->declaratorId() )
	return;

    bool isFriend = false;
    bool isVirtual = false;
    bool isStatic = false;
    bool isInline = false;

    if( funSpec ){
	QPtrList<AST> l = funSpec->nodeList();
	QPtrListIterator<AST> it( l );
	while( it.current() ){
	    QString text = it.current()->text();
	    if( text == "virtual" ) isVirtual = true;
	    else if( text == "inline" ) isInline = true;
	    ++it;
	}
    }

    if( storageSpec ){
	QPtrList<AST> l = storageSpec->nodeList();
	QPtrListIterator<AST> it( l );
	while( it.current() ){
	    QString text = it.current()->text();
	    if( text == "friend" ) isFriend = true;
	    else if( text == "static" ) isStatic = true;
	    ++it;
	}
    }

    QString id = d->declaratorId()->unqualifiedName()->text().stripWhiteSpace();

    if (m_currentClass.top() == NULL) {
        kdDebug() << "CppTree2Uml::parseFunctionDefinition: nothing on m_currentClass"
		  << endl;
	return;
    }
    QString returnType = typeOfDeclaration( typeSpec, d );
    UMLAttributeList parList;
    UMLOperation *m = m_importer->insertMethod( m_currentClass.top(),
						(Uml::Scope)m_currentAccess, id,
						returnType, isStatic,
						false,    // isAbstract
						"",       // doc
						&parList);
    parseFunctionArguments( d, m );

/* For reference, Kdevelop does some more:
    method->setFileName( m_fileName );
    if( m_inSignals )
	method->setSignal( true );
    if( m_inSlots )
	method->setSlot( true );
 */
}

void CppTree2Uml::parseClassSpecifier( ClassSpecifierAST* ast )
{
    int oldAccess = m_currentAccess;
    bool oldInSlots = m_inSlots;
    bool oldInSignals = m_inSignals;

    QString kind = ast->classKey()->text();
    if( kind == "class" )
	m_currentAccess = Uml::Private;
    else
	m_currentAccess = Uml::Public;
    m_inSlots = false;
    m_inSignals = false;

    QString className;
    if( !ast->name() && m_currentDeclarator && m_currentDeclarator->declaratorId() ) {
	className = m_currentDeclarator->declaratorId()->text().stripWhiteSpace();
    } else if( !ast->name() ){
	QFileInfo fileInfo( m_fileName );
	QString shortFileName = fileInfo.baseName();
	className.sprintf( "(%s_%d)", shortFileName.local8Bit().data(), m_anon++ );
    } else {
	className = ast->name()->unqualifiedName()->text().stripWhiteSpace();
    }
    kdDebug() << "CppTree2Uml::parseClassSpecifier: name=" << className << endl;

    if( !scopeOfName( ast->name(), QStringList() ).isEmpty() ){
	kdDebug() << "skip private class declarations" << endl;
	return;
    }

    UMLObject * o = m_importer->createUMLObject( Uml::ot_Class, className, "",
						 m_currentNamespace.top() );
    UMLClass *klass = (UMLClass *)o;

    if ( ast->baseClause() )
	parseBaseClause( ast->baseClause(), klass );

    m_currentScope.push_back( className );
    m_currentClass.push( klass );

    TreeParser::parseClassSpecifier( ast );

    m_currentClass.pop();

    m_currentScope.pop_back();

    m_currentAccess = oldAccess;
    m_inSlots = oldInSlots;
    m_inSignals = oldInSignals;
}

void CppTree2Uml::parseEnumSpecifier( EnumSpecifierAST* ast )
{
    QString typeName = ast->name()->unqualifiedName()->text().stripWhiteSpace();
    UMLObject *o = m_importer->createUMLObject( Uml::ot_Enum, typeName, "", /* comment */
						m_currentNamespace.top() );
    UMLEnum *e = static_cast<UMLEnum*>( o );
    UMLDoc *umldoc = UMLApp::app()->getDocument();

    QPtrList<EnumeratorAST> l = ast->enumeratorList();
    QPtrListIterator<EnumeratorAST> it( l );
    while ( it.current() ) {
	QString enumLiteral = it.current()->id()->text();
	e->addEnumLiteral( enumLiteral, umldoc->getUniqueID() );
	++it;
    }
}

void CppTree2Uml::parseDeclaration( GroupAST* funSpec, GroupAST* storageSpec,
				    TypeSpecifierAST* typeSpec, InitDeclaratorAST* decl )
{
    if( m_inStorageSpec )
	    return;

    DeclaratorAST* d = decl->declarator();

    if( !d )
	return;

    if( !d->subDeclarator() && d->parameterDeclarationClause() )
	return parseFunctionDeclaration( funSpec, storageSpec, typeSpec, decl );

    DeclaratorAST* t = d;
    while( t && t->subDeclarator() )
	t = t->subDeclarator();

    QString id;
    if( t && t->declaratorId() && t->declaratorId()->unqualifiedName() )
	id = t->declaratorId()->unqualifiedName()->text();

    kdDebug() << "CppTree2Uml::parseDeclaration(2): name=" << id << endl;
    if( !scopeOfDeclarator(d, QStringList()).isEmpty() ){
	kdDebug() << "CppTree2Uml::parseDeclaration(2): skipping declaration" << endl;
	return;
    }

    UMLClass *c = m_currentClass.top();
    if (c == NULL) {
	kdDebug() << "CppTree2Uml::parseDeclaration(2): attributes outside class not supported"
		  << endl;
	return;
    }
    UMLDoc *umldoc = UMLApp::app()->getDocument();
    UMLAttribute *attr = c->addAttribute( id, umldoc->getUniqueID() );
    attr->setScope( (Uml::Scope)m_currentAccess );
    QString typeName = typeOfDeclaration( typeSpec, d );
    if ( !typeName.isEmpty() )
	attr->setTypeName( typeName );

    bool isFriend = false;
    bool isStatic = false;
    //bool isInitialized = decl->initializer() != 0;

    if( storageSpec ){
	QPtrList<AST> l = storageSpec->nodeList();
	QPtrListIterator<AST> it( l );
	while( it.current() ){
	    QString text = it.current()->text();
	    if( text == "friend" ) isFriend = true;
	    else if( text == "static" ) isStatic = true;
	    ++it;
	}
    }

    attr->setStatic( isStatic );
}

void CppTree2Uml::parseAccessDeclaration( AccessDeclarationAST * access )
{
    QPtrList<AST> l = access->accessList();

    QString accessStr = l.at( 0 )->text();
    if( accessStr == "public" )
	m_currentAccess = Uml::Public;
    else if( accessStr == "protected" )
	m_currentAccess = Uml::Protected;
    else if( accessStr == "private" )
	m_currentAccess = Uml::Private;
    else if( accessStr == "signals" )
	m_currentAccess = Uml::Protected;
    else
	m_currentAccess = Uml::Public;

    m_inSlots = l.count() > 1 ? l.at( 1 )->text() == "slots" : false;
    m_inSignals = l.count() >= 1 ? l.at( 0 )->text() == "signals" : false;
}

void CppTree2Uml::parseFunctionDeclaration(  GroupAST* funSpec, GroupAST* storageSpec,
					     TypeSpecifierAST * typeSpec, InitDeclaratorAST * decl )
{
    bool isFriend = false;
    bool isVirtual = false;
    bool isStatic = false;
    bool isInline = false;
    bool isPure = decl->initializer() != 0;

    if( funSpec ){
	QPtrList<AST> l = funSpec->nodeList();
	QPtrListIterator<AST> it( l );
	while( it.current() ){
	    QString text = it.current()->text();
	    if( text == "virtual" ) isVirtual = true;
	    else if( text == "inline" ) isInline = true;
	    ++it;
	}
    }

    if( storageSpec ){
	QPtrList<AST> l = storageSpec->nodeList();
	QPtrListIterator<AST> it( l );
	while( it.current() ){
	    QString text = it.current()->text();
	    if( text == "friend" ) isFriend = true;
	    else if( text == "static" ) isStatic = true;
	    ++it;
	}
    }

    DeclaratorAST* d = decl->declarator();
    QString id = d->declaratorId()->unqualifiedName()->text();
    QString returnType = typeOfDeclaration( typeSpec, d );
    kdDebug() << "CppTree2Uml::parseFunctionDeclaration: name=" << id << endl;
    UMLAttributeList parList;
    UMLOperation *m = m_importer->insertMethod( m_currentClass.top(),
						(Uml::Scope)m_currentAccess, id,
		 				returnType, isStatic, isPure, "",
						&parList );
    parseFunctionArguments( d, m );
}

void CppTree2Uml::parseFunctionArguments(DeclaratorAST* declarator,
					 UMLOperation* method)
{
    ParameterDeclarationClauseAST* clause = declarator->parameterDeclarationClause();

    if( clause && clause->parameterDeclarationList() ){
	ParameterDeclarationListAST* params = clause->parameterDeclarationList();
	QPtrList<ParameterDeclarationAST> l( params->parameterList() );
	QPtrListIterator<ParameterDeclarationAST> it( l );
	while( it.current() ){
	    ParameterDeclarationAST* param = it.current();
	    ++it;

	    QString name;
	    if (param->declarator())
		name = declaratorToString(param->declarator(), QString::null, true );

	    QString tp = typeOfDeclaration( param->typeSpec(), param->declarator() );

	    method->addParm( tp, name, "" /*initialValue*/, "" /*doc*/);
	}
    }
}

QString CppTree2Uml::typeOfDeclaration( TypeSpecifierAST* typeSpec, DeclaratorAST* declarator )
{
    if( !typeSpec || !declarator )
	return QString::null;

    QString text;

    text += typeSpec->text();

    QPtrList<AST> ptrOpList = declarator->ptrOpList();
    for( QPtrListIterator<AST> it(ptrOpList); it.current(); ++it ){
	text += it.current()->text();
    }

    return text;
}

void CppTree2Uml::parseBaseClause( BaseClauseAST * baseClause, UMLClass* klass )
{
    QPtrList<BaseSpecifierAST> l = baseClause->baseSpecifierList();
    QPtrListIterator<BaseSpecifierAST> it( l );
    while( it.current() ){
	BaseSpecifierAST* baseSpecifier = it.current();
	++it;

	QString baseName;
	if( baseSpecifier->name() )
	    baseName = baseSpecifier->name()->text();

	// Create a generalization.
	UMLDoc *umldoc = UMLApp::app()->getDocument();
	UMLObject *parent = umldoc->findUMLObject( baseName, Uml::ot_Class );
	if (parent == NULL) {
	    kdDebug() << "CppTree2Uml::parseBaseClause: Could not find UML object for "
		      << baseName << endl;
	    continue;
	}
	UMLAssociation *assoc = new UMLAssociation( umldoc, Uml::at_Generalization,
						    klass, parent );
	umldoc->addAssociation(assoc);
    }
}

QStringList CppTree2Uml::scopeOfName( NameAST* id, const QStringList& startScope )
{
    QStringList scope = startScope;
    if( id && id->classOrNamespaceNameList().count() ){
	if( id->isGlobal() )
	    scope.clear();
	QPtrList<ClassOrNamespaceNameAST> l = id->classOrNamespaceNameList();
	QPtrListIterator<ClassOrNamespaceNameAST> it( l );
	while( it.current() ){
	    if( it.current()->name() ){
	       scope << it.current()->name()->text();
	    }
	    ++it;
	}
    }

    return scope;
}

QStringList CppTree2Uml::scopeOfDeclarator( DeclaratorAST* d, const QStringList& startScope )
{
    return scopeOfName( d->declaratorId(), startScope );
}
