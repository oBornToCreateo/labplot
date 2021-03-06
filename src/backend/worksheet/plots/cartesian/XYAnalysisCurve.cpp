/***************************************************************************
    File                 : XYAnalysisCurve.h
    Project              : LabPlot
    Description          : Base class for all analysis curves
    --------------------------------------------------------------------
    Copyright            : (C) 2017 Alexander Semke (alexander.semke@web.de)

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

/*!
  \class XYAnalysisCurve
  \brief Base class for all analysis curves

  \ingroup worksheet
*/

#include "XYAnalysisCurve.h"
#include "XYAnalysisCurvePrivate.h"
#include "backend/core/AbstractColumn.h"
#include "backend/core/column/Column.h"
#include "backend/lib/commandtemplates.h"
#include "backend/lib/macros.h"

#include <KLocale>

XYAnalysisCurve::XYAnalysisCurve(const QString& name)
		: XYCurve(name, new XYAnalysisCurvePrivate(this)) {
	init();
}

XYAnalysisCurve::XYAnalysisCurve(const QString& name, XYAnalysisCurvePrivate* dd)
		: XYCurve(name, dd) {
	init();
}

// XYCurve::XYCurve(const QString &name) : WorksheetElement(name), d_ptr(new XYCurvePrivate(this)) {
// 	init();
// }
//
// XYCurve::XYCurve(const QString& name, XYCurvePrivate* dd) : WorksheetElement(name), d_ptr(dd) {
// 	init();
// }


XYAnalysisCurve::~XYAnalysisCurve() {
	//no need to delete the d-pointer here - it inherits from QGraphicsItem
	//and is deleted during the cleanup in QGraphicsScene
}

void XYAnalysisCurve::init() {
	Q_D(XYAnalysisCurve);

	d->dataSourceType = XYAnalysisCurve::DataSourceSpreadsheet;
	d->dataSourceCurve = nullptr;
	d->lineType = XYCurve::Line;
	d->symbolsStyle = Symbol::NoSymbols;
}

//##############################################################################
//##########################  getter methods  ##################################
//##############################################################################
BASIC_SHARED_D_READER_IMPL(XYAnalysisCurve, XYAnalysisCurve::DataSourceType, dataSourceType, dataSourceType)
BASIC_SHARED_D_READER_IMPL(XYAnalysisCurve, const XYCurve*, dataSourceCurve, dataSourceCurve)
const QString& XYAnalysisCurve::dataSourceCurvePath() const {
	return d_ptr->dataSourceCurvePath;
}

BASIC_SHARED_D_READER_IMPL(XYAnalysisCurve, const AbstractColumn*, xDataColumn, xDataColumn)
BASIC_SHARED_D_READER_IMPL(XYAnalysisCurve, const AbstractColumn*, yDataColumn, yDataColumn)
const QString& XYAnalysisCurve::xDataColumnPath() const { Q_D(const XYAnalysisCurve); return d->xDataColumnPath; }
const QString& XYAnalysisCurve::yDataColumnPath() const { Q_D(const XYAnalysisCurve); return d->yDataColumnPath; }

//##############################################################################
//#################  setter methods and undo commands ##########################
//##############################################################################
STD_SETTER_CMD_IMPL_S(XYAnalysisCurve, SetDataSourceType, XYAnalysisCurve::DataSourceType, dataSourceType)
void XYAnalysisCurve::setDataSourceType(DataSourceType type) {
	Q_D(XYAnalysisCurve);
	if (type != d->dataSourceType)
		exec(new XYAnalysisCurveSetDataSourceTypeCmd(d, type, i18n("%1: data source type changed")));
}

STD_SETTER_CMD_IMPL_F_S(XYAnalysisCurve, SetDataSourceCurve, const XYCurve*, dataSourceCurve, retransform)
void XYAnalysisCurve::setDataSourceCurve(const XYCurve* curve) {
	Q_D(XYAnalysisCurve);
	if (curve != d->dataSourceCurve) {
		exec(new XYAnalysisCurveSetDataSourceCurveCmd(d, curve, i18n("%1: data source curve changed")));
		handleSourceDataChanged();

		//handle the changes when different columns were provided for the source curve
		connect(curve, SIGNAL(xColumnChanged(const AbstractColumn*)), this, SLOT(handleSourceDataChanged()));
		connect(curve, SIGNAL(yColumnChanged(const AbstractColumn*)), this, SLOT(handleSourceDataChanged()));

		//handle the changes when the data inside of the source curve columns
		connect(curve, SIGNAL(xDataChanged()), this, SLOT(handleSourceDataChanged()));
		connect(curve, SIGNAL(yDataChanged()), this, SLOT(handleSourceDataChanged()));

		//TODO: add disconnect in the undo-function
	}
}

