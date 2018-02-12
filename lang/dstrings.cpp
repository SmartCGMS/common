#include "dstrings.h"

const char* dsFile = "File";
const char* dsSave_Configuration = "Save Configruation";
const char* dsQuit = "Quit";
const char* dsTools = "Tools";
const char* dsFilters = "Filters";

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

const char* dsFilters_Window = "Filters window";
const char* dsApplied_Filters = "Applied filters";
const char* dsAvailable_Filters = "Available filters";

const wchar_t *dsDb_Reader = L"Db Reader";

const wchar_t *dsDb_Host = L"Host";
const wchar_t *dsDb_Provider = L"Provider";
const wchar_t *dsDb_Name = L"Name";
const wchar_t *dsDb_User_Name = L"User name";
const wchar_t *dsDb_Password = L"Password";
const wchar_t *dsTime_Segment_ID = L"Time Segment Id";

const char *dsConfiguration = "Configuration";
const char *dsSelect_Just_One_Item = "Select just one item.";

const char *dsInformation = "Information";
const char *dsMain_Parameters = "Main parameters";

const char* dsOK = "OK";
const char* dsCancel = "Cancel";
const char* dsApply = "Apply";
const char* dsCommit = "Commit";

const char* dsSubject = "Subject";
const char* dsSegment = "Segment";

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

//--------------------------------- do not translate any of the rs-prefixed texts --

const char* rsAbout_Text =	"<b>Glucose Predictor</b>, Version 3.0 Alpha<br>&nbsp;<br>"\
							"Tom\xc3\xa1\xc5\xa1 Koutn\xc3\xbd, Ph.D.<br><i>txkoutny@kiv.zcu.cz</i><br>&nbsp;<br>"\
							"Department of Computer Science and Engineering<br>"\
							"University of West Bohemia<br>"\
							"Plze\xc5\x88 306 14, Czech Republic<br>"\
							"&nbsp;<hr>diabetes.zcu.cz";
							
const wchar_t *rsDb_Host = L"Host";
const wchar_t *rsDb_Provider = L"Provider";
const wchar_t *rsDb_Name = L"Name";
const wchar_t *rsDb_User_Name = L"UserName";
const wchar_t *rsDb_Password = L"Password";
const wchar_t *rsTime_Segment_ID = L"TimeSegmentId";

const wchar_t *rsConfig_File_Name = L"config.ini";
const wchar_t *rs_Filter_Section_Prefix = L"Filter_";
const wchar_t *rsIni_Comment_Prefix = L"; ";

const char* rsSelect_Subjects_And_Segments_For_Db_Reader_Filter = "select timesegment.id, subject.name, timesegment.name from subject, timesegment where subject.id = timesegment.subjectid order by subject.name, timesegment.name asc";
