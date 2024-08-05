#ifndef CONGESTION_AIRPORT_H
#define CONGESTION_AIRPORT_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QtConcurrent>
#include "database.h"
#include "Struct.h"
#include "graphic.h"

#define FD 1000.0

namespace Ui {
class congestion_airport;
}

class congestion_airport : public QDialog
{
    Q_OBJECT

public:
    explicit congestion_airport(QWidget *parent = nullptr);
    ~congestion_airport();
    void full_request_arriving_aircraft(QString request,int type_req);

private slots:
    void on_pb_close_clicked();

    void on_rb_year_clicked();

    void on_rb_mounth_clicked();

    void on_cb_mounth_currentIndexChanged(int index);

    void show_graph(QVector<int> x, QVector<int> y, int typeGraph);

public slots:

    void received_signal(QString aircode, DataBase* db);

    void ScreenStatisticFromDB(const QTableWidget *widget, QVector<int> y_count_flights,int type_req);

signals:

    void sig_req(QString request_str, int type_req);

    void sig_graph(QVector<int> x, QVector<int> y, int typeGraph);

private:
    Ui::congestion_airport *ui;

    DataBase* dataBase;

    QMessageBox* msg;

    QString datemin ="2016-12-31";

    QString datemax = "2017-01-31";

    QSqlQuery* simpleQuery;

    QTableWidget* tableWidget;

    QString aircode;

    QString request_year_statistic;

    QString request_day_stat;

    QVector<int> x_days,x_mounth, y_count_flights;

    GraphicChart* graphClass;
    QChart* chart;
    QChartView* chartView;
    QGridLayout *layout;

    void ViewGraph(void);

    int i = 0;

};

#endif // CONGESTION_AIRPORT_H
