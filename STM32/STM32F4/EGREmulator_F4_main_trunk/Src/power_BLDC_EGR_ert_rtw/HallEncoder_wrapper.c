/*
  *
  *   --- THIS FILE GENERATED BY S-FUNCTION BUILDER: 3.0 ---
  *
  *   This file is a wrapper S-function produced by the S-Function
  *   Builder which only recognizes certain fields.  Changes made
  *   outside these fields will be lost the next time the block is
  *   used to load, edit, and resave this file. This file will be overwritten
  *   by the S-function Builder block. If you want to edit this file by hand, 
  *   you must change it only in the area defined as:  
  *
  *        %%%-SFUNWIZ_wrapper_XXXXX_Changes_BEGIN 
  *            Your Changes go here
  *        %%%-SFUNWIZ_wrapper_XXXXXX_Changes_END
  *
  *   For better compatibility with the Simulink Coder, the
  *   "wrapper" S-function technique is used.  This is discussed
  *   in the Simulink Coder User's Manual in the Chapter titled,
  *   "Wrapper S-functions".
  *
  *   Created: Fri Mar 28 12:37:02 2014
  */


/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
 
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void HallEncoder_Outputs_wrapper(const int32_T *theta,
                          boolean_T *Hallx,
                          int8_T *phase)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
int32_T theta_mod = theta[0];
if(theta[0]<0)
    theta_mod += 368640;

if(theta_mod>=0 && theta_mod<61440)
{
     phase[0] = 2;
     Hallx[0] = 1;
     Hallx[1] = 0;
     Hallx[2] = 0;
}
else if(theta_mod>=61440 && theta_mod<122880)
{
     phase[0] = 3;
     Hallx[0] = 1;
     Hallx[1] = 1;
     Hallx[2] = 0;
}
else if(theta_mod>=122880 && theta_mod<184320)
{
    phase[0] = 4;
    Hallx[0] = 0;
    Hallx[1] = 1;
    Hallx[2] = 0;
}
else if(theta_mod>=184320 && theta_mod<245760)
{
    phase[0] = 5;
    Hallx[0] = 0;
    Hallx[1] = 1;
    Hallx[2] = 1;
}
else if(theta_mod>=245760 && theta_mod<307200)
{
    phase[0] = 6;
    Hallx[0] = 0;
    Hallx[1] = 0;
    Hallx[2] = 1;
}
else
{
    phase[0] = 1;
    Hallx[0] = 1;
    Hallx[1] = 0;
    Hallx[2] = 1;
}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
