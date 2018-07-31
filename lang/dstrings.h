#pragma once

//UI strings used by the entire application

extern const char* dsFile;
extern const char* dsSave_Configuration;
extern const char* dsQuit;
extern const char* dsTools;
extern const char* dsFilters;
extern const char* dsSimulation;

extern const char* dsLog_Tab;
extern const char* dsErrors_Tab;

extern const char* dsClose;
extern const char* dsClose_All;

extern const char* dsTile_Vertically;
extern const char* dsTile_Horizontally;
extern const char* dsCascade;
extern const char* dsNext;
extern const char* dsPrevious;

extern const char* dsWindow;
extern const char* dsHelp;
extern const char* dsAbout;
extern const char* dsAbout_Amp;
extern const char* dsGlucose_Prediction;

extern const char* dsAdd;
extern const char* dsDelete;
extern const char* dsRemove;
extern const char* dsConfigure;
extern const char* dsMove_Up;
extern const char* dsMove_Down;

extern const char* dsStart;
extern const char* dsStop;
extern const char* dsSolve;
extern const char* dsSolve_Reset;
extern const char* dsSuspend;
extern const char* dsResume;
extern const char* dsStepUnit;
extern const char* dsStep;

extern const char* dsFilters_Window;
extern const char* dsApplied_Filters;
extern const char* dsAvailable_Filters;
extern const char* dsSimulation_Window;

extern const wchar_t *dsDb_Reader;
extern const wchar_t *dsDb_Writer;

extern const wchar_t *dsDb_Host;
extern const wchar_t *dsDb_Port;
extern const wchar_t *dsDb_Provider;
extern const wchar_t *dsDb_Name;
extern const wchar_t *dsDb_User_Name;
extern const wchar_t *dsDb_Password;
extern const wchar_t *dsTime_Segment_ID;
extern const wchar_t *dsShutdown_After_Last;

extern const wchar_t *dsGenerate_Primary_Keys;
extern const wchar_t *dsStore_Data;
extern const wchar_t *dsStore_Parameters;

extern const wchar_t *dsNet_Comm;
extern const wchar_t *dsNet_Host;
extern const wchar_t *dsNet_Port;
extern const wchar_t *dsNet_RecvSide;

extern const wchar_t *dsSolver_Filter;

extern const wchar_t *dsSelected_Model;
extern const wchar_t *dsSelected_Metric;
extern const wchar_t *dsSelected_Solver;
extern const wchar_t *dsSelected_Signal;
extern const wchar_t *dsSelected_Model_Bounds;

extern const wchar_t *dsUse_Relative_Error;
extern const wchar_t *dsUse_Squared_Diff;
extern const wchar_t *dsUse_Prefer_More_Levels;
extern const wchar_t *dsMetric_Threshold;
extern const wchar_t *dsMetric_Levels_Required;
extern const wchar_t *dsUse_Measured_Levels;

extern const wchar_t *dsRecalculate_On_Levels_Count;
extern const wchar_t *dsRecalculate_On_Segment_End;
extern const wchar_t *dsRecalculate_On_Calibration;

extern const wchar_t *dsFile_Reader;
extern const wchar_t *dsInput_Values_File;
extern const wchar_t *dsInput_Segment_Spacing;

extern const wchar_t *dsHold_Values_Delay;

extern const wchar_t *dsSignal_Source_Id;
extern const wchar_t *dsSignal_Destination_Id;

extern const wchar_t *dsSignal_Masked_Id;
extern const wchar_t *dsSignal_Value_Bitmask;

extern const wchar_t *dsHold_Filter;
extern const wchar_t *dsErrors_Filter;
extern const wchar_t *dsDrawing_Filter;
extern const wchar_t *dsMapping_Filter;
extern const wchar_t *dsMasking_Filter;
extern const wchar_t *dsCalculate_Filter;

extern const wchar_t *dsDrawing_Filter_Period;
extern const wchar_t *dsDiagnosis_Is_Type2;
extern const wchar_t *dsDrawing_Filter_Canvas_Width;
extern const wchar_t *dsDrawing_Filter_Canvas_Height;
extern const wchar_t *dsDrawing_Filter_Filename_Graph;
extern const wchar_t *dsDrawing_Filter_Filename_Day;
extern const wchar_t *dsDrawing_Filter_Filename_AGP;
extern const wchar_t *dsDrawing_Filter_Filename_Parkes;
extern const wchar_t *dsDrawing_Filter_Filename_Clark;
extern const wchar_t *dsDrawing_Filter_Filename_ECDF;

