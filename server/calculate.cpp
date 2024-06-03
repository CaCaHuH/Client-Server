#include "calculate.h"
#include <math.h>
#include <QDebug>
calculate::calculate()
{

}

result calculate::Run(float d,float dt,float v0,float y0,float cx0,float A,float cya,
         float teta,float m0,float mc,float tet,float tr,float aero)
{

    float t0=0,x0=0,ay1,ay2,ay3,by1,by2,by3,by11,by22,by33,cy1,cy2,cy3,cy11,cy22,cy33,
            dy1,dy2,dy3,dy11,dy22,dy33,m,y111,y222,y333,s,y1,y2,y3,y4,ny,nx,q,alfa,R;
    s = ((M_PI)*pow(d,2))/4;
    R = 2000*mc;

    do{

        alfa=0/57.3;

        if(t0<tr)
            m = m0 - mc*t0;
        else
        {
            m = m0 - mc*tr;
            R = 0;
        }

        if(aero == 1)
        {
            //CX/////////////////
            if((v0<=1.5*314)&&(v0>=0))
                cx0 = 0.9633*v0/314-0.08691*pow(v0/314,2)-0.07997*pow(v0/314,3)+0.00244*pow(v0/314,5);
            if((v0>1.5*314)&&(v0<3.8*314))
                cx0 = 1.60411*v0/314-0.78844*pow(v0/314,2)+0.109648*pow(v0/314,3)-0.00055924*pow(v0/314,5);
            if(v0>=3.8*314)
                cx0 = -0.00945*v0/314+0.31527;
            //////////////////////

            //CYA////////////////
            if((v0<3*314)&&(v0>=0))
                cya = 0.067957*v0/314-0.015033*pow(v0/314,2)-0.0017884*pow(v0/314,3)+0.000095*pow(v0/314,5);
            if((v0>=3*314)&&(v0<5*314))
                cya = -0.00295*v0/314+0.05225;
            if(v0>=5*314)
                cya = -0.00084*v0/314+0.04172;
            /////////////////////
            cya = cya*57.3;
            /////////////////////
        }

        ay1=-(0.5*(cx0+A*pow(alfa,2))*1.225*exp((-y0)/10000)*s*pow(v0,2))/m-9.816*sin(teta)+R*cos(alfa)/m;
        ay2=(-9.816*cos(teta))/v0+0.5*(cya*alfa*1.225*exp(-y0/10000)*pow(v0,2)*s)/(m*v0)+R*sin(alfa)/(m*v0);
        ay3=v0*sin(teta);

        by11=v0+ay1*(dt/2);
        by22=teta+ay2*(dt/2);
        by33=y0+ay3*(dt/2);

        by1=-(0.5*(cx0+A*pow(alfa,2))*1.225*exp((-by33)/10000)*s*pow(by11,2)/m)-9.816*sin(by22)+R*cos(alfa)/m;
        by2=(-9.816*cos(by22))/by11+0.5*(cya*alfa*1.225*exp(-by33/10000)*pow(by11,2)*s)/(m*by11)+R*sin(alfa)/(m*by11);
        by3=by11*sin(by22);

        cy11=v0+by1*(dt/2);
        cy22=teta+by2*(dt/2);
        cy33=y0+by3*(dt/2);

        cy1=-((0.5*(cx0+A*pow(alfa,2))*1.225*exp((-cy33)/10000)*s*pow(cy11,2))/m)-9.816*sin(cy22)+R*cos(alfa)/m;
        cy2=(-9.816*cos(cy22))/cy11+0.5*(cya*alfa*1.225*exp(-cy33/10000)*pow(cy11,2)*s)/(m*cy11)+R*sin(alfa)/(m*cy11);
        cy3=cy11*sin(cy22);

        dy11=v0+cy1*(dt/2);
        dy22=teta+cy2*(dt/2);
        dy33=y0+cy3*(dt/2);

        dy1=-(0.5*(cx0+A*pow(alfa,2))*1.225*exp((-dy33)/10000)*s*pow(dy11,2))/m-9.816*sin(dy22)+R*cos(alfa)/m;
        dy2=(-9.816*cos(dy22))/dy11+0.5*(cya*alfa*1.225*exp(-dy33/10000)*pow(dy11,2)*s)/(m*dy11)+R*sin(alfa)/(m*dy11);
        dy3=dy11*sin(dy22);

        y111=(dt/6)*(ay1+2*(by1+cy1)+dy1);
        y222=(dt/6)*(ay2+2*(by2+cy2)+dy2);
        y333=(dt/6)*(ay3+2*(by3+cy3)+dy3);

        y1=v0+y111;
        y2=teta+y222;
        y3=y0+y333;
        y4=x0+(y333/tan(y2));

        q=(1.225*exp((-y3)/10000)*pow(y1,2))/2;
        ny=(cya*alfa*q*s+R*alfa)/(m*9.816);
        nx=((cx0+A*pow(alfa,2))*q*s+R)/(m*9.816);

        t0=t0+dt;
        v0=y1;
        y0=y3;
        x0=y4;
        teta=y2;

    }
    while(t0<tet);

    result res;
    res.tetta = teta;
    res.v = v0;
    res.y = y0;
    res.x = x0;
    return res;
}
