/*
 * File: power_BLDC_EGR.c
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

/* Block signals and states (auto storage) */
DW_power_BLDC_EGR_T power_BLDC_EGR_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_power_BLDC_EGR_T power_BLDC_EGR_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_power_BLDC_EGR_T power_BLDC_EGR_Y;
int32_T div_s32(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T tempAbsQuotient;
  uint32_T quotientNeedsNegation;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    quotientNeedsNegation = (uint32_T)((numerator < 0) != (denominator < 0));
    tempAbsQuotient = (uint32_T)(numerator >= 0 ? numerator : -numerator) /
      (denominator >= 0 ? denominator : -denominator);
    quotient = quotientNeedsNegation ? -(int32_T)tempAbsQuotient : (int32_T)
      tempAbsQuotient;
  }

  return quotient;
}

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

int32_T mul_s32_s32_s32_sr42(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 10;
}

int32_T mul_s32_s32_s32_sr10(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = u32_chi << /*MW:OvBitwiseOk*/ 22U | u32_clo >> 10U;
  return (int32_T)u32_clo;
}

/*
 * Output and update for atomic system:
 *    '<S11>/rem'
 *    '<S12>/rem'
 *    '<S13>/rem'
 *    '<S9>/rem'
 */
void power_BLDC_EGR_rem(int32_T rtu_the, DW_rem_power_BLDC_EGR_T *localDW)
{
 /* DataTypeConversion: '<S14>/Data Type Conversion' */
  //localDW->DataTypeConversion = (real_T)rtu_the * 0.0009765625;

  /* S-Function (remf): '<S14>/S-Function Builder' */
  remf_Outputs_wrapper(&rtu_the,
                       &power_BLDC_EGR_ConstP.pooled1,
                       &localDW->SFunctionBuilder );

  /* DataTypeConversion: '<S14>/Data Type Conversion1' */
  localDW->DataTypeConversion1 = (int32_T)(localDW->SFunctionBuilder);
}