extern const wchar_t *dsDevice_Interop_Filter;

extern const wchar_t *dsInterop_Export_Filter;

extern const wchar_t *dsLog_Filter;
extern const wchar_t *dsLog_Output_File;
extern const wchar_t *dsLog_Header;
extern const wchar_t *dsLog_Filter_Replay;

extern const wchar_t *dsGUI_Filter;

extern const char* dsSave_Image_To_File;
extern const char* dsSave_Image_Ext_Spec;

extern const char* dsSave_Image_Default_Filename_Graph;
extern const char* dsSave_Image_Default_Filename_Day;
extern const char* dsSave_Image_Default_Filename_Parkes;
extern const char* dsSave_Image_Default_Filename_Clark;
extern const char* dsSave_Image_Default_Filename_AGP;
extern const char* dsSave_Image_Default_Filename_ECDF;

//extern const wchar_t *dsCalculate_Past_New_Params;
//extern const wchar_t *dsRecalculate_With_Every_Params;
extern const wchar_t* dsRecalculate_Past_On_Params;
//extern const wchar_t* dsRecalculate_Past_On_Segment_Stop;
extern const wchar_t *dsUse_Just_Opened_Segments;
extern const wchar_t* dsPrediction_Window;

extern const char *dsConfiguration;
extern const char *dsSelect_Just_One_Item;
extern const char *dsSimulation_Is_In_Progress;

extern const char *dsInformation;
extern const char *dsMain_Parameters;

extern const char* dsOK;
extern const char* dsCancel;
extern const char* dsApply;
extern const char* dsCommit;

extern const char* dsSubject;
extern const char* dsSegment;
extern const char* dsValue_Count;

extern const wchar_t* dsAvg_Abs;
extern const wchar_t* dsMax_Abs;
extern const wchar_t* dsPerc_Abs;
extern const wchar_t* dsThresh_Abs;
extern const wchar_t* dsLeal_2010;
extern const wchar_t* dsAIC;
extern const wchar_t* dsStd_Dev;
extern const wchar_t* dsCrosswalk;
extern const wchar_t* dsIntegral_CDF;
extern const wchar_t* dsAvg_Plus_Bessel_Std_Dev;

extern const wchar_t* dsBlood;
extern const wchar_t* dsInterstitial;

extern const wchar_t* dsDiffusion_Model_v2;
extern const wchar_t* dsSteil_Rebrin;

extern const wchar_t* dsP;
extern const wchar_t* dsCg;
extern const wchar_t* dsC;
extern const wchar_t* dsDt;
extern const wchar_t* dsK;
extern const wchar_t* dsH;

extern const wchar_t* dsTau;
extern const wchar_t* dsAlpha;
extern const wchar_t* dsBeta;
extern const wchar_t* dsGamma;

extern const wchar_t* dsNewUOA;
extern const wchar_t* dsMetaDE;
extern const wchar_t* dsBOBYQA;

extern const wchar_t *dsLine_Approx;
extern const wchar_t *dsAkima;

extern const char *dsDrawing_Tab_Graph;
extern const char *dsDrawing_Tab_Day;
extern const char *dsDrawing_Tab_Clark;
extern const char *dsDrawing_Tab_Parkes;
extern const char *dsDrawing_Tab_AGP;
extern const char *dsDrawing_Tab_ECDF;

extern const char *dsLower_Bounds;
extern const char *dsDefault_Parameters;
extern const char *dsUpper_Bounds;

extern const char *dsReset_Bounds;

extern const wchar_t *dsSignal_Measured_BG;
extern const wchar_t *dsSignal_Measured_IG;
extern const wchar_t *dsSignal_Measured_ISIG;
extern const wchar_t *dsSignal_Measured_Calibration;
extern const wchar_t *dsSignal_Measured_Insulin;
extern const wchar_t *dsSignal_Measured_Carb_Intake;
extern const wchar_t *dsSignal_Measured_Health_Stress;
//extern const wchar_t *dsSignal_Virtual_Base;
extern const wchar_t *dsSignal_Suffix_Measured;
extern const wchar_t *dsSignal_Suffix_Calculated;
//extern const wchar_t *dsSignal_Suffix_Virtual;
extern const wchar_t *dsSignal_Unknown;
extern const wchar_t *dsSignal_Prefix_Virtual;

