/***************************************************************************
    File                 : TeXRenderer.cc
    Project              : LabPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2008 by Alexander Semke (alexander.semke*web.de)
    Copyright            : (C) 2012 by Stefan Gerlach (stefan.gerlach*uni-konstanz.de)
    					(use @ for *)
    Description          : TeX renderer class
                           
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
#include "TeXRenderer.h"

#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
#include <KTempDir>
#include <KProcess>
#include <KDebug>
#endif
#include <QDir>
#include <QTemporaryFile>
#include <QTextStream>
#include <QProcess>

// use (pdf)latex to render LaTeX text (see tex2im, etc.)
// TODO: test convert to svg and render to qimage, test dvipng
// TODO: font size

bool TeXRenderer::renderImageLaTeX( const QString& teXString, QImage& image, int fontSize, QColor fontColor){
#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
	// kWarning()<<teXString<<endl;
#endif
	QTemporaryFile file("/dev/shm/labplot_XXXXXX.tex");
	// for debugging *.tex file
	//file.setAutoRemove(false);
	if(file.open()) {
		QDir::setCurrent("/dev/shm");
	}
	else {
#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
		kWarning()<<"/dev/shm failed. using /tmp"<<endl;
#endif
		file.setFileTemplate("/tmp/labplot_XXXXXX.tex");
		if(file.open())
			QDir::setCurrent("/tmp");
		else
			return false;
	}

	// create latex code
	QTextStream out(&file);
	//TODO: use font size (or later in convert process)
	out<<"\\documentclass[12pt]{article}\n\\usepackage[dvipsnames]{color}\n";
	out<<"\\pagestyle{empty}\n\\pagecolor{white}\n\\begin{document}\n";
	out<<"\\definecolor{fontcolor}{rgb}{"<<fontColor.redF()<<','<<fontColor.greenF()<<','<<fontColor.blueF()<<"}\n";
	out<<"{\\color{fontcolor}\n";
	out<<teXString;
	out<<"\n}\n\\end{document}";
	out.flush();

	// pdflatex: TeX -> PDF
	QProcess latexProcess, convertProcess;
	latexProcess.start("pdflatex", QStringList() << "-interaction=batchmode" << file.fileName());

	QFileInfo fi(file.fileName());
	if (latexProcess.waitForFinished()) { 	// pdflatex finished
		//qDebug()<<latexProcess.exitCode();
		if(latexProcess.exitCode() != 0)	// skip if pdflatex failed
			return false;

		// crop PDF
		QProcess cropProcess;
		cropProcess.start("pdfcrop", QStringList() << fi.completeBaseName()+".pdf");
		if (!cropProcess.waitForFinished()) {
#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
		kWarning()<<"pdfcrop failed. using latex"<<endl;
#endif
		}
		else {
			// convert: PDF -> PNG
			convertProcess.start("convert", QStringList() << "-density" << "300x300" << fi.completeBaseName()+"-crop.pdf" << fi.completeBaseName()+".png");
			if (convertProcess.waitForFinished()) {
				// read png file
				image.load(fi.completeBaseName()+".png");

				// clean up
				QFile::remove(fi.completeBaseName()+".pdf");
				QFile::remove(fi.completeBaseName()+"-crop.pdf");
				QFile::remove(fi.completeBaseName()+".png");
				// also possible: latexmf -C
				QFile::remove(fi.completeBaseName()+".aux");
				QFile::remove(fi.completeBaseName()+".log");
				return true;
			}
		}
	}
	else {
#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
		kWarning()<<"pdflatex failed. using latex"<<endl;
#endif
	}

	// latex: TeX -> DVI
	latexProcess.start("latex", QStringList() << "-interaction=batchmode" << file.fileName());
	if (!latexProcess.waitForFinished())
		return false;
	if(latexProcess.exitCode() != 0)	// skip if latex failed
		return false;

	// dvips: DVI -> PS
	QProcess dvipsProcess;
	dvipsProcess.start("dvips", QStringList() << "-E" << fi.completeBaseName());
	if (!dvipsProcess.waitForFinished())
		return false;

	// convert: PS -> PNG
	convertProcess.start("convert", QStringList() << "-density" << "300x300" << fi.completeBaseName()+".ps" << fi.completeBaseName()+".png");
	if (!convertProcess.waitForFinished())
		return false;

	// read png file
	image.load(fi.completeBaseName()+".png");

	//clean up
	QFile::remove(fi.completeBaseName()+".png");
	QFile::remove(fi.completeBaseName()+".dvi");
	QFile::remove(fi.completeBaseName()+".ps");
	// also possible: latexmf -C
	QFile::remove(fi.completeBaseName()+".aux");
	QFile::remove(fi.completeBaseName()+".log");

	return true;
}

// old method using texvc to render LaTeX text
//TODO make this function using Qt only?
bool TeXRenderer::renderImageTeXvc( const QString& texString, QImage& image){
#ifndef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
	KTempDir *tmpDir = new KTempDir();
	QString dirName = tmpDir->name();
// 	kDebug()<<"temporary directory "<<dirName<<" is used"<<endl;

	KProcess *proc = new KProcess;
	*proc<<"texvc";
	*proc<<"/tmp"<<dirName<<texString;

	int exitCode=proc->execute();
	kDebug()<<"texvc's exit code "<<exitCode<<endl;
	if( exitCode==-2 ) {
		kDebug()<<"Couldn't find texvc."<<endl;
		return false;
	}else if (exitCode==-1){
		kDebug()<<"Texvc crashed."<<endl;
		return false;
	}

	// take resulting image and show it
 	QDir d(dirName);
 	if (d.count()!=3){
 		kDebug()<<"No file created. Check the syntax."<<endl;
 		return false;

	}

 	QString fileName = dirName+QString(d[2]);
// 	kDebug()<<"file name "<<fileName<<" is used"<<endl;
	if ( !image.load(fileName) ){
		kDebug()<<"Error on loading the tex-image"<<endl;
		tmpDir->unlink();
		return false;
	}

 	tmpDir->unlink();
	kDebug()<<"image created."<<endl;
#endif
	return true;
}
