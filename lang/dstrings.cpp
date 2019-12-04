/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8
 * 301 00, Pilsen
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "dstrings.h"

const char* dsFile = "File";
const char* dsSave_Configuration = "Save Configruation";
const char* dsQuit = "Quit";
const char* dsTools = "Tools";
const char* dsFilters = "Filters";
const char* dsSimulation = "Simulation";
const char* dsOptimize_Parameters = "Optimize Parameters";

const char* dsLog_Tab = "Log";
const char* dsErrors_Tab = "Errors";

const char* dsClose_Tab = "Close";
const char* dsSave_Tab_State = "Save state";
const char* dsSaved_State_Tab_Suffix = " (saved)";
const char* dsBest_Metric_Label = "Best metric: %1";
const char* dsBest_Metric_NotAvailable = "N/A";

const char* dsSolver_Status_Disabled = "Disabled";
const char* dsSolver_Status_Idle = "Idle";
const char* dsSolver_Status_In_Progress = "In progress";
const char* dsSolver_Status_Completed_Improved = "Completed and improved";
const char* dsSolver_Status_Completed_Not_Improved = "Completed, but not improved";
const char* dsSolver_Status_Failed = "Failed";
const char* dsSolver_Status_Stopped = "Stopped";

const char* dsClose = "Close";
const char* dsClose_All = "Close All";

const char* dsTile_Vertically = "&Tile Vertically";
const char* dsTile_Horizontally = "&Tile Horizontally";
const char* dsCascade = "&Cascade";
const char* dsNext = "Ne&xt";
const char* dsPrevious = "Pre&vious";

const char* dsWindow = "Window";
const char* dsHelp = "&Help";
const char* dsAbout = "About";
const char* dsAbout_Amp = "&About";
const char* dsGlucose_Prediction = "Glucose Prediction - %1";

const char* dsAdd = "Add";
const char* dsDelete = "Delete";
const char* dsRemove = "Remove";
const char* dsConfigure = "Configure";
const char* dsMove_Up = "Move up";
const char* dsMove_Down = "Move down";

const char* dsStart = "Start";
const char* dsStop = "Stop";
const char* dsSolve = "Solve";
const char* dsSolve_Reset = "Reset and recalculate";
const char* dsSuspend = "Suspend solvers";
const char* dsResume = "Resume solvers";
const char* dsStepUnit = "levels";
const char* dsStep = "Step";

const char* dsFilters_Window = "Filters window";
const char* dsApplied_Filters = "Applied filters";
const char* dsAvailable_Filters = "Available filters";
const char* dsSimulation_Window = "Simulation window";
const char* dsExport_To_CSV = "Export to CSV";
const char* dsExport_CSV_Default_File_Name = "export.csv";
const char* dsExport_CSV_Ext_Spec = "CSV file (*.csv)";
const char* dsExport_CSV_Dialog_Title = "Export to CSV";

const wchar_t *dsDb_Reader = L"Db Reader";
const wchar_t *dsDb_Writer = L"Db Writer";
const wchar_t *dsSinCos_Generator = L"Sin/Cos IG/BG generator";
const wchar_t *dsDummy_Generator = L"Dummy IG/BG generator";

const wchar_t *dsDb_Host = L"Host";
const wchar_t *dsDb_Port = L"Port";
const wchar_t *dsDb_Provider = L"Provider";
const wchar_t *dsDb_Name = L"Name";
const wchar_t *dsDb_User_Name = L"User name";
const wchar_t *dsDb_Password = L"Password";
const wchar_t *dsTime_Segment_ID = L"Time Segment Id";
const wchar_t *dsShutdown_After_Last = L"Shutdown after last value";

const wchar_t *dsGenerate_Primary_Keys = L"Generate new primary keys";
const wchar_t *dsStore_Data = L"Store data";
const wchar_t *dsStore_Parameters = L"Store parameters";
const wchar_t *dsSubject_Id = L"Subject";

const wchar_t *dsGen_IG_Offset = L"IG level offset";
const wchar_t *dsGen_IG_Amplitude = L"IG amplitude";
const wchar_t *dsGen_IG_Sin_Period = L"IG sinus period";
const wchar_t *dsGen_IG_Sampling_Period = L"IG sampling period";
const wchar_t *dsGen_BG_Offset = L"BG level offset";
const wchar_t *dsGen_BG_Amplitude = L"BG amplitude";
const wchar_t *dsGen_BG_Cos_Period = L"BG cosinus period";
const wchar_t *dsGen_BG_Sampling_Period = L"BG sampling period";
const wchar_t *dsGen_Total_Time = L"Total generated time interval";

const wchar_t *dsFeedback_Channel_Identifier = L"Feedback channel identifier";
const wchar_t *dsDevice_Driver_Id = L"Device driver";
const wchar_t *dsSynchronize_With_Pump = L"Synchronize with pump";

const wchar_t *dsSolver_Filter = L"Solver";

const wchar_t *dsSelected_Model = L"Model";
const wchar_t *dsSelected_Metric = L"Metric";
const wchar_t *dsSelected_Solver = L"Solver";
const wchar_t *dsSelected_Signal = L"Signal";
const wchar_t *dsSelected_Model_Bounds = L"Model bounds";
const wchar_t *dsUse_Relative_Error = L"Use relative error";
const wchar_t *dsUse_Squared_Diff = L"Use squared differences";
const wchar_t *dsUse_Prefer_More_Levels = L"Prefer more levels";
const wchar_t *dsMetric_Threshold = L"Metric threshold";
const wchar_t *dsMetric_Levels_Required = L"Levels required";
const wchar_t *dsUse_Measured_Levels = L"Use measured levels";
const wchar_t *dsRecalculate_On_Levels_Count = L"Recalculate on reference level count (0 = off)";
const wchar_t *dsRecalculate_On_Segment_End = L"Recalculate on segment end";
const wchar_t *dsRecalculate_On_Calibration = L"Recalculate on calibration";

const wchar_t *dsFile_Reader = L"File Reader";
const wchar_t *dsInput_Values_File = L"Input file";
const wchar_t *dsInput_Segment_Spacing = L"Segment spacing [s]";
const wchar_t *dsMinimum_Segment_Levels = L"Segment value minimum count";
const wchar_t *dsRequire_IG_BG = L"Require both IG and BG values";

const wchar_t *dsHold_Values_Delay = L"Hold delay [ms] (0 for real-time)";

const wchar_t *dsSignal_Source_Id = L"Source ID";
const wchar_t *dsSignal_Destination_Id = L"Map to ID";

const wchar_t *rsSignal_Masked_Id = L"Signal";
const wchar_t *rsSignal_Value_Bitmask = L"Bitmask";

const wchar_t *dsSignal_Masked_Id = L"Signal ID";
const wchar_t *dsSignal_Value_Bitmask = L"(bit)Mask<br>(8-64 bit, reads left-to-right)";

const wchar_t *dsHold_Filter = L"Hold";
const wchar_t *dsDrawing_Filter = L"Drawing";
const wchar_t *dsMapping_Filter = L"Signal mapping";
const wchar_t *dsMasking_Filter = L"Signal value masking";
const wchar_t *dsCalculated_Signal_Filter = L"Calculated signal";

const wchar_t *dsDrawing_Filter_Period = L"Redraw period [ms]";
const wchar_t *dsDiagnosis_Is_Type2 = L"Diabetes type 2? (1 = off)";
const wchar_t *dsDrawing_Filter_Canvas_Width = L"Canvas width";
const wchar_t *dsDrawing_Filter_Canvas_Height = L"Canvas height";
const wchar_t *dsDrawing_Filter_Filename_Graph = L"Single plot file path";
const wchar_t *dsDrawing_Filter_Filename_Day = L"Daily plot file path";
const wchar_t *dsDrawing_Filter_Filename_AGP = L"AGP file path";
const wchar_t *dsDrawing_Filter_Filename_Parkes = L"Parkes grid file path";
const wchar_t *dsDrawing_Filter_Filename_Clark = L"Clark grid file path";
const wchar_t *dsDrawing_Filter_Filename_ECDF = L"ECDF file path";

const wchar_t *dsDevice_Interop_Filter = L"Real device interface";

