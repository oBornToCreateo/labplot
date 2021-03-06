/***************************************************************************
    File                 : FitTest.cpp
    Project              : LabPlot
    Description          : Tests for data fitting
    --------------------------------------------------------------------
    Copyright            : (C) 2017 Alexander Semke (alexander.semke@web.de)
    Copyright            : (C) 2018 Stefan Gerlach (stefan.gerlach@uni.kn)
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

#include "FitTest.h"
#include "backend/core/column/Column.h"
#include "backend/worksheet/plots/cartesian/XYFitCurve.h"

extern "C" {
#include "backend/nsl/nsl_sf_stats.h"
#include "backend/nsl/nsl_stats.h"
}

void FitTest::initTestCase() {
	// needed in order to have the signals triggered by SignallingUndoCommand, see LabPlot.cpp
	//TODO: redesign/remove this
	qRegisterMetaType<const AbstractAspect*>("const AbstractAspect*");
	qRegisterMetaType<const AbstractColumn*>("const AbstractColumn*");
}

//##############################################################################
//#################  linear regression with NIST datasets ######################
//##############################################################################
void FitTest::testLinearNorris() {
	//NIST data for Norris dataset
	QVector<double> xData = {0.2,337.4,118.2,884.6,10.1,226.5,666.3,996.3,448.6,777.0,558.2,0.4,0.6,775.5,666.9,338.0,447.5,11.6,556.0,228.1,
		995.8,887.6,120.2,0.3,0.3,556.8,339.1,887.2,999.0,779.0,11.1,118.3,229.2,669.1,448.9,0.5};
	QVector<double> yData = {0.1,338.8,118.1,888.0,9.2,228.1,668.5,998.5,449.1,778.9,559.2,0.3,0.1,778.1,668.8,339.3,448.9,10.8,557.7,228.3,
		998.0,888.8,119.6,0.3,0.6,557.6,339.3,888.0,998.5,778.9,10.2,117.6,228.9,668.4,449.2,0.2};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 1;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 2);

	QCOMPARE(fitResult.paramValues.at(0), -0.262323073774029);
	QCOMPARE(fitResult.errorValues.at(0), 0.232818234301152);
	QCOMPARE(fitResult.paramValues.at(1), 1.00211681802045);
	QCOMPARE(fitResult.errorValues.at(1), 0.429796848199937e-3);

	QCOMPARE(fitResult.rsd, 0.884796396144373);
	QCOMPARE(fitResult.rsquare, 0.999993745883712);
	QCOMPARE(fitResult.sse, 26.6173985294224);
	QCOMPARE(fitResult.rms, 0.782864662630069);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 5436419.54079774
	FuzzyCompare(fitResult.fdist_F, 5436385.54079785, 1.e-5);
}

void FitTest::testLinearPontius() {
	//NIST data for Pontius dataset
	QVector<int> xData = {150000,300000,450000,600000,750000,900000,1050000,1200000,1350000,1500000,1650000,1800000,1950000,2100000,
		2250000,2400000,2550000,2700000,2850000,3000000,150000,300000,450000,600000,750000,900000,1050000,1200000,1350000,1500000,
		1650000,1800000,1950000,2100000,2250000,2400000,2550000,2700000,2850000,3000000};
	QVector<double> yData = {.11019,.21956,.32949,.43899,.54803,.65694,.76562,.87487,.98292,1.09146,1.20001,1.30822,1.41599,1.52399,
		1.63194,1.73947,1.84646,1.95392,2.06128,2.16844,.11052,.22018,.32939,.43886,.54798,.65739,.76596,.87474,
		.98300,1.09150,1.20004,1.30818,1.41613,1.52408,1.63159,1.73965,1.84696,1.95445,2.06177,2.16829};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 2;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 3);

	QCOMPARE(fitResult.paramValues.at(0), 0.673565789473684e-3);
	QCOMPARE(fitResult.errorValues.at(0), 0.107938612033077e-3);
	QCOMPARE(fitResult.paramValues.at(1), 0.732059160401003e-6);
	QCOMPARE(fitResult.errorValues.at(1), 0.157817399981659e-9);
	QCOMPARE(fitResult.paramValues.at(2), -0.316081871345029e-14);
	QCOMPARE(fitResult.errorValues.at(2), 0.486652849992036e-16);

	QCOMPARE(fitResult.rsd, 0.205177424076185e-3);
	QCOMPARE(fitResult.rsquare, 0.999999900178537);
	QCOMPARE(fitResult.sse, 0.155761768796992e-5);
	QCOMPARE(fitResult.rms, 0.420977753505385e-7);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 370661768.991551
	//FuzzyCompare(fitResult.fdist_F, 185330865.995752, 1.);
}

void FitTest::testLinearNoInt1() {
	//NIST data for NoInt1 dataset
	QVector<int> xData = {60,61,62,63,64,65,66,67,68,69,70};
	QVector<int> yData = {130,131,132,133,134,135,136,137,138,139,140};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*x";
	fitData.paramNames << "b1";
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 1.;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max();
	//fitData.eps = 1.e-15;
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 1);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 2.07438016513166
	FuzzyCompare(fitResult.paramValues.at(0), 2.07438016528926, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 0.00463315628245255
//	QCOMPARE(fitResult.errorValues.at(0), 0.165289256198347e-1);

	QCOMPARE(fitResult.rsd, 3.56753034006338);
//	QCOMPARE(fitResult.rsquare, 0.999365492298663);
	QCOMPARE(fitResult.sse, 127.272727272727);
	QCOMPARE(fitResult.rms, 12.7272727272727);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 370661768.991551
//	FuzzyCompare(fitResult.fdist_F, 15750.25, 1.);
}

void FitTest::testLinearNoInt1_2() {
	//NIST data for NoInt1 dataset
	QVector<int> xData = {60,61,62,63,64,65,66,67,68,69,70};
	QVector<int> yData = {130,131,132,133,134,135,136,137,138,139,140};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 1;
	XYFitCurve::initFitData(fitData);
	fitData.paramStartValues[0] = 0;
	fitData.paramFixed[0] = true;
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 2);

	QCOMPARE(fitResult.paramValues.at(0), 0.);
	QCOMPARE(fitResult.paramValues.at(1), 2.07438016528926);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 0.0046331562857966
	//QCOMPARE(fitResult.errorValues.at(1), 0.165289256198347e-1);

//	QCOMPARE(fitResult.rsd, 3.56753034006338);
//	QCOMPARE(fitResult.rsquare, 0.999365492298663);
	QCOMPARE(fitResult.sse, 127.272727272727);
//	QCOMPARE(fitResult.rms, 12.7272727272727);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 7.77857142857144
//	FuzzyCompare(fitResult.fdist_F, 15750.25, 1.);
}

void FitTest::testLinearNoInt2() {
	//NIST data for NoInt2 dataset
	QVector<int> xData = {4,5,6};
	QVector<int> yData = {3,4,4};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "c * x";
	fitData.paramNames << "c";
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 1.;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max();
	//fitData.eps = 1.e-15;
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 1);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 0.727272727152573
	FuzzyCompare(fitResult.paramValues.at(0), 0.727272727272727, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 0.0420827316561797
	FuzzyCompare(fitResult.errorValues.at(0), 0.420827318078432E-01, 1.e-8);

	QCOMPARE(fitResult.rsd, 0.369274472937998);
//	QCOMPARE(fitResult.rsquare, 0.993348115299335);
	QCOMPARE(fitResult.sse, 0.272727272727273);
	QCOMPARE(fitResult.rms, 0.136363636363636);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 4.88888888888889
//	FuzzyCompare(fitResult.fdist_F, 298.666666666667, 1.);
}

void FitTest::testLinearNoInt2_2() {
	//NIST data for NoInt2 dataset
	QVector<int> xData = {4,5,6};
	QVector<int> yData = {3,4,4};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 1;
	XYFitCurve::initFitData(fitData);
	fitData.paramStartValues[0] = 0;
	fitData.paramFixed[0] = true;
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 2);

	QCOMPARE(fitResult.paramValues.at(0), 0.);
	QCOMPARE(fitResult.paramValues.at(1),  0.727272727272727);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 0.0595139700643615
//	QCOMPARE(fitResult.errorValues.at(1), 0.420827318078432e-1);

//	QCOMPARE(fitResult.rsd, 0.369274472937998);	// result: 0.522233
//	QCOMPARE(fitResult.rsquare, 0.993348115299335);	// result: 0.590909
	QCOMPARE(fitResult.sse, 0.272727272727273);
//	QCOMPARE(fitResult.rms, 0.136363636363636);	// result: 0.272727
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 2.44444444444445
//	FuzzyCompare(fitResult.fdist_F, 298.666666666667, 1.);
}

void FitTest::testLinearFilip() {
	//NIST data for Filip dataset
	QVector<double> xData = {-6.860120914,-4.324130045,-4.358625055,-4.358426747,-6.955852379,-6.661145254,-6.355462942,-6.118102026,
		-7.115148017,-6.815308569,-6.519993057,-6.204119983,-5.853871964,-6.109523091,-5.79832982,-5.482672118,-5.171791386,-4.851705903,
		-4.517126416,-4.143573228,-3.709075441,-3.499489089,-6.300769497,-5.953504836,-5.642065153,-5.031376979,-4.680685696,-4.329846955,
		-3.928486195,-8.56735134,-8.363211311,-8.107682739,-7.823908741,-7.522878745,-7.218819279,-6.920818754,-6.628932138,-6.323946875,
		-5.991399828,-8.781464495,-8.663140179,-8.473531488,-8.247337057,-7.971428747,-7.676129393,-7.352812702,-7.072065318,-6.774174009,
		-6.478861916,-6.159517513,-6.835647144,-6.53165267,-6.224098421,-5.910094889,-5.598599459,-5.290645224,-4.974284616,-4.64454848,
		-4.290560426,-3.885055584,-3.408378962,-3.13200249,-8.726767166,-8.66695597,-8.511026475,-8.165388579,-7.886056648,-7.588043762,
		-7.283412422,-6.995678626,-6.691862621,-6.392544977,-6.067374056,-6.684029655,-6.378719832,-6.065855188,-5.752272167,-5.132414673,
		-4.811352704,-4.098269308,-3.66174277,-3.2644011};
	QVector<double> yData = {0.8116,0.9072,0.9052,0.9039,0.8053,0.8377,0.8667,0.8809,0.7975,0.8162,0.8515,0.8766,0.8885,0.8859,0.8959,0.8913,
		0.8959,0.8971,0.9021,0.909,0.9139,0.9199,0.8692,0.8872,0.89,0.891,0.8977,0.9035,0.9078,0.7675,0.7705,0.7713,0.7736,0.7775,0.7841,
		0.7971,0.8329,0.8641,0.8804,0.7668,0.7633,0.7678,0.7697,0.77,0.7749,0.7796,0.7897,0.8131,0.8498,0.8741,0.8061,0.846,0.8751,0.8856,
		0.8919,0.8934,0.894,0.8957,0.9047,0.9129,0.9209,0.9219,0.7739,0.7681,0.7665,0.7703,0.7702,0.7761,0.7809,0.7961,0.8253,0.8602,
		0.8809,0.8301,0.8664,0.8834,0.8898,0.8964,0.8963,0.9074,0.9119,0.9228};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 10;
	XYFitCurve::initFitData(fitData);
	const int np = fitData.paramNames.size();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 11);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: -1467.48962615175
	FuzzyCompare(fitResult.paramValues.at(0), -1467.48961422980, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 298.084524514884
	FuzzyCompare(fitResult.errorValues.at(0), 298.084530995537, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: -2772.1796150428
	FuzzyCompare(fitResult.paramValues.at(1), -2772.17959193342, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 559.779853249694
	FuzzyCompare(fitResult.errorValues.at(1), 559.779865474950, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: -2316.37110148409
	FuzzyCompare(fitResult.paramValues.at(2), -2316.37108160893, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 466.477561928144
	FuzzyCompare(fitResult.errorValues.at(2), 466.477572127796, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result: -1127.97395097195
	FuzzyCompare(fitResult.paramValues.at(3), -1127.97394098372, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result: 227.204269523115
	FuzzyCompare(fitResult.errorValues.at(3), 227.204274477751, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(4));	// result: -354.478236951913
	FuzzyCompare(fitResult.paramValues.at(4), -354.478233703349, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(4));	// result: 71.6478645361214
	FuzzyCompare(fitResult.errorValues.at(4), 71.6478660875927, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(5));	// result: -75.1242024539908
	FuzzyCompare(fitResult.paramValues.at(5), -75.1242017393757, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(5));	// result: 15.289717547564
	FuzzyCompare(fitResult.errorValues.at(5), 15.2897178747400, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(6));	// result: -10.875318143236
	FuzzyCompare(fitResult.paramValues.at(6), -10.8753180355343, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(6));	// result: 2.23691155110776
	FuzzyCompare(fitResult.errorValues.at(6), 2.23691159816033, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(7));	// result: -1.06221499687347
	FuzzyCompare(fitResult.paramValues.at(7), -1.06221498588947, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(7));	// result: 0.221624317377432
	FuzzyCompare(fitResult.errorValues.at(7), 0.221624321934227, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(8));	// result: -0.0670191161850038
	FuzzyCompare(fitResult.paramValues.at(8), -0.670191154593408E-01, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(8));	// result: 0.0142363760310402
	FuzzyCompare(fitResult.errorValues.at(8), 0.142363763154724E-01, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(9));	// result: -0.00246781081080665
	FuzzyCompare(fitResult.paramValues.at(9), -0.246781078275479E-02, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(9));	// result: 0.000535617398555022
	FuzzyCompare(fitResult.errorValues.at(9), 0.535617408889821E-03, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(10));	// result: -4.02962529900222e-05
	FuzzyCompare(fitResult.paramValues.at(10), -0.402962525080404E-04, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(10));	// result: 8.96632820770946e-06
	FuzzyCompare(fitResult.errorValues.at(10), 0.896632837373868E-05, 1.e-7);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.00334801050105949
	FuzzyCompare(fitResult.rsd, 0.334801051324544E-02, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.rsquare);	// result: 0.996727416209443
	FuzzyCompare(fitResult.rsquare, 0.996727416185620, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.00079585137637953
	FuzzyCompare(fitResult.sse, 0.795851382172941E-03, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.rms);	// result: 1.12091743152047e-05
	FuzzyCompare(fitResult.rms, 0.112091743968020E-04, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 21695.3956090808
//	FuzzyCompare(fitResult.fdist_F, 2162.43954511489, 1.e-9);
}


void FitTest::testLinearWampler1() {
	//NIST data for Wampler1 dataset
	QVector<int> xData = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	QVector<int> yData = {1,6,63,364,1365,3906,9331,19608,37449,66430,111111,
		177156,271453,402234,579195,813616,1118481,1508598,2000719,2613660,3368421};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 5;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 6);

	for (int i = 0; i < np; i++) {
		const double paramValue = fitResult.paramValues.at(i);
		const double errorValue = fitResult.errorValues.at(i);
		QCOMPARE(paramValue, 1.0);
		QCOMPARE(errorValue, 0.0);
	}

	QCOMPARE(fitResult.rsd, 0.0);
	QCOMPARE(fitResult.rsquare,  1.0);
	QCOMPARE(fitResult.sse, 0.0);
	QCOMPARE(fitResult.rms, 0.0);
}

void FitTest::testLinearWampler2() {
	//NIST data for Wampler2 dataset
	QVector<int> xData = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	QVector<double> yData = {1.00000,1.11111,1.24992,1.42753,1.65984,1.96875,2.38336,2.94117,3.68928,4.68559,
		6.00000,7.71561,9.92992,12.75603,16.32384,20.78125,26.29536,33.05367,41.26528,51.16209,63.00000};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 5;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	qDebug() << "STATUS " << fitResult.status;
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 6);

	QCOMPARE(fitResult.paramValues.at(0), 1.0);
	QCOMPARE(fitResult.paramValues.at(1), 0.1);
	QCOMPARE(fitResult.paramValues.at(2), 0.01);
	QCOMPARE(fitResult.paramValues.at(3), 0.001);
	QCOMPARE(fitResult.paramValues.at(4), 0.0001);
	QCOMPARE(fitResult.paramValues.at(5), 0.00001);
	for (int i = 0; i < np; i++) {
		const double errorValue = fitResult.errorValues.at(i);
		FuzzyCompare(errorValue, 0., 1.);
	}

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 2.32458538254974e-15
	FuzzyCompare(fitResult.rsd, 0., 1.);
	QCOMPARE(fitResult.rsquare, 1.);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 8.1055458011459e-29
	FuzzyCompare(fitResult.sse, 0., 1.);
	DEBUG(std::setprecision(15) << fitResult.rms);	// result: 5.40369720076393e-30
	FuzzyCompare(fitResult.rms, 0., 1.);
	DEBUG(std::setprecision(15) << fitResult.fdist_F);	// result: 1.22192608844148e+33
}

void FitTest::testLinearWampler3() {
	//NIST data for Wampler3 dataset
	QVector<int> xData = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	QVector<double> yData = {760.,-2042.,2111.,-1684.,3888.,1858.,11379.,17560.,39287.,64382.,113159.,
		175108.,273291.,400186.,581243.,811568.,1121004.,1506550.,2002767.,2611612.,3369180.};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 5;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 6);

	for (int i = 0; i < np; i++) {
		const double paramValue = fitResult.paramValues.at(i);
		QCOMPARE(paramValue, 1.0);
	}
// TODO: actual errors are smaller
//	QCOMPARE(fitResult.errorValues.at(0), 2152.32624678170);
//	QCOMPARE(fitResult.errorValues.at(1), 2363.55173469681);
//	QCOMPARE(fitResult.errorValues.at(2), 779.343524331583);
//	QCOMPARE(fitResult.errorValues.at(3), 101.475507550350);
//	QCOMPARE(fitResult.errorValues.at(4), 5.64566512170752);
//	QCOMPARE(fitResult.errorValues.at(5), 0.112324854679312);

	QCOMPARE(fitResult.rsd, 2360.14502379268);
	QCOMPARE(fitResult.rsquare, 0.999995559025820);
	QCOMPARE(fitResult.sse, 83554268.0000000);
	QCOMPARE(fitResult.rms, 5570284.53333333);
//	QCOMPARE(fitResult.fdist_F, 675524.458240122);
}

void FitTest::testLinearWampler4() {
	//NIST data for Wampler4 dataset
	QVector<int> xData = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	QVector<int> yData = {75901,-204794,204863,-204436,253665,-200894,214131,-185192,221249,-138370,
		315911,-27644,455253,197434,783995,608816,1370781,1303798,2205519,2408860,3444321};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 5;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 6);

	for (int i = 0; i < np; i++) {
		const double paramValue = fitResult.paramValues.at(i);
		QCOMPARE(paramValue, 1.0);
	}
// TODO: actual errors are smaller
//	QCOMPARE(fitResult.errorValues.at(0), 215232.624678170);
//	QCOMPARE(fitResult.errorValues.at(1), 236355.173469681);
//	QCOMPARE(fitResult.errorValues.at(2), 77934.3524331583);
//	QCOMPARE(fitResult.errorValues.at(3), 10147.5507550350);
//	QCOMPARE(fitResult.errorValues.at(4), 564.566512170752);
//	QCOMPARE(fitResult.errorValues.at(5), 11.2324854679312);

	QCOMPARE(fitResult.rsd, 236014.502379268);
	QCOMPARE(fitResult.rsquare, 0.957478440825662);
	QCOMPARE(fitResult.sse, 835542680000.000);
	QCOMPARE(fitResult.rms, 55702845333.3333);
//	QCOMPARE(fitResult.fdist_F, 67.5524458240122);
}

void FitTest::testLinearWampler5() {
	//NIST data for Wampler5 dataset
	QVector<int> xData = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	QVector<int> yData = {7590001,-20479994,20480063,-20479636,25231365,-20476094,20489331,-20460392,18417449,-20413570,
				20591111,-20302844,18651453,-20077766,21059195,-19666384,26348481,-18971402,22480719,-17866340,10958421};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Integer);
	xDataColumn.replaceInteger(0, xData);

	Column yDataColumn("y", AbstractColumn::Integer);
	yDataColumn.replaceInteger(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_basic;
	fitData.modelType = nsl_fit_model_polynomial;
	fitData.degree = 5;
	XYFitCurve::initFitData(fitData);
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	const int np = fitData.paramNames.size();
	QCOMPARE(np, 6);

	for (int i = 0; i < np; i++) {
		const double paramValue = fitResult.paramValues.at(i);
		QCOMPARE(paramValue, 1.0);
	}
// TODO: actual errors are smaller
//	QCOMPARE(fitResult.errorValues.at(0), 21523262.4678170);
//	QCOMPARE(fitResult.errorValues.at(1), 23635517.3469681);
//	QCOMPARE(fitResult.errorValues.at(2), 7793435.24331583);
//	QCOMPARE(fitResult.errorValues.at(3), 1014755.07550350);
//	QCOMPARE(fitResult.errorValues.at(4), 56456.6512170752);
//	QCOMPARE(fitResult.errorValues.at(5), 1123.24854679312);

	QCOMPARE(fitResult.rsd, 23601450.2379268);
	QCOMPARE(fitResult.rsquare, 0.224668921574940E-02);
	QCOMPARE(fitResult.sse, 0.835542680000000E+16);
	QCOMPARE(fitResult.rms, 557028453333333.);
//	QCOMPARE(fitResult.fdist_F, 0.675524458240122E-02);
}

//##############################################################################
//#############  non-linear regression with NIST datasets  #####################
//##############################################################################

void FitTest::testNonLinearMisra1a() {
	//NIST data for Misra1a dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-exp(-b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 500. << 0.0001;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 238.942305251573
	FuzzyCompare(fitResult.paramValues.at(0), 2.3894212918E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 2.70651225218243
	FuzzyCompare(fitResult.errorValues.at(0), 2.7070075241E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000550155958419367
	FuzzyCompare(fitResult.paramValues.at(1), 5.5015643181E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 7.26565480949189e-06
	FuzzyCompare(fitResult.errorValues.at(1), 7.2668688436E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.101878763320394
	FuzzyCompare(fitResult.rsd, 1.0187876330E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.124551388988316
	FuzzyCompare(fitResult.sse, 1.2455138894E-01, 1.e-9);
}

void FitTest::testNonLinearMisra1a_2() {
	//NIST data for Misra1a dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-exp(-b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 250. << 5.e-4;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 238.942305251573
	FuzzyCompare(fitResult.paramValues.at(0), 2.3894212918E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 2.70651225218243
	FuzzyCompare(fitResult.errorValues.at(0), 2.7070075241E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000550155958419367
	FuzzyCompare(fitResult.paramValues.at(1), 5.5015643181E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 7.26565480949189e-06
	FuzzyCompare(fitResult.errorValues.at(1), 7.2668688436E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.101878763320394
	FuzzyCompare(fitResult.rsd, 1.0187876330E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.124551388988316
	FuzzyCompare(fitResult.sse, 1.2455138894E-01, 1.e-9);
}

void FitTest::testNonLinearMisra1a_3() {
	//NIST data for Misra1a dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-exp(-b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 2.3894212918E+02 << 5.5015643181E-04;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 238.942305251573
	FuzzyCompare(fitResult.paramValues.at(0), 2.3894212918E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 2.70651225218243
	FuzzyCompare(fitResult.errorValues.at(0), 2.7070075241E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000550155958419367
	FuzzyCompare(fitResult.paramValues.at(1), 5.5015643181E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 7.26565480949189e-06
	FuzzyCompare(fitResult.errorValues.at(1), 7.2668688436E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.101878763320394
	FuzzyCompare(fitResult.rsd, 1.0187876330E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.124551388988316
	FuzzyCompare(fitResult.sse, 1.2455138894E-01, 1.e-9);
}


void FitTest::testNonLinearMisra1b() {
	//NIST data for Misra1b dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./(1.+b2*x/2)^2)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 500. << 0.0001;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 337.99775062098
	FuzzyCompare(fitResult.paramValues.at(0), 3.3799746163E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.16358581006192
	FuzzyCompare(fitResult.errorValues.at(0), 3.1643950207E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000390390523934039
	FuzzyCompare(fitResult.paramValues.at(1), 3.9039091287E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 4.25373670682006e-06
	FuzzyCompare(fitResult.errorValues.at(1), 4.2547321834E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0793014720259488
	FuzzyCompare(fitResult.rsd, 7.9301471998E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0754646815857881
	FuzzyCompare(fitResult.sse, 7.5464681533E-02, 1.e-9);
}

void FitTest::testNonLinearMisra1b_2() {
	//NIST data for Misra1b dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./(1.+b2*x/2)^2)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 300. << 2.e-4;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 337.99775062098
	FuzzyCompare(fitResult.paramValues.at(0), 3.3799746163E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.16358581006192
	FuzzyCompare(fitResult.errorValues.at(0), 3.1643950207E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000390390523934039
	FuzzyCompare(fitResult.paramValues.at(1), 3.9039091287E-04, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 4.25373670682006e-06
	FuzzyCompare(fitResult.errorValues.at(1), 4.2547321834E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0793014720259488
	FuzzyCompare(fitResult.rsd, 7.9301471998E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0754646815857881
	FuzzyCompare(fitResult.sse, 7.5464681533E-02, 1.e-9);
}

void FitTest::testNonLinearMisra1b_3() {
	//NIST data for Misra1b dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./(1.+b2*x/2)^2)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 3.3799746163E+02 << 3.9039091287E-04;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 337.99775062098
	FuzzyCompare(fitResult.paramValues.at(0), 3.3799746163E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.16358581006192
	FuzzyCompare(fitResult.errorValues.at(0), 3.1643950207E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000390390523934039
	FuzzyCompare(fitResult.paramValues.at(1), 3.9039091287E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 4.25373670682006e-06
	FuzzyCompare(fitResult.errorValues.at(1), 4.2547321834E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0793014720259488
	FuzzyCompare(fitResult.rsd, 7.9301471998E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0754646815857881
	FuzzyCompare(fitResult.sse, 7.5464681533E-02, 1.e-9);
}

void FitTest::testNonLinearMisra1c() {
	//NIST data for Misra1c dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./sqrt(1.+2.*b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 500. << 0.0001;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 636.427904767969
	FuzzyCompare(fitResult.paramValues.at(0), 6.3642725809E+02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 4.66168062054875
	FuzzyCompare(fitResult.errorValues.at(0), 4.6638326572E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000208136026420746
	FuzzyCompare(fitResult.paramValues.at(1), 2.0813627256E-04, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 1.77209416674174e-06
	FuzzyCompare(fitResult.errorValues.at(1), 1.7728423155E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0584286153041661
	FuzzyCompare(fitResult.rsd, 5.8428615257E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0409668370363468
	FuzzyCompare(fitResult.sse, 4.0966836971E-02, 1.e-8);
}

void FitTest::testNonLinearMisra1c_2() {
	//NIST data for Misra1c dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./sqrt(1.+2.*b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 600. << 2.e-4;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 636.427904767969
	FuzzyCompare(fitResult.paramValues.at(0), 6.3642725809E+02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 4.66168062054875
	FuzzyCompare(fitResult.errorValues.at(0), 4.6638326572E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000208136026420746
	FuzzyCompare(fitResult.paramValues.at(1), 2.0813627256E-04, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 1.77209416674174e-06
	FuzzyCompare(fitResult.errorValues.at(1), 1.7728423155E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0584286153041661
	FuzzyCompare(fitResult.rsd, 5.8428615257E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0409668370363468
	FuzzyCompare(fitResult.sse, 4.0966836971E-02, 1.e-8);
}

void FitTest::testNonLinearMisra1c_3() {
	//NIST data for Misra1c dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(1.-1./sqrt(1.+2.*b2*x))";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 6.3642725809E+02 << 2.0813627256E-04;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 636.427904767969
	FuzzyCompare(fitResult.paramValues.at(0), 6.3642725809E+02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 4.66168062054875
	FuzzyCompare(fitResult.errorValues.at(0), 4.6638326572E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000208136026420746
	FuzzyCompare(fitResult.paramValues.at(1), 2.0813627256E-04, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 1.77209416674174e-06
	FuzzyCompare(fitResult.errorValues.at(1), 1.7728423155E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.0584286153041661
	FuzzyCompare(fitResult.rsd, 5.8428615257E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.0409668370363468
	FuzzyCompare(fitResult.sse, 4.0966836971E-02, 1.e-8);
}

void FitTest::testNonLinearMisra1d() {
	//NIST data for Misra1d dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*b2*x/(1.+b2*x)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 500. << 0.0001;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 437.370039987725
	FuzzyCompare(fitResult.paramValues.at(0), 4.3736970754E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.64772833062694
	FuzzyCompare(fitResult.errorValues.at(0), 3.6489174345E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000302272976784709
	FuzzyCompare(fitResult.paramValues.at(1), 3.0227324449E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 2.93256059733558e-06
	FuzzyCompare(fitResult.errorValues.at(1), 2.9334354479E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.068568272134244
	FuzzyCompare(fitResult.rsd, 6.8568272111E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.056419295321709
	FuzzyCompare(fitResult.sse, 5.6419295283E-02, 1.e-9);
}

void FitTest::testNonLinearMisra1d_2() {
	//NIST data for Misra1d dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*b2*x/(1.+b2*x)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 450. << 3.e-4;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 437.370039987725
	FuzzyCompare(fitResult.paramValues.at(0), 4.3736970754E+02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.64772833062694
	FuzzyCompare(fitResult.errorValues.at(0), 3.6489174345E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000302272976784709
	FuzzyCompare(fitResult.paramValues.at(1), 3.0227324449E-04, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 2.93256059733558e-06
	FuzzyCompare(fitResult.errorValues.at(1), 2.9334354479E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.068568272134244
	FuzzyCompare(fitResult.rsd, 6.8568272111E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.056419295321709
	FuzzyCompare(fitResult.sse, 5.6419295283E-02, 1.e-8);
}

void FitTest::testNonLinearMisra1d_3() {
	//NIST data for Misra1d dataset
	QVector<double> xData = {77.6E0,114.9E0,141.1E0,190.8E0,239.9E0,289.0E0,332.8E0,378.4E0,434.8E0,477.3E0,536.8E0,593.1E0,689.1E0,760.0E0};
	QVector<double> yData = {10.07E0,14.73E0,17.94E0,23.93E0,29.61E0,35.18E0,40.02E0,44.82E0,50.76E0,55.05E0,61.01E0,66.40E0,75.47E0,81.78E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*b2*x/(1.+b2*x)";
	fitData.paramNames << "b1" << "b2";
	fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 4.3736970754E+02 << 3.0227324449E-04;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 2);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 437.370039987725
	FuzzyCompare(fitResult.paramValues.at(0), 4.3736970754E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 3.64772833062694
	FuzzyCompare(fitResult.errorValues.at(0), 3.6489174345E+00, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 0.000302272976784709
	FuzzyCompare(fitResult.paramValues.at(1), 3.0227324449E-04, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 2.93256059733558e-06
	FuzzyCompare(fitResult.errorValues.at(1), 2.9334354479E-06, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 0.068568272134244
	FuzzyCompare(fitResult.rsd, 6.8568272111E-02, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 0.056419295321709
	FuzzyCompare(fitResult.sse, 5.6419295283E-02, 1.e-9);
}

void FitTest::testNonLinearMGH09() {
	//NIST data for MGH09 dataset
	QVector<double> xData = {4.000000E+00,2.000000E+00,1.000000E+00,5.000000E-01,2.500000E-01,1.670000E-01,1.250000E-01,1.000000E-01,
		8.330000E-02,7.140000E-02,6.250000E-02};
	QVector<double> yData = {1.957000E-01,1.947000E-01,1.735000E-01,1.600000E-01,8.440000E-02,6.270000E-02,4.560000E-02,3.420000E-02,
		3.230000E-02,2.350000E-02,2.460000E-02};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(x^2 + b2*x)/(x^2 + x*b3 + b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 2.5000000000E+01 << 3.9000000000E+01 << 4.1500000000E+01 << 3.9000000000E+01;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

// TODO: fit does not work
/*	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result:
	FuzzyCompare(fitResult.paramValues.at(0), 1.9280693458E-01, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result:
	FuzzyCompare(fitResult.errorValues.at(0), 1.1435312227E-02, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result:
	FuzzyCompare(fitResult.paramValues.at(1), 1.9128232873E-01, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result:
	FuzzyCompare(fitResult.errorValues.at(1), 1.9633220911E-01, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result:
	FuzzyCompare(fitResult.paramValues.at(2), 1.2305650693E-01, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result:
	FuzzyCompare(fitResult.errorValues.at(2), 8.0842031232E-02, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result:
	FuzzyCompare(fitResult.paramValues.at(3), 1.3606233068E-01, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result:
	FuzzyCompare(fitResult.errorValues.at(3), 9.0025542308E-02, 1.e-3);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result:
	FuzzyCompare(fitResult.rsd, 6.6279236551E-03, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result:
	FuzzyCompare(fitResult.sse, 3.0750560385E-04, 1.e-9);
*/
}

