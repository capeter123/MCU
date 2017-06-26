/*
 * File: engine_throttle.h
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

#ifndef RTW_HEADER_engine_throttle_h_
#define RTW_HEADER_engine_throttle_h_
#ifndef engine_throttle_COMMON_INCLUDES_
# define engine_throttle_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* engine_throttle_COMMON_INCLUDES_ */

#include "engine_throttle_types.h"

/* Macros for accessing real-time model data structure */

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  int32_T RelaxedPosition;             /* '<S2>/Relaxed Position' */
  int32_T Velocity_DSTATE;             /* '<S2>/Velocity' */
  int32_T DiscreteTimeIntegrator_DSTATE;/* '<S1>/Discrete-Time Integrator' */
  int32_T Position_DSTATE;             /* '<S2>/Position' */
} DW_engine_throttle_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  int16_T U;                           /* '<Root>/U' */
} ExtU_engine_throttle_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  int32_T ith_degree;                  /* '<Root>/ith_degree' */
  int32_T ith_pos;                     /* '<Root>/ith_pos' */
  uint16_T ith_pos_da;                 /* '<Root>/ith_pos_da' */
} ExtY_engine_throttle_T;

/* Parameters (auto storage) */
struct P_engine_throttle_T_ {
  int32_T K_ITH_POS_PHYS_K;            /* Variable: K_ITH_POS_PHYS_K
                                        * Referenced by: '<S4>/K_ITH_POS_PHYS_K'
                                        */
  int32_T Ksp;                         /* Variable: Ksp
                                        * Referenced by: '<S5>/Spring'
                                        */
  int32_T rad_open;                    /* Variable: rad_open
                                        * Referenced by: '<S3>/High'
                                        */
  int32_T rad_relaxed;                 /* Variable: rad_relaxed
                                        * Referenced by: '<S2>/Relaxed Position'
                                        */
  int32_T K_ITH_POS_OFFSET;            /* Variable: K_ITH_POS_OFFSET
                                        * Referenced by: '<S2>/K_ITH_POS_OFFSET'
                                        */
  int32_T rad_init;                    /* Variable: rad_init
                                        * Referenced by:
                                        *   '<S3>/Low'
                                        *   '<S5>/Relaxed Position'
                                        */
  int32_T springTorqueCompensation;    /* Variable: springTorqueCompensation
                                        * Referenced by: '<S5>/springTorqueCompensation'
                                        */
  int32_T K_ITH_POS_PHYS_OFFSET;       /* Variable: K_ITH_POS_PHYS_OFFSET
                                        * Referenced by: '<S4>/K_ITH_POS_PHYS_OFFSET'
                                        */
};

/* Block parameters (auto storage) */
extern P_engine_throttle_T engine_throttle_P;

/* Block signals and states (auto storage) */
extern DW_engine_throttle_T engine_throttle_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_engine_throttle_T engine_throttle_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_engine_throttle_T engine_throttle_Y;

/* Model entry point functions */
extern void engine_throttle_initialize(void);
extern void engine_throttle_step(void);

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
 * '<Root>' : 'engine_throttle'
 * '<S1>'   : 'engine_throttle/Motor'
 * '<S2>'   : 'engine_throttle/Throttle'
 * '<S3>'   : 'engine_throttle/Throttle/Hard Stop'
 * '<S4>'   : 'engine_throttle/Throttle/ITH Pos to AD Volt'
 * '<S5>'   : 'engine_throttle/Throttle/ITH Spring Model'
 */

/*-
 * Requirements for '<Root>': engine_throttle
 */
#endif                                 /* RTW_HEADER_engine_throttle_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
