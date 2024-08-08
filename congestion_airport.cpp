#include "congestion_airport.h"
#include "ui_congestion_airport.h"


congestion_airport::congestion_airport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::congestion_airport)
{
    ui->setupUi(this);
    ui->cb_mounth->setEnabled(false);
    dataBase = new DataBase(this);
    connect(this, &congestion_airport::sig_req, this, &congestion_airport::full_request_arriving_aircraft);



    chart = new QChart( );
    chart->legend()->setVisible(false);
    chartView = new QChartView(chart);

    layout = new QGridLayout;
    ui->tw_graph->setLayout(layout);
    layout->addWidget(chartView);

    chartView->show( );

    connect(this, &congestion_airport::sig_graph, this, &congestion_airport::show_graph);

}

congestion_airport::~congestion_airport()
{
    delete ui;
    delete chart;
    delete graphClass;
    delete chartView;
}

void congestion_airport::ScreenStatisticFromDB(const QTableWidget *widget, QVector<int> y_count_flight,int type_req, int mount)
{

    ui->tw_workload->setRowCount(widget->rowCount( ));
    ui->tw_workload->setColumnCount(widget->columnCount( ));
    y_count_flights.clear();

    QStringList hdrs;

    for(int i = 0; i < widget->columnCount(); ++i){
        hdrs << widget->horizontalHeaderItem(i)->text();
    }
    ui->tw_workload->setHorizontalHeaderLabels(hdrs);

    for(int i = 0; i<widget->rowCount(); ++i){
        y_count_flights.push_back( y_count_flight[i]);
        for(int j = 0; j<widget->columnCount(); ++j){
            ui->tw_workload->setItem(i,j, widget->item(i,j)->clone());
        }
    }

    ui->tw_workload->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if(type_req == 0){
        for(int i = 0; i<widget->rowCount(); ++i){
        year_req_flight.push_back(y_count_flights[i]);
        year_count_req_flight.push_back(x_mounth[i]);
        }
        emit sig_graph(x_mounth,y_count_flights,year_stat);
    }
    else if(type_req == 1){
        for(int i = 0; i<widget->rowCount(); ++i){

        switch(mount){
            case 0 : January_req_flight.push_back(y_count_flights[i]); January_count_req_flight.push_back(x_days[i]) ;break;
            case 1 : February_req_flight.push_back(y_count_flights[i]); February_count_req_flight.push_back(x_days[i]);break;
            case 2 : March_req_flight.push_back(y_count_flights[i]); March_count_req_flight.push_back(x_days[i]);break;
            case 3 : April_req_flight.push_back(y_count_flights[i]); April_count_req_flight.push_back(x_days[i]);break;
            case 4 : May_req_flight.push_back(y_count_flights[i]); May_count_req_flight.push_back(x_days[i]);break;
            case 5 : June_req_flight.push_back(y_count_flights[i]); June_count_req_flight.push_back(x_days[i]);break;
            case 6 : July_req_flight.push_back(y_count_flights[i]); July_count_req_flight.push_back(x_days[i]);break;
            case 7 : August_req_flight.push_back(y_count_flights[i]); August_count_req_flight.push_back(x_days[i]);break;
            case 8 : September_req_flight.push_back(y_count_flights[i]); September_count_req_flight.push_back(x_days[i]) ;break;
            case 9 : October_req_flight.push_back(y_count_flights[i]); October_count_req_flight.push_back(x_days[i]);break;
            case 10: November_req_flight.push_back(y_count_flights[i]); November_count_req_flight.push_back(x_days[i]);break;
            case 11: December_req_flight.push_back(y_count_flights[i]); December_count_req_flight.push_back(x_days[i]);break;

        }
        }

        emit sig_graph(x_days,y_count_flights,day_stat);
    }

}

void congestion_airport::received_signal(QString aircodes,DataBase* db){
 aircode = aircodes;
 dataBase = db;

 if(chart->series().isEmpty() == false){
     graphClass->ClearGraph(chart);
 }
 ui->rb_mounth->setChecked(true);
 ui->rb_year->setEnabled(true);
 ui->cb_mounth->setEnabled(true);
 ui->rb_mounth->setEnabled(false);
 emit sig_req(aircode,day_stat);
 auto January_req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,January);};
 for(int i = 0; i < 31;i++){
 x_days.push_back( i + 1);
 }

 graphClass = new GraphicChart(FD,day_stat);

 QtConcurrent::run(January_req_flight);

}

void congestion_airport::full_request_arriving_aircraft(QString request, int type_req ){
    if(type_req == year_stat){
        request_year_statistic = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = '"+request+"' or arrival_airport = '"+request+"' ) "
                "group by \"Month\"";
    }

    else if (type_req == day_stat ){
        request_day_stat = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                "WHERE(scheduled_departure::date > date('"+datemin+"') and scheduled_departure::date <= date('"+datemax+"')) and ( departure_airport = '"+request+"' or arrival_airport = '"+request+"') "
                "GROUP BY \"Day\"";

    }
}

void congestion_airport::on_rb_year_clicked()
{
    if(chart->series().isEmpty() == false){
        graphClass->ClearGraph(chart);
    }
    ui->rb_year->setEnabled(false);
    ui->rb_mounth->setEnabled(true);
    ui->cb_mounth->setEnabled(false);
    emit sig_req(aircode,year_stat);
    x_mounth.clear();
    for(int i = 0;i <12;i++ ){
    x_mounth.push_back(i + 1);
    }
    if(year_req_flight.empty() == true){
        auto req_flight = [&]{dataBase->RequestToDB(request_year_statistic,activated,statistic,year_stat,empty);};

        graphClass = new GraphicChart(FD,year_stat);



        QtConcurrent::run(req_flight);
     }
     else {
        graphClass = new GraphicChart(FD,year_stat);

        emit sig_graph(year_count_req_flight,year_req_flight,year_stat);
     }

}