const wchar_t *dsInterop_Export_Filter = L"Interop export filter";

const wchar_t *dsLog_Filter = L"CSV File Log";
const wchar_t *dsLog_Output_File = L"Log file";
const wchar_t *dsIgnore_Shutdown_Msg = L"Ignore the shutdown message";
const wchar_t *dsLog_Header = L"Logical Clock; Device Time; Event Code; Signal; Info; Segment Id; Event Code Id; Device Id; Signal Id;";
const wchar_t *dsLog_Filter_Replay = L"CSV File Log Replay";

const wchar_t *dsDevice_Feedback_Filter = L"Device feedback";
const wchar_t *dsDevice_Filter = L"Device";

const wchar_t *dsT1DMS_Device_Driver = L"T1DMS Device driver";

const wchar_t *dsGUI_Filter = L"Visualization";

const char* dsSave_Image_To_File = "Save to file";
const char* dsSave_Image_Ext_Spec = "SVG Image (*.svg)";
const char* dsSave_Viewport_To_File = "Save viewport to file";
const char* dsSave_Viewport_Ext_Spec = "Portable Network Graphics (*.png)";
const char* dsDefault_Viewport_File_Name = "image.png";
const char* dsReset_Zoom = "Reset zoom";

const char* dsDiagnosis_T1D = "Type 1";
const char* dsDiagnosis_T2D = "Type 2";
const char* dsDiagnosis_Gestational = "Gestational";

const char* dsSave_Image_Default_Filename_Graph = "graph.svg";
const char* dsSave_Image_Default_Filename_Day = "day.svg";
const char* dsSave_Image_Default_Filename_Parkes = "parkes.svg";
const char* dsSave_Image_Default_Filename_Clark = "clark.svg";
const char* dsSave_Image_Default_Filename_AGP = "agp.svg";
const char* dsSave_Image_Default_Filename_ECDF = "ecdf.svg";
const char* dsSave_Image_Default_Filename_Profile_Glucose = "mobile_glucose.svg";
const char* dsSave_Image_Default_Filename_Profile_Insulin = "mobile_insulin.svg";
const char* dsSave_Image_Default_Filename_Profile_Carbs = "mobile_carbs.svg";


const wchar_t *dsCalculated_Signal = L"Calculated Signal";
const wchar_t* dsPrediction_Window = L"Prediction window";
const wchar_t* dsSolving_Parameters_Separator = L"Solving";
const wchar_t* dsSolve_Parameters = L"Solve parameters";
const wchar_t* dsSolve_Using_All_Segments = L"Solve using all segments";
const wchar_t* dsMetric_Separator = L"Metric";
const wchar_t* dsMetric_Levels_Required_Hint = L"Enter negative number as complement to current number of all reference levels";
const wchar_t* dsSolve_On_Level_Count = L"Solve on level count (0=off)";
const wchar_t* dsSolve_On_Calibration = L"Solve on calibration";
const wchar_t* dsSolve_On_Time_Segment_End = L"Solve on time-segment end";

const wchar_t *dsUse_Just_Opened_Segments = L"Determine parameters using all known segments";

const char *dsConfiguration = "Configuration";
const char *dsSelect_Just_One_Item = "Select just one item.";
const char *dsSimulation_Is_In_Progress = "Simulation is in progress.";

const char *dsInformation = "Information";
const char *dsMain_Parameters = "Main parameters";

const char* dsOK = "OK";
const char* dsCancel = "Cancel";
const char* dsApply = "Apply";
const char* dsCommit = "Commit";

const char* dsSubject = "Subject";
const char* dsSegment = "Segment";
const char* dsValue_Count = "Measured value count";

const wchar_t* dsAvg_Abs = L"average error";
const wchar_t* dsMax_Abs = L"maximum error";
const wchar_t* dsPerc_Abs = L"error at percentil";
const wchar_t* dsThresh_Abs = L"number of errors above threshold";
const wchar_t* dsLeal_2010 = L"Leal 2010";
const wchar_t* dsAIC = L"Akaike Information Criterion";
const wchar_t* dsStd_Dev = L"standard deviation, within threshold to 1-threshold percentiles";
const wchar_t* dsCrosswalk = L"Crosswalk";
const wchar_t* dsIntegral_CDF = L"area under ECDF";
const wchar_t* dsAvg_Plus_Bessel_Std_Dev = L"average plus standard deviation with Bessel's correction, within threshold to 1-threshold percentiles";

const wchar_t* dsBlood = L"blood";
const wchar_t* dsInterstitial = L"interstitial fluid";

const wchar_t* dsConstant_Signal = L"Constant signal";

const wchar_t* dsDiffusion_Model_v2 = L"Diffusion model v2";
const wchar_t* dsSteil_Rebrin = L"Steil-Rebrin";
const wchar_t* dsSteil_Rebrin_Diffusion_Prediction = L"Steil-Rebrin Diffusion Prediction";
const wchar_t* dsDiffusion_Prediction = L"Diffusion Prediction";
const wchar_t* dsOref0_Basal_Insulin = L"oref0 basal insulin";
const wchar_t* dsOref0_Basal_Insulin_Orig = L"oref0 basal insulin (orig)";
const wchar_t* dsOref0_IOB_Model = L"oref0 IOB";
const wchar_t* dsOref0_COB_Model = L"oref0 COB";
const wchar_t* dsOref0_Insulin_Activity_Bilinear = L"oref0 insulin activity bilinear";
const wchar_t* dsOref0_Insulin_Activity_Exponential = L"oref0 insulin activity exponential";
const wchar_t* dsOref0_IOB_Bilinear = L"oref0 IOB bilinear";
const wchar_t* dsOref0_IOB_Exponential = L"oref0 IOB exponential";
const wchar_t* dsOref0_COB_Bilinear = L"oref0 COB bilinear";
const wchar_t* dsOref0_BG_Prediction = L"oref0 BG Prediction";
const wchar_t* dsOref0_BG_IOB_Prediction = L"oref0 IOB-based BG Prediction";
const wchar_t* dsOref0_BG_COB_Prediction = L"oref0 COB-based BG Prediction";
const wchar_t* dsOref0_BG_UAM_Prediction = L"oref0 UAM-based BG Prediction";
const wchar_t* dsOref0_BG_ZT_Prediction = L"oref0 ZT-based BG Prediction";
const wchar_t* dsOref0_BG_Prediction_Signal = L"oref0 aggregated BG Prediction";
const wchar_t* dsConstant_Model = L"Constant model";

const wchar_t* dsOref0_Solver = L"oref0 solver";

const wchar_t* dsIOB_Model = L"IOB";
const wchar_t* dsCOB_Model = L"COB";
const wchar_t* dsInsulin_Activity_Bilinear = L"Insulin activity bilinear";
const wchar_t* dsInsulin_Activity_Exponential = L"Insulin activity exponential";
const wchar_t* dsIOB_Bilinear = L"IOB bilinear";
const wchar_t* dsIOB_Exponential = L"IOB exponential";
const wchar_t* dsCOB_Bilinear = L"COB bilinear";

const wchar_t* dsP = L"p";
const wchar_t* dsCg = L"cg";
const wchar_t* dsC = L"c";
const wchar_t* dsDt = L"dt";
const wchar_t* dsK = L"k";
const wchar_t* dsH = L"h";

const wchar_t* dsTau = L"tau";
const wchar_t* dsAlpha = L"alpha";
const wchar_t* dsBeta = L"beta";
const wchar_t* dsGamma = L"gamma";

const wchar_t* dsInv_G = L"1/g";

const wchar_t* dsConstantParam = L"c";

const wchar_t* dsRetrospectiveP = L"p<sub>retro</sub>";
const wchar_t* dsRetrospectiveCg = L"cg<sub>retro</sub>";
const wchar_t* dsRetrospectiveC = L"c<sub>retro</sub>";
const wchar_t* dsRetrospectiveDt = L"&Delta;t<sub>retro</sub>";

const wchar_t* dsPredictiveP = L"p<sub>pred</sub>";
const wchar_t* dsPredictiveCg = L"cg<sub>pred</sub>";
const wchar_t* dsPredictiveC = L"c<sub>pred</sub>";
const wchar_t* dsPredictiveDt = L"&Delta;t<sub>pred</sub>";

