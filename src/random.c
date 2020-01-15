/******************************************************************************
 * $Id: random.c,v 1.5 2003/08/19 12:27:00 fang Exp $
 *   
 * File: random.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: random.c,v $
 *   Revision 1.5  2003/08/19 12:27:00  fang
 *   *** empty log message ***
 *
 *
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmaloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/05/31 08:57:34  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/05/31 07:45:30  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/05/30 13:49:46  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.2.2.1  2002/01/24 03:20:33  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: random.c                                      Part of CircleMUD *
*  Usage: pseudo-random number generator                                  *
************************************************************************ */

/*
 * I am bothered by the non-portablility of 'rand' and 'random' -- rand
 * is ANSI C, but on some systems such as Suns, rand has seriously tragic
 * spectral properties (the low bit alternates between 0 and 1!).  random
 * is better but isn't supported by all systems.  So, in my quest for Ultimate
 * CircleMUD Portability, I decided to include this code for a simple but
 * relatively effective random number generator.  It's not the best RNG code
 * around, but I like it because it's very short and simple, and for our
 * purposes it's "random enough".
 *               --Jeremy Elson  2/23/95
 *
 * Now that we're using GNU's autoconf, I've coded Circle to always use
 * random(), and automatically link in this object file if random() isn't
 * supported on the target system.  -JE 2/3/96
 */

/***************************************************************************/

/*
 *
 * This program is public domain and was written by William S. England
 * (Oct 1988).  It is based on an article by:
 *
 * Stephen K. Park and Keith W. Miller. RANDOM NUMBER GENERATORS:
 * GOOD ONES ARE HARD TO FIND. Communications of the ACM,
 * New York, NY.,October 1988 p.1192

 The following is a portable c program for generating random numbers.
 The modulus and multipilier have been extensively tested and should
 not be changed except by someone who is a professional Lehmer generator
 writer.  THIS GENERATOR REPRESENTS THE MINIMUM STANDARD AGAINST WHICH
 OTHER GENERATORS SHOULD BE JUDGED. ("Quote from the referenced article's
 authors. WSE" )
*/
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <time.h>

#include "utils.h"


#define	m  (unsigned long)2147483647
#define	q  (unsigned long)127773

#define	a (unsigned int)16807
#define	r (unsigned int)2836

/*
** F(z)	= (az)%m
**	= az-m(az/m)
**
** F(z)  = G(z)+mT(z)
** G(z)  = a(z%q)- r(z/q)
** T(z)  = (z/q) - (az/m)
**
** F(z)  = a(z%q)- rz/q+ m((z/q) - a(z/m))
** 	 = a(z%q)- rz/q+ m(z/q) - az
*/

static unsigned long seed;

void m_srandom(unsigned long initial_seed)
{
    seed = initial_seed; 
}


unsigned long m_random(void)
{
   register int lo, hi, test;

    hi   = seed/q;
    lo   = seed%q;

    test = a*lo - r*hi;

    if (test > 0)
    seed = test;
    else
    seed = test+ m;

    return seed;
}

static int ___inited = 0;

double gaussrand(void)                
{
  static double V1, V2, S;
  static int phase = 0;
  double X;

  if (!___inited) {
    srand(time(NULL));
    ___inited = 1;
  }

  if(phase == 0) {
    do {
      double U1 = (double)rand() / RAND_MAX;
      double U2 = (double)rand() / RAND_MAX;
                                        
      V1 = 2 * U1 - 1;
      V2 = 2 * U2 - 1;
      S = V1 * V1 + V2 * V2;
    } while(S >= 1 || S == 0);

    X = V1 * sqrt(-2 * log(S) / S);
  } else
    X = V2 * sqrt(-2 * log(S) / S);

  phase = 1 - phase;
                        
  return X;
}

/*
 * [-80 ; 30] : sparked,  call with (-28,18,-80,30)
 * [-50 ; 80] : wilder,   call with (15,18,-50,80)
 * [-80 ; 50] : channies, call with (-20,18,-80,50)
 */
int gaussian_var(int med,int d,int l,int h)
{
  int rh = 0,rl = 0, result = 0;
  double gauss = gaussrand(); /* N(0,1); */
  
  rl = (l <= h ? l : h);
  rh = (l <= h ? h : l);

  /*
   * Basic sanity checking if median+-deviation is outside our interval
   * Assume a rectangular distrubution between l and h. 
   */
  if (((med+d) >= rh) || ((med-d) <= rl)) 
    return rl + (rand() % (rh-rl));
 
  /*
   * Now, to the dificult part, mapping between the gaussian number, and an 
   * integer value in the range of h - l 
   * I'll try something ugly here. :)
   * Seems to work though, on a attempt with 3000 iterations
   * (30 iterations / sample, 100 samples) the average was ~30 all the time. 
   * Input: m = 30, d = 15, h = 60, l = 1
   */
  do {
    result = (int)((double)med + (d * (gauss + 0.5))+0.5); 
    gauss = gaussrand();
  } while (result < rl || result > rh);

  return result;
}

static double randDouble(void) { 
  return (double)rand()/(double)RAND_MAX; 
}

unsigned int randPoisson(double dMean)
{
  unsigned int rr = 0;
  double p = randDouble();
  dMean = exp(-dMean);

  while (p >= dMean) {
    p *= randDouble();
    rr++;
  }
  return rr;
}

