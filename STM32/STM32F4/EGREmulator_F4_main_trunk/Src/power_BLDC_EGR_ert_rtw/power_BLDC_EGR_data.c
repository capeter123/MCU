/*
 * File: power_BLDC_EGR_data.c
 *
 * Code generated for Simulink model 'power_BLDC_EGR'.
 *
 * Model version                  : 1.170
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Fri Apr 24 16:02:07 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "power_BLDC_EGR.h"
#include "power_BLDC_EGR_private.h"


/* Block parameters (auto storage) 
** The Default type of following variables are fix(1,32,10), but
**  K_EGR_BLDC_POS_OFFSET, Ksp and valve_pitch are excepting, these 
** are fix(1,16,7).
*/
P_power_BLDC_EGR_T power_BLDC_EGR_P = {
    1,                                   /* Variable: B
                                        * Referenced by: '<S20>/Damping Constant (B)'
                                        */
  87,                                  /* Variable: Flux
                                        * Referenced by:
                                        *   '<S11>/Flux Constant (Flux)'
                                        *   '<S12>/Flux Constant (Flux)'
                                        *   '<S13>/Flux Constant (Flux)'
                                        */
  1281746,                             /* Variable: J
                                        * Referenced by: '<S7>/Inertia Constant's Recip (1//J)'
                                        */
  213333,                              /* Variable: L
                                        * Referenced by:
                                        *   '<S18>/Inductance Constant's Recip (1//L)'
                                        *   '<S19>/Inductance Constant's Recip (1//L)'
                                        */
  1202,                                /* Variable: R
                                        * Referenced by:
                                        *   '<S18>/Resistance Constant (R)'
                                        *   '<S19>/Resistance Constant (R)'
                                        */
  0,                                   /* Variable: SpringCom
                                        * Referenced by: '<S3>/spring_compensation'
                                        */
  0,                                   /* Variable: Tf
                                        * Referenced by: '<S20>/Shaft static friction torque (Tf)'
                                        */
  -3548,                               /* Variable: motor_stopper_init_rad
                                        * Referenced by: '<S21>/Low'
                                        */
  15900,                               /* Variable: motor_stopper_open_rad
                                        * Referenced by:
                                        *   '<S3>/Saturation'
                                        *   '<S21>/High'
                                        */
  1072,                                /* Variable: thOffset
                                        * Referenced by: '<S7>/theOffset'
                                        */
  0,                                   /* Variable: tho
                                        * Referenced by: '<S7>/tho'
                                        */
  268,                                 /* Variable: valve_stopper_rad
                                        * Referenced by:
                                        *   '<S3>/Relaxed Position'
                                        *   '<S3>/Saturation'
                                        */
  -555,                                   /* Variable: K_EGR_BLDC_POS_OFFSET
                                        * Referenced by: '<S4>/K_EGR_BLDC_POS_OFFSET'
                                        */
  512,                                 /* Variable: valve_pitch
                                        * Referenced by: '<S3>/load pitch(mm)'
                                        */
  263,                                 /* Variable: Ksp
                                        * Referenced by: '<S3>/Spring Coefficient (Ksp)'
                                        */
  4,                                   /* Variable: p
                                        * Referenced by:
                                        *   '<S5>/Rotor Poles Num (p)'
                                        *   '<S7>/Rotor Poles Num (p)'
                                        *   '<S7>/Gain'
                                        */
  60U                                  /* Variable: EGR_BLDC_STEP
                                        * Referenced by: '<S1>/EGR_BLDC_STEP'
                                        */
};

/* Constant parameters (auto storage) */
const ConstP_power_BLDC_EGR_T power_BLDC_EGR_ConstP = {
  /* Pooled Parameter (Expression: 2*pi)
   * Referenced by:
   *   '<S17>/Constant'
   *   '<S14>/Constant'
   *   '<S15>/Constant'
   *   '<S16>/Constant'
   */
  6434//6.2831853071795862
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
