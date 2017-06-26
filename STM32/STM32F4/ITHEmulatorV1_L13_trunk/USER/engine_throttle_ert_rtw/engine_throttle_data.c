/*
 * File: engine_throttle_data.c
 *
 * Code generated for Simulink model 'engine_throttle'.
 *
 * Model version                  : 1.224
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Fri Apr 18 18:46:27 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "engine_throttle.h"
#include "engine_throttle_private.h"

/* Block parameters (auto storage) */
P_engine_throttle_T engine_throttle_P = {
  2121824376,                          /* Variable: K_ITH_POS_PHYS_K
                                        * Referenced by: '<S4>/K_ITH_POS_PHYS_K'
                                        */
  1707598466,                          /* Variable: Ksp
                                        * Referenced by: '<S5>/Spring'
                                        */
  1770963193,                          /* Variable: rad_open
                                        * Referenced by: '<S3>/High'
                                        */
  0,                                   /* Variable: rad_relaxed
                                        * Referenced by: '<S2>/Relaxed Position'
                                        */
  0,                                   /* Variable: K_ITH_POS_OFFSET
                                        * Referenced by: '<S2>/K_ITH_POS_OFFSET'
                                        */
  0,                                   /* Variable: rad_init
                                        * Referenced by:
                                        *   '<S3>/Low'
                                        *   '<S5>/Relaxed Position'
                                        */
  0,                                   /* Variable: springTorqueCompensation
                                        * Referenced by: '<S5>/springTorqueCompensation'
                                        */
  1067666165                           /* Variable: K_ITH_POS_PHYS_OFFSET
                                        * Referenced by: '<S4>/K_ITH_POS_PHYS_OFFSET'
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
