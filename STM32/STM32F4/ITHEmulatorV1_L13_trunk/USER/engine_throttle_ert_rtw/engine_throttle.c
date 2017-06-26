/*
 * File: engine_throttle.c
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

/* Block signals and states (auto storage) */
DW_engine_throttle_T engine_throttle_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_engine_throttle_T engine_throttle_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_engine_throttle_T engine_throttle_Y;
void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo)
{
  uint32_T absIn;
  uint32_T absIn_0;
  uint32_T in0Lo;
  uint32_T in0Hi;
  uint32_T in1Hi;
  uint32_T productHiLo;
  uint32_T productLoHi;
  absIn = (uint32_T)(in0 < 0 ? -in0 : in0);
  absIn_0 = (uint32_T)(in1 < 0 ? -in1 : in1);
  in0Hi = absIn >> 16U;
  in0Lo = absIn & 65535U;
  in1Hi = absIn_0 >> 16U;
  absIn = absIn_0 & 65535U;
  productHiLo = in0Hi * absIn;
  productLoHi = in0Lo * in1Hi;
  absIn *= in0Lo;
  absIn_0 = 0U;
  in0Lo = (productLoHi << /*MW:OvBitwiseOk*/ 16U) + /*MW:OvCarryOk*/ absIn;
  if (in0Lo < absIn) {
    absIn_0 = 1U;
  }

  absIn = in0Lo;
  in0Lo += /*MW:OvCarryOk*/ productHiLo << /*MW:OvBitwiseOk*/ 16U;
  if (in0Lo < absIn) {
    absIn_0++;
  }

  absIn = (((productLoHi >> 16U) + (productHiLo >> 16U)) + in0Hi * in1Hi) +
    absIn_0;
  if (!((in0 == 0) || ((in1 == 0) || ((in0 > 0) == (in1 > 0))))) {
    absIn = ~absIn;
    in0Lo = ~in0Lo;
    in0Lo++;
    if (in0Lo == 0U) {
      absIn++;
    }
  }

  *ptrOutBitsHi = absIn;
  *ptrOutBitsLo = in0Lo;
}

int32_T mul_s32_s32_s32_sr47(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 15;
}

int32_T mul_s32_s32_s32_sr36(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 4;
}

