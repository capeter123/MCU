clc;
clear all;

%% Simulink Sample Initialize %%
Ts = 1e-4;

%% EGR CAN Motor Parameters %%
K_STEPM_DAMPING_COEFF = Simulink.Parameter; % B
K_STEPM_DAMPING_COEFF.Value = 0.03;
K_STEPM_DAMPING_COEFF.CoderInfo.StorageClass = 'ExportedGlobal';

K_STEPM_INERTIA = Simulink.Parameter; % J
K_STEPM_INERTIA.Value = 1e-4;
K_STEPM_INERTIA.CoderInfo.StorageClass = 'ExportedGlobal';

K_STEPM_TF = Simulink.Parameter; % Tm
K_STEPM_TF.Value = 1.42; %Shaft static friction torque
K_STEPM_TF.CoderInfo.StorageClass = 'ExportedGlobal';

K_STEPM_STEP_NUM = Simulink.Parameter; %Zr
K_STEPM_STEP_NUM.Value = 50;
K_STEPM_STEP_NUM.CoderInfo.StorageClass = 'ExportedGlobal';

C_STEPM_ELEC_TIM_COEFF = Simulink.Parameter; %Trc
C_STEPM_ELEC_TIM_COEFF.Value = 460;
C_STEPM_ELEC_TIM_COEFF.CoderInfo.StorageClass = 'ExportedGlobal';

K_STEPM_SELF_DAMPING_COEFF = Simulink.Parameter; %Kp
K_STEPM_SELF_DAMPING_COEFF.Value = 0.1;
K_STEPM_SELF_DAMPING_COEFF.CoderInfo.StorageClass = 'ExportedGlobal';
%% EGR CAN Parameters %%
K_EGR_CAN_TARGET_POS_MIN = Simulink.Parameter;
K_EGR_CAN_TARGET_POS_MIN.Value = 16;
K_EGR_CAN_TARGET_POS_MIN.CoderInfo.StorageClass = 'ExportedGlobal';

K_EGR_CAN_TARGET_POS_MAX = Simulink.Parameter;
K_EGR_CAN_TARGET_POS_MAX.Value = 106;
K_EGR_CAN_TARGET_POS_MAX.CoderInfo.StorageClass = 'ExportedGlobal';

K_EGR_CAN_ACT_POS_MIN = Simulink.Parameter;
K_EGR_CAN_ACT_POS_MIN.Value = 0;
K_EGR_CAN_ACT_POS_MIN.CoderInfo.StorageClass = 'ExportedGlobal';

K_EGR_CAN_ACT_POS_MAX = Simulink.Parameter;
K_EGR_CAN_ACT_POS_MAX.Value = 128;
K_EGR_CAN_ACT_POS_MAX.CoderInfo.StorageClass = 'ExportedGlobal';

K_EGR_CAN_ACT_DC_COMP = Simulink.Parameter;
K_EGR_CAN_ACT_DC_COMP.Value = 10;
K_EGR_CAN_ACT_DC_COMP.CoderInfo.StorageClass = 'ExportedGlobal';
%% PID Controller Parameters
K_PID_P_TERM = Simulink.Parameter;
K_PID_P_TERM.Value = 0.005975;
K_PID_P_TERM.CoderInfo.StorageClass = 'ExportedGlobal';

K_PID_I_TERM = Simulink.Parameter;
K_PID_I_TERM.Value = 20000;
K_PID_I_TERM.CoderInfo.StorageClass = 'ExportedGlobal';

K_PID_D_TERM = Simulink.Parameter;
K_PID_D_TERM.Value = 0;
K_PID_D_TERM.CoderInfo.StorageClass = 'ExportedGlobal';
%% Set Storage Type %%
%SetStorageType('EGR_CAN.slx');