const wchar_t* dsInsulin_TwoTerm_Regulation = L"Insulin two-term regulation";
const wchar_t* dsInsulin_PID_Regulation = L"Insulin PID regulation";
const wchar_t* dsBetaPID = L"BetaPID";
const wchar_t* dsBetaPID3 = L"BetaPID3";
const wchar_t* dsInsulin_TwoTerm_OnOff_Rate = L"Two-term on-off regulator insulin rate";
const wchar_t* dsInsulin_TwoTerm_OnNeturalOff_Rate = L"Two-term on-neutral-off regulator insulin rate";
const wchar_t* dsInsulin_TwoTerm_OnOff_Hysteresis_Rate = L"Two-term on-off regulator with hysteresis insulin rate";
const wchar_t* dsInsulin_PID_Regulation_Rate = L"PID regulator insulin rate";
const wchar_t* dsInsulin_BetaPID_Rate = L"BetaPID regulator insulin rate";
const wchar_t* dsInsulin_BetaPID2_Rate = L"BetaPID2 regulator insulin rate";
const wchar_t* dsInsulin_BetaPID3_Rate = L"BetaPID3 regulator insulin rate";
const wchar_t* dsParam_K = L"K";
const wchar_t* dsKp = L"Kp";
const wchar_t* dsKi = L"Ki";
const wchar_t* dsKd = L"Kd";
const wchar_t* dsBIN = L"BIN";
const wchar_t* dsKiDecay = L"KiDecay";

const wchar_t* dsLGS_Basal_Insulin_Model = L"LGS Basal Insulin Rate";
const wchar_t* dsBolus_Calculator = L"Bolus calculator";
const wchar_t* dsCalculated_Bolus_Insulin = L"Calculated bolus insulin";
const wchar_t* dsConst_Basal_Insulin = L"Constant basal insulin rate";
const wchar_t* dsConst_Basal_Low_Suspend_Insulin = L"Constant basal insulin rate with low glucose suspend";

const wchar_t* dsLGS_Lower_Threshold = L"Lower threshold";
const wchar_t* dsLGS_Suspend_Duration = L"Suspend duration";

const wchar_t* dsISF = L"ISF";
const wchar_t* dsCSR = L"CSR";
const wchar_t* dsInsulinPeak = L"Insulin activity peak";
const wchar_t* dsDIA = L"DIA";

const wchar_t* dsCarbPeak = L"Carbohydrates activity peak";
const wchar_t* dsCarbDA = L"DCA?";

const wchar_t* dsNewUOA = L"NewUOA";
const wchar_t* dsMT_MetaDE = L"MT MetaDE";
const wchar_t* dsHalton_MetaDE = L"Halton MetaDE";
const wchar_t* dsRnd_MetaDE = L"RND MetaDE";
const wchar_t* dsMetaDE = L"MetaDE";
const wchar_t* dsBOBYQA = L"BOBYQA";
const wchar_t* dsHalton_Zooming = L"Halton Zooming";
const wchar_t* dsPathfinder = L"Pathfinder";
const wchar_t* dsPathfinder_LD_Directions = L"Pathfinder, LD Dir";
const wchar_t* dsPathfinder_LD_Population = L"Pathfinder, LD Pop";
const wchar_t* dsPathfinder_LD_Directions_Population = L"Pathfinder, LD Dir Pop";
const wchar_t* dsPathfinder_Fast = L"Pathfinder, fast";
const wchar_t* dsPSO = L"PSO";
const wchar_t* dsSADE = L"SADE";
const wchar_t* dsDE1220 = L"DE1220";
const wchar_t* dsABC = L"Artificial Bee Colony";
const wchar_t* dsCMAES = L"Cov. Matrix Adapt. Evo.";
const wchar_t* dsXNES = L"Exp. Evo. Strat.";
const wchar_t* dsGPSO = L"PSO Generational";
const wchar_t* dsIHS = L"Improved Harmony Search";
const wchar_t* dsSimplex = L"Simplex";
const wchar_t* dsSubplex = L"Subplex";
const wchar_t* dsPraxis = L"Praxis";

const char* dsMax_Generations = "Maximum generations";
const char* dsPopulation_Size = "Population size";


const wchar_t *dsLine_Approx = L"Line";
const wchar_t *dsAkima = L"Akima";

const char *dsDrawing_Tab_Graph = "Single plot";
const char *dsDrawing_Tab_Day = "Daily plot";
const char *dsDrawing_Tab_Clark = "Clarke's Grid";
const char *dsDrawing_Tab_Parkes = "Parkes' Grid";
const char *dsDrawing_Tab_AGP = "AGP";
const char *dsDrawing_Tab_ECDF = "ECDF";
const char *dsDrawing_Tab_Profile_Glucose = "Mobile - Glucose";
const char *dsDrawing_Tab_Profile_Carbs = "Mobile - Carbohydrates";
const char *dsDrawing_Tab_Profile_Insulin = "Mobile - Insulin";

const char *dsLower_Bounds = "Lower bounds";
const char *dsDefault_Parameters = "Default parameters";
const char *dsUpper_Bounds = "Upper bounds";

const char *dsReset_Bounds = "Reset";

const char *dsLog_Table_View = "Table";
const char *dsLog_Raw_View = "Raw";

const wchar_t *dsSignal_Measured_BG = L"blood glucose";
const wchar_t *dsSignal_Measured_IG = L"interstitial fluid glucose";
const wchar_t *dsSignal_Measured_ISIG = L"ISIG";
const wchar_t *dsSignal_Measured_Calibration = L"blood glucose calibration";
const wchar_t *dsSignal_Measured_Bolus_Insulin = L"insulin bolus amount";
const wchar_t *dsSignal_Measured_Basal_Insulin = L"insulin basal amount";
const wchar_t *dsSignal_Measured_Basal_Insulin_Rate = L"insulin basal rate";
const wchar_t *dsSignal_Measured_Insulin_Activity = L"insulin activity";
const wchar_t *dsSignal_Measured_IOB = L"insulin on board (IOB)";
const wchar_t *dsSignal_Measured_COB = L"carbohydrates on board (COB)";
const wchar_t *dsSignal_Measured_Carb_Intake = L"carbohydrates intake";
const wchar_t *dsSignal_Measured_Health_Physical_Activity = L"physical activity";
const wchar_t* dsSignal_Measured_Insulin_Sensitivity = L"insulin sensitivity factor";
const wchar_t* dsSignal_Measured_Carb_Ratio = L"carb-to-insulin ratio";
const wchar_t *dsSignal_Suffix_Measured = L"measured";
const wchar_t *dsSignal_Suffix_Calculated = L"calculated";
const wchar_t *dsSignal_Unknown = L"Unknown";
const wchar_t *dsSignal_Prefix_Virtual = L"virtual";

const wchar_t *dsSignal_GUI_Name_All = L"All signals";
const wchar_t *dsSignal_GUI_Name_BG = L"Blood glucose";
const wchar_t *dsSignal_GUI_Name_IG = L"Interstitial glucose";
const wchar_t *dsSignal_GUI_Name_ISIG = L"ISIG";
const wchar_t *dsSignal_GUI_Name_Calibration = L"Calibration";
const wchar_t *dsSignal_GUI_Name_Delivered_Insulin_Bolus = L"Bolus Insulin intake";
const wchar_t *dsSignal_GUI_Name_Delivered_Insulin_Basal = L"Basal Insulin intake";
const wchar_t *dsSignal_GUI_Name_Requested_Basal_Insulin_Rate = L"Basal Insulin rate";
const wchar_t *dsSignal_GUI_Name_Insulin_Activity = L"Insulin activity";
const wchar_t *dsSignal_GUI_Name_IOB = L"IOB";
const wchar_t *dsSignal_GUI_Name_COB = L"COB";
const wchar_t *dsSignal_GUI_Name_Carbs = L"Carbohydrates intake";
const wchar_t *dsSignal_GUI_Name_Physical_Activity = L"Physical activity";

const wchar_t *dsSignal_GUI_Name_Unknown = L"Unknown signal";

