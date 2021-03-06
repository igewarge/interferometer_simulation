// Misc.h - contains functions of other miscellaneous tasks, such as taking care of boundary conditions, checking the max value in an array, etc.
//also contains structures.

#ifndef MISC_H
#define MISC_H

#include "PhaseShifts.h"
#include "Gratings.h"
#include "BeamParams.h"

// global sp structure, contains all of the simulation parameters that can be modified. 
typedef struct {
int accountGrav;								// account for gravitational forces [1] yes, [0] no.
int elecOrAtom;                                   // electron modeled[1] or atom modeled[2]
double vel;                                    //velocity of particle
double energy;// = ((1.5 * pow(10,-18))/(pow(0.00000000001,2))) * (1); //energy of electron
int simchoice;								// choose whether or not to have a full simulation, or a final simulation interferance pattern.  
int logchoice;								// whether or not to scale interferance pattern with a log base in order to see smaller intensities
int useimagecharge;// = 0;                     // whether or not to consider image charge effects. 0 for False. //not used in program.
double eta1;// = .4;                           //G1 open fraction; how open the first grating is. With .4 open, a little over than half the muonium should pass through it.
double eta2;// = .4;                           //G2 open fraction; how open the second grating is.
double g_period;//                  // period of grating - 100 nanometers.
double r0;// = -4.04;                          //initial radius of wavefront curvature; comes from initial beam
double el0;//= 0.000001;                      // initial coherence width; 50e-9 can also be used. Depends on initial beam
double w0;// = 0.00003;                        // initial beam width -- this is probably assumed, the muonium beam width. Can also be: 2e-6, 1e-6, depends on initial beam
double G1_z;// = 0.000001;                     // It being 1 micron high is arbitrary, pretty sure. Also same as thickness of gratings.
double G2_z;// = 1;                            // assumed to be 1 meter away on z-axis.
double G2_x;// = 0.00000005;                   //50 nm. Initial lateral offset of G2.
double theta;// = 00.000001;                           // could be 0.05 or more. This is the twist between 1st and second gratings, in degrees. 2nd and 3rd grating are fixed to same rotational twist.
double thick;// = 0.000000014;                 // 14 nanometers. Not (real) thickness of gratings, most likely. Gratings are 1 micrometer thick.
double Gthick;// = 1000;                       // thickness of gratings; 1 micrometer = 1000 nm, this is in nm on purpose (see function ReTgenerator)
double wedgeangle;// = 0;                      // Grating wedge angle. The variable alpha below depends on this. This is a free parameter. Appears to be related to beam splitting.
double tilt;// =0;                             // A free parameter. Beta variable below depends on this. If beam is perp. to grating, then tilt (and thus Beta) are 0. This is the twist about the x-axis.
double res;// = 1000;                          // Resolution of the interaction in the gratings. Better varname?
double zstart;// = -0.1;                       // defining bounds of the grating structure? This definitely corresponds to a location, probably the bottom of the grating.
double zend;//= 2.1;                          // probably the end of the grating.
double xstart;// = -0.00020;                   // x position start, -200 microns
double xend;//= 0.00020;						// x position ends, 200 microns
double ystart;// = -0.00011;                   // y position start, 110 microns
double yend;// = 0.00011;					// y position ends, 110 microns
double height;								//maximum z height
double cutoff;								// =.000001  minimum value compared intensity to in order to truncate or neglect certain values below cutoff value
	                                         // at what point does the intensity cut off and be treated as 0. Can also be 5e-5 like in McMorran thesis. Or 0.001.
}simparam;

extern simparam sp;

double maximumvalue(double arr[], int rows); 
// obtains the max value in an array

double sinc(double x); 
// returns sin(x)/x

double ( *ixgenerator(double a[], double zloc, int logchoice, int rows)); 
// accounts for decay (if applicable) and normalizing the intensity scale

int x2pnts(int value, int *arr); 
// checks for a certain x-position in the array

#endif
