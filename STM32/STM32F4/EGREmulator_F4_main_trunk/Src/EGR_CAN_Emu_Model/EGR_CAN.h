/*
 * File: EGR_CAN.h
 *
 * Code generated for Simulink model 'EGR_CAN'.
 *
 * Model version                  : 1.107
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Fri Jun 05 12:29:01 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#ifndef RTW_HEADER_EGR_CAN_h_
#define RTW_HEADER_EGR_CAN_h_
#ifndef EGR_CAN_COMMON_INCLUDES_
# define EGR_CAN_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* EGR_CAN_COMMON_INCLUDES_ */

#include "EGR_CAN_types.h"

/* Macros for accessing real-time model data structure */

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T DiscreteTimeIntegrator2_DSTATE;/* '<S2>/Discrete-Time Integrator2' */
  real_T Integrator_DSTATE;            /* '<S3>/Integrator' */
  real_T Filter_DSTATE;                /* '<S3>/Filter' */
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<S2>/Discrete-Time Integrator1' */
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S2>/Discrete-Time Integrator' */
} DW_EGR_CAN_T;

/* Block signals and states (auto storage) */
extern DW_EGR_CAN_T EGR_CAN_DW;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T egr_can_target_pos;      /* '<Root>/egr_can_target_pos' */
extern real_T egr_can_act_pos;         /* '<S1>/Saturation1' */
extern real_T egr_can_act_dc;          /* '<S1>/Switch' */

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T C_STEPM_ELEC_TIM_COEFF;  /* Variable: C_STEPM_ELEC_TIM_COEFF
                                        * Referenced by:
                                        *   '<S2>/B+J*Trc'
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */
extern real_T K_EGR_CAN_ACT_DC_COMP;   /* Variable: K_EGR_CAN_ACT_DC_COMP
                                        * Referenced by: '<S1>/K_EGR_CAN_ACT_DC_COMP'
                                        */
extern real_T K_EGR_CAN_ACT_POS_MAX;   /* Variable: K_EGR_CAN_ACT_POS_MAX
                                        * Referenced by: '<S1>/Saturation1'
                                        */
extern real_T K_EGR_CAN_ACT_POS_MIN;   /* Variable: K_EGR_CAN_ACT_POS_MIN
                                        * Referenced by: '<S1>/Saturation1'
                                        */
extern real_T K_EGR_CAN_TARGET_POS_MAX;/* Variable: K_EGR_CAN_TARGET_POS_MAX
                                        * Referenced by: '<S1>/Saturation'
                                        */
extern real_T K_EGR_CAN_TARGET_POS_MIN;/* Variable: K_EGR_CAN_TARGET_POS_MIN
                                        * Referenced by:
                                        *   '<S1>/K_EGR_CAN_TARGET_POS_MIN'
                                        *   '<S1>/Saturation'
                                        */
extern real_T K_PID_D_TERM;            /* Variable: K_PID_D_TERM
                                        * Referenced by: '<S3>/Derivative Gain'
                                        */
extern real_T K_PID_I_TERM;            /* Variable: K_PID_I_TERM
                                        * Referenced by: '<S3>/Integral Gain'
                                        */
extern real_T K_PID_P_TERM;            /* Variable: K_PID_P_TERM
                                        * Referenced by: '<S3>/Proportional Gain'
                                        */
extern real_T K_STEPM_DAMPING_COEFF;   /* Variable: K_STEPM_DAMPING_COEFF
                                        * Referenced by:
                                        *   '<S2>/B+J*Trc'
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        */
extern real_T K_STEPM_INERTIA;         /* Variable: K_STEPM_INERTIA
                                        * Referenced by:
                                        *   '<S2>/1//J'
                                        *   '<S2>/B+J*Trc'
                                        */
extern real_T K_STEPM_SELF_DAMPING_COEFF;/* Variable: K_STEPM_SELF_DAMPING_COEFF
                                          * Referenced by: '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                          */
extern real_T K_STEPM_STEP_NUM;        /* Variable: K_STEPM_STEP_NUM
                                        * Referenced by:
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */
extern real_T K_STEPM_TF;              /* Variable: K_STEPM_TF
                                        * Referenced by:
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */

/* Model entry point functions */
extern void EGR_CAN_initialize(void);
extern void EGR_CAN_step(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'EGR_CAN'
 * '<S1>'   : 'EGR_CAN/EGR CAN Emulator System'
 * '<S2>'   : 'EGR_CAN/EGR CAN Emulator System/EGR CAN Motor Plant'
 * '<S3>'   : 'EGR_CAN/EGR CAN Emulator System/PID Controller1'
 */

/*-
 * Requirements for '<Root>': EGR_CAN
 */
#endif                                 /* RTW_HEADER_EGR_CAN_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