const wchar_t *dsSolver_Progress_Box_Title = L"Solver progress";
//const wchar_t *dsError_Absolute = L"absolute";
//const wchar_t *dsError_Relative = L"relative";

const wchar_t *dsError_Column_Average = L"Average";
const wchar_t *dsError_Column_StdDev = L"Std.Dev (Bessel)";
const wchar_t *dsError_Column_Count = L"Count";
const wchar_t *dsError_Column_Sum = L"Sum";
const wchar_t *dsError_Column_Minimum = L"Minimum";
const wchar_t *dsError_Column_First_Quantile = L"1. Quartile";
const wchar_t *dsError_Column_Median = L"Median";
const wchar_t *dsError_Column_Third_Quantile = L"3. Quartile";
const wchar_t *dsError_Column_95_Quantile = L"95% Quantile";
const wchar_t *dsError_Column_99_Quantile = L"99% Quantile";
const wchar_t *dsError_Column_Maximum = L"Maximum";
const wchar_t *dsError_Column_Range_5pct = L"<=5% prob.";
const wchar_t *dsError_Column_Range_10pct = L"<=10% prob.";
const wchar_t *dsError_Column_Range_25pct = L"<=25% prob.";
const wchar_t *dsError_Column_Range_50pct = L"<=50% prob.";

const wchar_t* rsInsert_New_Measured_Value = L"INSERT INTO measuredvalue (measuredat, blood, ist, isig, insulin_bolus, insulin_basal_rate, carbohydrates, calibration, segmentid) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

const wchar_t* rsInsert_Params_Base = L"INSERT INTO ";
const wchar_t* rsInsert_Params_Segmentid_Column = L"segmentid";
const wchar_t* rsInsert_Params_Values_Stmt = L"VALUES";

const wchar_t* rsCreated_Segment_Identifier_Base = L"New_Segment_Marker_";
const wchar_t* rsInsert_New_Time_Segment = L"INSERT INTO timesegment (name, comment, deleted, subjectid, parallel_id) VALUES (?, ?, ?, ?, ?)";
const wchar_t* rsSelect_Time_Segment_Id_By_Name = L"SELECT id FROM timesegment WHERE name = ?";
const wchar_t* rsRename_Time_Segment = L"UPDATE timesegment SET name = ? WHERE id = ?";
const wchar_t* rsDelete_Parameters_Of_Segment_Base = L"DELETE FROM ";
const wchar_t* rsDelete_Parameters_Of_Segment_Stmt = L" WHERE segmentid = ?";

const wchar_t* rsT1DMS_Receiver_Filter = L"T1DMS Receiver filter";
const wchar_t* rsT1DMS_Sender_Filter = L"T1DMS Sender filter";

const wchar_t* rsInsulin_Regulation = L"insulin-regulation";
const wchar_t* rsInsulin_PID_Regulation = L"insulin-pid-regulation";
const wchar_t* rsBetaPID = L"betapid";
const wchar_t* rsBetaPID3 = L"betapid3";
const wchar_t* rsKp = L"kp";
const wchar_t* rsKi = L"ki";
const wchar_t* rsKd = L"kd";
const wchar_t* rsBIN = L"bin";

const wchar_t* rsBolus_Calculator = L"boluscalc";
const wchar_t* rsConst_Basal_Insulin = L"const-basal";

// ---- drawing-related constants

const wchar_t* dsDrawingLocaleTitle = L"Calculated Blood Glucose Level";
const wchar_t* dsDrawingLocaleTitleAgp = L"Ambulatory Glucose Profile";
const wchar_t* dsDrawingLocaleTitleClark = L"Clarke's error grid";
const wchar_t* dsDrawingLocaleTitleParkes = L"Parkes' error grid";
const wchar_t* dsDrawingLocaleTitleECDF = L"ECDF";
const wchar_t* dsDrawingLocaleTitleProfileGlucose = L"Profile - glucose";
const wchar_t* dsDrawingLocaleTitleDay = L"Day";
const wchar_t* dsDrawingLocaleSubtitle = L"Blood glucose level that was calculated from CMGS-measured subcutanoues tissue glucose levels and sporadically measured blood glucose levels";
const wchar_t* dsDrawingLocaleDiabetes1 = L"Diabetes 1";
const wchar_t* dsDrawingLocaleDiabetes2 = L"Diabetes 2";
const wchar_t* dsDrawingLocaleTime = L"Time [day. month. year hour:minute]";
const wchar_t* dsDrawingLocaleTimeDay = L"Time [Hours]";
const wchar_t* dsDrawingLocaleConcentration = L"Glucose Level";
const wchar_t* dsDrawingLocaleHypoglycemy = L"Hypoglycemia";
const wchar_t* dsDrawingLocaleHyperglycemy = L"Hyperglycemia";
const wchar_t* dsDrawingLocaleBlood = L"Measured Blood Glucose Level";
const wchar_t* dsDrawingLocaleBloodCalibration = L"Calibration";
const wchar_t* dsDrawingLocaleIst = L"Subc. Tissue Glucose Level";
const wchar_t* dsDrawingLocaleResults = L"Calculated Blood Glucose Level";
const wchar_t* dsDrawingLocaleDiff2 = L"Diffusion 2 method";
const wchar_t* dsDrawingLocaleDiff3 = L"Diffusion 3 method";
const wchar_t* dsDrawingLocaleQuantile = L"Error Quantiles";
const wchar_t* dsDrawingLocaleRelative = L"Relative";
const wchar_t* dsDrawingLocaleAbsolute = L"Absolute";
const wchar_t* dsDrawingLocaleAverage = L"Average";
const wchar_t* dsDrawingLocaleType = L"Error type";
const wchar_t* dsDrawingLocaleError = L"Error";
const wchar_t* dsDrawingLocaleDescription = L"Description";
const wchar_t* dsDrawingLocaleColor = L"Color";
const wchar_t* dsDrawingLocaleCounted = L"Calculated concentration";
const wchar_t* dsDrawingLocaleMeasured = L"Measured concentration";
const wchar_t* dsDrawingLocaleAxisX = L"Time [Hours]";
const wchar_t* dsDrawingLocaleAxisY = L"Glucose";
const wchar_t* dsDrawingLocaleSvgDatetimeTitle = L"Measured at";
const wchar_t* dsDrawingLocaleSvgIstTitle = L"IST";
const wchar_t* dsDrawingLocaleSvgBloodTitle = L"Blood";
const wchar_t* dsDrawingLocaleSvgBloodCalibrationTitle = L"Blood (calibration)";
const wchar_t* dsDrawingLocaleSvgInsulinTitle = L"Insulin";
const wchar_t* dsDrawingLocaleSvgCarbohydratesTitle = L"Carbohydrates";
const wchar_t* dsDrawingLocaleSvgISIGTitle = L"ISIG";
const wchar_t* dsDrawingLocaleSvgDiff2Title = L"Diffusion 2";
const wchar_t* dsDrawingLocaleSvgDiff3Title = L"Diffusion 3";
const wchar_t* dsDrawingLocaleRelativeError = L"Relative error [%]";
const wchar_t* dsDrawingLocaleCummulativeProbability = L"Cummulative probability [%]";
const wchar_t* dsDrawingLocaleElevatedGlucose = L"Elevated glucose";


const wchar_t *dsCalculate_Past_New_Params = L"Calculate past with first parameter set";
const wchar_t *dsRecalculate_With_Every_Params = L"New parameters recalculate entire segment";
const wchar_t *dsHold_During_Solve = L"Hold during solve";

const wchar_t *dsHold_During_Solve_Tooltip = L"Hold all incoming messages while solver is in progress";

const char *dsTime_Segments_Panel_Title = "Time segments";
const char *dsSignals_Panel_Title = "Signals";
const char *dsRedraw_Button_Title = "Apply visibility";
const char *dsTime_Segments_Panel_Segment_Name = "Segment %1";
const char *dsSelect_All_Segments = "All";
const char *dsSelect_No_Segments = "None";

const char *dsAnonymous_Subject = "Anonymous subject";
const char *dsCreate_New_Subject = "Create new subject";
const char *dsExisting_Subject = "Select existing subject:";

// filter configuration tooltips

