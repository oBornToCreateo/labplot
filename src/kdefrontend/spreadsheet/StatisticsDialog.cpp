#include "StatisticsDialog.h"
#include "backend/core/column/Column.h"
#include "math.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QDesktopWidget>

StatisticsDialog::StatisticsDialog(const QString & title, QWidget *parent) :
    KDialog(parent){

    QWidget* mainWidget = new QWidget(this);
    ui.setupUi(mainWidget);
    setMainWidget(mainWidget);

    ui.tw_statisticsTabs->removeTab(0);
    ui.tw_statisticsTabs->removeTab(0);

    setWindowTitle(title);
    setButtons(KDialog::Ok);
    setButtonText(KDialog::Ok, i18n("&Ok"));

    m_htmlText = QString("<table border=0 width=100%>"
                         "<tr>"
                         "<td colspan=2 align=center bgcolor=#D1D1D1><b><big>Location measures</big><b></td>"
                         "</tr>"
                         "<tr></tr>"
                         "<tr>"
                         "<td width=38%><b>Minimum<b></td>"
                         "<td>%1</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Maximum<b></td>"
                         "<td>%2</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Arithmetic mean<b></td>"
                         "<td>%3</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Geometric mean<b></td>"
                         "<td>%4</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Harmonic mean<b></td>"
                         "<td>%5</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Contraharmonic mean<b></td>"
                         "<td>%6</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Median<b></td>"
                         "<td>%7</td>"
                         "</tr>"
                         "<tr></tr>"
                         "<tr>"
                         "<td colspan=2 align=center bgcolor=#D1D1D1><b><big>Dispersion measures</big></b></td>"
                         "</tr>"
                         "<tr></tr>"
                         "<tr>"
                         "<td><b>Variance<b></td>"
                         "<td>%8</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Standard deviation<b></td>"
                         "<td>%9</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Mean absolute deviation around mean<b></td>"
                         "<td>%10</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Mean absolute deviation around median<b></td>"
                         "<td>%11</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Median absolute deviation<b></td>"
                         "<td>%12</td>"
                         "</tr>"
                         "<tr></tr>"
                         "<tr>"
                         "<td colspan=2 align=center bgcolor=#D1D1D1><b><big>Shape measures</big></b></td>"
                         "</tr>"
                         "<tr></tr>"
                         "<tr>"
                         "<td><b>Skewness<b></td>"
                         "<td>%13</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Kurtosis<b></td>"
                         "<td>%14</td>"
                         "</tr>"
                         "<tr>"
                         "<td><b>Entropy<b></td>"
                         "<td>%15</td>"
                         "</tr>"
                         "</table>");

    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    connect(ui.tw_statisticsTabs, SIGNAL(currentChanged(int)), this, SLOT(calculateStatisticsOnCurrentTab(int)));
    connect(this, SIGNAL(okClicked()), this, SLOT(close()));
}

void StatisticsDialog::setColumns(const QList<Column *> &columns){
    m_columns = columns;
}

void StatisticsDialog::addColumn(Column *col){
    m_columns << col;
}

void StatisticsDialog::showEvent(QShowEvent * event){
    addTabs();
    KDialog::showEvent(event);
}

const QString StatisticsDialog::isNanValue(const double value){
    return (isnan(value) ? i18n("The value couldn't be calculated.") : QString::number(value));
}

QSize StatisticsDialog::sizeHint() const{
    return QSize(430, 500);
}

void StatisticsDialog::addTabs(){
    for (int i = 0; i < m_columns.size(); ++i){
        QTextEdit* textEdit = new QTextEdit;
        textEdit->setReadOnly(true);
        if (i == 0){
            m_columns[0]->calculateStatistics();
            textEdit->setHtml(m_htmlText.arg(isNanValue(m_columns[0]->statistics().minimum)).
                    arg(isNanValue(m_columns[0]->statistics().maximum)).
                    arg(isNanValue(m_columns[0]->statistics().arithmeticMean)).
                    arg(isNanValue(m_columns[0]->statistics().geometricMean)).
                    arg(isNanValue(m_columns[0]->statistics().harmonicMean)).
                    arg(isNanValue(m_columns[0]->statistics().contraharmonicMean)).
					arg(isNanValue(m_columns[0]->statistics().median)).
                    arg(isNanValue(m_columns[0]->statistics().variance)).
                    arg(isNanValue(m_columns[0]->statistics().standardDeviation)).
                    arg(isNanValue(m_columns[0]->statistics().meanDeviation)).
					arg(isNanValue(m_columns[0]->statistics().meanDeviationAroundMedian)).
                    arg(isNanValue(m_columns[0]->statistics().medianDeviation)).
                    arg(isNanValue(m_columns[0]->statistics().skewness)).
                    arg(isNanValue(m_columns[0]->statistics().kurtosis)).
                    arg(isNanValue(m_columns[0]->statistics().entropy)));
        }

        ui.tw_statisticsTabs->addTab(textEdit, m_columns[i]->name());
    }
}

void StatisticsDialog::calculateStatisticsOnCurrentTab(int index){

    if(!m_columns[index]->statisticsAvailable()){
        m_columns[index]->calculateStatistics();
    }
    QTextEdit* textEdit = static_cast<QTextEdit*>(ui.tw_statisticsTabs->currentWidget());
    textEdit->setHtml(m_htmlText.arg(isNanValue(m_columns[index]->statistics().minimum)).
            arg(isNanValue(m_columns[index]->statistics().maximum)).
            arg(isNanValue(m_columns[index]->statistics().arithmeticMean)).
            arg(isNanValue(m_columns[index]->statistics().geometricMean)).
            arg(isNanValue(m_columns[index]->statistics().harmonicMean)).
            arg(isNanValue(m_columns[index]->statistics().contraharmonicMean)).
			arg(isNanValue(m_columns[index]->statistics().median)).
            arg(isNanValue(m_columns[index]->statistics().variance)).
            arg(isNanValue(m_columns[index]->statistics().standardDeviation)).
            arg(isNanValue(m_columns[index]->statistics().meanDeviation)).
			arg(isNanValue(m_columns[index]->statistics().meanDeviationAroundMedian)).
            arg(isNanValue(m_columns[index]->statistics().medianDeviation)).
            arg(isNanValue(m_columns[index]->statistics().skewness)).
            arg(isNanValue(m_columns[index]->statistics().kurtosis)).
            arg(isNanValue(m_columns[index]->statistics().entropy)));
}

void StatisticsDialog::keyPressEvent(QKeyEvent * event){
    switch (event->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        this->close();
        break;
    case Qt::Key_Return:
        this->close();
        break;
    default:
        KDialog::keyPressEvent(event);
        break;
    }
}
