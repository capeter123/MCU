/*
 * File: power_BLDC_EGR.c
 *
 * Code generated for Simulink model 'power_BLDC_EGR'.
 *
 * Model version                  : 1.112
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Mar 24 12:27:13 2014
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

/* Forward declaration for local functions */
static int32_T rt_atan2_Us32En30_Ys32_D6NKW1_e(int32_T u0, int32_T u1);
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

int32_T mul_s32_s32_s32_sr39(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 7;
}

int32_T mul_s32_s32_s32_sr38(int32_T a, int32_T b)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  return (int32_T)u32_chi >> 6;
}

void MultiWordSignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[])
{
  int32_T n1m;
  int32_T i;
  uint32_T mask;
  uint32_T ys;
  n1m = n1 - 1;
  for (i = 0; i < n1m; i++) {
    y[i] = u1[i];
  }

  mask = 1U << (31U - n2);
  ys = (u1[n1m] & mask) != 0U ? MAX_uint32_T : 0U;
  mask = (mask << 1U) - 1U;
  y[n1m] = (u1[n1m] & mask) | (~mask & ys);
}

void sLong2MultiWord(int32_T u, uint32_T y[], int32_T n)
{
  uint32_T yi;
  int32_T i;
  y[0] = (uint32_T)u;
  yi = u < 0 ? MAX_uint32_T : 0U;
  for (i = 1; i < n; i++) {
    y[i] = yi;
  }
}

int32_T sMultiWord2sLongSat(const uint32_T u1[], int32_T n1)
{
  uint32_T y;
  sMultiWord2sMultiWordSat(u1, n1, &y, 1);
  return (int32_T)y;
}

void sMultiWord2sMultiWordSat(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n)
{
  int32_T nm;
  uint32_T ys;
  int32_T n1m1;
  boolean_T doSaturation = FALSE;
  nm = n - 1;
  n1m1 = n1 - 1;
  ys = (u1[n1m1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  if (n1 > n) {
    doSaturation = (((u1[n1m1] ^ u1[nm]) & 2147483648U) != 0U);
    while ((!doSaturation) && (n1m1 >= n)) {
      doSaturation = (u1[n1m1] != ys);
      n1m1--;
    }
  }

  if (doSaturation) {
    ys = ~ys;
    for (n1m1 = 0; n1m1 < nm; n1m1++) {
      y[n1m1] = ys;
    }

    y[n1m1] = ys ^ 2147483648U;
  } else {
    nm = n1 < n ? n1 : n;
    for (n1m1 = 0; n1m1 < nm; n1m1++) {
      y[n1m1] = u1[n1m1];
    }

    while (n1m1 < n) {
      y[n1m1] = ys;
      n1m1++;
    }
  }
}

void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T nb;
  int32_T i;
  uint32_T ys;
  uint32_T yi;
  uint32_T u1i;
  int32_T nc;
  uint32_T nr;
  uint32_T nl;
  int32_T i1;
  nb = (int32_T)(n2 >> 5);
  i = 0;
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - ((uint32_T)nb << 5);
    if (nr > (uint32_T)0) {
      nl = 32U - nr;
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = u1i << nl | yi;
        i++;
      }

      yi = u1i >> nr;
      u1i = nc < n1 ? u1[nc] : ys;
      y[i] = u1i << nl | yi;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void sMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T nb;
  int32_T nc;
  int32_T i;
  uint32_T ys;
  uint32_T u1i;
  uint32_T yi;
  uint32_T nr;
  uint32_T nl;
  nb = (int32_T)(n2 >> 5);
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  nc = nb > n ? n : nb;
  u1i = 0U;
  for (i = 0; i < nc; i++) {
    y[i] = 0U;
  }

  if (nb < n) {
    nl = n2 - ((uint32_T)nb << 5);
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      nr = 32U - nl;
      for (nc = 0; nc < nb; nc++) {
        yi = u1i >> nr;
        u1i = u1[nc];
        y[i] = u1i << nl | yi;
        i++;
      }

      if (i < n) {
        y[i] = u1i >> nr | ys << nl;
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void sMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[], int32_T
  n)
{
  uint32_T u1i;
  int32_T nm;
  int32_T i;
  nm = n1 < n ? n1 : n;
  for (i = 0; i < nm; i++) {
    y[i] = u1[i];
  }

  if (n > n1) {
    u1i = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
    for (i = nm; i < n; i++) {
      y[i] = u1i;
    }
  }
}

void MultiWordSub(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
                  int32_T n)
{
  uint32_T yi;
  uint32_T u1i;
  uint32_T borrow = 0U;
  int32_T i;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i - u2[i]) - borrow;
    y[i] = yi;
    borrow = borrow != 0U ? yi >= u1i ? 1U : 0U : yi > u1i ? 1U : 0U;
  }
}

boolean_T sMultiWordLt(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  return sMultiWordCmp(u1, u2, n) < 0 ? (int32_T)TRUE : (int32_T)FALSE;
}

int32_T sMultiWordCmp(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  uint32_T su;
  uint32_T su_0;
  int32_T i;
  int32_T cmp;
  su = u1[n - 1] & 2147483648U;
  su_0 = u2[n - 1] & 2147483648U;
  if ((su ^ su_0) != 0U) {
    cmp = su != 0U ? -1 : 1;
  } else {
    cmp = 0;
    i = n;
    while ((cmp == 0) && (i > 0)) {
      i--;
      su = u1[i];
      su_0 = u2[i];
      if (su != su_0) {
        cmp = su > su_0 ? 1 : -1;
      }
    }
  }

  return cmp;
}

void MultiWordAdd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
                  int32_T n)
{
  uint32_T yi;
  uint32_T u1i;
  uint32_T carry = 0U;
  int32_T i;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i + u2[i]) + carry;
    y[i] = yi;
    carry = carry != 0U ? yi <= u1i ? 1U : 0U : yi < u1i ? 1U : 0U;
  }
}