// data filter group
const wchar_t *dsDb_Host_Tooltip = L"Database host (hostname or IP address)";
const wchar_t *dsDb_Port_Tooltip = L"Database port (0-65535)";
const wchar_t *dsDb_Provider_Tooltip = L"Database driver provider; for Qt drivers, use QPSQL, QMYSQL, QSQLITE, etc.";
const wchar_t *dsDb_Name_Tooltip = L"Database or schema name relevant to chosen database driver";
const wchar_t *dsDb_Username_Tooltip = L"Username to be used when connecting to database";
const wchar_t *dsDb_Password_Tooltip = L"Password to be used when connecting to database";
const wchar_t *dsShutdown_After_Last_Tooltip = L"Should the filter chain terminate after last value? This is typically used for headless simulations";
const wchar_t *dsGenerate_Primary_Keys_Tooltip = L"Generate new time segment when storing data?";
const wchar_t *dsStore_Data_Tooltip = L"Should all incoming data (measured values) be stored to database?";
const wchar_t *dsStore_Parameters_Tooltip = L"Should all incoming parameters (of all known models) be stored to database? Existing set of parameters would be overwritten";
const wchar_t *dsInput_Values_File_Tooltip = L"File to be extracted and its contents to be sent to simulation";
const wchar_t *dsInput_Segment_Spacing_Tooltip = L"Minimal spacing between values (in seconds) to end currently running segment and start a new one";
const wchar_t *dsHold_Values_Delay_Tooltip = L"Input values will be held for this amount of milliseconds. For slowing down simulation to real-time, use 0";
const wchar_t *dsMinimum_Segment_Levels_Tooltip = L"How many values does the segment need to have to be accepted?";
const wchar_t *dsRequire_IG_BG_Tooltip = L"To accept a segment, it must contain both IG and BG values";

// drawing filter group
const wchar_t *dsCanvas_Width_Tooltip = L"Desired canvas width for output images. Also reflects GUI widget width";
const wchar_t *dsCanvas_Height_Tooltip = L"Desired canvas height for output images. Also reflects GUI widget height";
const wchar_t *dsFilename_Graph_Tooltip = L"Where to store 'Single plot' drawing at the end of simulation";
const wchar_t *dsFilename_Day_Tooltip = L"Where to store 'Daily plot' drawing at the end of simulation";
const wchar_t *dsFilename_AGP_Tooltip = L"Where to store 'AGP' drawing at the end of simulation";
const wchar_t *dsFilename_Parkes_Tooltip = L"Where to store 'Parkes grid' drawing at the end of simulation";
const wchar_t *dsFilename_Clark_Tooltip = L"Where to store 'Clarke grid' drawing at the end of simulation";
const wchar_t *dsFilename_ECDF_Tooltip = L"Where to store 'ECDF' drawing at the end of simulation";

// log filter group
const wchar_t *dsLog_File_Output_Tooltip = L"Log file output. Any existing file with such name will be overwritten";
const wchar_t *dsLog_File_Input_Tooltip = L"Log file to be parsed and its contents to be sent to simulation";

// virtual pump group
const wchar_t *dsPump_Interval_Tooltip = L"How often the pump should dose basal insulin";
const wchar_t *dsFeedback_Channel_Identifier_Tooltip = L"Identifier of the feedback channel used for pairing and communication";
const wchar_t *dsDevice_Driver_Id_Tooltip = L"Selected device driver to be loaded";
const wchar_t *dsSynchronize_With_Pump_Tooltip = L"Synchronize with pump (pump driver)";

// signals filter group
const wchar_t *dsSelected_Model_Tooltip = L"Model to be used. Make sure you also select valid signal from signal box below";
const wchar_t *dsSelected_Signal_Tooltip = L"Model signal to be used";
const wchar_t *dsPrediction_Window_Tooltip = L"Size of prediction window to be requested from signal calculator regardless of accuracy";
const wchar_t *dsMapping_Source_Signal_Tooltip = L"Incoming signal to be mapped";
const wchar_t *dsMapping_Destination_Signal_Tooltip = L"The incoming signal will be mapped to this signal identifier";
const wchar_t *dsMasked_Signal_Tooltip = L"Signal to be masked";
const wchar_t *dsSignal_Values_Mask_Tooltip = L"Recurring pattern (bitmask) to be applied when masking given signal, up to 64 bits are supported, use 1 to mask value, 0 to leave as-is, insert spaces as you wish";

// solver filter group
const wchar_t *dsSelected_Metric_Tooltip = L"Metric to be used for solution evaluation";
const wchar_t *dsSelected_Solver_Tooltip = L"Solver implementation to be used";
const wchar_t *dsUse_Relative_Error_Tooltip = L"Use relative errors when calculating metric values";
const wchar_t *dsUse_Squared_Diff_Tooltip = L"Use squared differences when calculating metric values";
const wchar_t *dsUse_Prefer_More_Levels_Tooltip = L"Prefer solutions with more levels?";
const wchar_t *dsMetric_Threshold_Tooltip = L"Metric value threshold";
const wchar_t *dsUse_Measured_Levels_Tooltip = L"Use measured signal instead of its approximation?";
const wchar_t *dsRecalculate_On_Levels_Count_Tooltip = L"Run solver after given amount of reference levels";
const wchar_t *dsRecalculate_On_Segment_End_Tooltip = L"Run solver after each segment end marker";
const wchar_t *dsRecalculate_On_Calibration_Tooltip = L"Run solver on every calibration (blood-glucose calibration signal)";
const wchar_t *dsRecalculate_On_Parameters_Tooltip = L"Request signal recalculation on every parameter set";
const wchar_t *dsUse_Opened_Segments_Only_Tooltip = L"Use currently opened segments only";

const wchar_t *dsParameter_Configuration_Failed_RC = L"Parameter (%1) configuration failed with HRESULT == %2";

const wchar_t* dsBergman_Minimal_Model = L"Bergman extended minimal model device";
const wchar_t* dsBergman_p1 = L"p1";
const wchar_t* dsBergman_p2 = L"p2";
const wchar_t* dsBergman_p3 = L"p3";
const wchar_t* dsBergman_p4 = L"p4";
const wchar_t* dsBergman_k12 = L"k12";
const wchar_t* dsBergman_k21 = L"k21";
const wchar_t* dsBergman_Vi = L"Vi";
const wchar_t* dsBergman_BW = L"body weight";
const wchar_t* dsBergman_VgDist = L"VgDist";
const wchar_t* dsBergman_d1rate = L"d1rate";
const wchar_t* dsBergman_d2rate = L"d2rate";
const wchar_t* dsBergman_irate = L"irate";
const wchar_t* dsBergman_Qb = L"Qb";
const wchar_t* dsBergman_Ib = L"Ib";
const wchar_t* dsBergman_Q10 = L"Q10";
const wchar_t* dsBergman_Q20 = L"Q20";
const wchar_t* dsBergman_X0 = L"X0";
const wchar_t* dsBergman_I0 = L"I0";
const wchar_t* dsBergman_D10 = L"D10";
const wchar_t* dsBergman_D20 = L"D20";
const wchar_t* dsBergman_Isc0 = L"Isc0";
const wchar_t* dsBergman_Gsc0 = L"Gsc0";
const wchar_t* dsBergman_BasalRate0 = L"BasalRate0";
const wchar_t* dsBergman_diff2_p = L"p";
const wchar_t* dsBergman_diff2_cg = L"cg";
const wchar_t* dsBergman_diff2_c = L"c";

const wchar_t *dsBergman_Signal_BG = L"Bergman - blood glucose";
const wchar_t *dsBergman_Signal_IG = L"Bergman - interst. glucose";
const wchar_t *dsBergman_Signal_IOB = L"Bergman - insulin on board";
const wchar_t *dsBergman_Signal_COB = L"Bergman - CHO on board";
const wchar_t *dsBergman_Signal_Basal_Insulin = L"Bergman - dosed basal insulin";
const wchar_t *dsBergman_Signal_Insulin_Activity = L"Bergman - insulin activity";