extern const wchar_t *dsSolver_Progress_Box_Title;
extern const wchar_t *dsError_Absolute;
extern const wchar_t *dsError_Relative;

extern const wchar_t *dsError_Column_Average;
extern const wchar_t *dsError_Column_StdDev;
extern const wchar_t *dsError_Column_AIC;
extern const wchar_t *dsError_Column_Minimum;
extern const wchar_t *dsError_Column_First_Quantile;
extern const wchar_t *dsError_Column_Median;
extern const wchar_t *dsError_Column_Third_Quantile;
extern const wchar_t *dsError_Column_95_Quantile;
extern const wchar_t *dsError_Column_99_Quantile;
extern const wchar_t *dsError_Column_Maximum;

extern const wchar_t* rsInsert_New_Measured_Value;

extern const wchar_t* rsInsert_Params_Base;
extern const wchar_t* rsInsert_Params_Segmentid_Column;
extern const wchar_t* rsInsert_Params_Values_Stmt;

// ---- drawing-related constants

extern const wchar_t* dsDrawingLocaleTitle;
extern const wchar_t* dsDrawingLocaleTitleAgp;
extern const wchar_t* dsDrawingLocaleTitleClark;
extern const wchar_t* dsDrawingLocaleTitleParkes;
extern const wchar_t* dsDrawingLocaleTitleECDF;
extern const wchar_t* dsDrawingLocaleTitleDay;
extern const wchar_t* dsDrawingLocaleSubtitle;
extern const wchar_t* dsDrawingLocaleDiabetes1;
extern const wchar_t* dsDrawingLocaleDiabetes2;
extern const wchar_t* dsDrawingLocaleTime;
extern const wchar_t* dsDrawingLocaleTimeDay;
extern const wchar_t* dsDrawingLocaleConcentration;
extern const wchar_t* dsDrawingLocaleHypoglycemy;
extern const wchar_t* dsDrawingLocaleHyperglycemy;
extern const wchar_t* dsDrawingLocaleBlood;
extern const wchar_t* dsDrawingLocaleIst;
extern const wchar_t* dsDrawingLocaleResults;
extern const wchar_t* dsDrawingLocaleDiff2;
extern const wchar_t* dsDrawingLocaleDiff3;
extern const wchar_t* dsDrawingLocaleQuantile;
extern const wchar_t* dsDrawingLocaleRelative;
extern const wchar_t* dsDrawingLocaleAbsolute;
extern const wchar_t* dsDrawingLocaleAverage;
extern const wchar_t* dsDrawingLocaleType;
extern const wchar_t* dsDrawingLocaleError;
extern const wchar_t* dsDrawingLocaleDescription;
extern const wchar_t* dsDrawingLocaleColor;
extern const wchar_t* dsDrawingLocaleCounted;
extern const wchar_t* dsDrawingLocaleMeasured;
extern const wchar_t* dsDrawingLocaleAxisX;
extern const wchar_t* dsDrawingLocaleAxisY;
extern const wchar_t* dsDrawingLocaleSvgDatetimeTitle;
extern const wchar_t* dsDrawingLocaleSvgIstTitle;
extern const wchar_t* dsDrawingLocaleSvgBloodTitle;
extern const wchar_t* dsDrawingLocaleSvgBloodCalibrationTitle;
extern const wchar_t* dsDrawingLocaleSvgInsulinTitle;
extern const wchar_t* dsDrawingLocaleSvgCarbohydratesTitle;
extern const wchar_t* dsDrawingLocaleSvgISIGTitle;
extern const wchar_t* dsDrawingLocaleSvgDiff2Title;
extern const wchar_t* dsDrawingLocaleSvgDiff3Title;
extern const wchar_t* dsDrawingLocaleRelativeError;
extern const wchar_t* dsDrawingLocaleCummulativeProbability;
extern const wchar_t* dsDrawingLocaleElevatedGlucose;

//xxxxxxxxxxxxxxxx

extern const wchar_t *dsRecalculate_With_Every_Params;
extern const wchar_t *dsUse_Just_Opened_Segments;
extern const wchar_t *dsHold_During_Solve;
extern const wchar_t *dsHold_During_Solve_Tooltip;