static int32_T rt_atan2_Us32En30_Ys32_D6NKW1_e(int32_T u0, int32_T u1)
{
  int32_T y;
  boolean_T y_quad_adjust;
  boolean_T y_nonzero;
  boolean_T x_quad_adjust;
  int16_T idx;
  static const int32_T cordicLUT_n11_s32En[11] = { 421657428, 248918915,
    131521918, 66762579, 33510843, 16771758, 8387925, 4194219, 2097141, 1048575,
    524288 };

  int32_T u;
  static int64m_T tmp = { { 0U, 0U } };

  static int64m_T tmp_0 = { { 1686629713U, 0U } };

  if (u0 < 0) {
    if (u0 <= MIN_int32_T) {
      u = MAX_int32_T;
    } else {
      u = -u0;
    }

    sLong2MultiWord(u, &power_BLDC_EGR_DW.r2.chunks[0U], 2);
    MultiWordSignedWrap(&power_BLDC_EGR_DW.r2.chunks[0U], 2, 30U,
                        &power_BLDC_EGR_DW.yAcc.chunks[0U]);
    y_quad_adjust = TRUE;
    y_nonzero = TRUE;
  } else {
    sLong2MultiWord(u0, &power_BLDC_EGR_DW.r2.chunks[0U], 2);
    MultiWordSignedWrap(&power_BLDC_EGR_DW.r2.chunks[0U], 2, 30U,
                        &power_BLDC_EGR_DW.yAcc.chunks[0U]);
    y_quad_adjust = FALSE;
    y_nonzero = (u0 > 0);
  }

  if (u1 < 0) {
    if (u1 <= MIN_int32_T) {
      u = MAX_int32_T;
    } else {
      u = -u1;
    }

    sLong2MultiWord(u, &power_BLDC_EGR_DW.r2.chunks[0U], 2);
    MultiWordSignedWrap(&power_BLDC_EGR_DW.r2.chunks[0U], 2, 30U,
                        &power_BLDC_EGR_DW.xAcc.chunks[0U]);
    x_quad_adjust = TRUE;
  } else {
    sLong2MultiWord(u1, &power_BLDC_EGR_DW.r2.chunks[0U], 2);
    MultiWordSignedWrap(&power_BLDC_EGR_DW.r2.chunks[0U], 2, 30U,
                        &power_BLDC_EGR_DW.xAcc.chunks[0U]);
    x_quad_adjust = FALSE;
  }

  y = 0;
  power_BLDC_EGR_DW.xtmp = power_BLDC_EGR_DW.xAcc;
  power_BLDC_EGR_DW.ytmp = power_BLDC_EGR_DW.yAcc;
  for (idx = 0; idx < 11; idx++) {
    power_BLDC_EGR_DW.r4 = tmp;
    if (sMultiWordLt(&power_BLDC_EGR_DW.yAcc.chunks[0U],
                     &power_BLDC_EGR_DW.r4.chunks[0U], 2)) {
      MultiWordSub(&power_BLDC_EGR_DW.xAcc.chunks[0U],
                   &power_BLDC_EGR_DW.ytmp.chunks[0U],
                   &power_BLDC_EGR_DW.r5.chunks[0U], 2);
      MultiWordSignedWrap(&power_BLDC_EGR_DW.r5.chunks[0U], 2, 30U,
                          &power_BLDC_EGR_DW.xAcc.chunks[0U]);
      MultiWordAdd(&power_BLDC_EGR_DW.yAcc.chunks[0U],
                   &power_BLDC_EGR_DW.xtmp.chunks[0U],
                   &power_BLDC_EGR_DW.r5.chunks[0U], 2);
      MultiWordSignedWrap(&power_BLDC_EGR_DW.r5.chunks[0U], 2, 30U,
                          &power_BLDC_EGR_DW.yAcc.chunks[0U]);
      y -= cordicLUT_n11_s32En[idx];
    } else {
      MultiWordAdd(&power_BLDC_EGR_DW.xAcc.chunks[0U],
                   &power_BLDC_EGR_DW.ytmp.chunks[0U],
                   &power_BLDC_EGR_DW.r5.chunks[0U], 2);
      MultiWordSignedWrap(&power_BLDC_EGR_DW.r5.chunks[0U], 2, 30U,
                          &power_BLDC_EGR_DW.xAcc.chunks[0U]);
      MultiWordSub(&power_BLDC_EGR_DW.yAcc.chunks[0U],
                   &power_BLDC_EGR_DW.xtmp.chunks[0U],
                   &power_BLDC_EGR_DW.r5.chunks[0U], 2);
      MultiWordSignedWrap(&power_BLDC_EGR_DW.r5.chunks[0U], 2, 30U,
                          &power_BLDC_EGR_DW.yAcc.chunks[0U]);
      y += cordicLUT_n11_s32En[idx];
    }

    sMultiWordShr(&power_BLDC_EGR_DW.xAcc.chunks[0U], 2, (uint32_T)(int16_T)(idx
      + 1), &power_BLDC_EGR_DW.xtmp.chunks[0U], 2);
    sMultiWordShr(&power_BLDC_EGR_DW.yAcc.chunks[0U], 2, (uint32_T)(int16_T)(idx
      + 1), &power_BLDC_EGR_DW.ytmp.chunks[0U], 2);
  }

  if (y_nonzero) {
    if (x_quad_adjust) {
      if (y_quad_adjust) {
        sLong2MultiWord(y, &power_BLDC_EGR_DW.r7.chunks[0U], 2);
        MultiWordSignedWrap(&power_BLDC_EGR_DW.r7.chunks[0U], 2, 31U,
                            &power_BLDC_EGR_DW.r6.chunks[0U]);
        power_BLDC_EGR_DW.r7 = tmp_0;
        MultiWordSub(&power_BLDC_EGR_DW.r6.chunks[0U],
                     &power_BLDC_EGR_DW.r7.chunks[0U],
                     &power_BLDC_EGR_DW.r5.chunks[0U], 2);
        sMultiWord2MultiWord(&power_BLDC_EGR_DW.r5.chunks[0U], 2,
                             &power_BLDC_EGR_DW.r1.chunks[0U], 3);
        sMultiWordShl(&power_BLDC_EGR_DW.r1.chunks[0U], 3, 31U,
                      &power_BLDC_EGR_DW.r0.chunks[0U], 3);
        sMultiWord2sMultiWordSat(&power_BLDC_EGR_DW.r0.chunks[0U], 3,
          &power_BLDC_EGR_DW.r4.chunks[0U], 2);
        sMultiWordShr(&power_BLDC_EGR_DW.r4.chunks[0U], 2, 31U,
                      &power_BLDC_EGR_DW.r3.chunks[0U], 2);
        y = sMultiWord2sLongSat(&power_BLDC_EGR_DW.r3.chunks[0U], 2);
      } else {
        power_BLDC_EGR_DW.r5 = tmp_0;
        sLong2MultiWord(y, &power_BLDC_EGR_DW.r7.chunks[0U], 2);
        MultiWordSignedWrap(&power_BLDC_EGR_DW.r7.chunks[0U], 2, 31U,
                            &power_BLDC_EGR_DW.r6.chunks[0U]);
        MultiWordSub(&power_BLDC_EGR_DW.r5.chunks[0U],
                     &power_BLDC_EGR_DW.r6.chunks[0U],
                     &power_BLDC_EGR_DW.r4.chunks[0U], 2);
        sMultiWord2MultiWord(&power_BLDC_EGR_DW.r4.chunks[0U], 2,
                             &power_BLDC_EGR_DW.r1.chunks[0U], 3);
        sMultiWordShl(&power_BLDC_EGR_DW.r1.chunks[0U], 3, 31U,
                      &power_BLDC_EGR_DW.r0.chunks[0U], 3);
        sMultiWord2sMultiWordSat(&power_BLDC_EGR_DW.r0.chunks[0U], 3,
          &power_BLDC_EGR_DW.r3.chunks[0U], 2);
        sMultiWordShr(&power_BLDC_EGR_DW.r3.chunks[0U], 2, 31U,
                      &power_BLDC_EGR_DW.r2.chunks[0U], 2);
        y = sMultiWord2sLongSat(&power_BLDC_EGR_DW.r2.chunks[0U], 2);
      }
    } else {
      if (y_quad_adjust) {
        if (y <= MIN_int32_T) {
          y = MAX_int32_T;
        } else {
          y = -y;
        }
      }
    }
  } else if (x_quad_adjust) {
    y = 1686629713;
  } else {
    y = 0;
  }

  return y;
}