const wchar_t* dsSignal_Generator = L"Signal generator";
const wchar_t* dsParameters = L"Parameters";
const wchar_t* dsFeedback_Name = L"Feedback name";
const wchar_t* dsSynchronize_to_Signal = L"Synchronize to signal";
const wchar_t* dsSynchronization_Signal = L"Synchronization signal";
const wchar_t* dsStepping = L"Stepping";
const wchar_t* dsMaximum_Time = L"Maximum time";

const wchar_t* dsSignal_Error = L"Signal error";
const wchar_t* dsReference_Signal = L"Reference signal";
const wchar_t* dsError_Signal = L"Error signal";
const wchar_t* dsError = L"Error";
const wchar_t* dsDescription = L"Description";
const wchar_t* dsAbsolute = L"Absolute [mmol/l]";
const wchar_t* dsRelative = L"Relative [%]";

const wchar_t* dsInsulin_LGS_Rate = L"LGS Basal Insulin Rate";

const wchar_t* dsConst_ISF = L"insulin sensitivity factor";
const wchar_t* dsConst_ISF_Model = L"Constant insulin sensitivity factor";
const wchar_t* dsConst_CR = L"carb-to-insulin ratio";
const wchar_t* dsConst_CR_Model = L"Constant carb-to-insulin ratio";

const wchar_t* dsSignal_Feedback = L"Signal feedback";
const wchar_t* dsForward_Clone = L"Forward a clone";

//--------------------------------- do not translate any of the rs-prefixed texts --

const char* rsAbout_Text = R"CLC(
<b>SmartCGMS</b> - continuous glucose monitoring and controlling framework<br>
<a href="https://diabetes.zcu.cz/smartcgms">https://diabetes.zcu.cz/</a><br>
&nbsp;<br>
Copyright&copy; since 2018 University of West Bohemia.<br>
&nbsp;<br>
<i>Contact:</i><br>
<a href="mailto:diabetes@mail.kiv.zcu.cz?Subject=SmartCGMS">diabetes@mail.kiv.zcu.cz</a><br>
Medical Informatics, Department of Computer Science and Engineering<br>
Faculty of Applied Sciences, University of West Bohemia<br>
Univerzitni 8<br>
301 00, Pilsen<br>
Czech Republic<br>
&nbsp;<br>
&nbsp;<br>
<i>Purpose of this software:</i><br>
 This software is intended to demonstrate work of the diabetes.zcu.cz research
 group to other scientists, to complement our published papers. It is strictly
 prohibited to use this software for diagnosis or treatment of any medical condition,
 without obtaining all required approvals from respective regulatory bodies.<br>
&nbsp;<br>
Especially, a diabetic patient is warned that unauthorized use of this software
may result into severe injure, including death.<br>
&nbsp;<br>
&nbsp;<br>
 <i>Licensing terms:</i><br>
 Unless required by applicable law or agreed to in writing, software
 distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.<br>
 
<ol type="a"> 
<li>For non-profit, academic research, this software is available under the <a href="https://www.gnu.org/licenses/gpl-3.0.html">GPLv3 license</a>.</li>
<li>For any other use, especially commercial use, you must contact us and obtain specific terms and conditions for the use of the software.<li>
<li>When publishing work with results obtained using this software, you agree to cite the following paper:<br>
Tomas Koutny and Martin Ubl, <i>"Parallel software architecture for the next generation of glucose monitoring"</i>, 
Procedia Computer Science, Volume 141C, pp. 279-286, 2018</li>
 </ol>
)CLC";

const wchar_t *rsDb_Host = L"Host";
const wchar_t *rsDb_Port = L"Port";
const wchar_t *rsDb_Provider = L"Provider";
const wchar_t *rsDb_Name = L"Name";
const wchar_t *rsDb_User_Name = L"User_Name";
const wchar_t *rsDb_Password = L"Password";
const wchar_t *rsTime_Segment_ID = L"Time_Segment_Id";

const wchar_t *rsDiffusion_v2_Table = L"difuse2params";
const wchar_t *rsDiffusion_v3_Table = L"difuse3params";
const wchar_t *rsSteil_Rebrin_Table = L"steilrebrinparams";
const wchar_t* rsSteil_Rebrin_Diffusion_Prediction_Table = L"steil_rebrin_diffusion_prediction";
const wchar_t* rsDiffusion_Prediction_Table = L"diffusion_prediction";
const wchar_t* rsOref0_Basal_Insulin = L"oref0_basal_insulin";
const wchar_t* rsOref0_IOB = L"oref0_iob";
const wchar_t* rsOref0_COB = L"oref0_cob";
const wchar_t* rsOref0_BG_Prediction = L"oref0_bg_prediction";
const wchar_t* rsOref0_BG_IOB_Prediction = L"oref0_bg_iob_prediction";
const wchar_t* rsOref0_BG_COB_Prediction = L"oref0_bg_cob_prediction";
const wchar_t* rsOref0_BG_UAM_Prediction = L"oref0_bg_uam_prediction";
const wchar_t* rsOref0_BG_ZT_Prediction = L"oref0_bg_zt_prediction";
const wchar_t* rsConstant_Model = L"constant_model";

const wchar_t* rsP_Column = L"p";
const wchar_t* rsCg_Column = L"cg";
const wchar_t* rsC_Column = L"c";
const wchar_t* rsDt_Column = L"dt";
const wchar_t* rsK_Column = L"k";
const wchar_t* rsH_Column = L"h";

const wchar_t* rsInv_G_Column = L"inv_g";

const wchar_t* rsRetrospectiveP = L"p_retro";
const wchar_t* rsRetrospectiveCg = L"cg_retro";
const wchar_t* rsRetrospectiveC = L"c_retro";
const wchar_t* rsRetrospectiveDt = L"dt_retro";

const wchar_t* rsPredictiveP = L"p_pred";
const wchar_t* rsPredictiveCg = L"cg_pred";
const wchar_t* rsPredictiveC = L"c_pred";
const wchar_t* rsPredictiveDt = L"dt_pred";

const wchar_t* rsTau_Column = L"t";
const wchar_t* rsAlpha_Column = L"alpha";
const wchar_t* rsBeta_Column = L"beta";
const wchar_t* rsGamma_Column = L"gamma";

const wchar_t* rsISF = L"isf";
const wchar_t* rsCSR = L"csr";
const wchar_t* rsInsulinPeak = L"ia_peak";
const wchar_t* rsDIA = L"dia";
const wchar_t* rsCarbPeak = L"ca_peak";
const wchar_t* rsCarbDA = L"carbda";

const wchar_t *rsNet_Host = L"Host";
const wchar_t *rsNet_Port = L"Port";
const wchar_t *rsNet_RecvSide = L"Is_Recv";

const wchar_t *rsInput_Values_File = L"Input_File";
const wchar_t *rsInput_Segment_Spacing = L"Segment_Spacing";

const wchar_t *rsHold_Values_Delay = L"Hold_Delay";

const wchar_t *rsSignal_Source_Id = L"Signal_Src_Id";
const wchar_t *rsSignal_Destination_Id = L"Signal_Dst_Id";

const wchar_t *rsRecalculate_Past_On_Params = L"Recalculate_Past_On_Params";
//const wchar_t *rsRecalculate_Past_On_Segment_Stop = L"Recalculate_Past_On_Segment_Stop";

const wchar_t *rsGen_IG_Offset = L"IG_Level_Offset";
const wchar_t *rsGen_IG_Amplitude = L"IG_Amplitude";
const wchar_t *rsGen_IG_Sin_Period = L"IG_Sinus_Period";
const wchar_t *rsGen_IG_Sampling_Period = L"IG_Sampling_Period";
const wchar_t *rsGen_BG_Level_Offset = L"BG_Level_Offset";
const wchar_t *rsGen_BG_Amplitude = L"BG_Amplitude";
const wchar_t *rsGen_BG_Cos_Period = L"BG_Cosinus_Period";
const wchar_t *rsGen_BG_Sampling_Period = L"BG_Sampling_Period";
const wchar_t *rsGen_Total_Time = L"Total_Time";

const wchar_t *rsParameters_Reset = L"Model_Signal_Params_Reset";
const wchar_t *rsParameters_Reset_Request = L"Model_Signal_Params_Reset_Request";
const wchar_t *rsSegment_Recalculate_Complete = L"Segment_Recalculate_Complete";

