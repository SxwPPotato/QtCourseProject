#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();

    simpleQuery = new QSqlQuery();

    tableWidget = new QTableWidget();

    comboBox = new QComboBox();

}

DataBase::~DataBase()
{
    delete dataBase;
}


void DataBase::AddDataBase( QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(nameDB);
}

bool DataBase::ConnectToDataBase()
{
    dataBase->setHostName("981757-ca08998.tmweb.ru");
    dataBase->setDatabaseName("demo");
    dataBase->setUserName("netology_usr_cpp");
    dataBase->setPassword("CppNeto3");
    dataBase->setPort(5432);

    bool status = dataBase->open();
    emit sig_SendStatusConnection(status);
    return dataBase->open();

}

void DataBase::RequestToDB(QString request,int Type_message, int type_req, int type_stat)
{

    *simpleQuery = QSqlQuery(*dataBase);

    QSqlError err;
    if(simpleQuery->exec(request) == false){
        err = simpleQuery->lastError();
    }


    emit sig_SendStatusRequest(err,Type_message,type_req,type_stat);


}

void DataBase::AirportList()
{

    uint32_t conterRows = 0;
    while(simpleQuery->next()){
        QString str;
        comboBox->insertSeparator(conterRows);
        str = simpleQuery->value(0).toString();


        QString strCode = simpleQuery->value(1).toString();
        airportsCode.push_back(strCode);

        comboBox->addItem(str);
        conterRows++;
    }

    emit sig_CbSendDataFromDB(comboBox,airportsCode);

}



void DataBase::ReadAnswerFromDB(int type_req,int type_stat)
{

    tableWidget->setRowCount(0);
    QStringList hdrs;

    QVector<int> y_count_flights;
    y_count_flights.clear();

    if(type_req == 0){
         tableWidget->setColumnCount(3);
        hdrs << "Номер рейса" << "Время вылета" << "Аэропорт отправления";
    }
    else if(type_req == 1){
         tableWidget->setColumnCount(3);
        hdrs << "Номер рейса" << "Время вылета" << "Аэропорт назначения";
    }
    else if(type_req == 2){
        tableWidget->setColumnCount(2);
        hdrs << "Количество рейсов" << "Месяц";     

    }
    tableWidget->setHorizontalHeaderLabels(hdrs);

    uint32_t conterRows = 0;

    while(simpleQuery->next()){
        QString str;
        tableWidget->insertRow(conterRows);
        y_count_flights.push_back(simpleQuery->value(0).toInt());

        for(int i = 0; i<tableWidget->columnCount(); ++i){

            str = simpleQuery->value(i).toString();


                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

        }
        ++conterRows;
    }
    if(type_req == 0 || type_req == 1){
        emit sig_SendDataFromDB(tableWidget);
    }
    else if (type_req == 2){
        emit sig_SendStatisticFromDB(tableWidget,y_count_flights,type_stat);
    }


}



void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}

QSqlError DataBase::GetLastError()
{
   return dataBase->lastError();
}
