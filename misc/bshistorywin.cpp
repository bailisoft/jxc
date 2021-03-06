#include "bshistorywin.h"
#include "main/bailigrid.h"
#include "main/bailicode.h"
#include "main/bailidata.h"
#include "main/bailiwins.h"
#include "main/bsmain.h"

namespace BailiSoft {

BsHistoryWin::BsHistoryWin(QMainWindow *parent, BsQryWin *qryWin, const QStringList &labelPairs, const QString &sql,
                           const QStringList &colTitles, const QString &sizerType, const bool hasQcValue)
    : QWidget(parent)
{
    mppQryWin = qryWin;
    mLabelPairs << labelPairs;

    mpCons = new QLabel(this);
    mpCons->setWordWrap(true);
    mpCons->setText(qryWin->pairTextToHtml(labelPairs, hasQcValue));

    mpGrid = new BsQueryGrid(this);
    mpGrid->loadData(sql, colTitles, sizerType);
    mpGrid->loadColWidths("his");

    QPushButton *btnPrint  = new QPushButton(QIcon(":/icon/print.png"), mapMsg.value("btn_print").split(QChar(9)).at(0), this);
    QPushButton *btnExport = new QPushButton(QIcon(":/icon/export.png"), mapMsg.value("btn_export").split(QChar(9)).at(0), this);
    QWidget *bar = new QWidget(this);
    QHBoxLayout *layb = new QHBoxLayout(bar);
    layb->setContentsMargins(0, 0, 0, 0);
    layb->addStretch();
    layb->addWidget(btnPrint);
    layb->addWidget(btnExport);

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(3, 3, 3, 3);
    lay->addWidget(mpCons);
    lay->addWidget(mpGrid, 1);
    lay->addWidget(bar);

    setWindowTitle(mapMsg.value("word_history"));

    connect(btnPrint, SIGNAL(clicked(bool)), this, SLOT(doPrint()));
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(doExport()));

    BsMain *mainWin = qobject_cast<BsMain*>(parent);
    if ( mainWin ) {
        connect(mpGrid, SIGNAL(requestOpenSheet(QString,int)), mainWin, SLOT(responseOpenSheet(QString,int)));
    }
}

void BsHistoryWin::closeEvent(QCloseEvent *e)
{
    mpGrid->saveColWidths("his");
    QWidget::closeEvent(e);
}

void BsHistoryWin::doPrint()
{
    mpGrid->doPrint(mapMsg.value("word_history"), mLabelPairs, loginer,
                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
}

void BsHistoryWin::doExport()
{
    mppQryWin->exportGrid(mpGrid, mLabelPairs);
}

}
