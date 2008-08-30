/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef COMBINEDFRAGMENTWIDGET_H
#define COMBINEDFRAGMENTWIDGET_H

#include <qlist.h>
#include "umlwidget.h"
#include "worktoolbar.h"
#include "floatingdashlinewidget.h"

/**
 * @short  A graphical version of a UML combined fragment.
 *
 * This widget is used in Sequence Diagrams.
 *
 * @author Hassan KOUCH <hkouch@hotmail.com>
 * @author Gopala Krishna
 *
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class CombinedFragmentWidget : public NewUMLRectWidget
{
    Q_OBJECT

public:
    enum CombinedFragmentType
    {
        Ref = 0,
        Opt,
        Break,
        Loop,
        Neg,
        Crit,
        Ass,
        Alt,
        Par
    };

    explicit CombinedFragmentWidget( CombinedFragmentType combinedfragmentType = Ref, Uml::IDType id = Uml::id_None );
    virtual ~CombinedFragmentWidget();

    virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *);

    /// @return The type of combined fragment.
    CombinedFragmentType combinedFragmentType() const {
        return m_combinedFragmentType;
    }

    /**
     * Sets the combined fragment type of this widget to \a
     * combinedFragmentType and then updates the dash lines based on the
     * new type.
     */
    void setCombinedFragmentType(CombinedFragmentType type);

    static CombinedFragmentType stringToCombinedFragementType(const QString& string);

    /**
     * Pops up a dialog box and asks for the name.
     */
    void askNameForWidgetType(NewUMLRectWidget* &targetWidget, const QString& dialogTitle,
                              const QString& dialogPrompt, const QString& defaultName);

    /**
     * Reimplemented from NewUMLRectWidget::loadFromXMI to load
     * CombinedFragmentWidget data from XMI.
     *
     * This method also loads the child FloatingDashLineWidgets.
     */
    bool loadFromXMI( QDomElement & qElement );

    /**
     * Reimplemented from NewUMLRectWidget::saveToXMI to save widget data
     * into 'combinedfragmentwidget' XMI element.
     */
    void saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

public Q_SLOTS:

    /**
     * Reimplemented from NewUMLRectWidget::slotMenuSelection to handle
     * some specific actions.
     */
    void slotMenuSelection(QAction* action);

protected:

    /**
     * Reimplemented from NewUMLRectWidget::updateGeometry to calculate
     * the minimum size for this widget.
     */
    void updateGeometry();

    /**
     * Reimplemented from NewUMLRectWidget::updateTextItemGroups to update
     * the TextItem's values and TextItems visibility.
     */
    void updateTextItemGroups();

    /**
     * Reimplemented from NewUMLRectWidget::attributeChange to handle
     *
     * - SizeHasChanged -> To set text position and update floating
     *                     widget's position and limits.
     *
     * - FontHasChanged -> Sets the CombinedFragmentWidget's changed font
     *                     to all the FloatingDashLineWidgets.
     *
     * - FontColorHasChanged -> Sets the CombinedFragmentWidget's changed
     *                          fontcolor to all the
     *                          FloatingDashLineWidgets.
     *
     * - LineColorHasChanged -> Sets the CombinedFragmentWidget's changed
     *                          line color to all the
     *                          FloatingDashLineWidgets.
     *
     * - LineWidthHasChanged -> Sets the CombinedFragmentWidget's changed
     *                          line width to all the
     *                          FloatingDashLineWidgets.
     */
    QVariant attributeChange(WidgetAttributeChange change, const QVariant& oldValue);

private:

    /**
     * Helper method which applies this CombinedFragmentWidget's
     * properties to the FloatingDashLineWidget.
     *
     * This method is used mostly during creation of
     * FloatingDashLineWidget.
     */
    void setupFloatingWidget(FloatingDashLineWidget *widget);

    /**
     * Helper method which updates the FloatingDashLineWidget's dimensions
     * and constraint values.
     */
    void updateFloatingWidgetsPosition();

    CombinedFragmentType m_combinedFragmentType;
    /// Dash lines of an alternative or parallel combined fragment
    QList<FloatingDashLineWidget*> m_dashLines ;

    QLineF m_fragmentBox[3];
    enum {
        TypeBoxIndex,
        ReferenceDiagramNameBoxIndex
    };

    enum {
        TypeItemIndex,
        FirstAlternativeItemIndex
    };
};

#endif
