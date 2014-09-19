#include <QString>
#include <QStringList>

#include "include/Hotkeys.h"



namespace util {
    String2QtKeyEnum *String2QtKeyEnum::_singletonInstance = NULL;

    String2QtKeyEnum::String2QtKeyEnum() {
    }

    String2QtKeyEnum::~String2QtKeyEnum() {
    }

    void String2QtKeyEnum::setup() {
        _singletonInstance = new String2QtKeyEnum();
        _singletonInstance->setupMap();
    }

    String2QtKeyEnum *String2QtKeyEnum::instance() {
        if (_singletonInstance == NULL) {
            setup();
        }
        return _singletonInstance;
    }

    void String2QtKeyEnum::reset() {
        if (_singletonInstance != NULL) {
            delete _singletonInstance;
            _singletonInstance = NULL;
        }
    }

    void String2QtKeyEnum::setupMap() {
        // See: http://qt-project.org/doc/qt-4.8/qt.html#Key-enum
        keymap["Escape"]                 = Qt::Key_Escape;
        keymap["Tab"]                    = Qt::Key_Tab;
        keymap["Backtab"]                = Qt::Key_Backtab;
        keymap["Backspace"]              = Qt::Key_Backspace;
        keymap["Return"]                 = Qt::Key_Return;
        keymap["Enter"]                  = Qt::Key_Enter;                 // Typically located on the keypad.
        keymap["Insert"]                 = Qt::Key_Insert;
        keymap["Delete"]                 = Qt::Key_Delete;
        keymap["Pause"]                  = Qt::Key_Pause;                 // The Pause/Break key (Note: Not anything to do with pausing media)
        keymap["Print"]                  = Qt::Key_Print;
        keymap["SysReq"]                 = Qt::Key_SysReq;
        keymap["Clear"]                  = Qt::Key_Clear;
        keymap["Home"]                   = Qt::Key_Home;
        keymap["End"]                    = Qt::Key_End;
        keymap["Left"]                   = Qt::Key_Left;
        keymap["Up"]                     = Qt::Key_Up;
        keymap["Right"]                  = Qt::Key_Right;
        keymap["Down"]                   = Qt::Key_Down;
        keymap["PageUp"]                 = Qt::Key_PageUp;
        keymap["PageDown"]               = Qt::Key_PageDown;
        keymap["Shift"]                  = Qt::Key_Shift;
        keymap["Control"]                = Qt::Key_Control;               // On Mac OS X, this corresponds to the Command keys.
        keymap["Meta"]                   = Qt::Key_Meta;                  // On Mac OS X, this corresponds to the Control keys. On Windows keyboards, this key is mapped to the Windows key.
        keymap["Alt"]                    = Qt::Key_Alt;
        keymap["AltGr"]                  = Qt::Key_AltGr;                 // On Windows, when the KeyDown event for this key is sent, the Ctrl+Alt modifiers are also set.
        keymap["CapsLock"]               = Qt::Key_CapsLock;
        keymap["NumLock"]                = Qt::Key_NumLock;
        keymap["ScrollLock"]             = Qt::Key_ScrollLock;
        keymap["F1"]                     = Qt::Key_F1;
        keymap["F2"]                     = Qt::Key_F2;
        keymap["F3"]                     = Qt::Key_F3;
        keymap["F4"]                     = Qt::Key_F4;
        keymap["F5"]                     = Qt::Key_F5;
        keymap["F6"]                     = Qt::Key_F6;
        keymap["F7"]                     = Qt::Key_F7;
        keymap["F8"]                     = Qt::Key_F8;
        keymap["F9"]                     = Qt::Key_F9;
        keymap["F10"]                    = Qt::Key_F10;
        keymap["F11"]                    = Qt::Key_F11;
        keymap["F12"]                    = Qt::Key_F12;
        keymap["F13"]                    = Qt::Key_F13;
        keymap["F14"]                    = Qt::Key_F14;
        keymap["F15"]                    = Qt::Key_F15;
        keymap["F16"]                    = Qt::Key_F16;
        keymap["F17"]                    = Qt::Key_F17;
        keymap["F18"]                    = Qt::Key_F18;
        keymap["F19"]                    = Qt::Key_F19;
        keymap["F20"]                    = Qt::Key_F20;
        keymap["F21"]                    = Qt::Key_F21;
        keymap["F22"]                    = Qt::Key_F22;
        keymap["F23"]                    = Qt::Key_F23;
        keymap["F24"]                    = Qt::Key_F24;
        keymap["F25"]                    = Qt::Key_F25;
        keymap["F26"]                    = Qt::Key_F26;
        keymap["F27"]                    = Qt::Key_F27;
        keymap["F28"]                    = Qt::Key_F28;
        keymap["F29"]                    = Qt::Key_F29;
        keymap["F30"]                    = Qt::Key_F30;
        keymap["F31"]                    = Qt::Key_F31;
        keymap["F32"]                    = Qt::Key_F32;
        keymap["F33"]                    = Qt::Key_F33;
        keymap["F34"]                    = Qt::Key_F34;
        keymap["F35"]                    = Qt::Key_F35;
        keymap["Super_L"]                = Qt::Key_Super_L;
        keymap["Super_R"]                = Qt::Key_Super_R;
        keymap["Menu"]                   = Qt::Key_Menu;
        keymap["Hyper_L"]                = Qt::Key_Hyper_L;
        keymap["Hyper_R"]                = Qt::Key_Hyper_R;
        keymap["Help"]                   = Qt::Key_Help;
        keymap["Direction_L"]            = Qt::Key_Direction_L;
        keymap["Direction_R"]            = Qt::Key_Direction_R;
        keymap["Space"]                  = Qt::Key_Space;
        keymap["Exclam"]                 = Qt::Key_Exclam;
        keymap["QuoteDbl"]               = Qt::Key_QuoteDbl;
        keymap["NumberSign"]             = Qt::Key_NumberSign;
        keymap["Dollar"]                 = Qt::Key_Dollar;
        keymap["Percent"]                = Qt::Key_Percent;
        keymap["Ampersand"]              = Qt::Key_Ampersand;
        keymap["Apostrophe"]             = Qt::Key_Apostrophe;
        keymap["ParenLeft"]              = Qt::Key_ParenLeft;
        keymap["ParenRight"]             = Qt::Key_ParenRight;
        keymap["Asterisk"]               = Qt::Key_Asterisk;
        keymap["Plus"]                   = Qt::Key_Plus;
        keymap["Comma"]                  = Qt::Key_Comma;
        keymap["Minus"]                  = Qt::Key_Minus;
        keymap["Period"]                 = Qt::Key_Period;
        keymap["Slash"]                  = Qt::Key_Slash;
        keymap["0"]                      = Qt::Key_0;
        keymap["1"]                      = Qt::Key_1;
        keymap["2"]                      = Qt::Key_2;
        keymap["3"]                      = Qt::Key_3;
        keymap["4"]                      = Qt::Key_4;
        keymap["5"]                      = Qt::Key_5;
        keymap["6"]                      = Qt::Key_6;
        keymap["7"]                      = Qt::Key_7;
        keymap["8"]                      = Qt::Key_8;
        keymap["9"]                      = Qt::Key_9;
        keymap["Colon"]                  = Qt::Key_Colon;
        keymap["Semicolon"]              = Qt::Key_Semicolon;
        keymap["Less"]                   = Qt::Key_Less;
        keymap["Equal"]                  = Qt::Key_Equal;
        keymap["Greater"]                = Qt::Key_Greater;
        keymap["Question"]               = Qt::Key_Question;
        keymap["At"]                     = Qt::Key_At;
        keymap["A"]                      = Qt::Key_A;
        keymap["B"]                      = Qt::Key_B;
        keymap["C"]                      = Qt::Key_C;
        keymap["D"]                      = Qt::Key_D;
        keymap["E"]                      = Qt::Key_E;
        keymap["F"]                      = Qt::Key_F;
        keymap["G"]                      = Qt::Key_G;
        keymap["H"]                      = Qt::Key_H;
        keymap["I"]                      = Qt::Key_I;
        keymap["J"]                      = Qt::Key_J;
        keymap["K"]                      = Qt::Key_K;
        keymap["L"]                      = Qt::Key_L;
        keymap["M"]                      = Qt::Key_M;
        keymap["N"]                      = Qt::Key_N;
        keymap["O"]                      = Qt::Key_O;
        keymap["P"]                      = Qt::Key_P;
        keymap["Q"]                      = Qt::Key_Q;
        keymap["R"]                      = Qt::Key_R;
        keymap["S"]                      = Qt::Key_S;
        keymap["T"]                      = Qt::Key_T;
        keymap["U"]                      = Qt::Key_U;
        keymap["V"]                      = Qt::Key_V;
        keymap["W"]                      = Qt::Key_W;
        keymap["X"]                      = Qt::Key_X;
        keymap["Y"]                      = Qt::Key_Y;
        keymap["Z"]                      = Qt::Key_Z;
        keymap["BracketLeft"]            = Qt::Key_BracketLeft;
        keymap["Backslash"]              = Qt::Key_Backslash;
        keymap["BracketRight"]           = Qt::Key_BracketRight;
        keymap["AsciiCircum"]            = Qt::Key_AsciiCircum;
        keymap["Underscore"]             = Qt::Key_Underscore;
        keymap["QuoteLeft"]              = Qt::Key_QuoteLeft;
        keymap["BraceLeft"]              = Qt::Key_BraceLeft;
        keymap["Bar"]                    = Qt::Key_Bar;
        keymap["BraceRight"]             = Qt::Key_BraceRight;
        keymap["AsciiTilde"]             = Qt::Key_AsciiTilde;
        keymap["nobreakspace"]           = Qt::Key_nobreakspace;
        keymap["exclamdown"]             = Qt::Key_exclamdown;
        keymap["cent"]                   = Qt::Key_cent;
        keymap["sterling"]               = Qt::Key_sterling;
        keymap["currency"]               = Qt::Key_currency;
        keymap["yen"]                    = Qt::Key_yen;
        keymap["brokenbar"]              = Qt::Key_brokenbar;
        keymap["section"]                = Qt::Key_section;
        keymap["diaeresis"]              = Qt::Key_diaeresis;
        keymap["copyright"]              = Qt::Key_copyright;
        keymap["ordfeminine"]            = Qt::Key_ordfeminine;
        keymap["guillemotleft"]          = Qt::Key_guillemotleft;
        keymap["notsign"]                = Qt::Key_notsign;
        keymap["hyphen"]                 = Qt::Key_hyphen;
        keymap["registered"]             = Qt::Key_registered;
        keymap["macron"]                 = Qt::Key_macron;
        keymap["degree"]                 = Qt::Key_degree;
        keymap["plusminus"]              = Qt::Key_plusminus;
        keymap["twosuperior"]            = Qt::Key_twosuperior;
        keymap["threesuperior"]          = Qt::Key_threesuperior;
        keymap["acute"]                  = Qt::Key_acute;
        keymap["mu"]                     = Qt::Key_mu;
        keymap["paragraph"]              = Qt::Key_paragraph;
        keymap["periodcentered"]         = Qt::Key_periodcentered;
        keymap["cedilla"]                = Qt::Key_cedilla;
        keymap["onesuperior"]            = Qt::Key_onesuperior;
        keymap["masculine"]              = Qt::Key_masculine;
        keymap["guillemotright"]         = Qt::Key_guillemotright;
        keymap["onequarter"]             = Qt::Key_onequarter;
        keymap["onehalf"]                = Qt::Key_onehalf;
        keymap["threequarters"]          = Qt::Key_threequarters;
        keymap["questiondown"]           = Qt::Key_questiondown;
        keymap["Agrave"]                 = Qt::Key_Agrave;
        keymap["Aacute"]                 = Qt::Key_Aacute;
        keymap["Acircumflex"]            = Qt::Key_Acircumflex;
        keymap["Atilde"]                 = Qt::Key_Atilde;
        keymap["Adiaeresis"]             = Qt::Key_Adiaeresis;
        keymap["Aring"]                  = Qt::Key_Aring;
        keymap["AE"]                     = Qt::Key_AE;
        keymap["Ccedilla"]               = Qt::Key_Ccedilla;
        keymap["Egrave"]                 = Qt::Key_Egrave;
        keymap["Eacute"]                 = Qt::Key_Eacute;
        keymap["Ecircumflex"]            = Qt::Key_Ecircumflex;
        keymap["Ediaeresis"]             = Qt::Key_Ediaeresis;
        keymap["Igrave"]                 = Qt::Key_Igrave;
        keymap["Iacute"]                 = Qt::Key_Iacute;
        keymap["Icircumflex"]            = Qt::Key_Icircumflex;
        keymap["Idiaeresis"]             = Qt::Key_Idiaeresis;
        keymap["ETH"]                    = Qt::Key_ETH;
        keymap["Ntilde"]                 = Qt::Key_Ntilde;
        keymap["Ograve"]                 = Qt::Key_Ograve;
        keymap["Oacute"]                 = Qt::Key_Oacute;
        keymap["Ocircumflex"]            = Qt::Key_Ocircumflex;
        keymap["Otilde"]                 = Qt::Key_Otilde;
        keymap["Odiaeresis"]             = Qt::Key_Odiaeresis;
        keymap["multiply"]               = Qt::Key_multiply;
        keymap["Ooblique"]               = Qt::Key_Ooblique;
        keymap["Ugrave"]                 = Qt::Key_Ugrave;
        keymap["Uacute"]                 = Qt::Key_Uacute;
        keymap["Ucircumflex"]            = Qt::Key_Ucircumflex;
        keymap["Udiaeresis"]             = Qt::Key_Udiaeresis;
        keymap["Yacute"]                 = Qt::Key_Yacute;
        keymap["THORN"]                  = Qt::Key_THORN;
        keymap["ssharp"]                 = Qt::Key_ssharp;
        keymap["division"]               = Qt::Key_division;
        keymap["ydiaeresis"]             = Qt::Key_ydiaeresis;
        keymap["Multi_key"]              = Qt::Key_Multi_key;
        keymap["Codeinput"]              = Qt::Key_Codeinput;
        keymap["SingleCandidate"]        = Qt::Key_SingleCandidate;
        keymap["MultipleCandidate"]      = Qt::Key_MultipleCandidate;
        keymap["PreviousCandidate"]      = Qt::Key_PreviousCandidate;
        keymap["Mode_switch"]            = Qt::Key_Mode_switch;
        keymap["Kanji"]                  = Qt::Key_Kanji;
        keymap["Muhenkan"]               = Qt::Key_Muhenkan;
        keymap["Henkan"]                 = Qt::Key_Henkan;
        keymap["Romaji"]                 = Qt::Key_Romaji;
        keymap["Hiragana"]               = Qt::Key_Hiragana;
        keymap["Katakana"]               = Qt::Key_Katakana;
        keymap["Hiragana_Katakana"]      = Qt::Key_Hiragana_Katakana;
        keymap["Zenkaku"]                = Qt::Key_Zenkaku;
        keymap["Hankaku"]                = Qt::Key_Hankaku;
        keymap["Zenkaku_Hankaku"]        = Qt::Key_Zenkaku_Hankaku;
        keymap["Touroku"]                = Qt::Key_Touroku;
        keymap["Massyo"]                 = Qt::Key_Massyo;
        keymap["Kana_Lock"]              = Qt::Key_Kana_Lock;
        keymap["Kana_Shift"]             = Qt::Key_Kana_Shift;
        keymap["Eisu_Shift"]             = Qt::Key_Eisu_Shift;
        keymap["Eisu_toggle"]            = Qt::Key_Eisu_toggle;
        keymap["Hangul"]                 = Qt::Key_Hangul;
        keymap["Hangul_Start"]           = Qt::Key_Hangul_Start;
        keymap["Hangul_End"]             = Qt::Key_Hangul_End;
        keymap["Hangul_Hanja"]           = Qt::Key_Hangul_Hanja;
        keymap["Hangul_Jamo"]            = Qt::Key_Hangul_Jamo;
        keymap["Hangul_Romaja"]          = Qt::Key_Hangul_Romaja;
        keymap["Hangul_Jeonja"]          = Qt::Key_Hangul_Jeonja;
        keymap["Hangul_Banja"]           = Qt::Key_Hangul_Banja;
        keymap["Hangul_PreHanja"]        = Qt::Key_Hangul_PreHanja;
        keymap["Hangul_PostHanja"]       = Qt::Key_Hangul_PostHanja;
        keymap["Hangul_Special"]         = Qt::Key_Hangul_Special;
        keymap["Dead_Grave"]             = Qt::Key_Dead_Grave;
        keymap["Dead_Acute"]             = Qt::Key_Dead_Acute;
        keymap["Dead_Circumflex"]        = Qt::Key_Dead_Circumflex;
        keymap["Dead_Tilde"]             = Qt::Key_Dead_Tilde;
        keymap["Dead_Macron"]            = Qt::Key_Dead_Macron;
        keymap["Dead_Breve"]             = Qt::Key_Dead_Breve;
        keymap["Dead_Abovedot"]          = Qt::Key_Dead_Abovedot;
        keymap["Dead_Diaeresis"]         = Qt::Key_Dead_Diaeresis;
        keymap["Dead_Abovering"]         = Qt::Key_Dead_Abovering;
        keymap["Dead_Doubleacute"]       = Qt::Key_Dead_Doubleacute;
        keymap["Dead_Caron"]             = Qt::Key_Dead_Caron;
        keymap["Dead_Cedilla"]           = Qt::Key_Dead_Cedilla;
        keymap["Dead_Ogonek"]            = Qt::Key_Dead_Ogonek;
        keymap["Dead_Iota"]              = Qt::Key_Dead_Iota;
        keymap["Dead_Voiced_Sound"]      = Qt::Key_Dead_Voiced_Sound;
        keymap["Dead_Semivoiced_Sound"]  = Qt::Key_Dead_Semivoiced_Sound;
        keymap["Dead_Belowdot"]          = Qt::Key_Dead_Belowdot;
        keymap["Dead_Hook"]              = Qt::Key_Dead_Hook;
        keymap["Dead_Horn"]              = Qt::Key_Dead_Horn;
        keymap["Back"]                   = Qt::Key_Back;
        keymap["Forward"]                = Qt::Key_Forward;
        keymap["Stop"]                   = Qt::Key_Stop;
        keymap["Refresh"]                = Qt::Key_Refresh;
        keymap["VolumeDown"]             = Qt::Key_VolumeDown;
        keymap["VolumeMute"]             = Qt::Key_VolumeMute;
        keymap["VolumeUp"]               = Qt::Key_VolumeUp;
        keymap["BassBoost"]              = Qt::Key_BassBoost;
        keymap["BassUp"]                 = Qt::Key_BassUp;
        keymap["BassDown"]               = Qt::Key_BassDown;
        keymap["TrebleUp"]               = Qt::Key_TrebleUp;
        keymap["TrebleDown"]             = Qt::Key_TrebleDown;
        keymap["MediaPlay"]              = Qt::Key_MediaPlay;             // A key setting the state of the media player to play
        keymap["MediaStop"]              = Qt::Key_MediaStop;             // A key setting the state of the media player to stop
        keymap["MediaPrevious"]          = Qt::Key_MediaPrevious;
        keymap["MediaNext"]              = Qt::Key_MediaNext;
        keymap["MediaRecord"]            = Qt::Key_MediaRecord;
        keymap["MediaPause"]             = Qt::Key_MediaPause;            // A key setting the state of the media player to pause (Note: not the pause/break key)
        keymap["MediaTogglePlayPause"]   = Qt::Key_MediaTogglePlayPause;  // A key to toggle the play/pause state in the media player (rather than setting an absolute state)
        keymap["HomePage"]               = Qt::Key_HomePage;
        keymap["Favorites"]              = Qt::Key_Favorites;
        keymap["Search"]                 = Qt::Key_Search;
        keymap["Standby"]                = Qt::Key_Standby;
        keymap["OpenUrl"]                = Qt::Key_OpenUrl;
        keymap["LaunchMail"]             = Qt::Key_LaunchMail;
        keymap["LaunchMedia"]            = Qt::Key_LaunchMedia;
        keymap["Launch0"]                = Qt::Key_Launch0;               // On X11 this key is mapped to "My Computer" (XF86XK_MyComputer) key for legacy reasons.
        keymap["Launch1"]                = Qt::Key_Launch1;               // On X11 this key is mapped to "Calculator" (XF86XK_Calculator) key for legacy reasons.
        keymap["Launch2"]                = Qt::Key_Launch2;               // On X11 this key is mapped to XF86XK_Launch0 key for legacy reasons.
        keymap["Launch3"]                = Qt::Key_Launch3;               // On X11 this key is mapped to XF86XK_Launch1 key for legacy reasons.
        keymap["Launch4"]                = Qt::Key_Launch4;               // On X11 this key is mapped to XF86XK_Launch2 key for legacy reasons.
        keymap["Launch5"]                = Qt::Key_Launch5;               // On X11 this key is mapped to XF86XK_Launch3 key for legacy reasons.
        keymap["Launch6"]                = Qt::Key_Launch6;               // On X11 this key is mapped to XF86XK_Launch4 key for legacy reasons.
        keymap["Launch7"]                = Qt::Key_Launch7;               // On X11 this key is mapped to XF86XK_Launch5 key for legacy reasons.
        keymap["Launch8"]                = Qt::Key_Launch8;               // On X11 this key is mapped to XF86XK_Launch6 key for legacy reasons.
        keymap["Launch9"]                = Qt::Key_Launch9;               // On X11 this key is mapped to XF86XK_Launch7 key for legacy reasons.
        keymap["LaunchA"]                = Qt::Key_LaunchA;               // On X11 this key is mapped to XF86XK_Launch8 key for legacy reasons.
        keymap["LaunchB"]                = Qt::Key_LaunchB;               // On X11 this key is mapped to XF86XK_Launch9 key for legacy reasons.
        keymap["LaunchC"]                = Qt::Key_LaunchC;               // On X11 this key is mapped to XF86XK_LaunchA key for legacy reasons.
        keymap["LaunchD"]                = Qt::Key_LaunchD;               // On X11 this key is mapped to XF86XK_LaunchB key for legacy reasons.
        keymap["LaunchE"]                = Qt::Key_LaunchE;               // On X11 this key is mapped to XF86XK_LaunchC key for legacy reasons.
        keymap["LaunchF"]                = Qt::Key_LaunchF;               // On X11 this key is mapped to XF86XK_LaunchD key for legacy reasons.
        keymap["LaunchG"]                = Qt::Key_LaunchG;               // On X11 this key is mapped to XF86XK_LaunchE key for legacy reasons.
        keymap["LaunchH"]                = Qt::Key_LaunchH;               // On X11 this key is mapped to XF86XK_LaunchF key for legacy reasons.
        keymap["MonBrightnessUp"]        = Qt::Key_MonBrightnessUp;
        keymap["MonBrightnessDown"]      = Qt::Key_MonBrightnessDown;
        keymap["KeyboardLightOnOff"]     = Qt::Key_KeyboardLightOnOff;
        keymap["KeyboardBrightnessUp"]   = Qt::Key_KeyboardBrightnessUp;
        keymap["KeyboardBrightnessDown"] = Qt::Key_KeyboardBrightnessDown;
        keymap["PowerOff"]               = Qt::Key_PowerOff;
        keymap["WakeUp"]                 = Qt::Key_WakeUp;
        keymap["Eject"]                  = Qt::Key_Eject;
        keymap["ScreenSaver"]            = Qt::Key_ScreenSaver;
        keymap["WWW"]                    = Qt::Key_WWW;
        keymap["Memo"]                   = Qt::Key_Memo;
        keymap["LightBulb"]              = Qt::Key_LightBulb;
        keymap["Shop"]                   = Qt::Key_Shop;
        keymap["History"]                = Qt::Key_History;
        keymap["AddFavorite"]            = Qt::Key_AddFavorite;
        keymap["HotLinks"]               = Qt::Key_HotLinks;
        keymap["BrightnessAdjust"]       = Qt::Key_BrightnessAdjust;
        keymap["Finance"]                = Qt::Key_Finance;
        keymap["Community"]              = Qt::Key_Community;
        keymap["AudioRewind"]            = Qt::Key_AudioRewind;
        keymap["BackForward"]            = Qt::Key_BackForward;
        keymap["ApplicationLeft"]        = Qt::Key_ApplicationLeft;
        keymap["ApplicationRight"]       = Qt::Key_ApplicationRight;
        keymap["Book"]                   = Qt::Key_Book;
        keymap["CD"]                     = Qt::Key_CD;
        keymap["Calculator"]             = Qt::Key_Calculator;            // On X11 this key is not mapped for legacy reasons. Use Qt::Key_Launch1 instead.
        keymap["ToDoList"]               = Qt::Key_ToDoList;
        keymap["ClearGrab"]              = Qt::Key_ClearGrab;
        keymap["Close"]                  = Qt::Key_Close;
        keymap["Copy"]                   = Qt::Key_Copy;
        keymap["Cut"]                    = Qt::Key_Cut;
        keymap["Display"]                = Qt::Key_Display;
        keymap["DOS"]                    = Qt::Key_DOS;
        keymap["Documents"]              = Qt::Key_Documents;
        keymap["Excel"]                  = Qt::Key_Excel;
        keymap["Explorer"]               = Qt::Key_Explorer;
        keymap["Game"]                   = Qt::Key_Game;
        keymap["Go"]                     = Qt::Key_Go;
        keymap["iTouch"]                 = Qt::Key_iTouch;
        keymap["LogOff"]                 = Qt::Key_LogOff;
        keymap["Market"]                 = Qt::Key_Market;
        keymap["Meeting"]                = Qt::Key_Meeting;
        keymap["MenuKB"]                 = Qt::Key_MenuKB;
        keymap["MenuPB"]                 = Qt::Key_MenuPB;
        keymap["MySites"]                = Qt::Key_MySites;
        keymap["News"]                   = Qt::Key_News;
        keymap["OfficeHome"]             = Qt::Key_OfficeHome;
        keymap["Option"]                 = Qt::Key_Option;
        keymap["Paste"]                  = Qt::Key_Paste;
        keymap["Phone"]                  = Qt::Key_Phone;
        keymap["Calendar"]               = Qt::Key_Calendar;
        keymap["Reply"]                  = Qt::Key_Reply;
        keymap["Reload"]                 = Qt::Key_Reload;
        keymap["RotateWindows"]          = Qt::Key_RotateWindows;
        keymap["RotationPB"]             = Qt::Key_RotationPB;
        keymap["RotationKB"]             = Qt::Key_RotationKB;
        keymap["Save"]                   = Qt::Key_Save;
        keymap["Send"]                   = Qt::Key_Send;
        keymap["Spell"]                  = Qt::Key_Spell;
        keymap["SplitScreen"]            = Qt::Key_SplitScreen;
        keymap["Support"]                = Qt::Key_Support;
        keymap["TaskPane"]               = Qt::Key_TaskPane;
        keymap["Terminal"]               = Qt::Key_Terminal;
        keymap["Tools"]                  = Qt::Key_Tools;
        keymap["Travel"]                 = Qt::Key_Travel;
        keymap["Video"]                  = Qt::Key_Video;
        keymap["Word"]                   = Qt::Key_Word;
        keymap["Xfer"]                   = Qt::Key_Xfer;
        keymap["ZoomIn"]                 = Qt::Key_ZoomIn;
        keymap["ZoomOut"]                = Qt::Key_ZoomOut;
        keymap["Away"]                   = Qt::Key_Away;
        keymap["Messenger"]              = Qt::Key_Messenger;
        keymap["WebCam"]                 = Qt::Key_WebCam;
        keymap["MailForward"]            = Qt::Key_MailForward;
        keymap["Pictures"]               = Qt::Key_Pictures;
        keymap["Music"]                  = Qt::Key_Music;
        keymap["Battery"]                = Qt::Key_Battery;
        keymap["Bluetooth"]              = Qt::Key_Bluetooth;
        keymap["WLAN"]                   = Qt::Key_WLAN;
        keymap["UWB"]                    = Qt::Key_UWB;
        keymap["AudioForward"]           = Qt::Key_AudioForward;
        keymap["AudioRepeat"]            = Qt::Key_AudioRepeat;
        keymap["AudioRandomPlay"]        = Qt::Key_AudioRandomPlay;
        keymap["Subtitle"]               = Qt::Key_Subtitle;
        keymap["AudioCycleTrack"]        = Qt::Key_AudioCycleTrack;
        keymap["Time"]                   = Qt::Key_Time;
        keymap["Hibernate"]              = Qt::Key_Hibernate;
        keymap["View"]                   = Qt::Key_View;
        keymap["TopMenu"]                = Qt::Key_TopMenu;
        keymap["PowerDown"]              = Qt::Key_PowerDown;
        keymap["Suspend"]                = Qt::Key_Suspend;
        keymap["ContrastAdjust"]         = Qt::Key_ContrastAdjust;
        keymap["MediaLast"]              = Qt::Key_MediaLast;
        keymap["unknown"]                = Qt::Key_unknown;
        keymap["Call"]                   = Qt::Key_Call;                  // A key to answer or initiate a call (see Qt::Key_ToggleCallHangup for a key to toggle current call state)
        keymap["Camera"]                 = Qt::Key_Camera;                // A key to activate the camera shutter
        keymap["CameraFocus"]            = Qt::Key_CameraFocus;           // A key to focus the camera
        keymap["Context1"]               = Qt::Key_Context1;
        keymap["Context2"]               = Qt::Key_Context2;
        keymap["Context3"]               = Qt::Key_Context3;
        keymap["Context4"]               = Qt::Key_Context4;
        keymap["Flip"]                   = Qt::Key_Flip;
        keymap["Hangup"]                 = Qt::Key_Hangup;                // A key to end an ongoing call (see Qt::Key_ToggleCallHangup for a key to toggle current call state)
        keymap["No"]                     = Qt::Key_No;
        keymap["Select"]                 = Qt::Key_Select;
        keymap["Yes"]                    = Qt::Key_Yes;
        keymap["ToggleCallHangup"]       = Qt::Key_ToggleCallHangup;      // A key to toggle the current call state (ie. either answer, or hangup) depending on current call state
        keymap["VoiceDial"]              = Qt::Key_VoiceDial;
        keymap["LastNumberRedial"]       = Qt::Key_LastNumberRedial;
        keymap["Execute"]                = Qt::Key_Execute;
        keymap["Printer"]                = Qt::Key_Printer;
        keymap["Play"]                   = Qt::Key_Play;
        keymap["Sleep"]                  = Qt::Key_Sleep;
        keymap["Zoom"]                   = Qt::Key_Zoom;
        keymap["Cancel"]                 = Qt::Key_Cancel;

        // See: http://qt-project.org/doc/qt-4.8/qt.html#KeyboardModifier-enum
        modmap["No"]                     = Qt::NoModifier;                // No modifier key is pressed.
        modmap["Shift"]                  = Qt::ShiftModifier;             // A Shift key on the keyboard is pressed.
        modmap["Control"]                = Qt::ControlModifier;           // A Ctrl key on the keyboard is pressed.
        modmap["Alt"]                    = Qt::AltModifier;               // An Alt key on the keyboard is pressed.
        modmap["Meta"]                   = Qt::MetaModifier;              // A Meta key on the keyboard is pressed.
        modmap["Keypad"]                 = Qt::KeypadModifier;            // A keypad button is pressed.
        modmap["GroupSwitch"]            = Qt::GroupSwitchModifier;       // X11 only. A Mode_switch key on the keyboard is pressed.
    }

    Qt::Key String2QtKeyEnum::operator[](const QString & combination) throw (anno::exc::FormatException *) {
        const QString ss = combination.mid(combination.lastIndexOf("+") + 1);
        const Qt::Key key = keymap.value(ss, Qt::Key_unknown);
        if (key == Qt::Key_unknown) {
            throw new anno::exc::FormatException(__FILE__, __LINE__, QString("Invalid key [%1].").arg(ss));
        }
        return key;
    }

    Qt::KeyboardModifiers String2QtKeyEnum::modifiers(const QString & combination) throw (anno::exc::FormatException *) {
        Qt::KeyboardModifiers flags = 0 ^ 0;
        QStringList ll = combination.split("+");
        for (int ii = 0; ii + 1 < ll.size(); ++ii) {
            const QString mm = ll.at(ii);
            if (!modmap.contains(mm)) {
                throw new anno::exc::FormatException(__FILE__, __LINE__, QString("Invalid modifier [%1].").arg(mm));
            }
            flags = flags | modmap.value(mm);
        }
        return flags;
    }

} // namespace util

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