void congestion_airport::on_rb_mounth_clicked()
{
    if(chart->series().isEmpty() == false){
        graphClass->ClearGraph(chart);
    }
    ui->rb_year->setEnabled(true);
    ui->cb_mounth->setEnabled(true);
    ui->rb_mounth->setEnabled(false);
    emit sig_req(aircode,day_stat);

    if(January_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
                auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,January);};
                for(int i = 0; i < 31;i++){
                x_days.push_back( i + 1);
                }
                graphClass = new GraphicChart(FD,day_stat);

                QtConcurrent::run(req_flight);}
            else {
         graphClass = new GraphicChart(FD,day_stat);
                emit sig_graph(January_count_req_flight,January_req_flight,day_stat);
            }

}


void congestion_airport::on_cb_mounth_currentIndexChanged(int index)
{
    int mounth = 0;
    x_days.clear();
    switch(index){
        case 0 : datemin ="2016-12-31"; datemax = "2017-01-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 0;break;
        case 1 : datemin ="2017-01-31"; datemax = "2017-02-28";for(int i = 0; i < 28;i++){x_days.push_back( i + 1);};mounth = 1;break;
        case 2 : datemin ="2017-02-28"; datemax = "2017-03-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 2;break;
        case 3 : datemin ="2017-03-31"; datemax = "2017-04-30";for(int i = 0; i < 30;i++){x_days.push_back( i + 1);};mounth = 3;break;
        case 4 : datemin ="2017-04-30"; datemax = "2017-05-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 4;break;
        case 5 : datemin ="2017-05-31"; datemax = "2017-06-30";for(int i = 0; i < 30;i++){x_days.push_back( i + 1);};mounth = 5;break;
        case 6 : datemin ="2017-06-30"; datemax = "2017-07-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 6;break;
        case 7 : datemin ="2017-07-31"; datemax = "2017-08-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 7;break;
        case 8 : datemin ="2016-08-31"; datemax = "2016-09-30";for(int i = 0; i < 30;i++){x_days.push_back( i + 1);};mounth = 8;break;
        case 9 : datemin ="2016-09-30"; datemax = "2016-10-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 9;break;
        case 10: datemin ="2016-10-31"; datemax = "2016-11-30";for(int i = 0; i < 30;i++){x_days.push_back( i + 1);};mounth = 10;break;
        case 11: datemin ="2016-11-30"; datemax = "2016-12-31";for(int i = 0; i < 31;i++){x_days.push_back( i + 1);};mounth = 11;break;

    }
    switch(mounth){
        case 0 : if(January_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(January_count_req_flight,January_req_flight,day_stat);
        };break;
        case 1 :if(February_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(February_count_req_flight,February_req_flight,day_stat);
        };break;
        case 2 :if(March_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(March_req_flight,March_count_req_flight,day_stat);
        };break;
        case 3 :if(April_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(April_count_req_flight,April_req_flight,day_stat);
        };break;
        case 4 :if(May_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(May_count_req_flight,May_req_flight,day_stat);
        };break;
        case 5 : if(June_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
                    auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

                    QtConcurrent::run(req_flight);}
        else {
                    emit sig_graph(June_count_req_flight,June_req_flight,day_stat);
        };break;
        case 6 :  if(July_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
                    auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

                    QtConcurrent::run(req_flight);}
        else {
                    emit sig_graph(July_count_req_flight,July_req_flight,day_stat);
        };break;
        case 7 :   if(August_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(August_count_req_flight,August_req_flight,day_stat);
        };break;
        case 8 :if(September_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(September_count_req_flight,September_req_flight,day_stat);
        };break;
        case 9 :if(October_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(October_count_req_flight,October_req_flight,day_stat);
        };break;
        case 10 :if(November_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(November_count_req_flight,November_req_flight,day_stat);
        };break;
        case 11 :if(December_req_flight.empty() == true){    emit sig_req(aircode,day_stat);
            auto req_flight = [&]{dataBase->RequestToDB(request_day_stat,activated,statistic,day_stat,mounth);};

            QtConcurrent::run(req_flight);}
        else {
            emit sig_graph(December_count_req_flight,December_req_flight,day_stat);
        };break;
    }

}



void congestion_airport::show_graph(QVector<int> x, QVector<int> y, int typeGraph){
    if(typeGraph == 0){
    if(chart->series().isEmpty() == false){
        graphClass->ClearGraph(chart);
    }

       graphClass->AddDataToGrahp(x,y, i,typeGraph);

       i++;


       graphClass->UpdateGraph(chart,typeGraph);
       ViewGraph();
    }

    else if(typeGraph == 1){
        if(chart->series().isEmpty() == false){
               graphClass->ClearGraph(chart);
           }

           graphClass->AddDataToGrahp(x,y, i,typeGraph);

           i++;

           graphClass->UpdateGraph(chart,typeGraph);
           ViewGraph();

    }

}

void congestion_airport::ViewGraph()
{
    chartView->chart()->createDefaultAxes();
    chartView->show( );
}

void congestion_airport::on_pb_close_clicked()
{
    close();
}


