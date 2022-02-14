#include "solver.h"

Solver::Solver(QVector<QVector<double>> points) : m_points(points),
    N(m_points.count())
{
    for(int i = 0; i<N; i++){
        x.append(m_points[i][0]);
        y.append(m_points[i][1]);
    }

}

QVector<QVector<double>> Solver::getLine(QString type, QString method)
{
    if(type == Ltypes[APPROXIMATION]){
        if(method == Lmethod_app[LINEAR]) return approximateLinear();
        if(method == Lmethod_app[POLINOM]) return approximatePolinom();
    }
    else{
        if(method == Lmethod_inter[NYTOM_POLINOM]) return interpolateNyPolinom();
        if(method == Lmethod_inter[LANGR_POLINOM]) return pointerPolinom(QVector<double>(0),0.1,2,LANGR_POLINOM);
        if(method == Lmethod_inter[LINEAT_SPLINE]) return QVector<QVector<double>>{getX(), getY()};
        //if(method == Lmethod_inter[CUB_SPLINE]) return pointerPolinom(QVector<double>(0),0.1,2,CUB_SPLINE);
    }

    return QVector<QVector<double>>(0);
}

/*========Реализация методов аппраксимации и интерполяции======= */

QVector<QVector<double>> Solver::approximateLinear(){
    double xi = getSum(&x,N);
    double xi2 = getSum(&x,N,2);
    double yi = getSum(&y, N);
    double xiyi = getSumMult(&x,&y,N);

    double k = (N*xiyi - yi*xi) / (N*xi2 - pow(xi,2));
    double b = (yi/N)-(k)*(xi/N);

    qDebug() <<(QVector<double>){b,k};
    return pointerPolinom((QVector<double>){b,k});
}

QVector<QVector<double>> Solver::approximatePolinom(int power){

    QVector<QVector<double>> a;
    QVector<double> b;

    for(int i=0; i<power;i++)
    {
        a.append((QVector<double>){});
        for(int j=0; j<power;j++)
            a[i].append(getSum(&x,N,j+i));
    }

    for(int i=0; i<power; i++)
        b.append(getSumMult(&x,&y,N,i,1));

    QVector<double> coef = gauss(a,b,power);

    return pointerPolinom(coef);
}


QVector<QVector<double>> Solver::interpolateNyPolinom()
{
    QVector<double> coef(N);

    for(int i = 0; i < N; i++)
        coef[i] = y[i];

    for(int i=0;i<N;i++){
        for(int j = i+1; j < N; j++)
            coef[j] = (coef[i] - coef[j]) / (x[i] - x[j]);
    }

    qDebug()<<"COEF"<<coef;
    return pointerPolinom(coef,0.1,2,true);

}

double Solver::lagrangePolinom(double t)
{

    double sum = 0;
    for (int i = 0; i < N; ++i){

        double l = 1;
        for (int j = 0; j < N; ++j)
            if (j != i)
                l *= (t - x[j]) / (x[i] - x[j]);
        sum += y[i] * l;
    }

    return sum;
}

double Solver::polinom(double t, QVector<double>* coef)
{
    double f = 0;
    double part = 0;
    for(int i = 0; i<N; i++)
    {
        part = coef->at(i);
        for(int j = 0; j<i; j++) part *= (t-x[j]);
        f +=part;
    }
    return f;
}

double Solver::cubSpline(double t)
{
        return 0;
}

QVector<QVector<double>> Solver::pointerPolinom(QVector<double> coef, double step, double margin,
                                                int role)
{
    double xMin = getXmin();
    double xMax = getXmax();

    qDebug()<< xMin;
    qDebug()<< xMax;

    QVector<QVector<double>> temp;
    QVector<double> tx;
    QVector<double> ty;
    double y = 0;
    qDebug() << role << CUB_SPLINE;
    for(double i = xMin-margin; i < xMax+margin; i+=step){
        if(role == NYTOM_POLINOM){
            tx.append(i);
            ty.append(polinom(i,&coef));
        }
        else if(role == LANGR_POLINOM){
            tx.append(i);
            ty.append(lagrangePolinom(i));
        }
        else if(role == CUB_SPLINE){
            if(i > xMin && i < xMax){
                tx.append(i);
                ty.append(cubSpline(i));
            }
        }
        else{
            tx.append(i);
            for(int j = 0; j < coef.size(); j++)
                y += coef[j]*pow(i,j);
            ty.append(y);
            y = 0;
        }
    }
    temp.append(tx);
    temp.append(ty);

    qDebug() <<"TEMP"<<temp;
    return temp;

}


