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
const char* dsStep = "Step";

const char* dsFilters_Window = "Filters window";
const char* dsApplied_Filters = "Applied filters";
const char* dsAvailable_Filters = "Available filters";
const char* dsSimulation_Window = "Simulation window";

const wchar_t *dsDb_Reader = L"Db Reader";

const wchar_t *dsDb_Host = L"Host";
const wchar_t *dsDb_Port = L"Port";
const wchar_t *dsDb_Provider = L"Provider";
const wchar_t *dsDb_Name = L"Name";
const wchar_t *dsDb_User_Name = L"User name";
const wchar_t *dsDb_Password = L"Password";
const wchar_t *dsTime_Segment_ID = L"Time Segment Id";

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

const wchar_t *dsHold_Filter = L"Hold";
const wchar_t *dsErrors_Filter = L"Error metrics";
const wchar_t *dsDrawing_Filter = L"Drawing";
const wchar_t *dsMapping_Filter = L"Signal mapping";
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

const wchar_t *dsDevice_Interop_Filter = L"Real device interface";

const wchar_t *dsInterop_Export_Filter = L"Interop export filter";

const wchar_t *dsConsume_Filter = L"/dev/null";

const wchar_t *dsLog_Filter = L"Log";
const wchar_t *dsLog_Output_File = L"Output file";

const wchar_t *dsGUI_Filter = L"Visualization";
const wchar_t *dsUser_Input_Filter = L"User input";

const wchar_t *dsCalculate_Past_New_Params = L"Calculate past with first parameter set";
const wchar_t *dsRecalculate_With_Every_Params = L"Recalculate with every parameter set";
const wchar_t *dsUse_Just_Opened_Segments = L"Use currently opened segments";

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

const wchar_t *dsLine_Approx = L"Line";

const char *dsDrawing_Tab_Graph = "Single plot";
const char *dsDrawing_Tab_Day = "Daily plot";
const char *dsDrawing_Tab_Clark = "Clarke's Grid";
const char *dsDrawing_Tab_Parkes = "Parkes' Grid";
const char *dsDrawing_Tab_AGP = "AGP";

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
const wchar_t *dsSignal_Virtual_Base = L"dummy";
const wchar_t *dsSignal_Suffix_Measured = L"measured";
const wchar_t *dsSignal_Suffix_Calculated = L"calculated";
const wchar_t *dsSignal_Suffix_Virtual = L"virtual";
const wchar_t *dsSignal_Unknown = L"Unknown";

const wchar_t *dsSolver_Progress_Box_Title = L"Solver progress";
const wchar_t *dsError_Absolute = L"absolute";
const wchar_t *dsError_Relative = L"relative";

const wchar_t *dsError_Column_Average = L"Average";
const wchar_t *dsError_Column_StdDev = L"Sample Std. Deviation";
const wchar_t *dsError_Column_AIC = L"AIC";
const wchar_t *dsError_Column_Minimum = L"Minimum";
const wchar_t *dsError_Column_First_Quantile = L"1. Quartile";
const wchar_t *dsError_Column_Median = L"Median";
const wchar_t *dsError_Column_Third_Quantile = L"3. Quartile";
const wchar_t *dsError_Column_95_Quantile = L"95% Quantile";
const wchar_t *dsError_Column_99_Quantile = L"99% Quantile";
const wchar_t *dsError_Column_Maximum = L"Maximum";

// ---- drawing-related constants

const wchar_t* dsDrawingLocaleTitle = L"Calculated Blood Glucose Level";
const wchar_t* dsDrawingLocaleTitleAgp = L"Ambulatory Glucose Profile";
const wchar_t* dsDrawingLocaleTitleClark = L"Clarke's error grid";
const wchar_t* dsDrawingLocaleTitleParkes = L"Parkes' error grid";
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
const wchar_t *rsDb_User_Name = L"UserName";
const wchar_t *rsDb_Password = L"Password";
const wchar_t *rsTime_Segment_ID = L"TimeSegmentId";

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
const wchar_t* rsBeta_Column = L"b";
const wchar_t* rsGamma_Column = L"gamma";

const wchar_t *rsNet_Host = L"Host";
const wchar_t *rsNet_Port = L"Port";
const wchar_t *rsNet_RecvSide = L"IsRecv";

const wchar_t *rsInput_Values_File = L"InputFile";
const wchar_t *rsInput_Segment_Spacing = L"SegmentSpacing";

const wchar_t *rsHold_Values_Delay = L"HoldDelay";

const wchar_t *rsSignal_Source_Id = L"SignalSrcId";
const wchar_t *rsSignal_Destination_Id = L"SignalDstId";

const wchar_t *rsCalculate_Past_New_Params = L"CalcPastFirstParamSet";
const wchar_t *rsRecalculate_With_Every_Params = L"RecalcEveryParamSet";
const wchar_t *rsUse_Just_Opened_Segments = L"UseJustOpenedSegments";

