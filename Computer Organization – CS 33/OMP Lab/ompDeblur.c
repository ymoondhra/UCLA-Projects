//OpenMP version.  Edit and submit only this file.
/* Enter your details below
 * Name : Yash Moondhra
 * UCLA ID: 105010814
 * Email id: ymoondhra@gmail.com
 * Input: NEW FILES
 * BEST TEST: 31.16 !! I HAVE PROOF
 */


#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int OMP_xMax;
#define xMax OMP_xMax
int OMP_yMax;
#define yMax OMP_yMax
int OMP_zMax;
#define zMax OMP_zMax

int OMP_Index(int x, int y, int z)
{
    return ((z * yMax + y) * xMax + x);
}
#define Index(x, y, z) OMP_Index(x, y, z)

double OMP_SQR(double x)
{
    return x*x;
}
#define SQR(x) OMP_SQR(x)

double* OMP_conv;
double* OMP_g;

void OMP_Initialize(int xM, int yM, int zM)
{
    xMax = xM;
    yMax = yM;
    zMax = zM;
    int tot = xMax*yMax*zMax;
    assert(OMP_conv = (double*)malloc(sizeof(double) * tot));
    assert(OMP_g = (double*)malloc(sizeof(double) * tot));
}
void OMP_Finish()
{
    free(OMP_conv);
    free(OMP_g);
}

void OMP_GaussianBlur(double *u, double Ksigma, int stepCount)
{
    double lambda = (Ksigma * Ksigma) / (double)(2 * stepCount);
    double nu = (1.0 + 2.0*lambda - sqrt(1.0 + 4.0*lambda))/(2.0*lambda);
    int x, y, z, step;
    double boundryScale = 1.0 / (1.0 - nu);
    double postScale = pow(nu / lambda, (double)(3 * stepCount));
    
    int B = 32;
    int xb, yb, zb;
    
    for(step = 0; step < stepCount; step++)
    {
        //1
        //#pragma omp parallel for num_threads(8)
        for(z = 0; z < zMax; z+=B)
        {
#pragma omp parallel for num_threads(8)
            for(y = 0; y < yMax; y++)
            {
                for(x = 1; x < xMax; x++)
                {
                    if(x==1)
                        u[Index(0, y, z)] *= boundryScale;
                    u[Index(x, y, z)] += u[Index(x - 1, y, z)] * nu;
                }
                for(x = xMax - 2; x >= 0; x--)
                {
                    if(x==xMax-2)
                        u[Index(0, y, z)] *= boundryScale;
                    u[Index(x, y, z)] += u[Index(x + 1, y, z)] * nu;
                }
            }
        }
        
        
        //(5)
#pragma omp parallel for private(y,x) num_threads(8)
        for(z = 0; z < zMax; z++)
        {
            for(y = 1; y < yMax; y++)
            {
                for(x = 0; x < xMax; x++)
                {
                    if(y==1)
                    {
                        u[Index(x, 0, z)] *= boundryScale;
                    }
                    u[Index(x, y, z)] += u[Index(x, y - 1, z)] * nu;
                }
            }
            for(y = yMax - 2; y >= 0; y--)
            {
                for(x = 0; x < xMax; x++)
                {
                    if(y == yMax - 2)
                    {
                        u[Index(x, yMax - 1, z)] *= boundryScale;
                    }
                    u[Index(x, y, z)] += u[Index(x, y + 1, z)] * nu;
                }
            }
        }
        
        //(9)
#pragma omp parallel for private(x) num_threads(8)
        for(y = 0; y < yMax; y++)
        {
            for(x = 0; x < xMax-1; x+=2)
            {
                u[Index(x, y, 0)] *= boundryScale;
                u[Index(x+1, y, 0)] *= boundryScale;
            }
        }
        
        //(10)

        for(z = 1; z < zMax; z++)
        {
            #pragma omp parallel for private(x) num_threads(8)
            for(y = 0; y < yMax; y++)
            {
                for(x = 0; x < xMax; x++)
                {
                    u[Index(x, y, z)] = u[Index(x, y, z - 1)] * nu;
                }
            }
        }
        
        //(11)
#pragma omp parallel for private(x) num_threads(8)
        for(y = 0; y < yMax; y++)
        {
            for(x = 0; x < xMax; x++)
            {
                u[Index(x, y, zMax - 1)] *= boundryScale;
            }
        }
        
        //(12)

        for(z = zMax - 2; z >= 0; z--)
        {
            #pragma omp parallel for private(x) num_threads(8)
            for(y = 0; y < yMax; y++)
            {
                for(x = 0; x < xMax; x++)
                {
                    u[Index(x,y,z)] += u[Index(x,y,z+1)] * nu;
                }
            }
        }
    }
    
    //(13)
#pragma omp parallel for private(y,x) num_threads(8)
    for(z = 0; z < zMax; z++)
    {
        for(y = 0; y < yMax; y++)
        {
            for(x = 0; x < xMax; x++)
            {
                u[Index(x, y, z)] *= postScale;
            }
        }
    }
}