STD_SETTER_CMD_IMPL_S(XYAnalysisCurve, SetXDataColumn, const AbstractColumn*, xDataColumn)
void XYAnalysisCurve::setXDataColumn(const AbstractColumn* column) {
	Q_D(XYAnalysisCurve);
	if (column != d->xDataColumn) {
		exec(new XYAnalysisCurveSetXDataColumnCmd(d, column, i18n("%1: assign x-data")));
		handleSourceDataChanged();
		if (column) {
			connect(column, SIGNAL(dataChanged(const AbstractColumn*)), this, SLOT(handleSourceDataChanged()));
			//TODO disconnect on undo
		}
	}
}

STD_SETTER_CMD_IMPL_S(XYAnalysisCurve, SetYDataColumn, const AbstractColumn*, yDataColumn)
void XYAnalysisCurve::setYDataColumn(const AbstractColumn* column) {
	Q_D(XYAnalysisCurve);
	if (column != d->yDataColumn) {
		exec(new XYAnalysisCurveSetYDataColumnCmd(d, column, i18n("%1: assign y-data")));
		handleSourceDataChanged();
		if (column) {
			connect(column, SIGNAL(dataChanged(const AbstractColumn*)), this, SLOT(handleSourceDataChanged()));
			//TODO disconnect on undo
		}
	}
}

//##############################################################################
//#################################  SLOTS  ####################################
//##############################################################################
void XYAnalysisCurve::handleSourceDataChanged() {
	Q_D(XYAnalysisCurve);
	d->sourceDataChangedSinceLastRecalc = true;
	emit sourceDataChanged();
}

//##############################################################################
//######################### Private implementation #############################
//##############################################################################
XYAnalysisCurvePrivate::XYAnalysisCurvePrivate(XYAnalysisCurve* owner) : XYCurvePrivate(owner),
	xDataColumn(nullptr), yDataColumn(nullptr),
	xColumn(nullptr), yColumn(nullptr),
	xVector(nullptr), yVector(nullptr),
	q(owner) {

}

XYAnalysisCurvePrivate::~XYAnalysisCurvePrivate() {
	//no need to delete xColumn and yColumn, they are deleted
	//when the parent aspect is removed
}


//##############################################################################
//##################  Serialization/Deserialization  ###########################
//##############################################################################
//! Save as XML
void XYAnalysisCurve::save(QXmlStreamWriter* writer) const {
	Q_D(const XYAnalysisCurve);

	writer->writeStartElement("xyAnalysisCurve");

	//write xy-curve information
	XYCurve::save(writer);

	//write data source specific information
	writer->writeStartElement("dataSource");
	writer->writeAttribute( "type", QString::number(d->dataSourceType) );
	WRITE_PATH(d->dataSourceCurve, dataSourceCurve);
	WRITE_COLUMN(d->xDataColumn, xDataColumn);
	WRITE_COLUMN(d->yDataColumn, yDataColumn);
	writer->writeEndElement();

	writer->writeEndElement(); //"xyAnalysiCurve"
}

//! Load from XML
bool XYAnalysisCurve::load(XmlStreamReader* reader, bool preview) {
	Q_D(XYAnalysisCurve);

	QString attributeWarning = i18n("Attribute '%1' missing or empty, default value is used");
	QXmlStreamAttributes attribs;
	QString str;

	while (!reader->atEnd()) {
		reader->readNext();
		if (reader->isEndElement() && reader->name() == "xyAnalysisCurve")
			break;

		if (!reader->isStartElement())
			continue;

		if (reader->name() == "xyCurve") {
			if ( !XYCurve::load(reader, preview) )
				return false;
		} else if (reader->name() == "dataSource") {
			attribs = reader->attributes();
			READ_INT_VALUE("type", dataSourceType, XYAnalysisCurve::DataSourceType);
			READ_PATH(dataSourceCurve);
			READ_COLUMN(xDataColumn);
			READ_COLUMN(yDataColumn);
		}
	}

	return true;
}
