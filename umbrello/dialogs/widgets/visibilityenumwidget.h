/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef VISIBILITYENUMWIDGET_H
#define VISIBILITYENUMWIDGET_H

#include "basictypes.h"

#include <QMap>
#include <QWidget>

class AssociationWidget;
class UMLObject;

class QVBoxLayout;
class QGroupBox;
class QRadioButton;

class VisibilityEnumWidget : public QWidget
{
    Q_OBJECT
public:
    typedef QMap<Uml::Visibility::Enum,QString> TextMap;
    typedef QMap<Uml::Visibility::Enum,QRadioButton*> ButtonMap;

    VisibilityEnumWidget(UMLObject *o, QWidget *parent=0);
    VisibilityEnumWidget(AssociationWidget *a, Uml::RoleType::Enum role, QWidget *parent=0);
    ~VisibilityEnumWidget();

    void addToLayout(QVBoxLayout *layout);
    void apply();

protected:
    TextMap m_texts;
    QGroupBox *m_box;
    ButtonMap m_buttons;
    UMLObject *m_object;
    AssociationWidget *m_widget;
    Uml::RoleType::Enum m_role;

    void init(const QString &title);
};

#endif // VISIBILITYENUMWIDGET_H
