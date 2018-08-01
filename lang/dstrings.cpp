#include "dstrings.h"

const char* dsFile = "File";
const char* dsSave_Configuration = "Save Configruation";
const char* dsQuit = "Quit";
const char* dsTools = "Tools";
const char* dsFilters = "Filters";
const char* dsSimulation = "Simulation";

const char* dsLog_Tab = "Log";
const char* dsErrors_Tab = "Errors";

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
const char* dsConfigure= "Configure";
const char* dsMove_Up = "Move up";
const char* dsMove_Down = "Move down";

const char* dsStart = "Start";
const char* dsStop = "Stop";
const char* dsSolve = "Solve";
const char* dsSolve_Reset = "Reset and solve";
const char* dsSuspend = "Suspend solvers";
const char* dsResume = "Resume solvers";
const char* dsStepUnit = "levels";
const char* dsStep = "Step";

const char* dsFilters_Window = "Filters window";
const char* dsApplied_Filters = "Applied filters";
const char* dsAvailable_Filters = "Available filters";
const char* dsSimulation_Window = "Simulation window";

const wchar_t *dsDb_Reader = L"Db Reader";
const wchar_t *dsDb_Writer = L"Db Writer";

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

const wchar_t *dsNet_Comm = L"Network";
const wchar_t *dsNet_Host = L"Host/Bind address";
const wchar_t *dsNet_Port = L"Port";
const wchar_t *dsNet_RecvSide = L"Receiving side?";

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

const wchar_t *dsHold_Values_Delay = L"Hold delay [ms] (0 for real-time)";

const wchar_t *dsSignal_Source_Id = L"Source ID";
const wchar_t *dsSignal_Destination_Id = L"Map to ID";

const wchar_t *rsSignal_Masked_Id = L"Signal_Masked_Id";
const wchar_t *rsSignal_Value_Bitmask = L"Signal_Value_Bitmask";

const wchar_t *dsSignal_Masked_Id = L"Signal ID";
const wchar_t *dsSignal_Value_Bitmask = L"Value bitmask (64 bit)";

const wchar_t *dsHold_Filter = L"Hold";
const wchar_t *dsErrors_Filter = L"Error metrics";
const wchar_t *dsDrawing_Filter = L"Drawing";
const wchar_t *dsMapping_Filter = L"Signal mapping";
const wchar_t *dsMasking_Filter = L"Signal value masking";
const wchar_t *dsCalculate_Filter = L"Calculate";

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
const wchar_t *dsLog_Output_File = L"Output file";
const wchar_t *dsLog_Header = L"Logical Clock; Device Time; Event Code; Signal; Info; Segment Id; Event Code Id; Device Id; Signal Id;";
const wchar_t *dsLog_Filter_Replay = L"CSV File Log Replay";

const wchar_t *dsGUI_Filter = L"Visualization";

const char* dsSave_Image_To_File = "Save to file";
const char* dsSave_Image_Ext_Spec = "SVG Image (*.svg)";

const char* dsSave_Image_Default_Filename_Graph = "graph.svg";
const char* dsSave_Image_Default_Filename_Day = "day.svg";
const char* dsSave_Image_Default_Filename_Parkes = "parkes.svg";
const char* dsSave_Image_Default_Filename_Clark = "clark.svg";
const char* dsSave_Image_Default_Filename_AGP = "agp.svg";
const char* dsSave_Image_Default_Filename_ECDF = "ecdf.svg";


//const wchar_t *dsCalculate_Past_New_Params = L"Calculate past with first parameter set";
//const wchar_t *dsRecalculate_With_Every_Params = L"New parameters recalculate entire segment";
//const wchar_t* dsRecalculate_Past_On_Params = L"Recalculate entire segment with new parameters";
//const wchar_t* dsRecalculate_Past_On_Segment_Stop = L"Recalculate entire segment on its end";
const wchar_t* dsPrediction_Window = L"Prediction window";

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

const wchar_t* dsDiffusion_Model_v2 = L"Diffusion model v2";
const wchar_t* dsSteil_Rebrin = L"Steil-Rebrin";

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

const wchar_t* dsNewUOA = L"NewUOA";
const wchar_t* dsMetaDE = L"MetaDE";
const wchar_t* dsBOBYQA = L"BOBYQA";

const wchar_t *dsLine_Approx = L"Line";
const wchar_t *dsAkima = L"Akima";

const char *dsDrawing_Tab_Graph = "Single plot";
const char *dsDrawing_Tab_Day = "Daily plot";
const char *dsDrawing_Tab_Clark = "Clarke's Grid";
const char *dsDrawing_Tab_Parkes = "Parkes' Grid";
const char *dsDrawing_Tab_AGP = "AGP";
const char *dsDrawing_Tab_ECDF = "ECDF";

const char *dsLower_Bounds = "Lower bounds";
const char *dsDefault_Parameters = "Default parameters";
const char *dsUpper_Bounds = "Upper bounds";

const char *dsReset_Bounds = "Reset";

const wchar_t *dsSignal_Measured_BG = L"blood glucose";
const wchar_t *dsSignal_Measured_IG = L"interstitial fluid glucose";
const wchar_t *dsSignal_Measured_ISIG = L"ISIG";
const wchar_t *dsSignal_Measured_Calibration = L"blood glucose calibration";
const wchar_t *dsSignal_Measured_Insulin = L"insulin amount";
const wchar_t *dsSignal_Measured_Carb_Intake = L"carbohydrates intake";
const wchar_t *dsSignal_Measured_Health_Stress = L"health stress";
//const wchar_t *dsSignal_Virtual_Base = L"dummy";
const wchar_t *dsSignal_Suffix_Measured = L"measured";
const wchar_t *dsSignal_Suffix_Calculated = L"calculated";
//const wchar_t *dsSignal_Suffix_Virtual = L"virtual";
const wchar_t *dsSignal_Unknown = L"Unknown";
const wchar_t *dsSignal_Prefix_Virtual = L"virtual";