void FitTest::testNonLinearMGH09_2() {
	//NIST data for MGH09 dataset
	QVector<double> xData = {4.000000E+00,2.000000E+00,1.000000E+00,5.000000E-01,2.500000E-01,1.670000E-01,1.250000E-01,1.000000E-01,
		8.330000E-02,7.140000E-02,6.250000E-02};
	QVector<double> yData = {1.957000E-01,1.947000E-01,1.735000E-01,1.600000E-01,8.440000E-02,6.270000E-02,4.560000E-02,3.420000E-02,
		3.230000E-02,2.350000E-02,2.460000E-02};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(x^2 + b2*x)/(x^2 + x*b3 + b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 2.5000000000E-01 << 3.9000000000E-01 << 4.1500000000E-01 << 3.9000000000E-01;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result:
	FuzzyCompare(fitResult.paramValues.at(0), 1.9280693458E-01, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result:
	FuzzyCompare(fitResult.errorValues.at(0), 1.1435312227E-02, 1.e-4);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result:
	FuzzyCompare(fitResult.paramValues.at(1), 1.9128232873E-01, 1.e-3);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result:
	FuzzyCompare(fitResult.errorValues.at(1), 1.9633220911E-01, 1.e-4);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result:
	FuzzyCompare(fitResult.paramValues.at(2), 1.2305650693E-01, 1.e-4);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result:
	FuzzyCompare(fitResult.errorValues.at(2), 8.0842031232E-02, 1.e-4);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result:
	FuzzyCompare(fitResult.paramValues.at(3), 1.3606233068E-01, 1.e-4);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result:
	FuzzyCompare(fitResult.errorValues.at(3), 9.0025542308E-02, 1.e-4);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result:
	FuzzyCompare(fitResult.rsd, 6.6279236551E-03, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result:
	FuzzyCompare(fitResult.sse, 3.0750560385E-04, 1.e-8);
}

void FitTest::testNonLinearMGH09_3() {
	//NIST data for MGH09 dataset
	QVector<double> xData = {4.000000E+00,2.000000E+00,1.000000E+00,5.000000E-01,2.500000E-01,1.670000E-01,1.250000E-01,1.000000E-01,
		8.330000E-02,7.140000E-02,6.250000E-02};
	QVector<double> yData = {1.957000E-01,1.947000E-01,1.735000E-01,1.600000E-01,8.440000E-02,6.270000E-02,4.560000E-02,3.420000E-02,
		3.230000E-02,2.350000E-02,2.460000E-02};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*(x^2 + b2*x)/(x^2 + x*b3 + b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 1.9280693458E-01 << 1.9128232873E-01 << 1.2305650693E-01 << 1.3606233068E-01;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result:
	FuzzyCompare(fitResult.paramValues.at(0), 1.9280693458E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result:
	FuzzyCompare(fitResult.errorValues.at(0), 1.1435312227E-02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result:
	FuzzyCompare(fitResult.paramValues.at(1), 1.9128232873E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result:
	FuzzyCompare(fitResult.errorValues.at(1), 1.9633220911E-01, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result:
	FuzzyCompare(fitResult.paramValues.at(2), 1.2305650693E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result:
	FuzzyCompare(fitResult.errorValues.at(2), 8.0842031232E-02, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result:
	FuzzyCompare(fitResult.paramValues.at(3), 1.3606233068E-01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result:
	FuzzyCompare(fitResult.errorValues.at(3), 9.0025542308E-02, 1.e-5);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result:
	FuzzyCompare(fitResult.rsd, 6.6279236551E-03, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result:
	FuzzyCompare(fitResult.sse, 3.0750560385E-04, 1.e-9);
}

void FitTest::testNonLinearMGH10() {
	//NIST data for MGH10 dataset
	QVector<double> xData = {5.000000E+01,5.500000E+01,6.000000E+01,6.500000E+01,7.000000E+01,7.500000E+01,8.000000E+01,8.500000E+01,9.000000E+01,
		9.500000E+01,1.000000E+02,1.050000E+02,1.100000E+02,1.150000E+02,1.200000E+02,1.250000E+02};
	QVector<double> yData = {3.478000E+04,2.861000E+04,2.365000E+04,1.963000E+04,1.637000E+04,1.372000E+04,1.154000E+04,9.744000E+03,8.261000E+03,
		7.030000E+03,6.005000E+03,5.147000E+03,4.427000E+03,3.820000E+03,3.307000E+03,2.872000E+03};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*exp(b2/(x+b3))";
	fitData.paramNames << "b1" << "b2" << "b3";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 2.0000000000E+00 << 4.0000000000E+05 << 2.5000000000E+04;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 3);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 0.00560963848336205 (Windows: 0.00560964247264364)
	FuzzyCompare(fitResult.paramValues.at(0), 5.6096364710E-03, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 6.03192041389405e-05
//	FuzzyCompare(fitResult.errorValues.at(0), 1.5687892471E-04, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 6181.34604697191 (Windows: 6181.34545410281)
	FuzzyCompare(fitResult.paramValues.at(1), 6.1813463463E+03, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 8.96223793887241
//	FuzzyCompare(fitResult.errorValues.at(1), 2.3309021107E+01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 345.223624540718
	FuzzyCompare(fitResult.paramValues.at(2), 3.4522363462E+02, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.301777581563284
//	FuzzyCompare(fitResult.errorValues.at(2), 7.8486103508E-01, 1.e-9);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 2.6009740064923	(Windows: 2.60097400662837)
	FuzzyCompare(fitResult.rsd, 2.6009740065E+00, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 87.9458551718321	(FreeBSD: 87.9458551726946, Windows: 87.9458551810338)
	FuzzyCompare(fitResult.sse, 8.7945855171E+01, 1.e-9);
}

void FitTest::testNonLinearMGH10_2() {
	//NIST data for MGH10 dataset
	QVector<double> xData = {5.000000E+01,5.500000E+01,6.000000E+01,6.500000E+01,7.000000E+01,7.500000E+01,8.000000E+01,8.500000E+01,9.000000E+01,
		9.500000E+01,1.000000E+02,1.050000E+02,1.100000E+02,1.150000E+02,1.200000E+02,1.250000E+02};
	QVector<double> yData = {3.478000E+04,2.861000E+04,2.365000E+04,1.963000E+04,1.637000E+04,1.372000E+04,1.154000E+04,9.744000E+03,8.261000E+03,
		7.030000E+03,6.005000E+03,5.147000E+03,4.427000E+03,3.820000E+03,3.307000E+03,2.872000E+03};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*exp(b2/(x+b3))";
	fitData.paramNames << "b1" << "b2" << "b3";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 2.0000000000E-02 << 4.0000000000E+03 << 2.5000000000E+02;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 3);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 0.00560963848336205
	FuzzyCompare(fitResult.paramValues.at(0), 5.6096364710E-03, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 6.03192041389405e-05
//	FuzzyCompare(fitResult.errorValues.at(0), 1.5687892471E-04, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 6181.34604697191
	FuzzyCompare(fitResult.paramValues.at(1), 6.1813463463E+03, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 8.96223793887241
//	FuzzyCompare(fitResult.errorValues.at(1), 2.3309021107E+01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 345.223624540718
	FuzzyCompare(fitResult.paramValues.at(2), 3.4522363462E+02, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.301777581563284
//	FuzzyCompare(fitResult.errorValues.at(2), 7.8486103508E-01, 1.e-9);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 2.6009740064923
	FuzzyCompare(fitResult.rsd, 2.6009740065E+00, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 87.9458551718321
	FuzzyCompare(fitResult.sse, 8.7945855171E+01, 1.e-9);
}

void FitTest::testNonLinearMGH10_3() {
	//NIST data for MGH10 dataset
	QVector<double> xData = {5.000000E+01,5.500000E+01,6.000000E+01,6.500000E+01,7.000000E+01,7.500000E+01,8.000000E+01,8.500000E+01,9.000000E+01,
		9.500000E+01,1.000000E+02,1.050000E+02,1.100000E+02,1.150000E+02,1.200000E+02,1.250000E+02};
	QVector<double> yData = {3.478000E+04,2.861000E+04,2.365000E+04,1.963000E+04,1.637000E+04,1.372000E+04,1.154000E+04,9.744000E+03,8.261000E+03,
		7.030000E+03,6.005000E+03,5.147000E+03,4.427000E+03,3.820000E+03,3.307000E+03,2.872000E+03};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1*exp(b2/(x+b3))";
	fitData.paramNames << "b1" << "b2" << "b3";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 5.6096364710E-03 << 6.1813463463E+03 << 3.4522363462E+02;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 3);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 0.00560963848336205
	FuzzyCompare(fitResult.paramValues.at(0), 5.6096364710E-03, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 6.03192041389405e-05
//	FuzzyCompare(fitResult.errorValues.at(0), 1.5687892471E-04, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 6181.34604697191
	FuzzyCompare(fitResult.paramValues.at(1), 6.1813463463E+03, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 8.96223793887241
//	FuzzyCompare(fitResult.errorValues.at(1), 2.3309021107E+01, 1.e-9);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 345.223624540718
	FuzzyCompare(fitResult.paramValues.at(2), 3.4522363462E+02, 1.e-7);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.301777581563284
//	FuzzyCompare(fitResult.errorValues.at(2), 7.8486103508E-01, 1.e-9);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 2.6009740064923
	FuzzyCompare(fitResult.rsd, 2.6009740065E+00, 1.e-11);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 87.9458551718321
	FuzzyCompare(fitResult.sse, 8.7945855171E+01, 1.e-11);
}

void FitTest::testNonLinearRat43() {
	//NIST data for Rat43 dataset
	QVector<double> xData = {1.0E0,2.0E0,3.0E0,4.0E0,5.0E0,6.0E0,7.0E0,8.0E0,9.0E0,10.0E0,11.0E0,12.0E0,13.0E0,14.0E0,15.0E0};
	QVector<double> yData = {16.08E0,33.83E0,65.80E0,97.20E0,191.55E0,326.20E0,386.87E0,520.53E0,590.03E0,651.92E0,724.93E0,699.56E0,689.96E0,637.56E0,717.41E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1/pow(1. + exp(b2-b3*x), 1/b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 1.0000000000E+02 << 1.0000000000E+01 << 1.0000000000E+00 << 1.0000000000E+00;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 699.641340982193
	FuzzyCompare(fitResult.paramValues.at(0), 6.9964151270E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 0.576817395963223
//	FuzzyCompare(fitResult.errorValues.at(0), 1.6302297817E+01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 5.2771555758844
	FuzzyCompare(fitResult.paramValues.at(1), 5.2771253025E+00, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 0.0736985984370786
//	FuzzyCompare(fitResult.errorValues.at(1), 2.0828735829E+00, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 0.759632366113637
	FuzzyCompare(fitResult.paramValues.at(2), 7.5962938329E-01, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.00692313004796671
//	FuzzyCompare(fitResult.errorValues.at(2), 1.9566123451E-01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result: 1.27925748993867
	FuzzyCompare(fitResult.paramValues.at(3), 1.2792483859E+00, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result: 0.0243301036770382
//	FuzzyCompare(fitResult.errorValues.at(3), 6.8761936385E-01, 1.e-10);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 28.2624146626284
	FuzzyCompare(fitResult.rsd, 2.8262414662E+01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 8786.4049081859
	FuzzyCompare(fitResult.sse, 8.7864049080E+03, 1.e-10);
}

void FitTest::testNonLinearRat43_2() {
	//NIST data for Rat43 dataset
	QVector<double> xData = {1.0E0,2.0E0,3.0E0,4.0E0,5.0E0,6.0E0,7.0E0,8.0E0,9.0E0,10.0E0,11.0E0,12.0E0,13.0E0,14.0E0,15.0E0};
	QVector<double> yData = {16.08E0,33.83E0,65.80E0,97.20E0,191.55E0,326.20E0,386.87E0,520.53E0,590.03E0,651.92E0,724.93E0,699.56E0,689.96E0,637.56E0,717.41E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1/pow(1. + exp(b2-b3*x), 1/b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 7.0000000000E+02 << 5.0000000000E+00 << 7.5000000000E-01 << 1.3000000000E+00;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 699.641340982193
	FuzzyCompare(fitResult.paramValues.at(0), 6.9964151270E+02, 1.e-6);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 0.576817395963223
//	FuzzyCompare(fitResult.errorValues.at(0), 1.6302297817E+01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 5.2771555758844
	FuzzyCompare(fitResult.paramValues.at(1), 5.2771253025E+00, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 0.0736985984370786
//	FuzzyCompare(fitResult.errorValues.at(1), 2.0828735829E+00, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 0.759632366113637
	FuzzyCompare(fitResult.paramValues.at(2), 7.5962938329E-01, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.00692313004796671
//	FuzzyCompare(fitResult.errorValues.at(2), 1.9566123451E-01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result: 1.27925748993867
	FuzzyCompare(fitResult.paramValues.at(3), 1.2792483859E+00, 1.e-5);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result: 0.0243301036770382
//	FuzzyCompare(fitResult.errorValues.at(3), 6.8761936385E-01, 1.e-10);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 28.2624146626284
	FuzzyCompare(fitResult.rsd, 2.8262414662E+01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 8786.4049081859
	FuzzyCompare(fitResult.sse, 8.7864049080E+03, 1.e-11);
}

void FitTest::testNonLinearRat43_3() {
	//NIST data for Rat43 dataset
	QVector<double> xData = {1.0E0,2.0E0,3.0E0,4.0E0,5.0E0,6.0E0,7.0E0,8.0E0,9.0E0,10.0E0,11.0E0,12.0E0,13.0E0,14.0E0,15.0E0};
	QVector<double> yData = {16.08E0,33.83E0,65.80E0,97.20E0,191.55E0,326.20E0,386.87E0,520.53E0,590.03E0,651.92E0,724.93E0,699.56E0,689.96E0,637.56E0,717.41E0};

	//data source columns
	Column xDataColumn("x", AbstractColumn::Numeric);
	xDataColumn.replaceValues(0, xData);

	Column yDataColumn("y", AbstractColumn::Numeric);
	yDataColumn.replaceValues(0, yData);

	XYFitCurve fitCurve("fit");
	fitCurve.setXDataColumn(&xDataColumn);
	fitCurve.setYDataColumn(&yDataColumn);

	//prepare the fit
	XYFitCurve::FitData fitData = fitCurve.fitData();
	fitData.modelCategory = nsl_fit_model_custom;
	XYFitCurve::initFitData(fitData);
	fitData.model = "b1/pow(1. + exp(b2-b3*x), 1/b4)";
	fitData.paramNames << "b1" << "b2" << "b3" << "b4";
	//fitData.eps = 1.e-12;
	const int np = fitData.paramNames.size();
	fitData.paramStartValues << 6.9964151270E+02 << 5.2771253025E+00 << 7.5962938329E-01 << 1.2792483859E+00;
	fitData.paramLowerLimits << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max() << -std::numeric_limits<double>::max();
	fitData.paramUpperLimits << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max() << std::numeric_limits<double>::max();
	fitCurve.setFitData(fitData);

	//perform the fit
	fitCurve.recalculate();
	const XYFitCurve::FitResult& fitResult = fitCurve.fitResult();

	//check the results
	QCOMPARE(fitResult.available, true);
	QCOMPARE(fitResult.valid, true);

	QCOMPARE(np, 4);

	DEBUG(std::setprecision(15) << fitResult.paramValues.at(0));	// result: 699.641340982193
	FuzzyCompare(fitResult.paramValues.at(0), 6.9964151270E+02, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(0));	// result: 0.576817395963223
//	FuzzyCompare(fitResult.errorValues.at(0), 1.6302297817E+01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(1));	// result: 5.2771555758844
	FuzzyCompare(fitResult.paramValues.at(1), 5.2771253025E+00, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(1));	// result: 0.0736985984370786
//	FuzzyCompare(fitResult.errorValues.at(1), 2.0828735829E+00, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(2));	// result: 0.759632366113637
	FuzzyCompare(fitResult.paramValues.at(2), 7.5962938329E-01, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(2));	// result: 0.00692313004796671
//	FuzzyCompare(fitResult.errorValues.at(2), 1.9566123451E-01, 1.e-10);
	DEBUG(std::setprecision(15) << fitResult.paramValues.at(3));	// result: 1.27925748993867
	FuzzyCompare(fitResult.paramValues.at(3), 1.2792483859E+00, 1.e-8);
	DEBUG(std::setprecision(15) << fitResult.errorValues.at(3));	// result: 0.0243301036770382
//	FuzzyCompare(fitResult.errorValues.at(3), 6.8761936385E-01, 1.e-10);

	DEBUG(std::setprecision(15) << fitResult.rsd);	// result: 28.2624146626284
	FuzzyCompare(fitResult.rsd, 2.8262414662E+01, 1.e-11);
	DEBUG(std::setprecision(15) << fitResult.sse);	// result: 8786.4049081859
	FuzzyCompare(fitResult.sse, 8.7864049080E+03, 1.e-11);
}

//TODO: more tests

//##############################################################################
//#########################  Fits with weights #################################
//##############################################################################

//TODO: add tests

QTEST_MAIN(FitTest)
