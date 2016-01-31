#include "CommonHttpManager.h"
#include "FDataManager.h"

#pragma once

#define _T(x) L##x
#define CHAR    wchar_t

//static const std::string httpUrl = "http://localhost:8080/sap";
static const std::string httpUrl = "http://139.196.195.185:8080/sap";

static const std::string imgPreUrl = "http://139.196.195.185:8080/res/";

static const char* loginTag = "L0";


static const char* momentsTag[4] =
{
	"PL0", "PML0", "PV0", "PD0"
};

static const char* noticeTag[2] =
{
	"NT0", "NT1"
};

static const char* iconUploadTag[2] =
{
    "UI0", "PS0"
};

static const char* surveyInfoTag[2] =
{
    "SSI0", "SSI1"
};

static const char* surveySubmitTag[2] =
{
    "SSS0", "SSS1"
};

static const char* firstSurveySubmitTag[2] =
{
    "SR0", "SR0"
};

static const char* mainViewTag[2] =
{
    "M0", "M1"
};

static const char* sessionViewTag[3] =
{
	"SL0", "SC0", "SV0"
};

static const char* sessionDetailViewTag[2] =
{
	"SD0", "SD1"
};

static const char* voteViewTag[2] =
{
    "DVL0", "VL0"
};

static const char* voteSubmitTag[4] =
{
    "DE0", "VE0","DV0","VV0"
};

static const char* userInfoTag[2] =
{
    "U0", "U1"
};

static const char* rankViewTag[2] =
{
    "R0", "R1"
};

static const CHAR* mainShort[3] =
{
    _T("Moments"), _T("Vote"), _T("On-site Guide")
};

static const CHAR* mainBar[3] =
{
    _T("Home"), _T("Agenda"), _T("Me")
};

static const char* filterItem[2] =
{
	"Track", "Format"
};

#define TrackNum 12
static const char* trackFilterItem[TrackNum] =
{
    "all", "HANA, HCP & Cloud Infrastructure", "User Experience", "Analytics", "S/4HANA", "Internet of Things", "Incubation & Emerging Trends",
	"LoB Applications", "Security & Engineering Excellence", "Small & Medium Enterprises", "Business Network Applications", "Industry Applications"
};

#define FormatNum 12
static const char* formatFilterItem[FormatNum] =
{
	"all", "SAP Talk", "Downtown Block", "Developer Faire Booth", "Interactive Zone", "Customer Talk", "People & SLN Booth", "Fiori Cafe", "Cloud Innovation Kitchen",
	"SAP Development University", "Quality Booth", "Digital Zone"
};

#define MOMENTSFILTERNUM 7
static const char* filterMoments[MOMENTSFILTERNUM] =
{
	"all", "developers", "food", "demo", "Voice of SAP", "Demo Jam", "Handsome"
};

static const char* surveyDetail[1] =
{
    "1.    I take public transportation and/or cycle or walk to d-kom Shanghai venue.\n\n2.    I save paper by using electronic onsite guide in d-kom app.\n\n3.    I finish off my meals and have “clean plate” today.\n\n4.    I drink bottled water and recycle plastic bottles to recycle bins, and/or used my own cup to drink.\n\n5.    I do not smoke today.\n\n6.    At d-kom, I support to use old laptops and furniture that were moved from Labs China Shanghai Campus.\n\n7.    I share pictures about sustainability on the “Moments” of d-kom Shanghai App."
};

static const char* iconTag[27] =
{
	"image/AlertView.png",
    "image/button.png",
    "image/ImageView.png",
    "image/Scale9ImageView.png",
    "image/indicatorView.png",
    "image/Label.png",
    "image/Progress.png",
    "image/slider.png",
    "image/Switch.png",
    "image/TextField.png",
    "image/TextView.png",
    "image/SegmentedControl.png",
    "image/TabBar.png",
    "image/PageView.png",
    "image/TableView.png",
    "image/ListView.png",
    "image/CollectionView.png",
    "image/ScrollView.png",
    "image/WebView.png",
    "image/FlashView.png",
    "image/GifView.png",
    "image/PickerView.png",
    "image/Stepper.png",
    "image/RenderImage.png",
    "image/Video.png",
    "image/Video.png",
    "image/Video.png"
};