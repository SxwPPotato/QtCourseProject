#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pb_get_data->setEnabled(false);
    ui->pb_congestion->setEnabled(false);
    ui->cb_airport_list->setEnabled(false);
    ui->DateEdit->setEnabled(false);
    ui->rb_arrival->setEnabled(false);
    ui->rb_departure->setEnabled(false);

    dataBase = new DataBase(this);

    msg = new QMessageBox(this);

    conqestion = new congestion_airport(this);

    request_arriving_aircraft;

    dataBase->AddDataBase(DB_NAME);

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);
    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);
    connect(dataBase, &DataBase::sig_CbSendDataFromDB, this, &MainWindow::CbScreenDataFromDB);


    connect(this, &MainWindow::sig_req, this, &MainWindow::full_request_arriving_aircraft);
    connect(this, &MainWindow::sig_aircode, conqestion, &congestion_airport::received_signal);
    connect(dataBase, &DataBase::sig_SendStatisticFromDB, conqestion, &congestion_airport::ScreenStatisticFromDB);

    dataBase->ConnectToDataBase();

    auto req = [&]{dataBase->RequestToDB(request_list_airports,automatic,empty,empty,empty);};

    QtConcurrent::run(req);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::full_request_arriving_aircraft(QString request, QString date, int type_req ){
    if(type_req == arrival){
        request_arriving_aircraft = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                "WHERE f.arrival_airport = '" + request + "' "
                "and scheduled_departure::date = '" + date + "'";
    }

    else if (type_req == departure ){
        request_departing_aircraft = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                    "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                    "WHERE f.departure_airport  ='" + request + "' "
                    "and scheduled_departure::date = '" + date + "'";

    }
}


void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->pb_get_data->setEnabled(true);
        ui->pb_congestion->setEnabled(true);
        ui->cb_airport_list->setEnabled(true);
        ui->DateEdit->setEnabled(true);
        ui->rb_arrival->setEnabled(true);
        ui->rb_departure->setEnabled(true);

        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");



    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
        dataBase->ConnectToDataBase();
    }

}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err,int Type_message, int type_req, int type_stat, int mount)
{

    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else{
        if(Type_message == automatic){
            dataBase->AirportList();
        }
        else if(Type_message == activated){
            dataBase->ReadAnswerFromDB(type_req,type_stat, mount);
        }
    }

}


void MainWindow::CbScreenDataFromDB(const QComboBox *cb, std::vector<QString> airportsCodes)
{
    for(int i = 104; i < cb->count();i++){

        QString str = cb->itemText(i);

        ui->cb_airport_list->addItem(str);

        airportsCode.push_back(airportsCodes[i-104]);
    }

}


void MainWindow::ScreenDataFromDB(const QTableWidget *widget)
{

            ui->tb_result->setRowCount(widget->rowCount( ));
            ui->tb_result->setColumnCount(widget->columnCount( ));

            QStringList hdrs;

            for(int i = 0; i < widget->columnCount(); ++i){
                hdrs << widget->horizontalHeaderItem(i)->text();
            }
            ui->tb_result->setHorizontalHeaderLabels(hdrs);

            for(int i = 0; i<widget->rowCount(); ++i){
                for(int j = 0; j<widget->columnCount(); ++j){
                    ui->tb_result->setItem(i,j, widget->item(i,j)->clone());
                }
            }

            ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::on_pb_get_data_clicked()
{
    int airport = ui->cb_airport_list->currentIndex();
    airportCode = airportsCode[airport];
    date = ui->DateEdit->text();

    if(ui->rb_arrival->isChecked() == true){

        emit sig_req(airportCode,date,arrival);

        auto req_flight = [&]{dataBase->RequestToDB(request_arriving_aircraft,activated,arrival,empty,empty);};

        QtConcurrent::run(req_flight);

    }

    else if(ui->rb_departure->isChecked() == true){

        emit sig_req(airportCode, date,departure);

        auto req_flight = [&]{dataBase->RequestToDB(request_departing_aircraft,activated,departure,empty,empty);};

        QtConcurrent::run(req_flight);

    }

}


void MainWindow::on_pb_congestion_clicked()
{ 
    int airport = ui->cb_airport_list->currentIndex();
    airportCode = airportsCode[airport];

    emit sig_aircode(airportCode, dataBase);
    conqestion->exec();
}


