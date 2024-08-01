#include "graphic.h"

GraphicChart::GraphicChart(uint32_t numberGr)
{

    numGraphs = numberGr;
    ptrGraph.resize(numGraphs);

    for(int i = 0; i<numGraphs; i++){
        ptrGraph[i] = new QLineSeries(this);
    }

}

void GraphicChart::AddDataToGrahp(QVector<int> x, QVector<int> y, uint32_t numGraph)
{
    uint32_t size = 0;
    if(x.size() >= y.size()){
        size = y.size();
    }
    else{
        size = x.size();
    }

    for(int j = 0; j<size; j++){
        ptrGraph[numGraph]->append(x[j],y[j]);
    }
}


void GraphicChart::ClearGraph(QChart *chart)
{
    for(int i = 0; i<numGraphs; i++){
        ptrGraph[i]->clear();
        chart->removeSeries(ptrGraph[i]);
    }
}


void GraphicChart::UpdateGraph(QChart *chart)
{
    for(int i = 0; i<numGraphs; i++){
        chart->addSeries(ptrGraph[i]);
    }

}