const wchar_t *rsParameters_Reset = L"ModelSignalParamsReset";
const wchar_t *rsParameters_Reset_Request = L"ModelSignalParamsResetRequest";
const wchar_t *rsSegment_Recalculate_Complete = L"SegmentRecalculateComplete";

const wchar_t *rsSelected_Model = L"Model";
const wchar_t *rsSelected_Metric = L"Metric";
const wchar_t *rsSelected_Solver = L"Solver";
const wchar_t *rsSelected_Signal = L"Signal";
const wchar_t *rsSelected_Model_Bounds = L"ModelBounds";
const wchar_t *rsUse_Relative_Error = L"RelativeError";
const wchar_t *rsUse_Squared_Diff = L"SquaredDiff";
const wchar_t *rsUse_Prefer_More_Levels = L"PreferMoreLevels";
const wchar_t *rsMetric_Threshold = L"MetricThreshold";
const wchar_t *rsMetric_Levels_Required = L"LevelsRequired";
const wchar_t *rsUse_Measured_Levels = L"MeasuredLevels";

const wchar_t *rsRecalculate_On_Levels_Count = L"RecalculateLevelsCount";
const wchar_t *rsRecalculate_On_Segment_End = L"RecalculateAtSegmentEnd";
const wchar_t *rsRecalculate_On_Calibration = L"RecalculateOnCalibration";

const wchar_t *rsLog_Output_File = L"OutputFile";

const wchar_t *rsDrawing_Filter_Period = L"RedrawPeriod";
const wchar_t *rsDiagnosis_Is_Type2 = L"DiabetesType2";
const wchar_t *rsDrawing_Filter_Canvas_Width = L"CanvasWidth";
const wchar_t *rsDrawing_Filter_Canvas_Height = L"CanvasHeight";
const wchar_t *rsDrawing_Filter_Filename_Graph = L"GraphFilePath";
const wchar_t *rsDrawing_Filter_Filename_Day = L"DayFilePath";
const wchar_t *rsDrawing_Filter_Filename_AGP = L"AgpFilePath";
const wchar_t *rsDrawing_Filter_Filename_Parkes = L"ParkesFilePath";
const wchar_t *rsDrawing_Filter_Filename_Clark = L"ClarkFilePath";

const wchar_t *rsCallback_Log_Message = L"CallbackLog_Message";
const wchar_t *rsCallback_Drawing_Graph = L"CallbackDrawing_Graph";
const wchar_t *rsCallback_Drawing_Day = L"CallbackDrawing_Day";
const wchar_t *rsCallback_Drawing_Clark = L"CallbackDrawing_Clark";
const wchar_t *rsCallback_Drawing_Parkes = L"CallbackDrawing_Parkes";
const wchar_t *rsCallback_Drawing_Parkes_Type2 = L"CallbackDrawing_Parkes_Type2";
const wchar_t *rsCallback_Drawing_AGP = L"CallbackDrawing_AGP";

const wchar_t *rsInfo_Redraw_Complete = L"RedrawComplete";
const wchar_t *rsInfo_Solver_Progress = L"SolverProgress";
const wchar_t *rsInfo_Error_Metrics_Ready = L"ErrorMetricsReady";

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

const char* rsSelect_Subjects_And_Segments_For_Db_Reader_Filter = "select timesegment.id, subject.name, timesegment.name, count(measuredvalue.id) from subject, timesegment, measuredvalue where subject.id = timesegment.subjectid and timesegment.id = measuredvalue.segmentid group by timesegment.id, subject.name order by subject.name, timesegment.name asc";
const char* rsSelect_Timesegment_Values_Filter = "select measuredat, blood, ist, isig, insulin, carbohydrates, calibration from measuredvalue where segmentid = ? order by measuredat asc";
const char* rsSelect_Params_Base = "select ";
const char* rsSelect_Params_From = " from ";
const char* rsSelect_Params_Condition = " where segmentid = ?";

// ---- drawing-related constants

const wchar_t* rsDrawingLocaleTitle = L"title";
const wchar_t* rsDrawingLocaleTitleDay = L"titleDay";
const wchar_t* rsDrawingLocaleSubtitle = L"subtitle";
const wchar_t* rsDrawingLocaleDiabetes1 = L"diabetes_type_1";
const wchar_t* rsDrawingLocaleDiabetes2 = L"diabetes_type_2";
const wchar_t* rsDrawingLocaleTime = L"time";
const wchar_t* rsDrawingLocaleTimeDay = L"timeDay";
const wchar_t* rsDrawingLocaleConcentration = L"concentration";
const wchar_t* rsDrawingLocaleHypoglycemy = L"hypoglycemy";
const wchar_t* rsDrawingLocaleHyperglycemy = L"hyperglycemy";
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
