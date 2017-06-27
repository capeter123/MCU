/*
 * File: power_BLDC_EGR.h
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

#ifndef RTW_HEADER_power_BLDC_EGR_h_
#define RTW_HEADER_power_BLDC_EGR_h_
#ifndef power_BLDC_EGR_COMMON_INCLUDES_
# define power_BLDC_EGR_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* power_BLDC_EGR_COMMON_INCLUDES_ */

#include "power_BLDC_EGR_types.h"

/* Macros for accessing real-time model data structure */

/* Block signals and states (auto storage) for system '<S11>/rem' */
typedef struct {
  //int32_T DataTypeConversion;           /* '<S14>/Data Type Conversion' */
  int32_T SFunctionBuilder;             /* '<S14>/S-Function Builder' */
  int32_T DataTypeConversion1;         /* '<S14>/Data Type Conversion1' */
} DW_rem_power_BLDC_EGR_T;

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  DW_rem_power_BLDC_EGR_T rem;         /* '<S9>/rem' */
  DW_rem_power_BLDC_EGR_T rem_m;       /* '<S13>/rem' */
  DW_rem_power_BLDC_EGR_T rem_d;       /* '<S12>/rem' */
  DW_rem_power_BLDC_EGR_T rem_c;       /* '<S11>/rem' */
  int32_T rad2deg1;                     /* '<S9>/rad2deg1' */
  int32_T DiscreteTimeIntegrator1_DSTATE;/* '<S7>/Discrete-Time Integrator1' */
  int32_T DiscreteTimeIntegrator_DSTATE;/* '<S7>/Discrete-Time Integrator' */
  int32_T DiscreteTimeIntegrator_DSTATE_i;/* '<S18>/Discrete-Time Integrator' */
  int32_T DiscreteTimeIntegrator1_DSTAT_d;/* '<S19>/Discrete-Time Integrator1' */
  int32_T xtmp;
  int32_T ytmp;
  int32_T z;
  int16_T UnitDelay1_DSTATE;           /* '<S4>/Unit Delay1' */
  int8_T UnitDelay_DSTATE;             /* '<S4>/Unit Delay' */
  uint8_T DiscreteTimeIntegrator1_IC_LOAD;/* '<S7>/Discrete-Time Integrator1' */
} DW_power_BLDC_EGR_T;

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Expression: 2*pi)
   * Referenced by:
   *   '<S17>/Constant'
   *   '<S14>/Constant'
   *   '<S15>/Constant'
   *   '<S16>/Constant'
   */
  int32_T pooled1;
} ConstP_power_BLDC_EGR_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  int32_T OM_EBM1;                     /* '<Root>/OM_EBM1' */
  int32_T OM_EBM2;                     /* '<Root>/OM_EBM2' */
  int32_T OM_EBM3;                     /* '<Root>/OM_EBM3' */
} ExtU_power_BLDC_EGR_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int16_T postion_percent;             /* '<Root>/postion_percent' */
  int8_T egr_bldc_phase_fnl;           /* '<Root>/egr_bldc_phase_fnl' */
  boolean_T Hallx[3];                  /* '<Root>/Hallx' */
  int32_T rotor_rpm;                   /* '<Root>/rotor_rpm' */
  int32_T TL;                          /* '<Root>/TL' */
} ExtY_power_BLDC_EGR_T;

