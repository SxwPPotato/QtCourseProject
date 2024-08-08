#ifndef DATABASE_H
#define DATABASE_H


#include "qcombobox.h"
#include <QTableWidget>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


#define DB_NAME "QPSQL"

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString nameDB);
    bool ConnectToDataBase();
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request, int Type_message,int type_req, int type_stat, int mount);
    QSqlError GetLastError();

    void ReadAnswerFromDB(int type_req,int type_stat, int mount);

    void AirportList();


signals:

    void sig_SendDataFromDB(const QTableWidget *tableWg);
    void sig_SendStatusConnection(bool);
    void sig_SendStatusRequest(QSqlError err,int Type_message, int type_req, int type_stat, int mount);
    void sig_CbSendDataFromDB(const QComboBox *cmBox, std::vector<QString> airportsCode);
    void sig_SendStatisticFromDB(const QTableWidget *tableWidget, QVector<int> y_count_flights,int type_req, int mount);


private:

    QSqlDatabase* dataBase;

    QSqlQuery* simpleQuery;

    QTableWidget* tableWidget;

    QComboBox* comboBox;

    std::vector<QString> airportsCode;

    std::vector<QString> scheduledArrival;


};

#endif // DATABASE_H