/* Model step function */
void power_BLDC_EGR_step(void)
{
  boolean_T negate;
  int32_T xtmp;
  int32_T ytmp;
  int32_T z;
  int16_T idx;
  int8_T rtb_Sum_n;
  int32_T rtb_Product1_k;
  int32_T rtb_Saturation;
  int32_T rtb_Product1_f;
  int32_T rtb_Product1;
  int32_T rtb_Gain4;
  static const int32_T cordicLUT_n11_s32En[11] = { 843314857, 497837829,
    263043837, 133525159, 67021687, 33543516, 16775851, 8388437, 4194283,
    2097149, 1048576 };

  int32_T rtb_Product_idx;
  int32_T qY;
  int32_T qY_0;
  int8_T rtb_Sum_d;

  /* DataTypeConversion: '<S14>/Data Type Conversion' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
   */
//  power_BLDC_EGR_DW.DataTypeConversion = (real_T)
//    power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE * 0.0009765625;

  /* S-Function (remf): '<S14>/S-Function Builder' */
  remf_Outputs_wrapper(&power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE,
                       &power_BLDC_EGR_ConstP.pooled1,
                       &power_BLDC_EGR_DW.SFunctionBuilder );

  /* DataTypeConversion: '<S14>/Data Type Conversion1' */
  rtb_Saturation = (int32_T)(power_BLDC_EGR_DW.SFunctionBuilder <<20);

  /* Trigonometry: '<S14>/Trigonometric Function3' */
  if (rtb_Saturation > 1647099) {
    if (rtb_Saturation - 3294199 <= 1647099) {
      rtb_Saturation -= 3294199;
      negate = TRUE;
    } else {
      rtb_Saturation -= 6588397;
      negate = FALSE;
    }
  } else if (rtb_Saturation < -1647099) {
    if (rtb_Saturation + 3294199 >= -1647099) {
      rtb_Saturation += 3294199;
      negate = TRUE;
    } else {
      rtb_Saturation += 6588397;
      negate = FALSE;
    }
  } else {
    negate = FALSE;
  }

  z = rtb_Saturation << 10;
  rtb_Product1_f = 0;
  rtb_Product_idx = 652032978;
  xtmp = 652032978;
  ytmp = 0;
  for (idx = 0; idx < 11; idx++) {
    if (z < 0) {
      z += cordicLUT_n11_s32En[idx];
      rtb_Product_idx += ytmp;
      rtb_Product1_f -= xtmp;
    } else {
      z -= cordicLUT_n11_s32En[idx];
      rtb_Product_idx -= ytmp;
      rtb_Product1_f += xtmp;
    }

    xtmp = rtb_Product_idx >> (idx + 1);
    ytmp = rtb_Product1_f >> (idx + 1);
  }

  if (negate) {
    rtb_Product_idx = -rtb_Product_idx;
    rtb_Product1_f = -rtb_Product1_f;
  }

  /* Gain: '<S14>/rad2deg' incorporates:
   *  DataTypeConversion: '<S14>/Data Type Conversion3'
   *  Trigonometry: '<S14>/Trigonometric Function2'
   *  Trigonometry: '<S14>/Trigonometric Function3'
   */
  rtb_Saturation = (rt_atan2_Us32En30_Ys32_D6NKW1_e(rtb_Product1_f,
    rtb_Product_idx) >> 19) * 58671 >> 10;

  /* Outport: '<Root>/Hallx' incorporates:
   *  Logic: '<S9>/Logical Operator'
   *  Logic: '<S9>/Logical Operator1'
   *  Logic: '<S9>/Logical Operator2'
   *  RelationalOperator: '<S9>/Relational Operator1'
   *  RelationalOperator: '<S9>/Relational Operator2'
   *  RelationalOperator: '<S9>/Relational Operator3'
   *  RelationalOperator: '<S9>/Relational Operator4'
   *  RelationalOperator: '<S9>/Relational Operator5'
   *  RelationalOperator: '<S9>/Relational Operator6'
   *  SignalConversion: '<S4>/TmpSignal ConversionAtS-Function HallDecoderInport1'
   */
  power_BLDC_EGR_Y.Hallx[0] = ((rtb_Saturation >= -61440) && (rtb_Saturation <=
    122880));
  power_BLDC_EGR_Y.Hallx[1] = ((rtb_Saturation >= 61440) || (rtb_Saturation <=
    -122880));
  power_BLDC_EGR_Y.Hallx[2] = ((rtb_Saturation >= -184320) && (rtb_Saturation <=
    0));

  /* Outport: '<Root>/rotor_rpm' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
   *  Gain: '<S1>/rad2rpm'
   */
  power_BLDC_EGR_Y.rotor_rpm = (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE >>
    3) * 4889 >> 6;

  /* Gain: '<S7>/Gain1' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
   */
  rtb_Saturation = power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE >> 2;

  /* Outport: '<Root>/rotor_theta' */
  power_BLDC_EGR_Y.rotor_theta = rtb_Saturation;

  /* DataTypeConversion: '<S11>/Data Type Conversion' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
   */
//  power_BLDC_EGR_DW.DataTypeConversion_m = (real_T)
//    power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE * 0.0009765625;

  /* S-Function (remf): '<S11>/S-Function Builder' */
  remf_Outputs_wrapper(&power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE,
                       &power_BLDC_EGR_ConstP.pooled1,
                       &power_BLDC_EGR_DW.SFunctionBuilder_b );

  /* DataTypeConversion: '<S11>/Data Type Conversion1' */
  rtb_Product1_f = (int32_T)(power_BLDC_EGR_DW.SFunctionBuilder_b << 20);

  /* Trigonometry: '<S11>/Trigonometric Function' */
  if (rtb_Product1_f > 1647099) {
    if (rtb_Product1_f - 3294199 <= 1647099) {
      rtb_Product1_f -= 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f -= 6588397;
      negate = FALSE;
    }
  } else if (rtb_Product1_f < -1647099) {
    if (rtb_Product1_f + 3294199 >= -1647099) {
      rtb_Product1_f += 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f += 6588397;
      negate = FALSE;
    }
  } else {
    negate = FALSE;
  }

  z = rtb_Product1_f << 10;
  rtb_Product1_f = 0;
  rtb_Product_idx = 652032978;
  xtmp = 652032978;
  ytmp = 0;
  for (idx = 0; idx < 11; idx++) {
    if (z < 0) {
      z += cordicLUT_n11_s32En[idx];
      rtb_Product_idx += ytmp;
      rtb_Product1_f -= xtmp;
    } else {
      z -= cordicLUT_n11_s32En[idx];
      rtb_Product_idx -= ytmp;
      rtb_Product1_f += xtmp;
    }

    xtmp = rtb_Product_idx >> (idx + 1);
    ytmp = rtb_Product1_f >> (idx + 1);
  }

  if (negate) {
    /* Saturate: '<S11>/Saturation' */
    z = -rtb_Product_idx >> 15;
  } else {
    /* Saturate: '<S11>/Saturation' */
    z = rtb_Product_idx >> 15;
  }

  /* End of Trigonometry: '<S11>/Trigonometric Function' */

  /* Saturate: '<S11>/Saturation' */
  if (z >= 16384) {
    z = 16384;
  } else {
    if (z <= -16384) {
      z = -16384;
    }
  }

  /* Gain: '<S11>/Gain4' */
  rtb_Product1 = 11469 * z >> 20;

  /* DataTypeConversion: '<S12>/Data Type Conversion' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
   *  Sum: '<S12>/Sum'
   */
  power_BLDC_EGR_DW.DataTypeConversion_d = (int32_T)
    (power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE - 2144);

  /* S-Function (remf): '<S12>/S-Function Builder' */
  remf_Outputs_wrapper(&power_BLDC_EGR_DW.DataTypeConversion_d,
                       &power_BLDC_EGR_ConstP.pooled1,
                       &power_BLDC_EGR_DW.SFunctionBuilder_n );

  /* DataTypeConversion: '<S12>/Data Type Conversion1' */
  rtb_Product1_f = (int32_T)(power_BLDC_EGR_DW.SFunctionBuilder_n <<20);

  /* Trigonometry: '<S12>/Trigonometric Function' */
  if (rtb_Product1_f > 1647099) {
    if (rtb_Product1_f - 3294199 <= 1647099) {
      rtb_Product1_f -= 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f -= 6588397;
      negate = FALSE;
    }
  } else if (rtb_Product1_f < -1647099) {
    if (rtb_Product1_f + 3294199 >= -1647099) {
      rtb_Product1_f += 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f += 6588397;
      negate = FALSE;
    }
  } else {
    negate = FALSE;
  }

  z = rtb_Product1_f << 10;
  rtb_Product1_f = 0;
  rtb_Product_idx = 652032978;
  xtmp = 652032978;
  ytmp = 0;
  for (idx = 0; idx < 11; idx++) {
    if (z < 0) {
      z += cordicLUT_n11_s32En[idx];
      rtb_Product_idx += ytmp;
      rtb_Product1_f -= xtmp;
    } else {
      z -= cordicLUT_n11_s32En[idx];
      rtb_Product_idx -= ytmp;
      rtb_Product1_f += xtmp;
    }

    xtmp = rtb_Product_idx >> (idx + 1);
    ytmp = rtb_Product1_f >> (idx + 1);
  }

  if (negate) {
    /* Saturate: '<S12>/Saturation' */
    z = -rtb_Product_idx >> 15;
  } else {
    /* Saturate: '<S12>/Saturation' */
    z = rtb_Product_idx >> 15;
  }

  /* End of Trigonometry: '<S12>/Trigonometric Function' */

  /* Saturate: '<S12>/Saturation' */
  if (z >= 16384) {
    z = 16384;
  } else {
    if (z <= -16384) {
      z = -16384;
    }
  }

  /* Gain: '<S12>/Gain4' */
  rtb_Gain4 = 11469 * z >> 20;

  /* DataTypeConversion: '<S13>/Data Type Conversion' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
   *  Sum: '<S13>/Sum'
   */
  power_BLDC_EGR_DW.DataTypeConversion_a = (int32_T)(2144 +
    power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE);

  /* S-Function (remf): '<S13>/S-Function Builder' */
  remf_Outputs_wrapper(&power_BLDC_EGR_DW.DataTypeConversion_a,
                       &power_BLDC_EGR_ConstP.pooled1,
                       &power_BLDC_EGR_DW.SFunctionBuilder_e );

  /* DataTypeConversion: '<S13>/Data Type Conversion1' */
  rtb_Product1_f = (int32_T)(power_BLDC_EGR_DW.SFunctionBuilder_e << 20);

  /* Trigonometry: '<S13>/Trigonometric Function1' */
  if (rtb_Product1_f > 1647099) {
    if (rtb_Product1_f - 3294199 <= 1647099) {
      rtb_Product1_f -= 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f -= 6588397;
      negate = FALSE;
    }
  } else if (rtb_Product1_f < -1647099) {
    if (rtb_Product1_f + 3294199 >= -1647099) {
      rtb_Product1_f += 3294199;
      negate = TRUE;
    } else {
      rtb_Product1_f += 6588397;
      negate = FALSE;
    }
  } else {
    negate = FALSE;
  }

  z = rtb_Product1_f << 10;
  rtb_Product1_f = 0;
  rtb_Product_idx = 652032978;
  xtmp = 652032978;
  ytmp = 0;
  for (idx = 0; idx < 11; idx++) {
    if (z < 0) {
      z += cordicLUT_n11_s32En[idx];
      rtb_Product_idx += ytmp;
      rtb_Product1_f -= xtmp;
    } else {
      z -= cordicLUT_n11_s32En[idx];
      rtb_Product_idx -= ytmp;
      rtb_Product1_f += xtmp;
    }

    xtmp = rtb_Product_idx >> (idx + 1);
    ytmp = rtb_Product1_f >> (idx + 1);
  }

  if (negate) {
    /* Saturate: '<S13>/Saturation' */
    z = -rtb_Product_idx >> 15;
  } else {
    /* Saturate: '<S13>/Saturation' */
    z = rtb_Product_idx >> 15;
  }

  /* End of Trigonometry: '<S13>/Trigonometric Function1' */

  /* Saturate: '<S13>/Saturation' */
  if (z >= 16384) {
    z = 16384;
  } else {
    if (z <= -16384) {
      z = -16384;
    }
  }

  /* Gain: '<S13>/Gain4' */
  rtb_Product1_k = 11469 * z >> 20;

  /* Product: '<S5>/Product' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion'
   *  DataTypeConversion: '<S5>/Data Type Conversion1'
   *  DiscreteIntegrator: '<S15>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S16>/Discrete-Time Integrator1'
   */
  z = (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_p >> 5) * (rtb_Product1 >>
    5);
  rtb_Product1_f = (power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o >> 5) *
    (rtb_Gain4 >> 5);

  /* Sum: '<S10>/Sum1' incorporates:
   *  DiscreteIntegrator: '<S15>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S16>/Discrete-Time Integrator1'
   */
  qY_0 = -power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_p;
  if ((power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_p < 0) && (qY_0 < 0)) {
    qY_0 = MAX_int32_T;
  }

  qY = qY_0 - /*MW:OvSatOk*/ power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o;
  if ((qY_0 < 0) && ((power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o >= 0) &&
                     (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((qY_0 >= 0) && ((power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o < 0) &&
                        (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Product: '<S5>/Product' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion'
   *  DataTypeConversion: '<S5>/Data Type Conversion1'
   *  Sum: '<S10>/Sum1'
   */
  rtb_Product_idx = (qY >> 5) * (rtb_Product1_k >> 5);

  /* Sum: '<S5>/Sum2' */
  qY_0 = z + /*MW:OvSatOk*/ rtb_Product1_f;
  if ((z < 0) && ((rtb_Product1_f < 0) && (qY_0 >= 0))) {
    qY_0 = MIN_int32_T;
  } else {
    if ((z > 0) && ((rtb_Product1_f > 0) && (qY_0 <= 0))) {
      qY_0 = MAX_int32_T;
    }
  }

  qY = qY_0 + /*MW:OvSatOk*/ rtb_Product_idx;
  if ((qY_0 < 0) && ((rtb_Product_idx < 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((qY_0 > 0) && ((rtb_Product_idx > 0) && (qY <= 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Gain: '<S5>/Gain3' incorporates:
   *  Sum: '<S5>/Sum2'
   */
  if (qY > 536870911) {
    power_BLDC_EGR_Y.Te = MAX_int32_T;
  } else if (qY <= -536870912) {
    power_BLDC_EGR_Y.Te = MIN_int32_T;
  } else {
    power_BLDC_EGR_Y.Te = qY << 2;
  }

  /* S-Function (HallDecoder): '<S4>/S-Function HallDecoder' */
  HallDecoder_Outputs_wrapper(&power_BLDC_EGR_Y.Hallx[0],
    &power_BLDC_EGR_Y.egr_bldc_phase_fnl );

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
    rtb_Sum_d = -6;
  } else if (rtb_Sum_n < -3) {
    /* Switch: '<S4>/Switch1' incorporates:
     *  Constant: '<S4>/Constant2'
     */
    rtb_Sum_d = 6;
  } else {
    rtb_Sum_d = 0;
  }

  /* End of Switch: '<S4>/Switch' */

  /* Sum: '<S4>/Sum1' */
  power_BLDC_EGR_Y.egr_bldc_rotor_dir = (int8_T)(rtb_Sum_n + rtb_Sum_d);

  /* Sum: '<S4>/Sum2' incorporates:
   *  UnitDelay: '<S4>/Unit Delay1'
   */
  power_BLDC_EGR_DW.UnitDelay1_DSTATE += power_BLDC_EGR_Y.egr_bldc_rotor_dir;

  /* Product: '<S4>/Divide' */
  z = power_BLDC_EGR_DW.UnitDelay1_DSTATE << 12;

  /* Sum: '<S4>/Sum3' incorporates:
   *  Constant: '<S1>/EGR_BLDC_STEP'
   *  Constant: '<S4>/egr_bldc_initpos'
   *  Product: '<S4>/Divide'
   *  Product: '<S4>/Product'
   */
  power_BLDC_EGR_Y.postion_percent = (int16_T)((int16_T)
    ((power_BLDC_EGR_P.EGR_BLDC_STEP == 0 ? z >= 0 ? MAX_int32_T : MIN_int32_T :
      z / power_BLDC_EGR_P.EGR_BLDC_STEP) * 25 >> 3) +
    power_BLDC_EGR_P.egr_bldc_initpos);

  /* Saturate: '<S4>/Saturation' */
  if (power_BLDC_EGR_Y.postion_percent >= power_BLDC_EGR_P.egr_pos_upper_limit)
  {
    /* Outport: '<Root>/postion_percent' */
    power_BLDC_EGR_Y.postion_percent = power_BLDC_EGR_P.egr_pos_upper_limit;
  } else {
    if (power_BLDC_EGR_Y.postion_percent <= power_BLDC_EGR_P.egr_pos_lower_limit)
    {
      /* Outport: '<Root>/postion_percent' */
      power_BLDC_EGR_Y.postion_percent = power_BLDC_EGR_P.egr_pos_lower_limit;
    }
  }

  /* End of Saturate: '<S4>/Saturation' */

  /* MinMax: '<S4>/MinMax' */
  if (power_BLDC_EGR_DW.UnitDelay1_DSTATE >= 0) {
    z = power_BLDC_EGR_DW.UnitDelay1_DSTATE;
  } else {
    z = 0;
  }

  /* Saturate: '<S4>/Saturation1' incorporates:
   *  MinMax: '<S4>/MinMax'
   */
  if (z >= 120) {
    /* Outport: '<Root>/actual_position' */
    power_BLDC_EGR_Y.actual_position = 120;
  } else {
    /* Outport: '<Root>/actual_position' */
    power_BLDC_EGR_Y.actual_position = (int8_T)z;
  }

  /* End of Saturate: '<S4>/Saturation1' */

  /* Gain: '<S7>/Gain' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
   */
  if (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE > 536870911) {
    z = MAX_int32_T;
  } else if (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE <= -536870912) {
    z = MIN_int32_T;
  } else {
    z = power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE << 2;
  }

  /* Product: '<S11>/Product1' incorporates:
   *  DataTypeConversion: '<S11>/Data Type Conversion4'
   *  DataTypeConversion: '<S11>/Data Type Conversion5'
   *  Gain: '<S7>/Gain'
   */
  rtb_Product1 = (rtb_Product1 >> 2) * (z >> 2) >> 6;

  /* Product: '<S12>/Product1' incorporates:
   *  DataTypeConversion: '<S12>/Data Type Conversion3'
   *  DataTypeConversion: '<S12>/Data Type Conversion4'
   *  Gain: '<S7>/Gain'
   */
  rtb_Product1_f = (rtb_Gain4 >> 2) * (z >> 2) >> 6;

  /* Product: '<S13>/Product1' incorporates:
   *  DataTypeConversion: '<S13>/Data Type Conversion3'
   *  DataTypeConversion: '<S13>/Data Type Conversion4'
   *  Gain: '<S7>/Gain'
   */
  rtb_Product1_k = (rtb_Product1_k >> 2) * (z >> 2) >> 6;

  /* Sum: '<S2>/Sum' incorporates:
   *  Inport: '<Root>/OM_EBM1'
   *  Inport: '<Root>/OM_EBM2'
   */
  xtmp = power_BLDC_EGR_U.OM_EBM1 - power_BLDC_EGR_U.OM_EBM2;

  /* Sum: '<S2>/Sum1' incorporates:
   *  Inport: '<Root>/OM_EBM2'
   *  Inport: '<Root>/OM_EBM3'
   */
  ytmp = power_BLDC_EGR_U.OM_EBM2 - power_BLDC_EGR_U.OM_EBM3;

  /* Saturate: '<S3>/Saturation' */
  if (rtb_Saturation >= power_BLDC_EGR_P.spring_maxpos) {
    rtb_Saturation = power_BLDC_EGR_P.spring_maxpos;
  } else {
    if (rtb_Saturation <= power_BLDC_EGR_P.spring_initpos) {
      rtb_Saturation = power_BLDC_EGR_P.spring_initpos;
    }
  }

  /* End of Saturate: '<S3>/Saturation' */

  /* Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' incorporates:
   *  Gain: '<S7>/Gain'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTATE += mul_s32_s32_s32_sr42
    (439804651, z);

  /* Sum: '<S7>/Sum' incorporates:
   *  Constant: '<S3>/spring_compensation'
   *  Gain: '<S3>/Ksp'
   *  Gain: '<S5>/Gain3'
   *  Sum: '<S3>/Sum1'
   */
  z = ((int16_T)(power_BLDC_EGR_P.Ksp * rtb_Saturation >> 13) << 3) +
    power_BLDC_EGR_P.spring_compensation;
  qY_0 = power_BLDC_EGR_Y.Te - /*MW:OvSatOk*/ z;
  if ((power_BLDC_EGR_Y.Te < 0) && ((z >= 0) && (qY_0 >= 0))) {
    qY_0 = MIN_int32_T;
  } else {
    if ((power_BLDC_EGR_Y.Te >= 0) && ((z < 0) && (qY_0 < 0))) {
      qY_0 = MAX_int32_T;
    }
  }

  /* Sum: '<S17>/Sum' incorporates:
   *  DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
   *  Gain: '<S17>/Gain'
   */
  z = mul_s32_s32_s32_sr38(274877907,
    power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE);

  /* Sum: '<S7>/Sum' incorporates:
   *  Sum: '<S17>/Sum'
   */
  qY = qY_0 - /*MW:OvSatOk*/ z;
  if ((qY_0 < 0) && ((z >= 0) && (qY >= 0))) {
    qY = MIN_int32_T;
  } else {
    if ((qY_0 >= 0) && ((z < 0) && (qY < 0))) {
      qY = MAX_int32_T;
    }
  }

  /* Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator' incorporates:
   *  DataTypeConversion: '<S7>/Data Type Conversion'
   *  Gain: '<S7>/Gain2'
   *  Sum: '<S7>/Sum'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE += mul_s32_s32_s32_sr39
    (439804651, (qY >> 3) * 1250);

  /* Update for DiscreteIntegrator: '<S15>/Discrete-Time Integrator' incorporates:
   *  DataTypeConversion: '<S15>/Data Type Conversion'
   *  DiscreteIntegrator: '<S15>/Discrete-Time Integrator'
   *  Gain: '<S15>/Gain'
   *  Gain: '<S15>/Gain1'
   *  Gain: '<S15>/Gain2'
   *  Gain: '<S15>/Gain3'
   *  Sum: '<S15>/Add'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_p += mul_s32_s32_s32_sr42
    (439804651, mul_s32_s32_s32_sr10(40157, (((((xtmp << 1) + ytmp) -
         (rtb_Product1 << 1)) + rtb_Product1_f) + rtb_Product1_k) -
      (power_BLDC_EGR_DW.DiscreteTimeIntegrator_DSTATE_p >> 3) * 69));

  /* Update for DiscreteIntegrator: '<S16>/Discrete-Time Integrator1' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion'
   *  DiscreteIntegrator: '<S16>/Discrete-Time Integrator1'
   *  Gain: '<S16>/Gain'
   *  Gain: '<S16>/Gain3'
   *  Gain: '<S16>/Gain4'
   *  Sum: '<S16>/Add'
   */
  power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o += mul_s32_s32_s32_sr42
    (439804651, mul_s32_s32_s32_sr10(40157, ((((ytmp - xtmp) + rtb_Product1) -
        (rtb_Product1_f << 1)) + rtb_Product1_k) -
      (power_BLDC_EGR_DW.DiscreteTimeIntegrator1_DSTAT_o >> 3) * 69));

  /* Update for UnitDelay: '<S4>/Unit Delay' */
  power_BLDC_EGR_DW.UnitDelay_DSTATE = power_BLDC_EGR_Y.egr_bldc_phase_fnl;
}

/* Model initialize function */
void power_BLDC_EGR_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
