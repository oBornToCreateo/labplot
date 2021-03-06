/***************************************************************************
File                 : HDF5Filter.h
Project              : LabPlot
Description          : HDF5 I/O-filter
--------------------------------------------------------------------
Copyright            : (C) 2015-2017 Stefan Gerlach (stefan.gerlach@uni.kn)
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
#ifndef HDF5FILTER_H
#define HDF5FILTER_H

#include "backend/datasources/filters/AbstractFileFilter.h"
#include <QStringList>

class QTreeWidgetItem;
class HDF5FilterPrivate;

class HDF5Filter : public AbstractFileFilter {
	Q_OBJECT

public:
	HDF5Filter();
	~HDF5Filter() override;

	void parse(const QString& fileName, QTreeWidgetItem* rootItem);
	QVector<QStringList> readDataFromFile(const QString& fileName, AbstractDataSource* = nullptr,
				  AbstractFileFilter::ImportMode = AbstractFileFilter::Replace, int lines = -1) override;
	QVector<QStringList> readCurrentDataSet(const QString& fileName, AbstractDataSource*, bool& ok,
						AbstractFileFilter::ImportMode = AbstractFileFilter::Replace, int lines = -1);
	void write(const QString& fileName, AbstractDataSource*) override;

	void loadFilterSettings(const QString&) override;
	void saveFilterSettings(const QString&) const override;

	void setCurrentDataSetName(const QString);
	const QString currentDataSetName() const;

	void setStartRow(const int);
	int startRow() const;
	void setEndRow(const int);
	int endRow() const;
	void setStartColumn(const int);
	int startColumn() const;
	void setEndColumn(const int);
	int endColumn() const;

	void save(QXmlStreamWriter*) const override;
	bool load(XmlStreamReader*) override;

private:
	std::unique_ptr<HDF5FilterPrivate> const d;
	friend class HDF5FilterPrivate;
};

#endif