//--------------------------------- do not translate any of the rs-prefixed texts --

extern const char* rsAbout_Text;

extern const wchar_t *rsDb_Reader;
extern const wchar_t *rsDb_Host;
extern const wchar_t *rsDb_Port;
extern const wchar_t *rsDb_Provider;
extern const wchar_t *rsDb_Name;
extern const wchar_t *rsDb_User_Name;
extern const wchar_t *rsDb_Password;
extern const wchar_t *rsTime_Segment_ID;

extern const wchar_t *rsDiffusion_v2_Table;
extern const wchar_t *rsDiffusion_v3_Table;
extern const wchar_t *rsSteil_Rebrin_Table;

extern const wchar_t* rsP_Column;
extern const wchar_t* rsCg_Column;
extern const wchar_t* rsC_Column;
extern const wchar_t* rsDt_Column;
extern const wchar_t* rsK_Column;
extern const wchar_t* rsH_Column;

extern const wchar_t* rsTau_Column;
extern const wchar_t* rsAlpha_Column;
extern const wchar_t* rsBeta_Column;
extern const wchar_t* rsGamma_Column;

extern const wchar_t *rsNet_Host;
extern const wchar_t *rsNet_Port;
extern const wchar_t *rsNet_RecvSide;

extern const wchar_t *rsInput_Values_File;
extern const wchar_t *rsInput_Segment_Spacing;

extern const wchar_t *rsHold_Values_Delay;

extern const wchar_t *rsSignal_Source_Id;
extern const wchar_t *rsSignal_Destination_Id;

extern const wchar_t *rsSignal_Masked_Id;
extern const wchar_t *rsSignal_Value_Bitmask;

//extern const wchar_t *rsRecalculate_Past_On_Params;
//extern const wchar_t *rsRecalculate_Past_On_Segment_Stop;

extern const wchar_t *rsParameters_Reset;
extern const wchar_t *rsParameters_Reset_Request;
extern const wchar_t *rsSegment_Recalculate_Complete;

extern const wchar_t *rsSelected_Model;
extern const wchar_t *rsSelected_Metric;
extern const wchar_t *rsSelected_Solver;
extern const wchar_t *rsSelected_Signal;
extern const wchar_t *rsSelected_Model_Bounds;
extern const wchar_t *rsUse_Relative_Error;
extern const wchar_t *rsUse_Squared_Diff;
extern const wchar_t *rsUse_Prefer_More_Levels;
extern const wchar_t *rsMetric_Threshold;
extern const wchar_t *rsMetric_Levels_Required;
extern const wchar_t *rsUse_Measured_Levels;
extern const wchar_t *rsRecalculate_On_Levels_Count;
extern const wchar_t *rsRecalculate_On_Segment_End;
extern const wchar_t *rsRecalculate_On_Calibration;

extern const wchar_t *rsLog_Output_File;

extern const wchar_t *rsDrawing_Filter_Period;
extern const wchar_t *rsDiagnosis_Is_Type2;
extern const wchar_t *rsDrawing_Filter_Canvas_Width;
extern const wchar_t *rsDrawing_Filter_Canvas_Height;
extern const wchar_t *rsDrawing_Filter_Filename_Graph;
extern const wchar_t *rsDrawing_Filter_Filename_Day;
extern const wchar_t *rsDrawing_Filter_Filename_AGP;
extern const wchar_t *rsDrawing_Filter_Filename_Parkes;
extern const wchar_t *rsDrawing_Filter_Filename_Clark;
extern const wchar_t *rsDrawing_Filter_Filename_ECDF;

extern const wchar_t *rsCallback_Log_Message;
extern const wchar_t *rsCallback_Drawing_Graph;
extern const wchar_t *rsCallback_Drawing_Day;
extern const wchar_t *rsCallback_Drawing_Clark;
extern const wchar_t *rsCallback_Drawing_Parkes;
extern const wchar_t *rsCallback_Drawing_Parkes_Type2;
extern const wchar_t *rsCallback_Drawing_AGP;

extern const wchar_t *rsInfo_Redraw_Complete;
extern const wchar_t *rsInfo_Solver_Progress;
extern const wchar_t *rsInfo_Solver_Failed;
extern const wchar_t *rsInfo_Error_Metrics_Ready;
extern const wchar_t *rsInfo_Marker;