const wchar_t *rsSelected_Model = L"Model";
const wchar_t *rsSelected_Metric = L"Metric";
const wchar_t *rsSelected_Solver = L"Solver";
const wchar_t *rsSelected_Signal = L"Signal";
const wchar_t *rsSelected_Model_Bounds = L"Model_Bounds";
const wchar_t *rsUse_Relative_Error = L"Relative_Error";
const wchar_t *rsUse_Squared_Diff = L"Squared_Diff";
const wchar_t *rsUse_Prefer_More_Levels = L"Prefer_More_Levels";
const wchar_t *rsMetric_Threshold = L"Metric_Threshold";
const wchar_t *rsMetric_Levels_Required = L"Levels_Required";
const wchar_t *rsUse_Measured_Levels = L"Measured_Levels";

const wchar_t *rsRecalculate_On_Levels_Count = L"Recalculate_Levels_Count";
const wchar_t *rsRecalculate_On_Segment_End = L"Recalculate_At_Segment_End";
const wchar_t *rsRecalculate_On_Calibration = L"Recalculate_On_Calibration";

const wchar_t *rsLog_Output_File = L"Log_File";
const wchar_t *rsIgnore_Shutdown_Msg = L"Ignore_Shutdown";

const wchar_t *rsPump_Interval = L"Pump_Interval";
const wchar_t *rsFeedback_Channel_Identifier = L"Feedback_Channel_Identifier";
const wchar_t *rsDevice_Driver_Id = L"Device_Driver_Id";
const wchar_t *rsSynchronize_With_Pump = L"Synchronize_With_Pump";

const wchar_t *rsDrawing_Filter_Period = L"Redraw_Period";
const wchar_t *rsDiagnosis_Is_Type2 = L"Diabetes_Type_2";
const wchar_t *rsDrawing_Filter_Canvas_Width = L"Canvas_Width";
const wchar_t *rsDrawing_Filter_Canvas_Height = L"Canvas_Height";
const wchar_t *rsDrawing_Filter_Filename_Graph = L"Graph_File_Path";
const wchar_t *rsDrawing_Filter_Filename_Day = L"Day_File_Path";
const wchar_t *rsDrawing_Filter_Filename_AGP = L"AGP_File_Path";
const wchar_t *rsDrawing_Filter_Filename_Parkes = L"Parkes_File_Path";
const wchar_t *rsDrawing_Filter_Filename_Clark = L"Clark_File_Path";
const wchar_t *rsDrawing_Filter_Filename_ECDF = L"ECDF_File_Path";

const wchar_t *rsCallback_Log_Message = L"CallbackLog_Message";
const wchar_t *rsCallback_Drawing_Graph = L"CallbackDrawing_Graph";
const wchar_t *rsCallback_Drawing_Day = L"CallbackDrawing_Day";
const wchar_t *rsCallback_Drawing_Clark = L"CallbackDrawing_Clark";
const wchar_t *rsCallback_Drawing_Parkes = L"CallbackDrawing_Parkes";
const wchar_t *rsCallback_Drawing_Parkes_Type2 = L"CallbackDrawing_Parkes_Type2";
const wchar_t *rsCallback_Drawing_AGP = L"CallbackDrawing_AGP";

const wchar_t *rsInfo_Redraw_Complete = L"RedrawComplete";
const wchar_t *rsInfo_Solver_Progress = L"SolverProgress";
const wchar_t *rsInfo_Solver_Failed = L"SolverFailed";
const wchar_t *rsInfo_Solver_Completed_But_No_Improvement = L"Solver_Completed_But_No_Improvement";
const wchar_t *rsInfo_Error_Metrics_Ready = L"ErrorMetricsReady";
const wchar_t *rsInfo_Shut_Down_Marker = L"Shut_Down_Marker";

const wchar_t *rsConfig_File_Name = L"config.ini";
const wchar_t *rsFilter_Section_Prefix = L"Filter";
const wchar_t rsFilter_Section_Separator = L'_';
const wchar_t *rsIni_Comment_Prefix = L"; ";

const wchar_t* rsSolversDir = L"filters";

const char* rsFilter_Get_SVG_AGP = "get_svg_agp";
const char* rsFilter_Get_SVG_Clark = "get_svg_clark";
const char* rsFilter_Get_SVG_Day = "get_svg_day";
const char* rsFilter_Get_SVG_Graph = "get_svg_graph";
const char* rsFilter_Get_SVG_Parkes = "get_svg_parkes";
const char* rsFilter_Get_SVG_Parkes_Type2 = "get_svg_parkes_type2";

const char* rsFilter_Get_Errors = "get_error_metrics";

const wchar_t* rsSelect_Subjects_And_Segments_For_Db_Reader_Filter = L"select timesegment.id, subject.name, timesegment.name, count(measuredvalue.id) from subject, timesegment, measuredvalue where subject.id = timesegment.subjectid and timesegment.id = measuredvalue.segmentid group by timesegment.id, subject.name order by subject.name, timesegment.name asc";
const wchar_t* rsSelect_Subjects = L"select id, name from subject";
const wchar_t* rsSelect_Timesegment_Values_Filter = L"select measuredat, blood, ist, isig, insulin_bolus, insulin_basal_rate, carbohydrates, calibration from measuredvalue where segmentid = ? order by measuredat asc";
const wchar_t* rsSelect_Params_Base = L"select ";
const wchar_t* rsSelect_Params_From = L" from ";
const wchar_t* rsSelect_Params_Condition = L" where segmentid = ?";

// ---- drawing-related constants

const wchar_t* rsDrawingLocaleTitle = L"title";
const wchar_t* rsDrawingLocaleTitleDay = L"titleDay";
const wchar_t* rsDrawingLocaleSubtitle = L"subtitle";
const wchar_t* rsDrawingLocaleDiabetes1 = L"diabetes_type_1";
const wchar_t* rsDrawingLocaleDiabetes2 = L"diabetes_type_2";
const wchar_t* rsDrawingLocaleTime = L"time";
const wchar_t* rsDrawingLocaleTimeDay = L"timeDay";
const wchar_t* rsDrawingLocaleConcentration = L"concentration";
const wchar_t* rsDrawingLocaleHypoglycemy = L"hypoglycemia";
const wchar_t* rsDrawingLocaleHyperglycemy = L"hyperglycemia";
const wchar_t* rsDrawingLocaleBlood = L"blood";
const wchar_t* rsDrawingLocaleBloodCalibration = L"bloodCalibration";
const wchar_t* rsDrawingLocaleIst = L"ist";
const wchar_t* rsDrawingLocaleResults = L"results";
const wchar_t* rsDrawingLocaleDiff2 = L"diff2";
const wchar_t* rsDrawingLocaleDiff3 = L"diff3";
const wchar_t* rsDrawingLocaleQuantile = L"quantil";
const wchar_t* rsDrawingLocaleRelative = L"relative";
const wchar_t* rsDrawingLocaleAbsolute = L"absolute";
const wchar_t* rsDrawingLocaleAverage = L"average";
const wchar_t* rsDrawingLocaleType = L"type";
const wchar_t* rsDrawingLocaleError = L"error";
const wchar_t* rsDrawingLocaleDescription = L"description";
const wchar_t* rsDrawingLocaleColor = L"color";
const wchar_t* rsDrawingLocaleCounted = L"counted";
const wchar_t* rsDrawingLocaleMeasured = L"measured";
const wchar_t* rsDrawingLocaleAxisX = L"axis_x";
const wchar_t* rsDrawingLocaleAxisY = L"axis_y";
const wchar_t* rsDrawingLocaleSvgDatetimeTitle = L"title_datetime";
const wchar_t* rsDrawingLocaleSvgIstTitle = L"title_ist";
const wchar_t* rsDrawingLocaleSvgBloodTitle = L"title_blood";
const wchar_t* rsDrawingLocaleSvgBloodCalibrationTitle = L"title_blood_calibration";
const wchar_t* rsDrawingLocaleSvgInsulinTitle = L"title_insulin";
const wchar_t* rsDrawingLocaleSvgCarbohydratesTitle = L"title_carbohydrates";
const wchar_t* rsDrawingLocaleSvgISIGTitle = L"title_isig";
const wchar_t* rsDrawingLocaleSvgDiff2Title = L"title_diff2";
const wchar_t* rsDrawingLocaleSvgDiff3Title = L"title_diff3";
const wchar_t* rsDrawingLocaleRelativeError = L"rel_error";
const wchar_t* rsDrawingLocaleCummulativeProbability = L"cummulative_probability";
const wchar_t* rsDrawingLocaleElevatedGlucose = L"elevatedglucose";