const wchar_t *dsSolver_Progress_Box_Title = L"Solver progress";
const wchar_t *dsError_Absolute = L"absolute";
const wchar_t *dsError_Relative = L"relative";

const wchar_t *dsError_Column_Average = L"Average";
const wchar_t *dsError_Column_StdDev = L"Std.Dev (Bessel)";
const wchar_t *dsError_Column_AIC = L"AIC";
const wchar_t *dsError_Column_Minimum = L"Minimum";
const wchar_t *dsError_Column_First_Quantile = L"1. Quartile";
const wchar_t *dsError_Column_Median = L"Median";
const wchar_t *dsError_Column_Third_Quantile = L"3. Quartile";
const wchar_t *dsError_Column_95_Quantile = L"95% Quantile";
const wchar_t *dsError_Column_99_Quantile = L"99% Quantile";
const wchar_t *dsError_Column_Maximum = L"Maximum";

const wchar_t* rsInsert_New_Measured_Value = L"INSERT INTO measuredvalue (measuredat, blood, ist, isig, insulin, carbohydrates, calibration, segmentid) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

const wchar_t* rsInsert_Params_Base = L"INSERT INTO ";
const wchar_t* rsInsert_Params_Segmentid_Column = L"segmentid";
const wchar_t* rsInsert_Params_Values_Stmt = L"VALUES";


// ---- drawing-related constants

const wchar_t* dsDrawingLocaleTitle = L"Calculated Blood Glucose Level";
const wchar_t* dsDrawingLocaleTitleAgp = L"Ambulatory Glucose Profile";
const wchar_t* dsDrawingLocaleTitleClark = L"Clarke's error grid";
const wchar_t* dsDrawingLocaleTitleParkes = L"Parkes' error grid";
const wchar_t* dsDrawingLocaleTitleECDF = L"ECDF";
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

//xxxx

const wchar_t *dsCalculate_Past_New_Params = L"Calculate past with first parameter set";
const wchar_t *dsRecalculate_With_Every_Params = L"New parameters recalculate entire segment";
const wchar_t *dsHold_During_Solve = L"Hold during solve";

const wchar_t *dsHold_During_Solve_Tooltip = L"Hold all incoming messages while solver is in progress";

//--------------------------------- do not translate any of the rs-prefixed texts --

const char* rsAbout_Text =	"<b>Glucose Predictor</b>, Version 3.0 Alpha<br>&nbsp;<br>"\
							"Tom\xc3\xa1\xc5\xa1 Koutn\xc3\xbd, Ph.D.<br><i>txkoutny@kiv.zcu.cz</i><br>&nbsp;<br>"\
							"Department of Computer Science and Engineering<br>"\
							"University of West Bohemia<br>"\
							"Plze\xc5\x88 306 14, Czech Republic<br>"\
							"&nbsp;<hr>diabetes.zcu.cz";

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

const wchar_t* rsP_Column = L"p";
const wchar_t* rsCg_Column = L"cg";
const wchar_t* rsC_Column = L"c";
const wchar_t* rsDt_Column = L"dt";
const wchar_t* rsK_Column = L"k";
const wchar_t* rsH_Column = L"h";

const wchar_t* rsTau_Column = L"t";
const wchar_t* rsAlpha_Column = L"alpha";
const wchar_t* rsBeta_Column = L"beta";
const wchar_t* rsGamma_Column = L"gamma";

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
const wchar_t *rsRecalculate_On_Segment_End = L"Recalculate_A_tSegment_End";
const wchar_t *rsRecalculate_On_Calibration = L"Recalculate_On_Calibration";

const wchar_t *rsLog_Output_File = L"Output_File";

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
const wchar_t *rsInfo_Error_Metrics_Ready = L"ErrorMetricsReady";
const wchar_t *rsInfo_Marker = L"Marker";

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
const wchar_t* rsSelect_Timesegment_Values_Filter = L"select measuredat, blood, ist, isig, insulin, carbohydrates, calibration from measuredvalue where segmentid = ? order by measuredat asc";
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

const wchar_t *rsGenerate_Primary_Keys = L"Generate_Primary_Keys";
const wchar_t *rsStore_Data = L"Store_Data";
const wchar_t *rsStore_Parameters = L"Store_Parameters";

const wchar_t* rsReserved_Segment_Name = L"RESERVED_SEGMENT_NAME";
const wchar_t* rsFound_New_Segment = L"insert into timesegment (name) values (?)";
const wchar_t* rsSelect_Founded_Segment = L"select id from timesegment where name = ?";
const wchar_t* rsUpdate_Founded_Segment = L"update timesegment set name = ?, comment = ?, deleted=?, subjectid=?, parallel_id=? where id=?";

//**************
const wchar_t *rsCalculate_Past_New_Params = L"Calc_Past_First_Params";
const wchar_t *rsRecalculate_With_Every_Params = L"Recalc_Every_Params";
const wchar_t *rsUse_Just_Opened_Segments = L"Use_Just_Opened_Segments";
const wchar_t *rsHold_During_Solve = L"Hold_During_Solve";
const wchar_t* rsPrediction_Window = L"Prediction_Window";

