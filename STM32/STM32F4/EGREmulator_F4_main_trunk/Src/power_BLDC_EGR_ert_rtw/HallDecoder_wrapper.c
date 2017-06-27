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
  *   Created: Sat Mar  1 13:29:08 2014
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
#define u_width 3
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
void HallDecoder_Outputs_wrapper(const boolean_T *Hallx,
                          int8_T *egr_bldc_actual_phase)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
if(Hallx[0]) // rotor phase 1,2,3
{
    if(Hallx[1])
    {
        egr_bldc_actual_phase[0] = 3;
    }
    else
    {
        switch(Hallx[2])
        {
            case 0x0:
                egr_bldc_actual_phase[0] = 2;
                break;
            case 0x1:
                egr_bldc_actual_phase[0] = 1;
                break;
            default:
                break;
        }
    }
}
else // rotor phase 4,5,6
{
    if(!Hallx[1])
    {
        egr_bldc_actual_phase[0] = 6;
    }
    else
    {
        switch(Hallx[2])
        {
            case 0x0:
                egr_bldc_actual_phase[0] = 4;
                break;
            case 0x1:
                egr_bldc_actual_phase[0] = 5;
                break;
            default:
                break;
        }
    }
}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}