/* Parameters (auto storage) */
struct P_power_BLDC_EGR_T_ {
  int32_T B;                           /* Variable: B
                                        * Referenced by: '<S20>/Damping Constant (B)'
                                        */
  int32_T Flux;                        /* Variable: Flux
                                        * Referenced by:
                                        *   '<S11>/Flux Constant (Flux)'
                                        *   '<S12>/Flux Constant (Flux)'
                                        *   '<S13>/Flux Constant (Flux)'
                                        */
  int32_T J;                           /* Variable: J
                                        * Referenced by: '<S7>/Inertia Constant's Recip (1//J)'
                                        */
  int32_T L;                           /* Variable: L
                                        * Referenced by:
                                        *   '<S18>/Inductance Constant's Recip (1//L)'
                                        *   '<S19>/Inductance Constant's Recip (1//L)'
                                        */
  int32_T R;                           /* Variable: R
                                        * Referenced by:
                                        *   '<S18>/Resistance Constant (R)'
                                        *   '<S19>/Resistance Constant (R)'
                                        */
  int32_T SpringCom;                   /* Variable: SpringCom
                                        * Referenced by: '<S3>/spring_compensation'
                                        */
  int32_T Tf;                          /* Variable: Tf
                                        * Referenced by: '<S20>/Shaft static friction torque (Tf)'
                                        */
  int32_T motor_stopper_init_rad;      /* Variable: motor_stopper_init_rad
                                        * Referenced by: '<S21>/Low'
                                        */
  int32_T motor_stopper_open_rad;      /* Variable: motor_stopper_open_rad
                                        * Referenced by:
                                        *   '<S3>/Saturation'
                                        *   '<S21>/High'
                                        */
  int32_T thOffset;                    /* Variable: thOffset
                                        * Referenced by: '<S7>/theOffset'
                                        */
  int32_T tho;                         /* Variable: tho
                                        * Referenced by: '<S7>/tho'
                                        */
  int32_T valve_stopper_rad;           /* Variable: valve_stopper_rad
                                        * Referenced by:
                                        *   '<S3>/Relaxed Position'
                                        *   '<S3>/Saturation'
                                        */
  int16_T K_EGR_BLDC_POS_OFFSET;       /* Variable: K_EGR_BLDC_POS_OFFSET
                                        * Referenced by: '<S4>/K_EGR_BLDC_POS_OFFSET'
                                        */
  int16_T valve_pitch;                 /* Variable: valve_pitch
                                        * Referenced by: '<S3>/load pitch(mm)'
                                        */
  int16_T Ksp;                         /* Variable: Ksp
                                        * Referenced by: '<S3>/Spring Coefficient (Ksp)'
                                        */
  int8_T p;                            /* Variable: p
                                        * Referenced by:
                                        *   '<S5>/Rotor Poles Num (p)'
                                        *   '<S7>/Rotor Poles Num (p)'
                                        *   '<S7>/Gain'
                                        */
  uint8_T EGR_BLDC_STEP;               /* Variable: EGR_BLDC_STEP
                                        * Referenced by: '<S1>/EGR_BLDC_STEP'
                                        */
};

/* Block parameters (auto storage) */
extern P_power_BLDC_EGR_T power_BLDC_EGR_P;

/* Block signals and states (auto storage) */
extern DW_power_BLDC_EGR_T power_BLDC_EGR_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_power_BLDC_EGR_T power_BLDC_EGR_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_power_BLDC_EGR_T power_BLDC_EGR_Y;

/* Constant parameters (auto storage) */
extern const ConstP_power_BLDC_EGR_T power_BLDC_EGR_ConstP;

/* Model entry point functions */
extern void power_BLDC_EGR_initialize(void);
extern void power_BLDC_EGR_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S4>/Constant5' : Unused code path elimination
 * Block '<S4>/MinMax' : Unused code path elimination
 * Block '<S4>/Saturation1' : Unused code path elimination
 * Block '<S4>/Data Type Conversion' : Eliminate redundant data type conversion
 */

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
 * '<Root>' : 'power_BLDC_EGR'
 * '<S1>'   : 'power_BLDC_EGR/EGR BLDC System'
 * '<S2>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor'
 * '<S3>'   : 'power_BLDC_EGR/EGR BLDC System/EGR Screw Linkage Model'
 * '<S4>'   : 'power_BLDC_EGR/EGR BLDC System/EGR_BLDC_Actual_Rotor_Pos_Determination'
 * '<S5>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model'
 * '<S6>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Measurements'
 * '<S7>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Mechanical model'
 * '<S8>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux'
 * '<S9>'   : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/Hall effect sensor'
 * '<S10>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/State equations'
 * '<S11>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase a'
 * '<S12>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase b'
 * '<S13>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase c'
 * '<S14>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase a/rem'
 * '<S15>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase b/rem'
 * '<S16>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/BEMF, Flux/Phase c/rem'
 * '<S17>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/Hall effect sensor/rem'
 * '<S18>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/State equations/State Ia'
 * '<S19>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Electrical model/State equations/State Ib'
 * '<S20>'  : 'power_BLDC_EGR/EGR BLDC System/BLDCMotor/Mechanical model/Viscous Friction'
 * '<S21>'  : 'power_BLDC_EGR/EGR BLDC System/EGR Screw Linkage Model/Hard Stop'
 */

/*-
 * Requirements for '<Root>': power_BLDC_EGR
 */
#endif                                 /* RTW_HEADER_power_BLDC_EGR_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
