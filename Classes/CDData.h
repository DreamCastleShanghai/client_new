#include "CommonHttpManager.h"
#include "FDataManager.h"

#pragma once

#define _T(x) L##x
#define CHAR    wchar_t

static const std::string httpUrl = "http://139.196.195.185:8080/sap";//"http://10.88.225.99:8080/login";//"http://localhost/httpUrl";

static const std::string imgPreUrl = "http://139.196.195.185:8080/res/";

static const char* loginTag = "L0";


static const char* momentsTag[4] =
{
	"PL0", "PML0", "PV0", "PLIKE0"
};

static const char* noticeTag[2] =
{
	"NT0", "NT1"
};

static const char* iconUploadTag[2] =
{
    "UI0", "PS0"
};

static const char* suveyTag[2] =
{
    "SU0", "SU1"
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

static const char* filterItem[8] =
{
    "Customer", "Success", "S/4HANA", "Data", "DB", "Design", "Market", "Other"
};

#define MOMENTSFILTERNUM 7
static const char* filterMoments[MOMENTSFILTERNUM] =
{
	"all", "developers", "food", "demo", "Voice of SAP", "Demo Jam", "Handsome"
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