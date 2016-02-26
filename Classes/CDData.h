#include "CommonHttpManager.h"
#include "FDataManager.h"

#pragma once

#define _T(x) L##x
#define CHAR    wchar_t

//#define LOCAL_DEBUG

#ifdef LOCAL_DEBUG
static const std::string httpUrl = "http://localhost:8080/sap";
static const std::string imgPreUrl = "http://localhost:8080/res/";
#else
static const std::string httpUrl = "http://139.196.195.185:8080/sap";
static const std::string imgPreUrl = "http://139.196.195.185:8080/res/";
#endif

static const char* loginTag = "L0";

#define SAP_DEFAULT_COLOR   (ccc4(0x0f, 0xaa, 0xff, 0xff))
#define SAP_WHITE_LOW       (ccc4(0xce, 0xea, 0xfd, 0xff))
#define SAP_GRAY            (ccc4(0x96, 0x96, 0x96, 0xff))

// Font Size
#define SAP_TITLE_FONT_SIZE ((36))

// Font
#define SAP_FONT_ARIAL      ("fonts/arial.ttf")

#define DKOM_SURVEY_LINK    ("https://surveys.sap.com/SE/?SID=SV_cOYQMsNXeLGjo7H")


static const char* noticeTokenTag[2] =
{
    "T0", "T1"
};

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

static const char* mapViewTag[2] =
{
    "MP0", "MP1",
};

static const char* meViewTag[2] =
{
    "I0", "I1",
};

static const char* sessionDetailViewTag[2] =
{
	"SD0", "SD1"
};

static const char* voteViewTag[2] =
{
    "DVL0", "VL0"
};

static const char* voteSubmitTag[6] =
{
    "DE0", "VE0", "EE0", "DV0", "VV0", "EV0",
};

static const char* userInfoTag[2] =
{
    "U0", "U1"
};

static const char* rankViewTag[2] =
{
    "R0", "R1"
};

static const char* scoreHistoryTag[2] =
{
    "SH0", "SH1"
};

static const char* noticeIcon[3] =
{
    "notification/favorite.png", "notification/event.png", "notification/prize.png"
};

static const CHAR* mainShort[3] =
{
    _T("Survey"), _T("Vote"), _T("Onsite Guide")
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
    "All",
    "HANA, HCP & Cloud Infrastructure",
    "User Experience",
    "Analytics",
    "S/4HANA",
    "Internet of Things",
    "Incubation & Emerging Trends",
	"LoB Applications",
    "Security & Engineering Excellence",
    "Small & Medium Enterprises",
    "Business Network Applications",
    "Industry Applications"
};

#define FormatNum 13
static const char* formatFilterItem[FormatNum] =
{
	"All",
    "Keynote",
    "SAP Talk",
    "Downtown Block",
    "Developer Faire",
    "Interactive Zone",
    "Digital Zone",
    "Customer Talk",
    "Ecosystem Talk",
    "Customer Round Table",
    "Cloud Innovation Kitchen",
    "People Booth",
    "Quality Booth",
};

#define MOMENTSFILTERNUM 9
static const char* filterMoments[MOMENTSFILTERNUM] =
{
	"All",
    "Keynote",
    "Session",
    "Evening Party",
    "Digital Zone",
    "Fun",
    "Selfie",
    "Smile",
    "Others",
};

static const char* surveyDetail[1] =
{
    "1.    I take public transportation and/or cycle or walk to d-kom Shanghai venue.\n\n2.    I save paper by using electronic onsite guide in d-kom app.\n\n3.    I finish off my meals and have “clean plate” today.\n\n4.    I drink bottled water and recycle plastic bottles to recycle bins, and/or used my own cup to drink.\n\n5.    I do not smoke today.\n\n6.    At d-kom, I support to use old laptops and furniture that were moved from Labs China Shanghai Campus.\n\n7.    I share pictures about sustainability on the “Moments” of d-kom Shanghai App.\n\n      I agree with the above statements and I am proud to be a Sustainability Guardian!"
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

static const char* scoreType[] =
{
    "Session Survey",
    "Demo Jam Vote",
    "Upload Photo",
    "Upload Avatar",
    "Sustainability Campaign",
    "Staff or Ambassador",
    "Speaker of own Session",
};

static const char gameRuleContent[] =
{
    "This is the rule content of this game, please read it carefully."
};



