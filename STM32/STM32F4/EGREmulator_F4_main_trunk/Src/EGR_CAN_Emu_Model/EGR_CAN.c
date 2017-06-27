/*
 * File: EGR_CAN.c
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

#include "EGR_CAN.h"
#include "EGR_CAN_private.h"

/* Exported block signals */
real_T egr_can_target_pos;             /* '<Root>/egr_can_target_pos' */
real_T egr_can_act_pos;                /* '<S1>/Saturation1' */
real_T egr_can_act_dc;                 /* '<S1>/Switch' */

/* Exported block parameters */
real_T C_STEPM_ELEC_TIM_COEFF = 460.0; /* Variable: C_STEPM_ELEC_TIM_COEFF
                                        * Referenced by:
                                        *   '<S2>/B+J*Trc'
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */
real_T K_EGR_CAN_ACT_DC_COMP = 10.0;   /* Variable: K_EGR_CAN_ACT_DC_COMP
                                        * Referenced by: '<S1>/K_EGR_CAN_ACT_DC_COMP'
                                        */
real_T K_EGR_CAN_ACT_POS_MAX = 128.0;  /* Variable: K_EGR_CAN_ACT_POS_MAX
                                        * Referenced by: '<S1>/Saturation1'
                                        */
real_T K_EGR_CAN_ACT_POS_MIN = 0.0;    /* Variable: K_EGR_CAN_ACT_POS_MIN
                                        * Referenced by: '<S1>/Saturation1'
                                        */
real_T K_EGR_CAN_TARGET_POS_MAX = 106.0;/* Variable: K_EGR_CAN_TARGET_POS_MAX
                                         * Referenced by: '<S1>/Saturation'
                                         */
real_T K_EGR_CAN_TARGET_POS_MIN = 16.0;/* Variable: K_EGR_CAN_TARGET_POS_MIN
                                        * Referenced by:
                                        *   '<S1>/K_EGR_CAN_TARGET_POS_MIN'
                                        *   '<S1>/Saturation'
                                        */
real_T K_PID_D_TERM = 0.0;             /* Variable: K_PID_D_TERM
                                        * Referenced by: '<S3>/Derivative Gain'
                                        */
real_T K_PID_I_TERM = 20000.0;         /* Variable: K_PID_I_TERM
                                        * Referenced by: '<S3>/Integral Gain'
                                        */
real_T K_PID_P_TERM = 0.005975;        /* Variable: K_PID_P_TERM
                                        * Referenced by: '<S3>/Proportional Gain'
                                        */
real_T K_STEPM_DAMPING_COEFF = 0.03;   /* Variable: K_STEPM_DAMPING_COEFF
                                        * Referenced by:
                                        *   '<S2>/B+J*Trc'
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        */
real_T K_STEPM_INERTIA = 0.0001;       /* Variable: K_STEPM_INERTIA
                                        * Referenced by:
                                        *   '<S2>/1//J'
                                        *   '<S2>/B+J*Trc'
                                        */
real_T K_STEPM_SELF_DAMPING_COEFF = 0.1;/* Variable: K_STEPM_SELF_DAMPING_COEFF
                                         * Referenced by: '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                         */
real_T K_STEPM_STEP_NUM = 50.0;        /* Variable: K_STEPM_STEP_NUM
                                        * Referenced by:
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */
real_T K_STEPM_TF = 1.42;              /* Variable: K_STEPM_TF
                                        * Referenced by:
                                        *   '<S2>/Tm*Zr*(1+Kp)+Trc*B'
                                        *   '<S2>/Trc*Tm*Zr'
                                        */

/* Block signals and states (auto storage) */
DW_EGR_CAN_T EGR_CAN_DW;

