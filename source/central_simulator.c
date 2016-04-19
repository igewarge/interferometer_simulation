/*
Current set of tasks.
- find new term for wave particle
- find unused variables
- flow chart of what everything does for drive
- create struct for waveparticle , charged particle
- insert main cmd line args for different predefined structs and user inputs.
- remove error testing and user input frames to replace with cmd line arguments.
*/

//***
//* Central Simulator
//* 
//*     Adam, put concise description here.
/* Icomment: This central program takes user input for:
			 electronic or atomic beam type,
			 inclusion or exclusion of gravitational acceleration,
			 resolution of output graph,
			 velocity of particles, 
			 pitch of gratings,
			 and intensity or final simulation paths,
			 and calculates simulation parameters in order to perform the simulation 
			 with the users desired arguments. 
*/
//*   
//* Code inspired by thesis by Dr. Benjamin McMorran
//* Put title of thesis and location here
//*
//* Collaborators:
//*     Arthur Romero, 16 July to 24 August 2015
//*         -- Original author
//*     Adam Denchfield, 20 October 2015 to present
//*         -- Additional parameters and functionality
//*     Melanie Cornelius (Dooley), 1 Nov 2015 to present
//*         -- Optimizations, standards, and readability edits
//*		Isaac Gewarges, January 2016 to present
//*			-- Optimizations, standards, and readability edits
//* 
//***

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <cfloat>
#include <ctype.h>
#include <complex.h>
#include <string> 
#include <complex.h>
#include <limits.h> 		//fix for islimit error


#include "TCanvas.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"

#include "SimplePlot.h"          //Icomment: This program uses Root's TApplication.h and creates
						    	//  		a plot based on values passed into the function. 
#include "Misc.h"                //          contains functions of other miscellaneous tasks, 
							    //          such as taking care of boundary conditions, 
							    //          checking the max value in an array, etc.
#include "BeamParams.h"          //          This program contains the functions defining the GSM beam behavior
#include "Gratings.h"
#include "PhaseShifts.h"

simparam sp;
/*
None of these variables are currently used.

//double mu_debroglie = 0.0000000005584;      // the result of the above equation  NOT USED IN MAIN PROGRAM
//double muonium_freq = 536890000000000000;   // muonium frequency
//double mu_lifetime = 0.0000022;             // half life average decay time of a muon, in seconds
//float C3 = 0.020453;                        // the VdW coefficient for hydrogen (assumed to be the same for muonium)


double Coulomb = 0.00000000898755179;       // force; m^2/(Coulomb^-2)
double pi = 3.14159265358979;               // the constant irrational number pi.
double const_e = 2.71828182845905;          // the irrational constant e.

double cutoff = 0.000001;                   // at what point does the intensity cut off and be treated as 0. Can also be 5e-5 like in McMorran thesis. Or 0.001.
*/

/* 
   from McMorran's thesis, a value epsilon = ratio of permittivity of grating material to permittivity of free space
   and the image charge is  + e  *  (epsilon - 1) / (epsilon  +  1)
   for an ideal conductor image charge =  + e (electric charge), the energy due to this strength of image charge 1 nm from surface is U = -0.75eV
*/
//Top down view of gratings:

//                                                ---------------------   -> at G2_z = 1,   z2
//
//                                                ---------------------   -> at G1_z = 0.000001,  z1
//                                                  (source going up)



int col = 2;                                // can be removed, colomns of ix array // remnant of the old code, even though it's not used (it's an argument for the gp0, gp1, gp2 functions, but doesn't do anything)
int rowsT =41;                              // rows of ReT and ImT arrays; used to calculate phase shift



