
/***************************************************************************
    File                 : CartesianPlot.h
    Project              : LabPlot
    Description          : Cartesian plot
    --------------------------------------------------------------------
    Copyright            : (C) 2011-2017 by Alexander Semke (alexander.semke@web.de)

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

#ifndef CARTESIANPLOT_H
#define CARTESIANPLOT_H

#include "backend/worksheet/plots/AbstractPlot.h"
#include "backend/worksheet/plots/cartesian/Histogram.h"

#include <math.h>

class QDropEvent;
class QToolBar;
class CartesianPlotPrivate;
class CartesianPlotLegend;
class AbstractColumn;
class XYCurve;
class XYEquationCurve;
class XYDataReductionCurve;
class XYDifferentiationCurve;
class XYIntegrationCurve;
class XYInterpolationCurve;
class XYSmoothCurve;
class XYFitCurve;
class XYFourierFilterCurve;
class KConfig;
class XYFourierTransformCurve;

class CartesianPlot:public AbstractPlot {
	Q_OBJECT

public:
	explicit CartesianPlot(const QString &name);
	~CartesianPlot() override;

	enum Scale {ScaleLinear, ScaleLog10, ScaleLog2, ScaleLn, ScaleSqrt, ScaleX2};
	enum Type {FourAxes, TwoAxes, TwoAxesCentered, TwoAxesCenteredZero};
	enum RangeType {RangeFree, RangeLast, RangeFirst};
	enum RangeBreakStyle {RangeBreakSimple, RangeBreakVertical, RangeBreakSloped};
	enum MouseMode {SelectionMode, ZoomSelectionMode, ZoomXSelectionMode, ZoomYSelectionMode};
	enum NavigationOperation {ScaleAuto, ScaleAutoX, ScaleAutoY, ZoomIn, ZoomOut, ZoomInX, ZoomOutX,
	                          ZoomInY, ZoomOutY, ShiftLeftX, ShiftRightX, ShiftUpY, ShiftDownY
	                         };

	struct RangeBreak {
		RangeBreak() : start(NAN), end(NAN), position(0.5), style(RangeBreakSloped) {}
		bool isValid() const {
			return (!std::isnan(start) && !std::isnan(end));
		}
		double start;
		double end;
		double position;
		RangeBreakStyle style;
	};

	//simple wrapper for QList<RangeBreaking> in order to get our macros working
	struct RangeBreaks {
		RangeBreaks() : lastChanged(-1) {
			RangeBreak b;
			list << b;
		};
		QList<RangeBreak> list;
		int lastChanged;
	};

	void initDefault(Type = FourAxes);
	QIcon icon() const override;
	QMenu* createContextMenu() override;
	QMenu* analysisMenu();
	QVector<AbstractAspect*> dependsOn() const override;
	void setRect(const QRectF&) override;
	QRectF dataRect() const;
	void setMouseMode(const MouseMode);
	MouseMode mouseMode() const;
	void navigate(NavigationOperation);
	void setSuppressDataChangedSignal(bool);
	const QList<QColor>& themeColorPalette() const;
	void processDropEvent(QDropEvent*);

	void save(QXmlStreamWriter*) const override;
	bool load(XmlStreamReader*, bool preview) override;
	void loadThemeConfig(const KConfig&) override;
	void saveTheme(KConfig& config);

	BASIC_D_ACCESSOR_DECL(CartesianPlot::RangeType, rangeType, RangeType)
	BASIC_D_ACCESSOR_DECL(int, rangeLastValues, RangeLastValues)
	BASIC_D_ACCESSOR_DECL(int, rangeFirstValues, RangeFirstValues)
	BASIC_D_ACCESSOR_DECL(bool, autoScaleX, AutoScaleX)
	BASIC_D_ACCESSOR_DECL(bool, autoScaleY, AutoScaleY)
	BASIC_D_ACCESSOR_DECL(double, xMin, XMin)
	BASIC_D_ACCESSOR_DECL(double, xMax, XMax)
	BASIC_D_ACCESSOR_DECL(double, yMin, YMin)
	BASIC_D_ACCESSOR_DECL(double, yMax, YMax)
	BASIC_D_ACCESSOR_DECL(CartesianPlot::Scale, xScale, XScale)
	BASIC_D_ACCESSOR_DECL(CartesianPlot::Scale, yScale, YScale)
	BASIC_D_ACCESSOR_DECL(bool, xRangeBreakingEnabled, XRangeBreakingEnabled)
	BASIC_D_ACCESSOR_DECL(bool, yRangeBreakingEnabled, YRangeBreakingEnabled)
	CLASS_D_ACCESSOR_DECL(RangeBreaks, xRangeBreaks, XRangeBreaks)
	CLASS_D_ACCESSOR_DECL(RangeBreaks, yRangeBreaks, YRangeBreaks)

	QString theme() const;

	typedef CartesianPlotPrivate Private;

public slots:
	void setTheme(const QString&);

private:
	void init();
	void initActions();
	void initMenus();
	void setColorPalette(const KConfig&);
	const XYCurve* currentCurve() const;

	CartesianPlotLegend* m_legend;
	double m_zoomFactor;
	QList<QColor> m_themeColorPalette;
	bool m_menusInitialized;

	QAction* visibilityAction;

	//"add new" actions
	QAction* addCurveAction;
	QAction* addEquationCurveAction;
	QAction* addHistogramPlot;
	QAction* addDataReductionCurveAction;
	QAction* addDifferentiationCurveAction;
	QAction* addIntegrationCurveAction;
	QAction* addInterpolationCurveAction;
	QAction* addSmoothCurveAction;
	QAction* addFitCurveAction;
	QAction* addFourierFilterCurveAction;
	QAction* addFourierTransformCurveAction;
	QAction* addHorizontalAxisAction;
	QAction* addVerticalAxisAction;
	QAction* addLegendAction;
	QAction* addCustomPointAction;

	//scaling, zooming, navigation actions
	QAction* scaleAutoXAction;
	QAction* scaleAutoYAction;
	QAction* scaleAutoAction;
	QAction* zoomInAction;
	QAction* zoomOutAction;
	QAction* zoomInXAction;
	QAction* zoomOutXAction;
	QAction* zoomInYAction;
	QAction* zoomOutYAction;
	QAction* shiftLeftXAction;
	QAction* shiftRightXAction;
	QAction* shiftUpYAction;
	QAction* shiftDownYAction;

	//analysis menu actions
	QAction* addDataOperationAction;
	QAction* addDataReductionAction;
	QAction* addDifferentiationAction;
	QAction* addIntegrationAction;
	QAction* addInterpolationAction;
	QAction* addSmoothAction;
	QVector <QAction *> addFitAction;
	QAction* addFourierFilterAction;

	QMenu* addNewMenu;
	QMenu* zoomMenu;
	QMenu* dataAnalysisMenu;
	QMenu* themeMenu;

	Q_DECLARE_PRIVATE(CartesianPlot)

public slots:
	void addHorizontalAxis();
	void addVerticalAxis();
	void addCurve();
	void addHistogram();
	void addEquationCurve();
	void addDataReductionCurve();
	void addDifferentiationCurve();
	void addIntegrationCurve();
	void addInterpolationCurve();
	void addSmoothCurve();
	void addFitCurve();
	void addFourierFilterCurve();
	void addFourierTransformCurve();
	void addLegend();
	void addCustomPoint();
	void scaleAuto();
	void scaleAutoX();
	void scaleAutoY();
	void zoomIn();
	void zoomOut();
	void zoomInX();
	void zoomOutX();
	void zoomInY();
	void zoomOutY();
	void shiftLeftX();
	void shiftRightX();
	void shiftUpY();
	void shiftDownY();
	void dataChanged();

private slots:
	void updateLegend();
	void childAdded(const AbstractAspect*);
	void childRemoved(const AbstractAspect* parent, const AbstractAspect* before, const AbstractAspect* child);

	void xDataChanged();
	void yDataChanged();
	void HistogramdataChanged();
	void xHistogramDataChanged();
	void yHistogramDataChanged();
	void curveVisibilityChanged();

	//SLOTs for changes triggered via QActions in the context menu
	void visibilityChanged();
	void loadTheme(const QString&);

protected:
	CartesianPlot(const QString &name, CartesianPlotPrivate *dd);

signals:
	friend class CartesianPlotSetCRangeTypeCmd;
	friend class CartesianPlotSetCRangeLastValuesCmd;
	friend class CartesianPlotSetCRangeFirstValuesCmd;
	friend class CartesianPlotSetRectCmd;
	friend class CartesianPlotSetAutoScaleXCmd;
	friend class CartesianPlotSetXMinCmd;
	friend class CartesianPlotSetXMaxCmd;
	friend class CartesianPlotSetXScaleCmd;
	friend class CartesianPlotSetAutoScaleYCmd;
	friend class CartesianPlotSetYMinCmd;
	friend class CartesianPlotSetYMaxCmd;
	friend class CartesianPlotSetYScaleCmd;
	friend class CartesianPlotSetXRangeBreakingEnabledCmd;
	friend class CartesianPlotSetYRangeBreakingEnabledCmd;
	friend class CartesianPlotSetXRangeBreaksCmd;
	friend class CartesianPlotSetYRangeBreaksCmd;
	friend class CartesianPlotSetThemeCmd;
	void rangeTypeChanged(CartesianPlot::RangeType);
	void rangeLastValuesChanged(int);
	void rangeFirstValuesChanged(int);
	void rectChanged(QRectF&);
	void xAutoScaleChanged(bool);
	void xMinChanged(double);
	void xMaxChanged(double);
	void xScaleChanged(int);
	void yAutoScaleChanged(bool);
	void yMinChanged(double);
	void yMaxChanged(double);
	void yScaleChanged(int);
	void xRangeBreakingEnabledChanged(bool);
	void xRangeBreaksChanged(const CartesianPlot::RangeBreaks&);
	void yRangeBreakingEnabledChanged(bool);
	void yRangeBreaksChanged(const CartesianPlot::RangeBreaks&);
	void themeChanged(const QString&);
};

#endif