/* Model step function */
void EGR_CAN_step(void)
{
  real_T rtb_egr_can_target_pos_fnl;
  real_T rtb_Sum_m;
  real_T rtb_FilterCoefficient;
  real_T rtb_ProportionalGain;

  /* Saturate: '<S1>/Saturation1' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator2'
   */
  if (EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE >= K_EGR_CAN_ACT_POS_MAX) {
    egr_can_act_pos = K_EGR_CAN_ACT_POS_MAX;
  } else if (EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE <= K_EGR_CAN_ACT_POS_MIN)
  {
    egr_can_act_pos = K_EGR_CAN_ACT_POS_MIN;
  } else {
    egr_can_act_pos = EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE;
  }

  /* End of Saturate: '<S1>/Saturation1' */

  /* Saturate: '<S1>/Saturation' incorporates:
   *  Inport: '<Root>/egr_can_target_pos'
   */
  if (egr_can_target_pos >= K_EGR_CAN_TARGET_POS_MAX) {
    rtb_egr_can_target_pos_fnl = K_EGR_CAN_TARGET_POS_MAX;
  } else if (egr_can_target_pos <= K_EGR_CAN_TARGET_POS_MIN) {
    rtb_egr_can_target_pos_fnl = K_EGR_CAN_TARGET_POS_MIN;
  } else {
    rtb_egr_can_target_pos_fnl = egr_can_target_pos;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Sum: '<S1>/Sum' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator2'
   */
  rtb_Sum_m = rtb_egr_can_target_pos_fnl -
    EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE;

  /* Gain: '<S3>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S3>/Filter'
   *  Gain: '<S3>/Derivative Gain'
   *  Sum: '<S3>/SumD'
   */
  rtb_FilterCoefficient = (K_PID_D_TERM * rtb_Sum_m - EGR_CAN_DW.Filter_DSTATE) *
    100.0;

  /* Gain: '<S3>/Proportional Gain' incorporates:
   *  DiscreteIntegrator: '<S3>/Integrator'
   *  Sum: '<S3>/Sum'
   */
  rtb_ProportionalGain = ((rtb_Sum_m + EGR_CAN_DW.Integrator_DSTATE) +
    rtb_FilterCoefficient) * K_PID_P_TERM;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/C_EGR_CAN_INIT_DC'
   *  Constant: '<S1>/K_EGR_CAN_ACT_DC_COMP'
   *  Constant: '<S1>/K_EGR_CAN_TARGET_POS_MIN'
   *  RelationalOperator: '<S1>/ '
   *  Sum: '<S1>/Add'
   */
  if (K_EGR_CAN_TARGET_POS_MIN >= rtb_egr_can_target_pos_fnl) {
    egr_can_act_dc = 0.0;
  } else {
    egr_can_act_dc = (K_EGR_CAN_ACT_DC_COMP + rtb_egr_can_target_pos_fnl) -
      rtb_ProportionalGain;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Sum: '<S2>/Add' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator1'
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator2'
   *  Gain: '<S2>/B+J*Trc'
   *  Gain: '<S2>/Tm*Zr*(1+Kp)+Trc*B'
   *  Gain: '<S2>/Trc*Tm*Zr'
   *  Sum: '<S2>/Sum'
   */
  rtb_egr_can_target_pos_fnl = (K_STEPM_TF * C_STEPM_ELEC_TIM_COEFF *
    K_STEPM_STEP_NUM * (rtb_ProportionalGain -
                        EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE) - (K_STEPM_TF
    * K_STEPM_STEP_NUM * (1.0 + K_STEPM_SELF_DAMPING_COEFF) +
    C_STEPM_ELEC_TIM_COEFF * K_STEPM_DAMPING_COEFF) *
    EGR_CAN_DW.DiscreteTimeIntegrator1_DSTATE) - (K_STEPM_INERTIA *
    C_STEPM_ELEC_TIM_COEFF + K_STEPM_DAMPING_COEFF) *
    EGR_CAN_DW.DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator2' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator1'
   */
  EGR_CAN_DW.DiscreteTimeIntegrator2_DSTATE += 0.0001 *
    EGR_CAN_DW.DiscreteTimeIntegrator1_DSTATE;

  /* Update for DiscreteIntegrator: '<S3>/Integrator' incorporates:
   *  Gain: '<S3>/Integral Gain'
   */
  EGR_CAN_DW.Integrator_DSTATE += K_PID_I_TERM * rtb_Sum_m * 0.0001;

  /* Update for DiscreteIntegrator: '<S3>/Filter' */
  EGR_CAN_DW.Filter_DSTATE += 0.0001 * rtb_FilterCoefficient;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator1' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   */
  EGR_CAN_DW.DiscreteTimeIntegrator1_DSTATE += 0.0001 *
    EGR_CAN_DW.DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S2>/1//J'
   */
  EGR_CAN_DW.DiscreteTimeIntegrator_DSTATE += 1.0 / K_STEPM_INERTIA *
    rtb_egr_can_target_pos_fnl * 0.0001;
}

/* Model initialize function */
void EGR_CAN_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
