/**
	@name:     mmgk_splines
	@author:   Marek Jenda
	@revision: 1.0
	@date:     29.03.2014
	@file:     splines.cpp
	@depends:  none
**/

#include<iostream>
#include<vector>

using namespace std;

typedef unsigned long ul;

struct Coef {
    double a, b, c, d;
};

struct Point {
    double x, a;
};

Coef* splineNatural(vector<Point>& f) {
    ul n = f.size() - 1; // f.size() = n+1
    // Step 1
    double * h = new double[n];
    for(ul i = 0; i < n; i++)
        h[i] = f[i+1].x - f[i].x;

    // Step 2
    double * alpha = new double[n];
    for(ul i = 1; i < n; i++)
        alpha[i] = (3 / h[ i ]) * (f[i+1].a - f[ i ].a)
                 - (3 / h[i-1]) * (f[ i ].a - f[i-1].a);

    // Step 3
    double * l  = new double[n];
    double * mi = new double[n];
    double * z  = new double[n];
    l[ 0] = 1;
    mi[0] = 0;
    z[ 0] = 0;

    // Step 4
    for(ul i = 1; i < n; i++) {
        l[i] = 2*(f[i+1].x - f[i-1].x) - h[i-1]*mi[i-1];
        mi[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }

    //Step 5
    double * c  = new double[n];
    l[n] = 1;
    z[n] = 0;
    c[n] = 0;

    // Step 6
    double * b  = new double[n];
    double * d  = new double[n];
    for(long  j = n-1; j >= 0; j--) {
        c[j] = z[j] - mi[j]*c[j+1];
        b[j] = (f[j+1].a - f[j].a)/h[j] - h[j]*(c[j+1] + 2*c[j])/3;
        d[j] = (c[j+1] - c[j])/(3*h[j]);
    }

    // Step 7 (preparing return)
    Coef * coef = new Coef[n];
    for(ul i = 0; i < n; i++) {
        coef[i].a = f[i].a;
        coef[i].b = b[i];
        coef[i].c = c[i];
        coef[i].d = d[i];
    }

    // Step 7 (tiding)
    delete [] b;
    delete [] c;
    delete [] d;
    delete [] l;
    delete [] z;
    delete [] mi;
    delete [] alpha;
    delete [] h;

    // Step 8 (return)
    return coef;
}

Coef* splineClamped(vector<Point>& f, double fp0, double fpN) {
    ul n = f.size() - 1; // f.size() = n+1
    // Step 1
    double * h = new double[n];
    for(ul i = 0; i < n; i++)
        h[i] = f[i+1].x - f[i].x;

    // Step 2
    double * alpha = new double[n+1];
    alpha[0] = 3*(f[1].a - f[0].a)/h[0] - 3 * fp0;
    alpha[n] = 3*fpN - 3*(f[n].a - f[n-1].a)/h[n-1];

    // Step 3
    for(ul i = 1; i < n; i++)
        alpha[i] = (3 / h[ i ]) * (f[i+1].a - f[ i ].a)
                 - (3 / h[i-1]) * (f[ i ].a - f[i-1].a);

    // Step 4
    double * l  = new double[n];
    double * mi = new double[n];
    double * z  = new double[n];
    l[ 0] = 2*h[0];
    mi[0] = 0.5f;
    z[ 0] = alpha[0]/l[0];

    // Step 5
    for(ul i = 1; i < n; i++) {
        l[i] = 2*(f[i+1].x - f[i-1].x) - h[i-1]*mi[i-1];
        mi[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }

    //Step 6
    double * c  = new double[n];
    l[n] = h[n-1]*(2 - mi[n-1]);
    z[n] = (alpha[n] - h[n-1]*z[n-1])/l[n];
    c[n] = z[n];

    // Step 7
    double * b  = new double[n];
    double * d  = new double[n];
    for(long  j = n-1; j >= 0; j--) {
        c[j] = z[j] - mi[j]*c[j+1];
        b[j] = (f[j+1].a - f[j].a)/h[j] - h[j]*(c[j+1] + 2*c[j])/3;
        d[j] = (c[j+1] - c[j])/(3*h[j]);
    }

    // Step 8 (preparing return)
    Coef * coef = new Coef[n];
    for(ul i = 0; i < n; i++) {
        coef[i].a = f[i].a;
        coef[i].b = b[i];
        coef[i].c = c[i];
        coef[i].d = d[i];
    }

    // Step 9 (tiding)
    delete [] b;
    delete [] c;
    delete [] d;
    delete [] l;
    delete [] z;
    delete [] mi;
    delete [] alpha;
    delete [] h;

    // Step 10 (return)
    return coef;
}

int main()
{
    cout << "Podaj ile chcesz wprowadzic punktow." << endl;
    int n;
    cin >> n;
    cout << "\nPunkty wprowadzaj na zasadzie 'x f(x)'." << endl;
    vector<Point> points(n);
    for(int i = 0; i < n; i++)
        cin >> points[i].x >> points[i].a;

    Coef * coef = splineClamped(points, 2.0f, 1.0f);
    for(int i = 0; i < n-1; i++)
        cout << coef[i].a << " "
             << coef[i].b << " "
             << coef[i].c << " "
             << coef[i].d << endl;

    delete [] coef;



    return 0;
}

