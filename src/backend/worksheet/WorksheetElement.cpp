/***************************************************************************
    File                 : WorksheetElement.cpp
    Project              : LabPlot
    Description          : Base class for all Worksheet children.
    --------------------------------------------------------------------
    Copyright            : (C) 2009 Tilman Benkert (thzs@gmx.net)
    Copyright            : (C) 2012-2015 by Alexander Semke (alexander.semke@web.de)

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#include "backend/worksheet/Worksheet.h"
#include "backend/worksheet/WorksheetElement.h"
#include "backend/worksheet/plots/cartesian/Axis.h"

#include <QMenu>
#include <KLocale>

QPen WorksheetElement::hoveredPen = QPen(QColor(128,179,255), 3, Qt::SolidLine);
float WorksheetElement::hoveredOpacity = 0.6;
QPen WorksheetElement::selectedPen = QPen(Qt::darkBlue, 3, Qt::SolidLine);
float WorksheetElement::selectedOpacity = 0.3;

/**
 * \class WorksheetElement
 * \brief Base class for all Worksheet children.
 *
 */
WorksheetElement::WorksheetElement(const QString &name) : AbstractAspect(name) {
	m_drawingOrderMenu = new QMenu(i18n("Drawing &order"));
	m_moveBehindMenu = new QMenu(i18n("Move &behind"));
	m_moveInFrontOfMenu = new QMenu(i18n("Move in &front of"));
	m_drawingOrderMenu->addMenu(m_moveBehindMenu);
	m_drawingOrderMenu->addMenu(m_moveInFrontOfMenu);

	connect(m_moveBehindMenu, SIGNAL(aboutToShow()), this, SLOT(prepareMoveBehindMenu()));
	connect(m_moveInFrontOfMenu, SIGNAL(aboutToShow()), this, SLOT(prepareMoveInFrontOfMenu()));
	connect(m_moveBehindMenu, SIGNAL(triggered(QAction*)), this, SLOT(execMoveBehind(QAction*)));
	connect(m_moveInFrontOfMenu, SIGNAL(triggered(QAction*)), this, SLOT(execMoveInFrontOf(QAction*)));
}

WorksheetElement::~WorksheetElement() {
	delete m_moveBehindMenu;
	delete m_moveInFrontOfMenu;
	delete m_drawingOrderMenu;
}

/**
 * \fn QGraphicsItem *WorksheetElement::graphicsItem() const
 * \brief Return the graphics item representing this element.
 *
 *
 */

/**
 * \fn void WorksheetElement::setVisible(bool on)
 * \brief Show/hide the element.
 *
 */

/**
 * \fn bool WorksheetElement::isVisible() const
 * \brief Return whether the element is (at least) partially visible.
 *
 */

/**
 * \brief Return whether the element is fully visible (i.e., including all child elements).
 *
 * The standard implementation returns isVisible().
 */
bool WorksheetElement::isFullyVisible() const {
	return isVisible();
}

/**
 * \fn void WorksheetElement::setPrinting(bool on)
 * \brief Switches the printing mode on/off
 *
 */

/**
 * \fn void WorksheetElement::retransform()
 * \brief Tell the element to newly transform its graphics item into its coordinate system.
 *
 * This method must not change the undo-aware data of the element, only
 * the graphics item which represents the item is to be updated.
 */

void WorksheetElement::setZValue(qreal value) {
	graphicsItem()->setZValue(value);
}

/**
    This does exactly what Qt internally does to creates a shape from a painter path.
*/
QPainterPath WorksheetElement::shapeFromPath(const QPainterPath &path, const QPen &pen) {
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath())
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

QMenu *WorksheetElement::createContextMenu() {
	QMenu *menu = AbstractAspect::createContextMenu();

	if (!dynamic_cast<Axis*>(this)) {
		if (parentAspect()->childCount<WorksheetElement>()>1){
			menu->addSeparator();
			menu->addMenu(m_drawingOrderMenu);
			menu->addSeparator();
		}
	}
	return menu;
}

void WorksheetElement::prepareMoveBehindMenu() {
	m_moveBehindMenu->clear();
	AbstractAspect *parent = parentAspect();
	if (parent) {
		QList<WorksheetElement *> childElements = parent->children<WorksheetElement>();
		foreach(const WorksheetElement *elem, childElements) {
			if (elem != this && !dynamic_cast<const Axis*>(elem)) {
				QAction *action = m_moveBehindMenu->addAction(elem->name());
				action->setData(parent->indexOfChild<WorksheetElement>(elem));
			}
		}
	}
}

void WorksheetElement::prepareMoveInFrontOfMenu() {
	m_moveInFrontOfMenu->clear();
	AbstractAspect *parent = parentAspect();
	if (parent) {
		QList<WorksheetElement *> childElements = parent->children<WorksheetElement>();
		foreach(const WorksheetElement *elem, childElements) {
			if (elem != this && !dynamic_cast<const Axis*>(elem)) {
				QAction *action = m_moveInFrontOfMenu->addAction(elem->name());
				action->setData(parent->indexOfChild<WorksheetElement>(elem));
			}
		}
	}
}

void WorksheetElement::execMoveBehind(QAction *action) {
	Q_ASSERT(action != NULL);
	AbstractAspect *parent = parentAspect();
	if (parent) {
		int index = action->data().toInt();
		AbstractAspect *sibling1 = parent->child<WorksheetElement>(index);
		beginMacro(i18n("%1: move behind %2.", name(), sibling1->name()));
		remove();
		AbstractAspect *sibling2 = parent->child<WorksheetElement>(index + 1);
		parent->insertChildBefore(this, sibling2);
		endMacro();
	}
}

void WorksheetElement::execMoveInFrontOf(QAction *action) {
	Q_ASSERT(action != NULL);
	AbstractAspect *parent = parentAspect();
	if (parent) {
		int index = action->data().toInt();
		AbstractAspect *sibling = parent->child<WorksheetElement>(index);
		beginMacro(i18n("%1: move in front of %2.", name(), sibling->name()));
		remove();
		parent->insertChildBefore(this, sibling);
		endMacro();
	}
}

/**
 * This function is called every time the page is resized.
 *
 * \param horizontalRatio New page width divided by old page width.
 * \param verticalRatio   New page height divided by old page height.
 *
 * Override this function with a handler which rescales all properties
 * which are in page coordinates (such as line widths). Don't forget
 * to call the base class's handler in the overridden version.
 */
void WorksheetElement::handlePageResize(double horizontalRatio, double verticalRatio){
	Q_UNUSED(horizontalRatio);
	Q_UNUSED(verticalRatio);
}