int32_T mul_s32_s32_s32_sr19(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 13U | u32_clo >> 19U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr41(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 9;
}

int32_T mul_s32_s32_s32_sr29(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 3U | u32_clo >> 29U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr31(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 1U | u32_clo >> 31U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr21(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 11U | u32_clo >> 21U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr30(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 2U | u32_clo >> 30U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr28(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 4U | u32_clo >> 28U;
  return (int32_T)u32_clo;
}

int32_T mul_s32_s32_s32_sr22(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 10U | u32_clo >> 22U;
  return (int32_T)u32_clo;
}

/* Model step function */
void engine_throttle_step(void)
{
  int32_T u;
  int32_T rtb_Sum2;
  int32_T rtb_LGain;
  int32_T qY;
  int32_T q;
  int32_T qY_0;

  /* Constant: '<S2>/Relaxed Position' */
  engine_throttle_DW.RelaxedPosition = engine_throttle_P.rad_relaxed;

  /* Gain: '<S2>/rad-to-deg' incorporates:
   *  DiscreteIntegrator: '<S2>/Position'
   */
  rtb_Sum2 = mul_s32_s32_s32_sr30(961263669, engine_throttle_DW.Position_DSTATE);

  /* Outport: '<Root>/ith_degree' */
  engine_throttle_Y.ith_degree = rtb_Sum2;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Constant: '<S2>/Constant'
   *  Constant: '<S2>/K_ITH_POS_OFFSET'
   *  Product: '<S2>/Product'
   */
  rtb_Sum2 = (engine_throttle_P.K_ITH_POS_OFFSET << 11) + mul_s32_s32_s32_sr30
    (rtb_Sum2, 1193046471);

  /* Saturate: '<S2>/Saturation' */
  if (rtb_Sum2 >= 2097152000) {
    rtb_Sum2 = 2097152000;
  } else {
    if (rtb_Sum2 <= -419430400) {
      rtb_Sum2 = -419430400;
    }
  }

  /* End of Saturate: '<S2>/Saturation' */

  /* Sum: '<S2>/Sum3' incorporates:
   *  Constant: '<S2>/Constant1'
   */
  rtb_Sum2 = 1677721600 - rtb_Sum2;

  /* Outport: '<Root>/ith_pos' */
  engine_throttle_Y.ith_pos = rtb_Sum2;

  /* Sum: '<S4>/Sum' incorporates:
   *  Constant: '<S4>/K_ITH_POS_PHYS_OFFSET'
   *  Gain: '<S4>/K_ITH_POS_PHYS_K'
   */
  rtb_Sum2 = (engine_throttle_P.K_ITH_POS_PHYS_OFFSET >> 2) +
    mul_s32_s32_s32_sr31(engine_throttle_P.K_ITH_POS_PHYS_K, rtb_Sum2);

  /* Saturate: '<S2>/Saturation1' */
  if (rtb_Sum2 >= 2146959360) {
    /* Outport: '<Root>/ith_pos_da' */
    engine_throttle_Y.ith_pos_da = 65520U;
  } else if (rtb_Sum2 <= 0) {
    /* Outport: '<Root>/ith_pos_da' */
    engine_throttle_Y.ith_pos_da = 0U;
  } else {
    /* Outport: '<Root>/ith_pos_da' */
    engine_throttle_Y.ith_pos_da = (uint16_T)(rtb_Sum2 >> 15);
  }

  /* End of Saturate: '<S2>/Saturation1' */

  /* Gain: '<S1>/L Gain' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S2>/Velocity'
   *  Gain: '<S1>/Kemf'
   *  Gain: '<S1>/R Gain'
   *  Inport: '<Root>/U'
   *  Sum: '<S1>/Add'
   */
  rtb_LGain = mul_s32_s32_s32_sr30(880116249, ((engine_throttle_U.U << 19) -
    (mul_s32_s32_s32_sr28(439804651, engine_throttle_DW.Velocity_DSTATE) >> 2))
    - mul_s32_s32_s32_sr22(2025204587,
    engine_throttle_DW.DiscreteTimeIntegrator_DSTATE));

  /* MinMax: '<S3>/Min1' incorporates:
   *  Constant: '<S3>/Low'
   *  DiscreteIntegrator: '<S2>/Position'
   */
  u = engine_throttle_DW.Position_DSTATE >> 17;
  if (!(u <= engine_throttle_P.rad_init)) {
    u = engine_throttle_P.rad_init;
  }

  /* Sum: '<S2>/Sum2' incorporates:
   *  Constant: '<S5>/Relaxed Position'
   *  Constant: '<S5>/springTorqueCompensation'
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S2>/Position'
   *  Gain: '<S1>/Torque Gain'
   *  Gain: '<S5>/Spring'
   *  Sum: '<S5>/Sum1'
   *  Sum: '<S5>/Sum2'
   */
  rtb_Sum2 = mul_s32_s32_s32_sr21(1224563543,
    engine_throttle_DW.DiscreteTimeIntegrator_DSTATE) >> 5;
  q = (mul_s32_s32_s32_sr31(engine_throttle_P.Ksp,
        engine_throttle_DW.Position_DSTATE - (engine_throttle_P.rad_init << 17))
       + (engine_throttle_P.springTorqueCompensation << 12)) >> 7;
  qY_0 = rtb_Sum2 - /*MW:OvSatOk*/ q;
  if ((rtb_Sum2 < 0) && ((q >= 0) && (qY_0 >= 0))) {
    qY_0 = MIN_int32_T;
  } else {
    if ((rtb_Sum2 >= 0) && ((q < 0) && (qY_0 < 0))) {
      qY_0 = MAX_int32_T;
    }
  }

  /* MinMax: '<S3>/Min3' incorporates:
   *  Constant: '<S3>/High'
   *  DiscreteIntegrator: '<S2>/Position'
   */
  if (engine_throttle_P.rad_open >= engine_throttle_DW.Position_DSTATE) {
    rtb_Sum2 = engine_throttle_P.rad_open;
  } else {
    rtb_Sum2 = engine_throttle_DW.Position_DSTATE;
  }

  /* End of MinMax: '<S3>/Min3' */

  /* Sum: '<S3>/Sum5' incorporates:
   *  Constant: '<S3>/High'
   */
  rtb_Sum2 >>= 18;
  q = engine_throttle_P.rad_open >> 18;
  qY = rtb_Sum2 - /*MW:OvSatOk*/ q;
  if ((rtb_Sum2 < 0) && ((q >= 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((rtb_Sum2 >= 0) && ((q < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* MinMax: '<S3>/Min1' */
  if (u > 16383) {
    u = MAX_int32_T;
  } else if (u <= -16384) {
    u = MIN_int32_T;
  } else {
    u <<= 17;
  }

  /* Sum: '<S3>/Sum5' incorporates:
   *  Constant: '<S3>/Low'
   */
  q = u >> 18;
  rtb_Sum2 = qY + /*MW:OvSatOk*/ q;
  if ((qY < 0) && ((q < 0) && (rtb_Sum2 >= 0))) {
    rtb_Sum2 = MIN_int32_T;
  } else {
    if ((qY > 0) && ((q > 0) && (rtb_Sum2 <= 0))) {
      rtb_Sum2 = MAX_int32_T;
    }
  }

  q = engine_throttle_P.rad_init >> 1;
  qY = rtb_Sum2 - /*MW:OvSatOk*/ q;
  if ((rtb_Sum2 < 0) && ((q >= 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((rtb_Sum2 >= 0) && ((q < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Sum: '<S2>/Sum2' incorporates:
   *  DiscreteIntegrator: '<S2>/Velocity'
   *  Gain: '<S2>/Damping'
   *  Gain: '<S3>/Gain2'
   *  Sum: '<S3>/Sum5'
   */
  q = 18304000 * qY;
  qY = qY_0 - /*MW:OvSatOk*/ q;
  if ((qY_0 < 0) && ((q >= 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((qY_0 >= 0) && ((q < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  q = mul_s32_s32_s32_sr29(555426513, engine_throttle_DW.Velocity_DSTATE) >> 5;
  qY_0 = qY - /*MW:OvSatOk*/ q;
  if ((qY < 0) && ((q >= 0) && (qY_0 >= 0))) {
    qY_0 = MIN_int32_T;
  } else {
    if ((qY >= 0) && ((q < 0) && (qY_0 < 0))) {
      qY_0 = MAX_int32_T;
    }
  }

  /* Update for DiscreteIntegrator: '<S2>/Position' incorporates:
   *  DiscreteIntegrator: '<S2>/Velocity'
   */
  engine_throttle_DW.Position_DSTATE += mul_s32_s32_s32_sr41(351843721,
    engine_throttle_DW.Velocity_DSTATE);

  /* Update for DiscreteIntegrator: '<S2>/Velocity' incorporates:
   *  Gain: '<S2>/Inertia'
   *  Sum: '<S2>/Sum2'
   */
  engine_throttle_DW.Velocity_DSTATE += mul_s32_s32_s32_sr36(351843721,
    mul_s32_s32_s32_sr19(906759, qY_0));

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  engine_throttle_DW.DiscreteTimeIntegrator_DSTATE += mul_s32_s32_s32_sr47
    (351843721, rtb_LGain);
}

/* Model initialize function */
void engine_throttle_initialize(void)
{
  /* Start for Constant: '<S2>/Relaxed Position' */
  engine_throttle_DW.RelaxedPosition = engine_throttle_P.rad_relaxed;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Position' */
  engine_throttle_DW.Position_DSTATE = engine_throttle_DW.RelaxedPosition;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