void OMP_Deblur(double* u, const double* f, int maxIterations, double dt, double gamma, double sigma, double Ksigma)
{
    double epsilon = 1.0e-7;
    double sigma2 = SQR(sigma);
    int x, y, z, iteration;
    int converged = 0;
    int lastConverged = 0;
    int fullyConverged = (xMax - 1) * (yMax - 1) * (zMax - 1);
    double* conv = OMP_conv;
    double* g = OMP_g;
    
//    int B = 32;
//    int xb, yb, zb;
    
    for(iteration = 0; iteration < maxIterations && converged != fullyConverged; iteration++)
    {
#pragma omp parallel for private(y,x) num_threads(8)
        for(z = 1; z < zMax - 1; z++)
        {
            for(y = 1; y < yMax - 1; y++)
            {
                for(x = 1; x < xMax - 1; x++)
                {
                    int ind = Index(x, y, z);
                    g[ind] = 1.0 / sqrt(epsilon +
                                        SQR(u[ind] - u[ind+1]) +
                                        SQR(u[ind] - u[ind-1]) +
                                        SQR(u[ind] - u[ind+xMax]) +
                                        SQR(u[ind] - u[ind-xMax]) +
                                        SQR(u[ind] - u[ind+(xMax*yMax)]) +
                                        SQR(u[ind] - u[ind-(xMax*yMax)]));
                }
            }
        }
        
        memcpy(conv, u, sizeof(double) * xMax * yMax * zMax);
        OMP_GaussianBlur(conv, Ksigma, 3);
        
#pragma omp parallel for private(y,x) num_threads(8)
        for(z = 0; z < zMax; z++)
        {
            for(y = 0; y < yMax; y++)
            {
                for(x = 0; x < xMax; x++)
                {
                    int ind = Index(x,y,z);
                    double r = conv[ind] * f[ind] / sigma2;
                    r = (r * (2.38944 + r * (0.950037 + r))) / (4.65314 + r * (2.57541 + r * (1.48937 + r)));
                    conv[ind] -= f[ind] * r;
                }
            }
        }
        OMP_GaussianBlur(conv, Ksigma, 3);
        converged = 0;
        for(z = 1; z < zMax - 1; z++)
        {
            for(y = 1; y < yMax - 1; y++)
            {
                for(x = 1; x < xMax - 1; x++)
                {
                    int indie = Index(x, y, z);
                    double oldVal = u[indie];
                    double newVal = (u[indie] + dt * (
                                                      u[indie-1] * g[indie-1] +
                                                      u[indie+1] * g[indie+1] +
                                                      u[indie-xMax] * g[indie-xMax] +
                                                      u[indie+xMax] * g[indie+xMax] +
                                                      u[indie-(xMax*yMax)] * g[indie-(xMax*yMax)] +
                                                      u[indie+(xMax*yMax)] * g[indie+(xMax*yMax)] - gamma * conv[indie])) /
                    (1.0 + dt * (g[Index(x + 1, y, z)] + g[Index(x - 1, y, z)] + g[Index(x, y + 1, z)] + g[Index(x, y - 1, z)] + g[Index(x, y, z + 1)] + g[Index(x, y, z - 1)]));
                    if(fabs(oldVal - newVal) < epsilon)
                    {
                        converged++;
                    }
                    u[indie] = newVal;
                }
            }
        }
        if(converged > lastConverged)
        {
            printf("%d pixels have converged on iteration %d\n", converged, iteration);
            lastConverged = converged;
        }
    }
}