QVector<double> Solver::getLinRegCoef()
{
    double xi = getSum(&x,N);
    double xi2 = getSum(&x,N,2);
    double yi = getSum(&y, N);
    double xiyi = getSumMult(&x,&y,N);

    double k = (N*xiyi - yi*xi) / (N*xi2 - pow(xi,2));
    double b = (yi/N)-(k)*(xi/N);

    return (QVector<double>){b,k};
}


double Solver::getSum(QVector<double> *a, int N, int p, double add)
{
    if(p==0) return N;
    double res = 0;
    for(int i = 0; i < N; i++)
        res += pow(a->at(i)+add,p);
    return res;
}
double Solver::getSumMult(QVector<double> *a, QVector<double> *b, int N, int p1,
                         int p2, double adda, double addb)
{
    double res = 0;
    for(int i = 0; i < N; i++)
    res += pow((a->at(i)+adda),p1) * pow((b->at(i)+addb),p2);
    return res;
}

QVector<double> Solver::gauss(QVector<QVector<double>> a, QVector<double> b, int n)
{
double max;
QVector<double> x = QVector<double>(n);
QVector<double> y = QVector<double>(n);
int k, index;
const double eps = 0.00001; // точность

k = 0;
    while (k < n)
    {
        max = abs(a[k][k]);
        index = k;
        for (int i = k + 1; i < n; i++)
        {
            if (abs(a[i][k]) > max)
            {
                max = abs(a[i][k]);
                index = i;
            }
        }
        if (max < eps)
        {
            return QVector<double>{};
        }
        for (int j = 0; j < n; j++)
        {
            double temp = a[k][j];
            a[k][j] = a[index][j];
            a[index][j] = temp;
        }
        double temp = b[k];
        b[k] = b[index];
        b[index] = temp;
        for (int i = k; i < n; i++)
        {
            double temp = a[i][k];
            if (abs(temp) < eps) continue;
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] / temp;
            b[i] = b[i] / temp;
            if (i == k) continue;
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] - a[k][j];
            b[i] = b[i] - b[k];
        }
        k++;
     }

    qDebug()<< a;
    qDebug()<< b;

    for(int i = 0; i < n; i++)
        y[i] = b[i];

    for (k = n - 1; k >= 0; k--)
    {
        x[k] = y[k];
        for (int i = 0; i < k; i++)
            y[i] = y[i] - a[i][k] * x[k];
    }
    return x;
}

double Solver::getXmin(){
    double xMin = x[0];
    foreach (double a, x) xMin = qMin(xMin, a);
    return xMin;
}

double Solver::getXmax(){
    double xMax = x[0];
    foreach (double a, x) xMax = qMax(xMax, a);
    return xMax;
}

double Solver::getYmax(){
    double yMax = y[0];
    foreach (double a, y) yMax = qMax(yMax, a);
    return yMax;
}

double Solver::getYmin(){
    double yMin = y[0];
    foreach (double a, x) yMin = qMin(yMin, a);
    return yMin;
}

double Solver::getCorr(){
    int sum_X = 0, sum_Y = 0, sum_XY = 0;
    int squareSum_X = 0, squareSum_Y = 0;
    for (int i = 0; i < N; i++)
    {
        sum_X = sum_X + x[i];
        sum_Y = sum_Y + y[i];

        sum_XY = sum_XY + x[i] *y[i];
        squareSum_X = squareSum_X + x[i] * x[i];
        squareSum_Y = squareSum_Y + y[i] * y[i];

    }
    double corr = (double)(N * sum_XY - sum_X * sum_Y)
            / sqrt((N * squareSum_X - sum_X * sum_X)
                      * (N * squareSum_Y - sum_Y * sum_Y));
    return corr;
}

double Solver::getMSD(){
    QVector<double> linRegCoef = getLinRegCoef();
    double b = linRegCoef[0];
    double k = linRegCoef[1];

    float sq_err = 0;
    for(int i=0; i<N; i++)
        sq_err += pow(y[i] - b - k*x[i],2);
    return pow(sq_err, 0.5);
}