extern const wchar_t *rsConfig_File_Name;
extern const wchar_t *rsFilter_Section_Prefix;
extern const wchar_t rsFilter_Section_Separator;
extern const wchar_t *rsIni_Comment_Prefix;

extern const wchar_t* rsSolversDir;

extern const char* rsFilter_Get_SVG_AGP;
extern const char* rsFilter_Get_SVG_Clark;
extern const char* rsFilter_Get_SVG_Day;
extern const char* rsFilter_Get_SVG_Graph;
extern const char* rsFilter_Get_SVG_Parkes;
extern const char* rsFilter_Get_SVG_Parkes_Type2;

extern const char* rsFilter_Get_Errors;

extern const wchar_t* rsSelect_Subjects_And_Segments_For_Db_Reader_Filter;
extern const wchar_t* rsSelect_Timesegment_Values_Filter;
extern const wchar_t* rsSelect_Params_Base;
extern const wchar_t* rsSelect_Params_From;
extern const wchar_t* rsSelect_Params_Condition;

// ---- drawing-related constants

extern const wchar_t* rsDrawingLocaleTitle;
extern const wchar_t* rsDrawingLocaleTitleDay;
extern const wchar_t* rsDrawingLocaleSubtitle;
extern const wchar_t* rsDrawingLocaleDiabetes1;
extern const wchar_t* rsDrawingLocaleDiabetes2;
extern const wchar_t* rsDrawingLocaleTime;
extern const wchar_t* rsDrawingLocaleTimeDay;
extern const wchar_t* rsDrawingLocaleConcentration;
extern const wchar_t* rsDrawingLocaleHypoglycemy;
extern const wchar_t* rsDrawingLocaleHyperglycemy;
extern const wchar_t* rsDrawingLocaleBlood;
extern const wchar_t* rsDrawingLocaleIst;
extern const wchar_t* rsDrawingLocaleResults;
extern const wchar_t* rsDrawingLocaleDiff2;
extern const wchar_t* rsDrawingLocaleDiff3;
extern const wchar_t* rsDrawingLocaleQuantile;
extern const wchar_t* rsDrawingLocaleRelative;
extern const wchar_t* rsDrawingLocaleAbsolute;
extern const wchar_t* rsDrawingLocaleAverage;
extern const wchar_t* rsDrawingLocaleType;
extern const wchar_t* rsDrawingLocaleError;
extern const wchar_t* rsDrawingLocaleDescription;
extern const wchar_t* rsDrawingLocaleColor;
extern const wchar_t* rsDrawingLocaleCounted;
extern const wchar_t* rsDrawingLocaleMeasured;
extern const wchar_t* rsDrawingLocaleAxisX;
extern const wchar_t* rsDrawingLocaleAxisY;
extern const wchar_t* rsDrawingLocaleSvgDatetimeTitle;
extern const wchar_t* rsDrawingLocaleSvgIstTitle;
extern const wchar_t* rsDrawingLocaleSvgBloodTitle;
extern const wchar_t* rsDrawingLocaleSvgBloodCalibrationTitle;
extern const wchar_t* rsDrawingLocaleSvgInsulinTitle;
extern const wchar_t* rsDrawingLocaleSvgCarbohydratesTitle;
extern const wchar_t* rsDrawingLocaleSvgISIGTitle;
extern const wchar_t* rsDrawingLocaleSvgDiff2Title;
extern const wchar_t* rsDrawingLocaleSvgDiff3Title;
extern const wchar_t* rsDrawingLocaleRelativeError;
extern const wchar_t* rsDrawingLocaleCummulativeProbability;
extern const wchar_t* rsDrawingLocaleElevatedGlucose;

extern const wchar_t* rsCannot_Get_Parameters;
extern const wchar_t* rsLog_Date_Time_Format;

extern const wchar_t* rsLog_CSV_Separator;

extern const char* rsRattime_Edit_Mask;
extern const wchar_t *rsShutdown_After_Last;

extern const wchar_t *rsGenerate_Primary_Keys;
extern const wchar_t *rsStore_Data;
extern const wchar_t *rsStore_Parameters;

//*******
extern const wchar_t *rsCalculate_Past_New_Params;
extern const wchar_t *rsRecalculate_With_Every_Params;
extern const wchar_t *rsUse_Just_Opened_Segments;
extern const wchar_t *rsHold_During_Solve;
extern const wchar_t* rsPrediction_Window;

