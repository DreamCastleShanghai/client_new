#include "CommonHttpManager.h"
#include "FDataManager.h"

#pragma once

#define _T(x) L##x
#define CHAR    wchar_t

static const std::string httpUrl = "http://10.88.225.121:8080/sap";//"http://10.88.225.99:8080/login";//"http://localhost/httpUrl";

static const char* loginTag = "L0";

static const char* mainViewTag[2] =
{
    "M0", "M1"
};

static const char* sessionViewTag[2] =
{
    "S0", "VS0"
};

static const char* voteViewTag[2] =
{
    "V0", "V1"
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
    _T("Share photo"), _T("Vote"), _T("Map")
};

static const CHAR* mainBar[3] =
{
    _T("Home"), _T("Agenda"), _T("Me")
};

static const char* filterItem[8] =
{
    "Customer", "Success", "S/4HANA", "Data", "DB", "Design", "Market", "Other"
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