const wchar_t* rsCannot_Get_Parameters = L"Cannot get parameters!";
const wchar_t* rsLog_Date_Time_Format = L"%Y-%m-%d %H:%M:%S";

const wchar_t* rsLog_CSV_Separator = L"; ";
const char* rsRattime_Edit_Mask = "h:mm:ss.zzz";
const wchar_t *rsShutdown_After_Last = L"Shutdown_After_Last";
const wchar_t *rsMinimum_Segment_Levels = L"Minimum_Segment_Levels";
const wchar_t *rsRequire_IG_BG = L"Require_IG_BG";

const wchar_t* rsConstantParam = L"c";

const wchar_t *rsGenerate_Primary_Keys = L"Generate_Primary_Keys";
const wchar_t *rsStore_Data = L"Store_Data";
const wchar_t *rsStore_Parameters = L"Store_Parameters";
const wchar_t *rsSubject_Id = L"Subject_Id";

const wchar_t* rsReserved_Segment_Name = L"RESERVED_SEGMENT_NAME";
const wchar_t* rsFound_New_Segment = L"insert into timesegment (name, comment, deleted) values (?, ?, ?)";
const wchar_t* rsSelect_Founded_Segment = L"select id from timesegment where name = ?";
const wchar_t* rsUpdate_Founded_Segment = L"update timesegment set name = ?, comment = ?, deleted=?, subjectid=?, parallel_id=? where id=?";

const wchar_t* rsReserved_Subject_Name = L"RESERVED_SUBJECT_NAME";
const wchar_t* rsFound_New_Subject = L"insert into subject (name, comments, sex, weight) values (?, ?, ?, ?)";
const wchar_t* rsSelect_Founded_Subject = L"select id from subject where name = ?";
const wchar_t* rsUpdate_Founded_Subject = L"update subject set name = ?, comments = ?, sex = ?, weight = ? where id=?";

const wchar_t* rsPrediction_Window = L"Prediction_Window";
const wchar_t* rsSolve_Parameters = L"Solve_Parameters";
const wchar_t* rsSolve_Using_All_Segments = L"Solve_Using_All_Segments";
const wchar_t* rsSolve_On_Level_Count = L"Solve_On_Level_Count";
const wchar_t* rsSolve_On_Calibration = L"Solve_On_Calibration";
const wchar_t* rsSolve_On_Time_Segment_End = L"Solve_On_Time_Segment_End";

const wchar_t* rsMatlab_Manifest_File = L"matlab_manifest.xml";
const wchar_t* rsMatlab_Manifest_Session_Name_Path = L"manifest.session:name";
const wchar_t* rsMatlab_Manifest_Workdir_Path = L"manifest.directory:path";
const wchar_t* rsMatlab_Manifest_Models_Path = L"manifest.models";
const wchar_t* rsMatlab_Manifest_Solvers_Path = L"manifest.solvers";
const wchar_t* rsMatlab_Manifest_Model_Tag = L"model";
const wchar_t* rsMatlab_Manifest_Solver_Tag = L"solver";
const wchar_t* rsMatlab_Manifest_Id_Parameter = L"id";
const wchar_t* rsMatlab_Manifest_Description_Parameter = L"description";
const wchar_t* rsMatlab_Manifest_Name_Parameter = L"name";
const wchar_t* rsMatlab_Manifest_DB_Table_Parameter = L"dbtable";
const wchar_t* rsMatlab_Manifest_Parameters_Tag = L"parameters";
const wchar_t* rsMatlab_Manifest_Parameter_Tag = L"param";
const wchar_t* rsMatlab_Manifest_DB_Column_Parameter = L"dbname";
const wchar_t* rsMatlab_Manifest_Default_Parameter = L"default";
const wchar_t* rsMatlab_Manifest_Param_Lowbound = L"lowerbound";
const wchar_t* rsMatlab_Manifest_Param_Upbound = L"upperbound";
const wchar_t* rsMatlab_Manifest_Signals_Tag = L"signals";
const wchar_t* rsMatlab_Manifest_Signal_Tag = L"signal";
const wchar_t* rsMatlab_Manifest_Reference_Signal_Id_Param = L"reference";
const wchar_t* rsMatlab_Manifest_Script_Name = L"scriptname";
const wchar_t* rsMatlab_Manifest_Models_Tag = L"models";

const char16_t* rsMatlab_Variable_Model_Parameters = u"model_parameters";
const char16_t* rsMatlab_Variable_Model_Times = u"model_times";
const char16_t* rsMatlab_Variable_Model_Derivation = u"model_derivation";
const char16_t* rsMatlab_Variable_Model_Output = u"model_output";
const char16_t* rsMatlab_Variable_Solver_Times = u"solver_times";
const char16_t* rsMatlab_Variable_Solver_Values = u"solver_values";
const char16_t* rsMatlab_Variable_Solver_Hints = u"solver_hints";
const char16_t* rsMatlab_Variable_Solver_Lowbounds = u"solver_lowerbounds";
const char16_t* rsMatlab_Variable_Solver_Upbounds = u"solver_upperbounds";
const char16_t* rsMatlab_Variable_Solver_Output = u"solver_output";

const wchar_t* rsT1DMS_Feedback_Request = L"T1DMS_Feedback_Request";
const wchar_t* rsBergman_Feedback_Request = L"Bergman_Feedback_Request";

const wchar_t* rsOref0_Manifest_File = L"oref0_manifest.xml";
const wchar_t* rsOref0_Manifest_Workdir_Path = L"manifest.workdir:path";
const wchar_t* rsOref0_Manifest_Runner_Script_Path = L"manifest.runner:path";
const wchar_t* rsOref0_Manifest_Node_Command_Path = L"manifest.node:command";
const wchar_t* rsOref0_Manifest_Socket_Timeout_Path = L"manifest.socket:timeout";
const wchar_t* rsOref0_Manifest_Socket_Port_Path = L"manifest.socket:port";
const wchar_t* rsOref0_Manifest_Default_Runner_Script = L"runner.js";
const wchar_t* rsOref0_Manifest_Default_Node_Command = L"node";

//**************
const wchar_t *rsCalculate_Past_New_Params = L"Calc_Past_First_Params";
const wchar_t *rsRecalculate_With_Every_Params = L"Recalc_Every_Params";
const wchar_t *rsUse_Just_Opened_Segments = L"Use_Just_Opened_Segments";
const wchar_t *rsHold_During_Solve = L"Hold_During_Solve";


const wchar_t* rsParameters = L"Parameters";
const wchar_t* rsFeedback_Name = L"Feedback_Name";
const wchar_t* rsSynchronize_to_Signal = L"Synchronize_To_Signal";
const wchar_t* rsSynchronization_Signal = L"Synchronization_Signal";
const wchar_t* rsStepping = L"Stepping";
const wchar_t* rsMaximum_Time = L"Maximum_Time";

const wchar_t* rsReference_Signal = L"Reference_Signal";
const wchar_t* rsError_Signal = L"Error_Signal";
const wchar_t* rsDescription = L"Description";

const wchar_t* rsIOB_Model = L"iob";
const wchar_t* rsCOB_Model = L"cob";


const wchar_t* rsParam_K = L"k";
const wchar_t* rsKiDecay = L"kidecay";

const wchar_t* rsLGS_Basal_Insulin_Model = L"lgs_basal_insulin_rate";
const wchar_t* rsLGS_Lower_Threshold = L"Lower_Threshold";
const wchar_t* rsLGS_Suspend_Duration = L"Suspend_Duration";

const wchar_t* rsConst_ISF = L"const_insulin_sensitivity_factor";
const wchar_t* rsConst_CR = L"const_carb_to_insulin_ratio";

const wchar_t* rsForward_Clone = L"Forward_a_clone";