// BeamParams.c - contains the functions defining the GSM (Gaussian-Schell Model) beam behavior

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string> 

#include "BeamParams.h"
#include "PhaseShifts.h"
#include "Misc.h"
#include "Gratings.h"

double zp(double z, double v) 
{
// compute magnification factor due to wavefront curvature
    double zp;
    zp = (v * z)/(z + v); 
    // v = GSM radius of wavefront curvature, z = current z location; currently, zp is only called with v = r0 (defined as a global variable) above. 
    return(zp);
}

//double w(double z,double r0, double el0, double w0, double energy) 
double w(double z,double r0, double el0, double w0)
{
    // Compute GSM beam width (GSM = Gaussian-Schell Model of gratings)
    double lambda = sqrt((1.5 * pow(10,-18))/(sp.energy)); 
    // variable containing value of the wavelength; wavelength of what?
    double w;
    w = (el0) * (fabs((z)/(zp(z,r0))) * ((sqrt((1 + (pow(((lambda * zp(z,r0))/(el0 * w0)),2))))))); 
    // what does this correspond to?
    return(w);
}

//double el(double z,double r0, double el0, double w0, double energy) 
double el(double z,double r0, double el0, double w0) 
{
    // GSM beam coherence width
    double lambda = sqrt((1.5 * pow(10,-18))/(sp.energy)); 
    // again, really small value; should this be global? value containing the wavelength; wavelength of what?
    double w;
    w = (el0) * (fabs((z)/(zp(z,r0))) * ((sqrt((1 + (pow(((lambda * zp(z,r0))/(el0 * w0)),2))))))); 
    return(w);
}

//double v(double z,double r0, double el0, double w0, double energy) 
double v(double z,double r0, double el0, double w0) 

{
    // compute GSM radius of wavefront curvature
    double lambda = sqrt((1.5 * pow(10,-18))/(sp.energy)); 
    // value containing wavelength; this is approx. wavelength of xrays.
    double v;
    v=(z)/(1-zp(z,r0)/(z * (1 + pow(((lambda * zp(z,r0)/(el0 * w0))),2)))); 
    return(v);
}
