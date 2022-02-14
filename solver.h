#ifndef SOLVER_H
#define SOLVER_H

#include<QVector>
#include<QDebug>
#include <cmath>

#include "namespace.h"

using namespace base;

class Solver
{
public:
    Solver(QVector<QVector<double>> points);
    QVector<QVector<double>> getLine(QString type, QString method);
    QVector<double> getX(){return x;}
    QVector<double> getY(){return y;}
    double getXmin();
    double getXmax();
    double getYmax();
    double getYmin();
    double getCorr();
    double getMSD();

private:
    QVector<QVector<double>> m_points;
    QVector<double> x;
    QVector<double> y;
    int N;
    QVector<QVector<double>> approximateLinear();
    QVector<QVector<double>> approximatePolinom(int power = 4);
    QVector<QVector<double>> interpolateNyPolinom();

    double polinom(double t, QVector<double>* coef);
    double lagrangePolinom(double t);
    double cubSpline(double t);

    QVector<QVector<double>> pointerPolinom(QVector<double> coef, double step = 0.1, double margin = 2,
                                            int role = -1);
    double getSum(QVector<double> *a, int N, int p = 1, double add = 0);
    double getSumMult(QVector<double> *a, QVector<double> *b, int N, int p1 = 1,
                     int p2 = 1, double adda = 0, double addb = 0);
    QVector<double> gauss(QVector<QVector<double>> a, QVector<double> b, int n);
    QVector<double> getLinRegCoef();
};

#endif // SOLVER_H
