#ifndef GRAPHICCHART_H
#define GRAPHICCHART_H

#include <QObject>
#include <QLineSeries>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>

#define FIRST_GRAPH 0

class GraphicChart : public QObject
{
    Q_OBJECT
public:
    GraphicChart(uint32_t numberGr = 0, uint32_t typeGraph = 0);

    void AddDataToGrahp(QVector<int> x, QVector<int> y, uint32_t numGraph, uint32_t typeGraph);
    void ClearGraph(QChart* chart);
    void UpdateGraph(QChart* chart, uint32_t typeGraph);


signals:

private:

    QList<QLineSeries*> ptrGraph;
    QList<QBarSeries*> ptrBarGraph;
    QBarSeries* ptrBarGraphs;
    QVector<QBarSet*> set;
    uint32_t numGraphs = 0;


};

#endif // GRAPHICCHART_H
