#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"
#include "congestion_airport.h"
#include <QSqlTableModel>
#include <QtConcurrent>
#include "Struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void full_request_arriving_aircraft(QString request, QString date, int type_req);

public slots:
    void ReceiveStatusConnectionToDB(bool status);
    void ReceiveStatusRequestToDB(QSqlError err, int Type_message, int type_req, int type_stat, int mount);
    void ScreenDataFromDB(const QTableWidget *widget);

    void CbScreenDataFromDB(const QComboBox * cb,std::vector<QString> airpoetsCode);

private slots:

    void on_pb_get_data_clicked();

    void on_pb_congestion_clicked();

signals:
    void sig_req(QString request_str, QString date, int type_req);
    void sig_aircode( QString airportCode,DataBase *db);

private:
    Ui::MainWindow *ui;
    DataBase* dataBase;
    QMessageBox* msg;
    congestion_airport* conqestion;   

    QString airportCode = "";
    QString date;
    std::vector<QString> airportsCode;
    std::vector<QString> scheduledArrival;


    QString request_list_airports = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data";


    /*QString request_list_airports_test = "SELECT airport_name->>'ru' as \"airportName\", airport_code, f.scheduled_arrival, f.scheduled_departure FROM bookings.airports_data ad "
            "JOIN bookings.flights f on f.departure_airport = ad.airport_code";*/

    /*QString request_test = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                           "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                           "WHERE f.arrival_airport = '" + airportCode + "'";*/

    //Запрос прибывающих самолетов
    QString request_arriving_aircraft;

    //Запрос вылетающих самолетов
    QString request_departing_aircraft;

    //Запрос общей статистики за год
    QString request_year_statistic = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
            "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = airportCode or arrival_airport = airportCode ) "
            "group by \"Month\"";

    //Запрос статистики по дням за год
    QString request_day_stat = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
            "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = airportCode or arrival_airport = airportCode) "
            "GROUP BY \"Day\"";

};
#endif // MAINWINDOW_H