int main(int argc, char *argv[]){ 
	sp.accountGrav = atoi(argv[1]);
	sp.elecOrAtom = atoi(argv[2]);
	sp.vel = atoi(argv[4]); //velocity of particle
	sp.energy = ((1.5 * pow(10,-18))/(pow(0.00000000001,2))) * (1);
	sp.simchoice = atoi(argv[6]);
	sp.logchoice = atoi(argv[7]);
	sp.useimagecharge =	0;
	sp.eta1 = .4;
	sp.eta2 =.4;
	sp.g_period = atoi(argv[5]);
	sp.r0 = -4.04;
	sp.el0 = 0.000001;
	sp.w0 =	0.00003;
	sp.G1_z =0.000001;
	sp.G2_z = 1;
	sp.G2_x = 0.00000005;
	sp.theta = 00.000001;
	sp.thick = 0.000000014;
	sp.Gthick = 1000;
	sp.wedgeangle =	0; // wedge angle
	sp.tilt = 0; //tilt
	sp.res = atoi(argv[3]);// resolution
	sp.zstart =	-0.1;
	sp.zend = 2.1;
	sp.xstart =	-0.00020;
	sp.xend = 0.00020; //xend
	sp.ystart =	-0.00011; //ystart
	sp.yend = 0.00011; //yend
	sp.height = (sp.g_period / 2) / 1000000000;
	sp.cutoff = 0.000001;
	sp.logchoice = 0; //scaled logarithmically so they can see where more of the particles go [0] = no, [1] = yes.
	/*arguments are currently in the order of:
argv[#]
	
	1. Account gravity? 1 = True, 0 = False.
	2. Electron beam or atom beam? Electron beam = 1, Atom beam = 2. //ELECTRON BEAM NOT MODELED!!!
	3. Resolution [300-400 recommended]. 
	4. Velocity of particles in m/s
	5. Pitch of gratings [in nm] 
	6. Total simulation [1]? or final interference pattern [2]?
	7. (if total simulation = 1), logscale[1] or normal scale[0]?
	
	*/ 
 	
    // initializing two arrays to contain the intensities and xpositions of each intensity -- Mcomment: I tried to fix this, but what is it actually saying?  It's the intensities of the x-positions and the intensities?  That doesn't make sense to me.
    int zlocstart;				// where z position begins
	int rows = sp.res;					// times to repeate diffraction phase shifts (2, 1 for each grating)
	double max;
	double *Grat3I;             // intensity array
    double *Grat3x;             // array of x position of intensity
	Grat3I = (double*) calloc(sp.res, sizeof(double));
    Grat3x = (double*) calloc(sp.res, sizeof(double));
	
   
    // Mcomment: what is all of this?  Please describe.  Also, these do not have helpful
    // variable names - please fix.  It's very important for readability.  Just tell
    // me what you want them to be, and I'll make the change.
   	int izxnumels = rows * rows;
    double izxsize = izxnumels * sizeof(double);
    double *izx = (double*) calloc(izxnumels, sizeof(double)); 
    double zres = (sp.zend-sp.zstart)/sp.res; // step resolution used in computation

    // Following three are used to calculate GSM values at the first grating. (Gaussian-Schell Model)
    // Mcomment: GSM?  Did you define this elsewhere?  Avoid acronyms when and where possible.  More keystrokes are better.
    //double w1=w(sp.G1_z,sp.r0,sp.el0,sp.w0,sp.energy); 
    double w1=w(sp.G1_z, sp.r0, sp.el0, sp.w0); 

	// okay, using a function w to initialize this variable w1... should change the names if possible. w1 = coherence width of beam. 
    //double r1 = v(sp.G1_z,sp.r0,sp.el0,sp.w0,sp.energy); 
    double r1 = v(sp.G1_z, sp.r0, sp.el0, sp.w0); 

	// same as above. r1 defined in the function.
	//double el1=el(sp.G1_z,sp.r0,sp.el0,sp.w0,sp.energy); 
    double el1=el(sp.G1_z, sp.r0, sp.el0, sp.w0); 
    // same as above? All three different functions, but same parameters passed.
 
    // Mcomment - this is an example of embarassingly parallel computation.  Melanie will parallelize once cleaned up.
    // Mcomment - I cleaned up this indent structure.  Follow this indent structure in the future.
    //            The previous indent structure was outdated, unconventional, and unreadable.
    if (sp.simchoice == 1) {
       //  printf("Do you want to have a logarithmic scale? [1 for yes, 2 for no] [Makes small intensities more visible]: ");
       // scanf("%d", &logchoice);
		sp.logchoice = atoi(argv[7]);
        zlocstart = 0;
    }
    else if (sp.simchoice == 2) {
        zlocstart = sp.res - 1;
    }

    for (int i=(zlocstart); i<rows; i++) {
        // i = 299 is just to get last row of the z if resolution is set to 300. 
        memset(Grat3x, 0, rows * sizeof(double)); 
        // so each time the loop repeats, you reset the array's positions and intensities to zero. 
        memset(Grat3I, 0, rows * sizeof(double));
             
        double zloc = sp.zstart  +  i * zres; // where you are with respect to z. 
        
  
        // if, else if, etc. is to determine where you are on zloc, and depending on where you are, you interact with different gratings. 

        if (zloc > sp.G2_z) { 
            // if the location is above G2_z [which is 1 currently]
        
            // Mcomment: This line is exactly and 100% why we MUST use better 
            // variable names.  This is an absolute mess.
            //gp2(sp.G2_z-sp.G1_z, zloc-sp.G2_z, sp.theta, el1, w1, r1, el1, w1, r1, sp.G2_x, Grat3x, Grat3I, sp.energy, rows, col, sp.elecOrAtom, sp.vel, sp.res, sp.height, zloc, sp.accountGrav); 
            gp2(zloc, el1, w1, r1, Grat3x, Grat3I); 

			// gp2 returns two values, has to, since q is an array of 2 doubles
                                                                      
            max = maximumvalue(Grat3I, rows); 
            // the largest value of ix.

            ixgenerator(Grat3I, zloc, sp.logchoice, rows); 
            // ixgenerator returns two values, has to, since q1 is an array of 2 doubles
 
         
        }
        else {
            // can probably be replaced, more efficiently, by elseifs. 
            if (zloc > sp.G1_z) {
                //gp1(zloc - sp.G1_z, r1, el1, w1, Grat3x, Grat3I,sp.energy,rows,col, sp.elecOrAtom, sp.vel, sp.res, sp.height, zloc, sp.accountGrav); 
				gp1(zloc, r1, el1, w1, Grat3x, Grat3I); 

                // if interacting with the first grating

                max = maximumvalue(Grat3I, rows); 
                // max value of ix here.
                ixgenerator(Grat3I, zloc, sp.logchoice, rows); 
                // still ixgenerator is returning two values, since q1 is a pointer to an array of 2 doubles


            }
            else {
                // simple GSM propagation until it hits the first grating
                //gp0(zloc, r0, el0, w0,Grat3x, Grat3I,energy,rows,col, rows); 
				gp0(zloc,Grat3x, Grat3I);
                // if at the origin?

                max = maximumvalue(Grat3I, rows); 
                // as above
                ixgenerator(Grat3I, zloc, sp.logchoice, rows); 
                // as above

            }
        }   
        
        
   
        // REMEMBER: STILL INSIDE PREVIOUS FOR LOOP: for (i = 0; i < zpnts; i++ ) // Can optimize or make more efficient? Ranges or vectorize?
        // Mcomment - for now, if you have to mention this, it really indicates your code is unreadable.

        for (int j=0; j<rows; j++ ) {
        // perhaps this for loop could be avoided with clever programming? Its purpose appears to just translate over values.
            
            int f = rows * i + j; 
            //400 * i + j; still keeping track of location.
            
            izx[f] = Grat3I[j]; 
            // the f-th element of izx is set to be ix[j][1]; the intensity of the beam at the j-th point.
            

            // Grat3x[int(izxnumels - f)] = izxnumels - f; // xpos from 0 to 299.
            // Grat3I[int(izxnumels - f)] = izx[f]; // intensity for each expos 
            
printf("the value of izx is:%0.15f \t %d \t %f \n", izx[f],f, max); // debug print            
            // printing out izf, f, and the max value of ix.
        }
        // Mcomment - this isn't necessary to say.
        // There is some code that's approximately here in McMorran's thesis, but not here now. 
    }
    if (sp.simchoice == 1) {
        free(Grat3x); 
        // free the memory used by this array, since the simulation is over, izx has all the data.
        free(Grat3I); 
        // same as above.

        SimplePlot::twoD("Intensity graph as particles diffract through gratings",izx,0.0,1.0,0.0,1.0,rows,rows); 
        // using ROOT to plot izx
    }
    else if(sp.simchoice == 2) {
        SimplePlot::graph("Relative Intensity Along Final Grating", Grat3x, Grat3I, rows);  
        // using ROOT to plot intensity vs. position at end of interferometer
        free(Grat3x); 
        // free the memory used by this array, since the simulation is over.
        free(Grat3I); 
        // same as above.
    }
    // free up the space used by the izx array.
    free(izx);
}
