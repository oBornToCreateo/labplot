/***************************************************************************
    File                 : DockHelpers.h
    Project              : LabPlot
    Description          : Dock widget helper functions
    --------------------------------------------------------------------
    Copyright            : (C) 2015 Minh Ngo (minh@fedoraproject.org)

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

#ifndef DOCKHELPERS_H
#define DOCKHELPERS_H

#include <QWidget>

#include "backend/core/AspectTreeModel.h"
#include "backend/core/AbstractColumn.h"
#include "backend/matrix/Matrix.h"

namespace DockHelpers {
	// Locks AbstractAspect changes!
	struct Lock {
		inline Lock(bool& variable)
			: variable(variable = true){
		}

		inline ~Lock(){
			variable = false;
		}

	private:
		bool& variable;
	};

	struct SignalBlocker {
		inline SignalBlocker(const QSet<QObject*>& objects)
			: objects(objects) {
			foreach(QObject* object, objects) {
				object->blockSignals(true);
			}
		}

		inline ~SignalBlocker() {
			foreach(QObject* object, objects) {
				object->blockSignals(false);
			}
		}

	private:
		const QSet<QObject*>& objects;
	};

	inline void showItem(QWidget* labelWidget, QWidget* inputWidget, bool pred = true) {
		labelWidget->setVisible(pred);
		inputWidget->setVisible(pred);
	}

	inline void hideItem(QWidget* labelWidget, QWidget* inputWidget) {
		showItem(labelWidget, inputWidget, false);
	}

	inline AbstractColumn* getColumn(const QModelIndex& index) {
		AbstractAspect* aspect = static_cast<AbstractAspect*>(index.internalPointer());
		return aspect ? dynamic_cast<AbstractColumn*>(aspect) : 0;
	}

	inline Matrix* getMatrix(const QModelIndex& index) {
		AbstractAspect* aspect = static_cast<AbstractAspect*>(index.internalPointer());
		return aspect ? dynamic_cast<Matrix*>(aspect) : 0;
	}

	inline QModelIndex modelIndexOfAspect(AspectTreeModel *model, const AbstractAspect *aspect) {
		if (!aspect)
			return QModelIndex();
		return model->modelIndexOfAspect(aspect);
	}

	class ChildrenRecorder {
		public:
			inline ChildrenRecorder(QObject* dockWidget) : dockWidget(dockWidget) {}
			inline const QSet<QObject*>& children() const { return objects; }
			inline void connect(QObject* sender, const char* signal, const char* member) {
				dockWidget->connect(sender, signal, member);
				objects.insert(sender);
			}

		private:
			QObject* dockWidget;
			QSet<QObject*> objects;
	};
}

#endif