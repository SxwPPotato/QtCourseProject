#include "graphic.h"

GraphicChart::GraphicChart(uint32_t numberGr, uint32_t typeGraph)
{

    numGraphs = numberGr;


    if(typeGraph == 1){
         ptrGraph.resize(numGraphs);
    for(int i = 0; i<numGraphs; i++){
        ptrGraph[i] = new QLineSeries(this);
    }
    }
    else if(typeGraph == 0){
        for(int i = 0; i < numGraphs; i++){
           ptrBarGraphs = new QBarSeries(this);
        }
    }

}

void GraphicChart::AddDataToGrahp(QVector<int> x, QVector<int> y, uint32_t numGraph, uint32_t typeGraph)
{
    uint32_t size = 0;
    if(x.size() >= y.size()){
        size = y.size();
    }
    else{
        size = x.size();
    }

    for(int j = 0; j<size; j++){
        if(typeGraph == 1){

        ptrGraph[numGraph]->append(x[j],y[j]);
        }

        else if(typeGraph == 0){
            set.push_back(new QBarSet(QString::number(x[j])));
            *set[j] << y[j];

           ptrBarGraphs->append(set[j]);

        }
    }
}


void GraphicChart::ClearGraph(QChart *chart)
{
    for(int i = 0; i<numGraphs; i++){

        if(ptrGraph.isEmpty() == false){
        ptrGraph[i]->clear();
        chart->removeSeries(ptrGraph[i]);
        }
        else if(set.isEmpty() == false){
            set.clear();
            ptrBarGraphs->clear();
            chart->removeSeries(ptrBarGraphs);
        }
    }
}


void GraphicChart::UpdateGraph(QChart *chart, uint32_t typeGraph)
{

    if(typeGraph == 1){
        for(int i = 0; i<numGraphs; i++){
            chart->addSeries(ptrGraph[i]);
        }
    }

    else if(typeGraph == 0){
            chart->addSeries(ptrBarGraphs);
    }

}

