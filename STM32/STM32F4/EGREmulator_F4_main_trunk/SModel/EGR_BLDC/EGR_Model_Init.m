clc;
clear all;

%% Simulink Sample Initialize %%
Ts = 1e-4;

%% BLDC Motor Parameters %%
K_BLDCM_DAMPING_COEFF = Simulink.Parameter;
K_BLDCM_DAMPING_COEFF.Value = 1e-3;
K_BLDCM_DAMPING_COEFF.CoderInfo.StorageClass = 'ExportedGlobal';

K_BLDCM_FLUX = Simulink.Parameter;
K_BLDCM_FLUX.Value = 0.0854;
K_BLDCM_FLUX.CoderInfo.StorageClass = 'ExportedGlobal';

K_BLDCM_INERTIA = Simulink.Parameter;
K_BLDCM_INERTIA.Value = 1.2517e3;
K_BLDCM_INERTIA.CoderInfo.StorageClass = 'ExportedGlobal';

K_BLDCM_INDUCTANCE = Simulink.Parameter;
K_BLDCM_INDUCTANCE.Value = 208.3333;
K_BLDCM_INDUCTANCE.CoderInfo.StorageClass = 'ExportedGlobal';

K_BLDCM_RESISTANCE = Simulink.Parameter;
K_BLDCM_RESISTANCE.Value = 1.1735;
K_BLDCM_RESISTANCE.CoderInfo.StorageClass = 'ExportedGlobal';

K_BLDCM_TF = Simulink.Parameter;
K_BLDCM_TF.Value = 1.1735; %Shaft static friction torque
K_BLDCM_TF.CoderInfo.StorageClass = 'ExportedGlobal';
C_BLDCM_POLE_NUM = 4;

C_BLDCM_ROTOR_RAD_INIT = Simulink.Parameter;
C_BLDCM_ROTOR_RAD_INIT.Value = 0;
C_BLDCM_ROTOR_RAD_INIT.CoderInfo.StorageClass = 'ExportedGlobal';

%% EGR Parameters %%
C_EGR_BLDC_VALVE_SCREW_LOAD_PITCH = 4; %mm
C_EGR_BLDC_STEP = 60;
K_EGR_BLDC_LCD_POS_OFFSET = 0; %The offset for internal calculated egr_bldc_pos, EX: adjust the position used for LCD display.
K_EGR_SPRING_TORQUE_COMP = 0;

K_EGR_SPRING_COEFF = Simulink.Parameter;
K_EGR_SPRING_COEFF.Value = 0.2568;
K_EGR_SPRING_COEFF.CoderInfo.StorageClass = 'ExportedGlobal';

%% Valve related Calis %%
K_EGR_BLDC_VALVE_HS_RAD_LOW = -3.27;% EGR-BLDC valve's hard stop min rad.
K_EGR_BLDC_VALVE_HS_RAD_HIGH = 15.9082; %EGR-BLDC valve's Hard Stop max rad.
K_EGR_BLDC_VALVE_POS_RAD_INIT = 0; %the init(that is the status after power up) position/phare for EGR-BLDC.  the cali control the init stopper pos/phase after system power on.
K_EGR_BLDC_VALVE_POS_RAD_OFFSET = 1.047; %the offset(that is the status after power up) position/phare for EGR-BLDC. 
K_EGR_BLDC_VALVE_STOPPER_RAD = 0.2617; %the cali is dedicated to determinate the zero-pos of EGR.

%% Set Storage Type %%
SetStorageType('power_BLDC_EGR.slx');