/* Model step function */
void power_BLDC_EGR_step(void)
{
  /* local block i/o variables */
  int32_T rtb_DiscreteTimeIntegrator1;
  int32_T rtb_Sum;
  int32_T rtb_Sum_d;
  boolean_T negate;
  int8_T rtb_Sum_n;
  int16_T rtb_Sum2_i;
  int32_T rtb_wm;
  int32_T rtb_Gain_f;
  int32_T rtb_Add;
  int32_T rtb_Gain;
  int32_T rtb_Vab;
  int32_T rtb_Product1;
  int32_T rtb_Product1_p;
  int32_T rtb_Product1_f;
  static const int32_T cordicLUT_n11_s32En[11] = { 843314857, 497837829,
    263043837, 133525159, 67021687, 33543516, 16775851, 8388437, 4194283,
    2097149, 1048576 };

  int32_T rtb_Product_idx;
  int32_T rtb_Product_idx_0;
  int32_T qY;
  int8_T rtb_Sum_a;
  int16_T rtb_wm_0;

  /* DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' incorporates:
   *  Constant: '<S7>/theOffset'
   *  Constant: '<S7>/tho'
   *  Sum: '<S7>/Sum2'
   */
  if (power_BLDC_EGR_DW.DiscreteTimeIntegrator1_IC_LOAD != 0) {
    power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE = power_BLDC_EGR_P.tho +
      power_BLDC_EGR_P.thOffset;
  }

  rtb_DiscreteTimeIntegrator1 = power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE;

  /* End of DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' */

  /* Outputs for Atomic SubSystem: '<S9>/rem' */
  power_BLDC_EGR_rem(rtb_DiscreteTimeIntegrator1, &power_BLDC_EGR_DW.rem);

  /* End of Outputs for SubSystem: '<S9>/rem' */

  /* Gain: '<S9>/rad2deg1' */
  power_BLDC_EGR_DW.rad2deg1 = power_BLDC_EGR_DW.rem.SFunctionBuilder * 58671 >> 10;

  /* S-Function (HallEncoder): '<S9>/S-Function HallDecoder' */
  HallEncoder_Outputs_wrapper(&power_BLDC_EGR_DW.rad2deg1,
    &power_BLDC_EGR_Y.Hallx[0], &power_BLDC_EGR_Y.egr_bldc_phase_fnl );

  /* Sum: '<S4>/Sum' incorporates:
   *  UnitDelay: '<S4>/Unit Delay'
   */
  rtb_Sum_n = (int8_T)(power_BLDC_EGR_Y.egr_bldc_phase_fnl -
                       power_BLDC_EGR_DW.UnitDelay_DSTATE);

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/Constant'
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant3'
   *  Constant: '<S4>/Constant4'
   *  RelationalOperator: '<S4>/Relational Operator'
   *  RelationalOperator: '<S4>/Relational Operator1'
   *  Switch: '<S4>/Switch1'
   */
  if (rtb_Sum_n > 2) {
    rtb_Sum_a = -6;
  } else if (rtb_Sum_n < -3) {
    /* Switch: '<S4>/Switch1' incorporates:
     *  Constant: '<S4>/Constant2'
     */
    rtb_Sum_a = 6;
  } else {
    rtb_Sum_a = 0;
  }

  /* End of Switch: '<S4>/Switch' */

  /* Sum: '<S4>/Sum2' incorporates:
   *  Sum: '<S4>/Sum1'
   *  UnitDelay: '<S4>/Unit Delay1'
   */
  rtb_Sum2_i = (int16_T)((int8_T)(rtb_Sum_n + rtb_Sum_a) +
    power_BLDC_EGR_DW.UnitDelay1_DSTATE);

  /* Product: '<S4>/Divide' incorporates:
   *  Constant: '<S1>/EGR_BLDC_STEP'
   */
  power_BLDC_EGR_DW.z = rtb_Sum2_i << 12;
  rtb_wm = power_BLDC_EGR_P.EGR_BLDC_STEP == 0 ? power_BLDC_EGR_DW.z >= 0 ?
    MAX_int32_T : MIN_int32_T : power_BLDC_EGR_DW.z /
    power_BLDC_EGR_P.EGR_BLDC_STEP;

  /* Saturate: '<S4>/Saturation2' */
  if (rtb_wm >= 5120) {
    rtb_wm_0 = 5120;
  } else if (rtb_wm <= -1024) {
    rtb_wm_0 = -1024;
  } else {
    rtb_wm_0 = (int16_T)rtb_wm;
  }

  /* End of Saturate: '<S4>/Saturation2' */

  /* Sum: '<S4>/Sum3' incorporates:
   *  Constant: '<S4>/K_EGR_BLDC_POS_OFFSET'
   *  Product: '<S4>/Product'
   */
  power_BLDC_EGR_Y.postion_percent = (int16_T)((rtb_wm_0 * 25 >> 3) +
    power_BLDC_EGR_P.K_EGR_BLDC_POS_OFFSET);

  /* Saturate: '<S4>/Saturation' */
  if (power_BLDC_EGR_Y.postion_percent >= 16000) {
    /* Outport: '<Root>/postion_percent' */
    power_BLDC_EGR_Y.postion_percent = 16000;
  } else {
    if (power_BLDC_EGR_Y.postion_percent <= -3200) {
      /* Outport: '<Root>/postion_percent' */
      power_BLDC_EGR_Y.postion_percent = -3200;
    }
  }

  /* End of Saturate: '<S4>/Saturation' */

  /* DiscreteIntegrator: '<S7>/Discrete-Time Integrator' */
  rtb_wm = power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE;

  /* Outport: '<Root>/rotor_rpm' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
   *  Gain: '<S1>/rad2rpm'
   */
  power_BLDC_EGR_Y.rotor_rpm = (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE >>
    4) * 4889 >> 5;

  /* Product: '<S7>/Divide' incorporates:
   *  Constant: '<S7>/Rotor Poles Num (p)'
   *  Constant: '<S7>/theOffset'
   *  Sum: '<S7>/Sum1'
   */
  rtb_Gain_f = div_s32(rtb_DiscreteTimeIntegrator1 - power_BLDC_EGR_P.thOffset,
                       power_BLDC_EGR_P.p);

  /* MinMax: '<S21>/Min3' incorporates:
   *  Constant: '<S21>/High'
   */
  if (power_BLDC_EGR_P.motor_stopper_open_rad >= rtb_Gain_f) {
    rtb_Product_idx_0 = power_BLDC_EGR_P.motor_stopper_open_rad;
  } else {
    rtb_Product_idx_0 = rtb_Gain_f;
  }

  /* End of MinMax: '<S21>/Min3' */

  /* MinMax: '<S21>/Min1' incorporates:
   *  Constant: '<S21>/Low'
   */
  if (rtb_Gain_f <= power_BLDC_EGR_P.motor_stopper_init_rad) {
    power_BLDC_EGR_DW.z = rtb_Gain_f;
  } else {
    power_BLDC_EGR_DW.z = power_BLDC_EGR_P.motor_stopper_init_rad;
  }

  /* End of MinMax: '<S21>/Min1' */

  /* Sum: '<S21>/Sum5' incorporates:
   *  Constant: '<S21>/High'
   *  Constant: '<S21>/Low'
   */
  qY = rtb_Product_idx_0 - /*MW:OvSatOk*/
    power_BLDC_EGR_P.motor_stopper_open_rad;
  if ((rtb_Product_idx_0 < 0) && ((power_BLDC_EGR_P.motor_stopper_open_rad >= 0)
       && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((rtb_Product_idx_0 >= 0) && ((power_BLDC_EGR_P.motor_stopper_open_rad <
          0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  power_BLDC_EGR_DW.ytmp = qY + /*MW:OvSatOk*/ power_BLDC_EGR_DW.z;
  if ((qY < 0) && ((power_BLDC_EGR_DW.z < 0) && (power_BLDC_EGR_DW.ytmp >= 0)))
  {
    power_BLDC_EGR_DW.ytmp = MIN_int32_T;
  } else {
    if ((qY > 0) && ((power_BLDC_EGR_DW.z > 0) && (power_BLDC_EGR_DW.ytmp <= 0)))
    {
      power_BLDC_EGR_DW.ytmp = MAX_int32_T;
    }
  }

  qY = power_BLDC_EGR_DW.ytmp - /*MW:OvSatOk*/
    power_BLDC_EGR_P.motor_stopper_init_rad;
  if ((power_BLDC_EGR_DW.ytmp < 0) && ((power_BLDC_EGR_P.motor_stopper_init_rad >=
        0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((power_BLDC_EGR_DW.ytmp >= 0) &&
        ((power_BLDC_EGR_P.motor_stopper_init_rad < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Saturate: '<S3>/Saturation' */
  if (rtb_Gain_f >= power_BLDC_EGR_P.motor_stopper_open_rad) {
    rtb_Gain_f = power_BLDC_EGR_P.motor_stopper_open_rad;
  } else {
    if (rtb_Gain_f <= power_BLDC_EGR_P.valve_stopper_rad) {
      rtb_Gain_f = power_BLDC_EGR_P.valve_stopper_rad;
    }
  }

  /* End of Saturate: '<S3>/Saturation' */

  /* Sum: '<S3>/Add' incorporates:
   *  Constant: '<S3>/Relaxed Position'
   *  Constant: '<S3>/load pitch(mm)'
   *  Constant: '<S3>/spring_compensation'
   *  Gain: '<S21>/Gain2'
   *  Gain: '<S3>/Spring Coefficient (Ksp)'
   *  Gain: '<S3>/rad-to-deg and theta to length '
   *  Product: '<S3>/Product'
   *  Sum: '<S21>/Sum5'
   *  Sum: '<S3>/Sum2'
   */
  rtb_Add = (((int16_T)((int16_T)(mul_s32_s32_s32_sr10(163, rtb_Gain_f -
    power_BLDC_EGR_P.valve_stopper_rad) * power_BLDC_EGR_P.valve_pitch >> 10) *
                        power_BLDC_EGR_P.Ksp >> 10) << 3) +
             power_BLDC_EGR_P.SpringCom) + 800 * qY;

  /* Outport: '<Root>/TL' */
  power_BLDC_EGR_Y.TL = rtb_Add;

  /* Gain: '<S7>/Gain' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
   */
  rtb_Gain = power_BLDC_EGR_P.p *
    power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE;

  /* Outputs for Atomic SubSystem: '<S11>/rem' */
  power_BLDC_EGR_rem(rtb_DiscreteTimeIntegrator1, &power_BLDC_EGR_DW.rem_c);

  /* End of Outputs for SubSystem: '<S11>/rem' */

  /* Trigonometry: '<S11>/Trigonometric Function' */
  if (power_BLDC_EGR_DW.rem_c.DataTypeConversion1 > 1608) {
    if (power_BLDC_EGR_DW.rem_c.DataTypeConversion1 - 3217 <= 1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_c.DataTypeConversion1 - 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_c.DataTypeConversion1 - 6434;
      negate = FALSE;
    }
  } else if (power_BLDC_EGR_DW.rem_c.DataTypeConversion1 < -1608) {
    if (power_BLDC_EGR_DW.rem_c.DataTypeConversion1 + 3217 >= -1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_c.DataTypeConversion1 + 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_c.DataTypeConversion1 + 6434;
      negate = FALSE;
    }
  } else {
    power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_c.DataTypeConversion1;
    negate = FALSE;
  }

  power_BLDC_EGR_DW.z <<= 20;
  rtb_Product_idx_0 = 0;
  rtb_Product_idx = 652032978;
  power_BLDC_EGR_DW.xtmp = 652032978;
  power_BLDC_EGR_DW.ytmp = 0;
  for (rtb_wm_0 = 0; rtb_wm_0 < 11; rtb_wm_0++) {
    if (power_BLDC_EGR_DW.z < 0) {
      power_BLDC_EGR_DW.z += cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx += power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 -= power_BLDC_EGR_DW.xtmp;
    } else {
      power_BLDC_EGR_DW.z -= cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx -= power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 += power_BLDC_EGR_DW.xtmp;
    }

    power_BLDC_EGR_DW.xtmp = rtb_Product_idx >> (rtb_wm_0 + 1);
    power_BLDC_EGR_DW.ytmp = rtb_Product_idx_0 >> (rtb_wm_0 + 1);
  }

  if (negate) {
    /* Saturate: '<S11>/Saturation' */
    power_BLDC_EGR_DW.z = -rtb_Product_idx >> 20;
  } else {
    /* Saturate: '<S11>/Saturation' */
    power_BLDC_EGR_DW.z = rtb_Product_idx >> 20;
  }

  /* End of Trigonometry: '<S11>/Trigonometric Function' */

  /* Saturate: '<S11>/Saturation' */
  if (power_BLDC_EGR_DW.z >= 512) {
    power_BLDC_EGR_DW.z = 512;
  } else {
    if (power_BLDC_EGR_DW.z <= -512) {
      power_BLDC_EGR_DW.z = -512;
    }
  }

  /* Gain: '<S11>/Flux Constant (Flux)' */
  rtb_Vab = mul_s32_s32_s32_sr10(power_BLDC_EGR_P.Flux, power_BLDC_EGR_DW.z);

  /* Product: '<S11>/Product1' incorporates:
   *  DataTypeConversion: '<S11>/Data Type Conversion4'
   *  DataTypeConversion: '<S11>/Data Type Conversion5'
   */
  rtb_Product1 = (rtb_Vab >> 2) * (rtb_Gain >> 2) >> 6;

  /* Sum: '<S12>/Sum' */
  rtb_Sum = rtb_DiscreteTimeIntegrator1 - 2144;

  /* Outputs for Atomic SubSystem: '<S12>/rem' */
  power_BLDC_EGR_rem(rtb_Sum, &power_BLDC_EGR_DW.rem_d);

  /* End of Outputs for SubSystem: '<S12>/rem' */

  /* Trigonometry: '<S12>/Trigonometric Function' */
  if (power_BLDC_EGR_DW.rem_d.DataTypeConversion1 > 1608) {
    if (power_BLDC_EGR_DW.rem_d.DataTypeConversion1 - 3217 <= 1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_d.DataTypeConversion1 - 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_d.DataTypeConversion1 - 6434;
      negate = FALSE;
    }
  } else if (power_BLDC_EGR_DW.rem_d.DataTypeConversion1 < -1608) {
    if (power_BLDC_EGR_DW.rem_d.DataTypeConversion1 + 3217 >= -1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_d.DataTypeConversion1 + 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_d.DataTypeConversion1 + 6434;
      negate = FALSE;
    }
  } else {
    power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_d.DataTypeConversion1;
    negate = FALSE;
  }

  power_BLDC_EGR_DW.z <<= 20;
  rtb_Product_idx_0 = 0;
  rtb_Product_idx = 652032978;
  power_BLDC_EGR_DW.xtmp = 652032978;
  power_BLDC_EGR_DW.ytmp = 0;
  for (rtb_wm_0 = 0; rtb_wm_0 < 11; rtb_wm_0++) {
    if (power_BLDC_EGR_DW.z < 0) {
      power_BLDC_EGR_DW.z += cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx += power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 -= power_BLDC_EGR_DW.xtmp;
    } else {
      power_BLDC_EGR_DW.z -= cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx -= power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 += power_BLDC_EGR_DW.xtmp;
    }

    power_BLDC_EGR_DW.xtmp = rtb_Product_idx >> (rtb_wm_0 + 1);
    power_BLDC_EGR_DW.ytmp = rtb_Product_idx_0 >> (rtb_wm_0 + 1);
  }

  if (negate) {
    /* Saturate: '<S12>/Saturation' */
    power_BLDC_EGR_DW.z = -rtb_Product_idx >> 20;
  } else {
    /* Saturate: '<S12>/Saturation' */
    power_BLDC_EGR_DW.z = rtb_Product_idx >> 20;
  }

  /* End of Trigonometry: '<S12>/Trigonometric Function' */

  /* Saturate: '<S12>/Saturation' */
  if (power_BLDC_EGR_DW.z >= 512) {
    power_BLDC_EGR_DW.z = 512;
  } else {
    if (power_BLDC_EGR_DW.z <= -512) {
      power_BLDC_EGR_DW.z = -512;
    }
  }

  /* Gain: '<S12>/Flux Constant (Flux)' */
  rtb_Gain_f = mul_s32_s32_s32_sr10(power_BLDC_EGR_P.Flux, power_BLDC_EGR_DW.z);

  /* Product: '<S12>/Product1' incorporates:
   *  DataTypeConversion: '<S12>/Data Type Conversion3'
   *  DataTypeConversion: '<S12>/Data Type Conversion4'
   */
  rtb_Product1_p = (rtb_Gain_f >> 2) * (rtb_Gain >> 2) >> 6;

  /* Sum: '<S13>/Sum' */
  rtb_Sum_d = 2144 + rtb_DiscreteTimeIntegrator1;

  /* Outputs for Atomic SubSystem: '<S13>/rem' */
  power_BLDC_EGR_rem(rtb_Sum_d, &power_BLDC_EGR_DW.rem_m);

  /* End of Outputs for SubSystem: '<S13>/rem' */

  /* Trigonometry: '<S13>/Trigonometric Function' */
  if (power_BLDC_EGR_DW.rem_m.DataTypeConversion1 > 1608) {
    if (power_BLDC_EGR_DW.rem_m.DataTypeConversion1 - 3217 <= 1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_m.DataTypeConversion1 - 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_m.DataTypeConversion1 - 6434;
      negate = FALSE;
    }
  } else if (power_BLDC_EGR_DW.rem_m.DataTypeConversion1 < -1608) {
    if (power_BLDC_EGR_DW.rem_m.DataTypeConversion1 + 3217 >= -1608) {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_m.DataTypeConversion1 + 3217;
      negate = TRUE;
    } else {
      power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_m.DataTypeConversion1 + 6434;
      negate = FALSE;
    }
  } else {
    power_BLDC_EGR_DW.z = power_BLDC_EGR_DW.rem_m.DataTypeConversion1;
    negate = FALSE;
  }

  power_BLDC_EGR_DW.z <<= 20;
  rtb_Product_idx_0 = 0;
  rtb_Product_idx = 652032978;
  power_BLDC_EGR_DW.xtmp = 652032978;
  power_BLDC_EGR_DW.ytmp = 0;
  for (rtb_wm_0 = 0; rtb_wm_0 < 11; rtb_wm_0++) {
    if (power_BLDC_EGR_DW.z < 0) {
      power_BLDC_EGR_DW.z += cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx += power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 -= power_BLDC_EGR_DW.xtmp;
    } else {
      power_BLDC_EGR_DW.z -= cordicLUT_n11_s32En[rtb_wm_0];
      rtb_Product_idx -= power_BLDC_EGR_DW.ytmp;
      rtb_Product_idx_0 += power_BLDC_EGR_DW.xtmp;
    }

    power_BLDC_EGR_DW.xtmp = rtb_Product_idx >> (rtb_wm_0 + 1);
    power_BLDC_EGR_DW.ytmp = rtb_Product_idx_0 >> (rtb_wm_0 + 1);
  }

  if (negate) {
    /* Saturate: '<S13>/Saturation' */
    power_BLDC_EGR_DW.z = -rtb_Product_idx >> 20;
  } else {
    /* Saturate: '<S13>/Saturation' */
    power_BLDC_EGR_DW.z = rtb_Product_idx >> 20;
  }

  /* End of Trigonometry: '<S13>/Trigonometric Function' */

  /* Saturate: '<S13>/Saturation' */
  if (power_BLDC_EGR_DW.z >= 512) {
    power_BLDC_EGR_DW.z = 512;
  } else {
    if (power_BLDC_EGR_DW.z <= -512) {
      power_BLDC_EGR_DW.z = -512;
    }
  }

  /* Gain: '<S13>/Flux Constant (Flux)' */
  power_BLDC_EGR_DW.z = mul_s32_s32_s32_sr10(power_BLDC_EGR_P.Flux,
    power_BLDC_EGR_DW.z);

  /* Product: '<S13>/Product1' incorporates:
   *  DataTypeConversion: '<S13>/Data Type Conversion3'
   *  DataTypeConversion: '<S13>/Data Type Conversion4'
   */
  rtb_Product1_f = (power_BLDC_EGR_DW.z >> 2) * (rtb_Gain >> 2) >> 6;

  /* Sum: '<S10>/Sum1' incorporates:
   *  DiscreteIntegrator: '<S18>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S19>/Discrete-Time Integrator1'
   */
  qY = -power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_i;
  if ((power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_i < 0) && (qY < 0)) {
    qY = MAX_int32_T;
  }

  power_BLDC_EGR_DW.ytmp = qY - /*MW:OvSatOk*/
    power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d;
  if ((qY < 0) && ((power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d >= 0) &&
                   (power_BLDC_EGR_DW.ytmp >= 0))) {
    power_BLDC_EGR_DW.ytmp = MIN_int32_T;
  } else {
    if ((qY >= 0) && ((power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d < 0) &&
                      (power_BLDC_EGR_DW.ytmp < 0))) {
      power_BLDC_EGR_DW.ytmp = MAX_int32_T;
    }
  }

  /* Product: '<S5>/Product' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion'
   *  DataTypeConversion: '<S5>/Data Type Conversion1'
   *  DiscreteIntegrator: '<S18>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S19>/Discrete-Time Integrator1'
   *  Sum: '<S10>/Sum1'
   */
  rtb_Product_idx = (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_i >> 5) *
    (rtb_Vab >> 5);
  rtb_Product_idx_0 = (power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d >> 5) *
    (rtb_Gain_f >> 5);
  power_BLDC_EGR_DW.z = (power_BLDC_EGR_DW.ytmp >> 5) * (power_BLDC_EGR_DW.z >>
    5);

  /* Sum: '<S5>/Sum2' */
  qY = rtb_Product_idx + /*MW:OvSatOk*/ rtb_Product_idx_0;
  if ((rtb_Product_idx < 0) && ((rtb_Product_idx_0 < 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((rtb_Product_idx > 0) && ((rtb_Product_idx_0 > 0) && (qY <= 0))) {
      qY = MAX_int32_T;
    }
  }

  power_BLDC_EGR_DW.ytmp = qY + /*MW:OvSatOk*/ power_BLDC_EGR_DW.z;
  if ((qY < 0) && ((power_BLDC_EGR_DW.z < 0) && (power_BLDC_EGR_DW.ytmp >= 0)))
  {
    power_BLDC_EGR_DW.ytmp = MIN_int32_T;
  } else {
    if ((qY > 0) && ((power_BLDC_EGR_DW.z > 0) && (power_BLDC_EGR_DW.ytmp <= 0)))
    {
      power_BLDC_EGR_DW.ytmp = MAX_int32_T;
    }
  }

  /* Gain: '<S5>/Rotor Poles Num (p)' incorporates:
   *  Sum: '<S5>/Sum2'
   */
  power_BLDC_EGR_DW.z = power_BLDC_EGR_P.p * power_BLDC_EGR_DW.ytmp;

  /* Sum: '<S2>/Sum' incorporates:
   *  Inport: '<Root>/OM_EBM1'
   *  Inport: '<Root>/OM_EBM2'
   */
  rtb_Vab = power_BLDC_EGR_U.OM_EBM1 - power_BLDC_EGR_U.OM_EBM2;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Inport: '<Root>/OM_EBM2'
   *  Inport: '<Root>/OM_EBM3'
   */
  power_BLDC_EGR_DW.xtmp = power_BLDC_EGR_U.OM_EBM2 - power_BLDC_EGR_U.OM_EBM3;

  /* Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_IC_LOAD = 0U;
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE += mul_s32_s32_s32_sr42
    (439804651, rtb_Gain);

  /* Update for UnitDelay: '<S4>/Unit Delay' */
  power_BLDC_EGR_DW.UnitDelay_DSTATE = power_BLDC_EGR_Y.egr_bldc_phase_fnl;

  /* Update for UnitDelay: '<S4>/Unit Delay1' */
  power_BLDC_EGR_DW.UnitDelay1_DSTATE = rtb_Sum2_i;

  /* Sum: '<S7>/Sum' */
  qY = power_BLDC_EGR_DW.z - /*MW:OvSatOk*/ rtb_Add;
  if ((power_BLDC_EGR_DW.z < 0) && ((rtb_Add >= 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((power_BLDC_EGR_DW.z >= 0) && ((rtb_Add < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Signum: '<S20>/Sign' */
  if (rtb_wm < 0) {
    power_BLDC_EGR_DW.z = -1;
  } else if (rtb_wm > 0) {
    power_BLDC_EGR_DW.z = 1;
  } else {
    power_BLDC_EGR_DW.z = 0;
  }

  /* End of Signum: '<S20>/Sign' */

  /* Sum: '<S20>/Sum2' incorporates:
   *  Gain: '<S20>/Damping Constant (B)'
   *  Gain: '<S20>/Shaft static friction torque (Tf)'
   */
  rtb_Product_idx_0 = power_BLDC_EGR_P.Tf * power_BLDC_EGR_DW.z;
  power_BLDC_EGR_DW.z = mul_s32_s32_s32_sr10(power_BLDC_EGR_P.B, rtb_wm);
  power_BLDC_EGR_DW.ytmp = rtb_Product_idx_0 + /*MW:OvSatOk*/
    power_BLDC_EGR_DW.z;
  if ((rtb_Product_idx_0 < 0) && ((power_BLDC_EGR_DW.z < 0) &&
       (power_BLDC_EGR_DW.ytmp >= 0))) {
    power_BLDC_EGR_DW.ytmp = MIN_int32_T;
  } else {
    if ((rtb_Product_idx_0 > 0) && ((power_BLDC_EGR_DW.z > 0) &&
         (power_BLDC_EGR_DW.ytmp <= 0))) {
      power_BLDC_EGR_DW.ytmp = MAX_int32_T;
    }
  }

  /* Sum: '<S7>/Sum' incorporates:
   *  Sum: '<S20>/Sum2'
   */
  power_BLDC_EGR_DW.z = qY - /*MW:OvSatOk*/ power_BLDC_EGR_DW.ytmp;
  if ((qY < 0) && ((power_BLDC_EGR_DW.ytmp >= 0) && (power_BLDC_EGR_DW.z >= 0)))
  {
    power_BLDC_EGR_DW.z = MIN_int32_T;
  } else {
    if ((qY >= 0) && ((power_BLDC_EGR_DW.ytmp < 0) && (power_BLDC_EGR_DW.z < 0)))
    {
      power_BLDC_EGR_DW.z = MAX_int32_T;
    }
  }

  /* Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S7>/Inertia Constant's Recip (1//J)'
   *  Sum: '<S7>/Sum'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE += mul_s32_s32_s32_sr42
    (439804651, mul_s32_s32_s32_sr10(power_BLDC_EGR_P.J, power_BLDC_EGR_DW.z));

  /* Update for DiscreteIntegrator: '<S18>/Discrete-Time Integrator' incorporates:
   *  DiscreteIntegrator: '<S18>/Discrete-Time Integrator'
   *  Gain: '<S18>/Gain'
   *  Gain: '<S18>/Gain1'
   *  Gain: '<S18>/Gain3'
   *  Gain: '<S18>/Gain4'
   *  Gain: '<S18>/Inductance Constant's Recip (1//L)'
   *  Gain: '<S18>/Resistance Constant (R)'
   *  Sum: '<S18>/Add'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_i += mul_s32_s32_s32_sr42
    (439804651, mul_s32_s32_s32_sr10(power_BLDC_EGR_P.L, mul_s32_s32_s32_sr10
      (341, (((((rtb_Vab << 1) + power_BLDC_EGR_DW.xtmp) - (rtb_Product1 << 1))
              + rtb_Product1_p) + rtb_Product1_f) - 3 * mul_s32_s32_s32_sr10
       (power_BLDC_EGR_P.R, power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_i))));

  /* Update for DiscreteIntegrator: '<S19>/Discrete-Time Integrator1' incorporates:
   *  DiscreteIntegrator: '<S19>/Discrete-Time Integrator1'
   *  Gain: '<S19>/Gain'
   *  Gain: '<S19>/Gain1'
   *  Gain: '<S19>/Gain2'
   *  Gain: '<S19>/Inductance Constant's Recip (1//L)'
   *  Gain: '<S19>/Resistance Constant (R)'
   *  Sum: '<S19>/Add'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d += mul_s32_s32_s32_sr42
    (439804651, mul_s32_s32_s32_sr10(power_BLDC_EGR_P.L, mul_s32_s32_s32_sr10
      (341, ((((power_BLDC_EGR_DW.xtmp - rtb_Vab) + rtb_Product1) -
              (rtb_Product1_p << 1)) + rtb_Product1_f) - 3 *
       mul_s32_s32_s32_sr10(power_BLDC_EGR_P.R,
        power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_d))));
}

/* Model initialize function */
void power_BLDC_EGR_initialize(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_IC_LOAD = 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
