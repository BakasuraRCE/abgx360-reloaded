//---------------------------------------------------------------------------
//
// Name:        abgx360gui.cpp
// Author:      Seacrest
// Created:     10/8/2007 9:06:46 PM
// Description: abgx360gui class implementation
//
//---------------------------------------------------------------------------

/*

replace all:

Notebook = new wxNotebook(Panel, ID_NOTEBOOK, wxPoint(8,168),wxSize(498,198));
    #ifdef __APPLE__
        Notebook = new wxNotebook(Panel, ID_NOTEBOOK, wxPoint(8,168),wxSize(498,212));
    #else
        Notebook = new wxNotebook(Panel, ID_NOTEBOOK, wxPoint(8,168),wxSize(498,198));
    #endif

wxConfig("abgx360gui");
wxConfig(wxT("abgx360gui"));

wxFont(9, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Segoe UI"))
ABGX360GUI_FONT

*/

#ifdef WIN32
#define NEWLINE "\r\n"
#define ABGX360GUI_FACENAME wxT("Segoe UI")
#else
#define NEWLINE "\n"
#ifdef __APPLE__
#define ABGX360GUI_FACENAME wxEmptyString
#else
#define ABGX360GUI_FACENAME wxEmptyString
#endif
#endif

#define ABGX360GUI_FONT wxFont(guifontpointsize, wxSWISS, wxNORMAL, wxNORMAL, false, ABGX360GUI_FACENAME)

#include <list>
#include "abgx360gui.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_abgx360gui_XPM.xpm"
#include "Images/abgx360gui_ID_MNU_OPENINPUTFILE_1748_XPM.xpm"
#include "Images/abgx360gui_ID_MNU_MAKEADONATION_1868_XPM.xpm"
#include "Images/abgx360gui_ID_MNU_GOTOABGX360_NET_1869_XPM.xpm"
#include "Images/InfoTip_XPM.xpm"
#include "Images/abgx360gui_SaveButtonDisabled_XPM.xpm"
#include "Images/abgx360gui_dottedOpenButtonDisabled_XPM.xpm"
#include "Images/abgx360gui_TopBitmap_XPM.xpm"
#include "Images/abgx360gui_SaveButton_XPM.xpm"
#include "Images/abgx360gui_ExtractSSSaveButton_XPM.xpm"
#include "Images/abgx360gui_ExtractDMISaveButton_XPM.xpm"
#include "Images/abgx360gui_ExtractPFISaveButton_XPM.xpm"
#include "Images/abgx360gui_ExtractVideoSaveButton_XPM.xpm"
#include "Images/abgx360gui_PatchSSOpenButton_XPM.xpm"
#include "Images/abgx360gui_PatchDMIOpenButton_XPM.xpm"
#include "Images/abgx360gui_PatchPFIOpenButton_XPM.xpm"
#include "Images/abgx360gui_PatchVideoOpenButton_XPM.xpm"
#include "Images/abgx360gui_OpenButton_XPM.xpm"
////Header Include End
#include "Images/LaunchClick.xpm"
#include "Images/LaunchNormal.xpm"
#include "Images/LaunchOver.xpm"

//----------------------------------------------------------------------------
// Utils
//----------------------------------------------------------------------------

wxBoxSizer *generate_box_sizer_with_controls(const std::list<wxWindow *> &controls, int orient = wxHORIZONTAL) {
  auto *sizer = new wxBoxSizer(orient);
  for (auto control : controls) {
    sizer->Add(control, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL).Border(wxRIGHT, 5));
  }
  return sizer;
}

//----------------------------------------------------------------------------
// InfoTip
//----------------------------------------------------------------------------

////Event Table Start
BEGIN_EVENT_TABLE(InfoTip, wxPanel)EVT_ENTER_WINDOW(InfoTip::onMouseOver) EVT_PAINT(InfoTip::onPaint)

END_EVENT_TABLE()
////Event Table End

InfoTip::InfoTip(wxWindow *parent, const wxBitmap &label, const wxString &infotext, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize) : wxPanel(parent,
                                                                                                                                                                          wxID_ANY,
                                                                                                                                                                          pos,
                                                                                                                                                                          size) {
  mLabel = label;
  mInfotext = infotext;
  this->SetMinSize(wxSize(16, 16));
}

void InfoTip::onMouseOver(wxMouseEvent &WXUNUSED(event)) {
  iconbounds = this->GetScreenRect();
  tip = new wxTipWindow(this, mInfotext, 400, NULL, &iconbounds);
}

void InfoTip::onPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  dc.DrawBitmap(mLabel, 0, 0, true);
  event.Skip();
}

//----------------------------------------------------------------------------
// PrettyButton
//----------------------------------------------------------------------------

////Event Table Start
BEGIN_EVENT_TABLE(PrettyButton, wxPanel)
        EVT_ERASE_BACKGROUND(PrettyButton::onEraseBackground) EVT_PAINT(PrettyButton::onPaint) EVT_ENTER_WINDOW(PrettyButton::onMouseEnter) EVT_LEAVE_WINDOW(PrettyButton::onMouseLeave)
        EVT_LEFT_DOWN(PrettyButton::onMouseClick) EVT_LEFT_UP(PrettyButton::onMouseUp)

END_EVENT_TABLE()
////Event Table End

PrettyButton::PrettyButton(wxWindow *parent, const wxBitmap &normalimage, const wxBitmap &mouseonimage, const wxBitmap &clickimage, const wxPoint &pos, const wxSize &size, int id) : wxPanel(
    parent,
    wxID_ANY,
    pos,
    size) {
  //SetBackgroundStyle(wxBG_STYLE_CUSTOM);  // supposed to reduce flicker
  hover = false;
  click = false;
  mNormalImage = normalimage;
  mOverImage = mouseonimage;
  mClickImage = clickimage;
  mParent = parent;
  mEvent = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, id);
  mSW.Start(0);
}

void PrettyButton::onEraseBackground(wxEraseEvent &WXUNUSED(event)) {
  return;
}

void PrettyButton::onPaint(wxPaintEvent &event) {
  //wxBufferedPaintDC dc(this);
  wxPaintDC dc(this);
  if (click) dc.DrawBitmap(mClickImage, 0, 0);
  else if (hover) dc.DrawBitmap(mOverImage, 0, 0);
  else dc.DrawBitmap(mNormalImage, 0, 0);
  //event.Skip();
}

void PrettyButton::onMouseEnter(wxMouseEvent &event) {
  hover = true;
  if (event.LeftIsDown()) click = true;
  else click = false;
  Refresh();
  //event.Skip();
}

void PrettyButton::onMouseLeave(wxMouseEvent &WXUNUSED(event)) {
  hover = false;
  click = false;
  Refresh();
  //event.Skip();
}

void PrettyButton::onMouseClick(wxMouseEvent &WXUNUSED(event)) {
  click = true;
  Refresh();
  //event.Skip();
}

void PrettyButton::onMouseUp(wxMouseEvent &WXUNUSED(event)) {
  click = false;
  Refresh();
  if (mSW.Time() > 2500) {
    mParent->GetEventHandler()->ProcessEvent(*mEvent);
    mSW.Start(0);
  }
  //event.Skip();
}

//----------------------------------------------------------------------------
// abgx360gui
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(abgx360gui, wxFrame)
////Manual Code Start
        EVT_UPDATE_UI(ID_PANEL, abgx360gui::UIUpdate) EVT_MENU_RANGE(ID_DIR1, ID_DIR9, abgx360gui::MnuDirHistoryClick)

////Manual Code End

        EVT_CLOSE(abgx360gui::OnClose)

        EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, abgx360gui::MnuDirHistoryClick) EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, abgx360gui::MnuFileHistoryClick)

        EVT_MENU(ID_MNU_OPENINPUTFILE_1748, abgx360gui::OpenButtonClick) EVT_MENU(ID_MNU_CLEARHISTORY_1749, abgx360gui::MnuClearHistoryClick)
        EVT_MENU(ID_MNU_CLEARDIRHISTORY_1773, abgx360gui::MnuClearDirHistoryClick) EVT_MENU(ID_MNU_LOADSETTINGS_1743, abgx360gui::MnuLoadSettingsClick)
        EVT_MENU(ID_MNU_SAVESETTINGS_1668, abgx360gui::MnuSaveSettingsClick) EVT_MENU(ID_MNU_DELETESETTINGS_1750, abgx360gui::MnuDeleteSettingsClick)
        EVT_MENU(ID_MNU_EXIT_1671, abgx360gui::MnuExitClick) EVT_MENU(ID_MNU_ABOUT_1678, abgx360gui::MnuAboutClick) EVT_MENU(ID_MNU_GOTOABGX360_NET_1869, abgx360gui::abgx360_netClick)
        EVT_MENU(ID_MNU_WHERE_STEALTHFILES_1872, abgx360gui::WhereStealthFilesClick) EVT_MENU(ID_MNU_WHERE_IMAGES_1875, abgx360gui::WhereImagesClick)

//        EVT_CHOICE(ID_INPUTCHOICE, abgx360gui::InputChoiceSelected)
//        EVT_CHOICE(ID_PROGRAMOUTPUT, abgx360gui::ProgramOutputSelected)
//        EVT_CHOICE(ID_QUICKSTARTCHOICE, abgx360gui::QuickstartChoiceSelected)
//        EVT_BUTTON(ID_SAVEBUTTON, abgx360gui::SaveButtonClick)
//        EVT_BUTTON(ID_OPENBUTTON, abgx360gui::OpenButtonClick)
//        EVT_BUTTON(ID_MATCHONLYBUTTON, abgx360gui::MatchOnlyButtonClick)
//        EVT_BUTTON(ID_RUNBUTTON, abgx360gui::RunButtonClick)
//        EVT_BUTTON(ID_MYREGIONBUTTON, abgx360gui::MyRegionButtonClick)
//        EVT_BUTTON(ID_EXTRACTSSSAVEBUTTON, abgx360gui::ExtractSSSaveButtonClick)
//        EVT_BUTTON(ID_EXTRACTDMISAVEBUTTON, abgx360gui::ExtractDMISaveButtonClick)
//        EVT_BUTTON(ID_EXTRACTPFISAVEBUTTON, abgx360gui::ExtractPFISaveButtonClick)
//        EVT_BUTTON(ID_EXTRACTVIDEOSAVEBUTTON, abgx360gui::ExtractVideoSaveButtonClick)
//        EVT_BUTTON(ID_PATCHSSOPENBUTTON, abgx360gui::PatchSSOpenButtonClick)
//        EVT_BUTTON(ID_PATCHDMIOPENBUTTON, abgx360gui::PatchDMIOpenButtonClick)
//        EVT_BUTTON(ID_PATCHPFIOPENBUTTON, abgx360gui::PatchPFIOpenButtonClick)
//        EVT_BUTTON(ID_PATCHVIDEOOPENBUTTON, abgx360gui::PatchVideoOpenButtonClick)

END_EVENT_TABLE()
////Event Table End

void abgx360gui::assign_events() {

//  OpenButton->Bind(wxEVT_MENU, &abgx360gui::OpenButtonClick, this);
//  MnuClearHistory->Bind(wxEVT_MENU, &abgx360gui::MnuClearHistoryClick, this);
//  MnuClearDirHistory->Bind(wxEVT_MENU, &abgx360gui::MnuClearDirHistoryClick, this);
//  MnuLoadSettings->Bind(wxEVT_MENU, &abgx360gui::MnuLoadSettingsClick, this);
//  MnuSaveSettings->Bind(wxEVT_MENU, &abgx360gui::MnuSaveSettingsClick, this);
//  MnuDeleteSettings->Bind(wxEVT_MENU, &abgx360gui::MnuDeleteSettingsClick, this);
//  MnuExit->Bind(wxEVT_MENU, &abgx360gui::MnuExitClick, this);
//  MnuAbout->Bind(wxEVT_MENU, &abgx360gui::MnuAboutClick, this);
//  abgx360_net->Bind(wxEVT_MENU, &abgx360gui::abgx360_netClick, this);
//  WhereStealthFiles->Bind(wxEVT_MENU, &abgx360gui::WhereStealthFilesClick, this);
//  WhereImages->Bind(wxEVT_MENU, &abgx360gui::WhereImagesClick, this);

  InputChoice->Bind(wxEVT_CHOICE, &abgx360gui::InputChoiceSelected, this);
  ProgramOutput->Bind(wxEVT_CHOICE, &abgx360gui::ProgramOutputSelected, this);
  QuickstartChoice->Bind(wxEVT_CHOICE, &abgx360gui::QuickstartChoiceSelected, this);

  SaveButton->Bind(wxEVT_BUTTON, &abgx360gui::SaveButtonClick, this);
  OpenButton->Bind(wxEVT_BUTTON, &abgx360gui::OpenButtonClick, this);
  MatchOnlyButton->Bind(wxEVT_BUTTON, &abgx360gui::MatchOnlyButtonClick, this);
  LaunchButton->Bind(wxEVT_BUTTON, &abgx360gui::RunButtonClick, this);
  MyRegionButton->Bind(wxEVT_BUTTON, &abgx360gui::MyRegionButtonClick, this);
  ExtractSSSaveButton->Bind(wxEVT_BUTTON, &abgx360gui::ExtractSSSaveButtonClick, this);
  ExtractDMISaveButton->Bind(wxEVT_BUTTON, &abgx360gui::ExtractDMISaveButtonClick, this);
  ExtractPFISaveButton->Bind(wxEVT_BUTTON, &abgx360gui::ExtractPFISaveButtonClick, this);
  ExtractVideoSaveButton->Bind(wxEVT_BUTTON, &abgx360gui::ExtractVideoSaveButtonClick, this);
  PatchSSOpenButton->Bind(wxEVT_BUTTON, &abgx360gui::PatchSSOpenButtonClick, this);
  PatchDMIOpenButton->Bind(wxEVT_BUTTON, &abgx360gui::PatchDMIOpenButtonClick, this);
  PatchPFIOpenButton->Bind(wxEVT_BUTTON, &abgx360gui::PatchPFIOpenButtonClick, this);
  PatchVideoOpenButton->Bind(wxEVT_BUTTON, &abgx360gui::PatchVideoOpenButtonClick, this);
}

abgx360gui::abgx360gui(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize &size, long style) : wxFrame(parent, id, title, position, size, style) {

  this->InfoTip_BITMAP = wxBitmap(InfoTip_XPM);

  this->SetSizeHints(wxSize(700, 800), wxDefaultSize);

  int x, y;
  wxFont tempfont;
  guifontpointsize = 12;
  while (guifontpointsize > 4) {
    tempfont = ABGX360GUI_FONT;
    GetTextExtent(wxT("The quick brown fox jumps over the lazy dog"), &x, &y, NULL, NULL, &tempfont);
    if (x <= 237) break;
    guifontpointsize--;
  }

  //Do not add custom code between
  //GUI Items Creation Start and GUI Items Creation End
  //wxDev-C++ designer will remove them.
  //Add the custom code before or after the blocks
  ////GUI Items Creation Start

  Panel = new wxPanel(this, ID_PANEL, wxPoint(0, 0), wxSize(620, 606));
//    Panel->SetFont(ABGX360GUI_FONT);

  MainSizer = new wxBoxSizer(wxVERTICAL);

  // Top logo
  wxBitmap TopBitmap_BITMAP(abgx360gui_TopBitmap_XPM);
  TopBitmap = new wxStaticBitmap(Panel, ID_TOPBITMAP, TopBitmap_BITMAP, wxPoint(-35, -3), wxSize(600, 67), wxCLIP_CHILDREN | wxNO_BORDER);
  TopBitmap->Enable(false);
//    TopBitmap->SetFont(ABGX360GUI_FONT);
  MainSizer->Add(TopBitmap, 0, wxEXPAND | wxBOTTOM, 5);

  wxStaticBox *InputSizer_StaticBoxObj = new wxStaticBox(Panel, wxID_ANY, wxT("Input"));
  InputSizer = new wxStaticBoxSizer(InputSizer_StaticBoxObj, wxVERTICAL);
  MainSizer->Add(InputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  InputPanel = new wxPanel(Panel, ID_INPUTPANEL, wxPoint(9, 15), wxSize(485, 64));
//    InputPanel->SetFont(ABGX360GUI_FONT);
//    InputSizer->Add(InputPanel, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 4);

  InputSub1Sizer = new wxBoxSizer(wxHORIZONTAL);
  InputSub2Sizer = new wxBoxSizer(wxHORIZONTAL);
  InputSizer->Add(InputSub1Sizer, 0, wxEXPAND, 0);
  InputSizer->Add(InputSub2Sizer, 0, wxEXPAND, 0);

  wxArrayString arrayStringFor_InputChoice;
  arrayStringFor_InputChoice.Add(wxT("File(s)"));
  arrayStringFor_InputChoice.Add(wxT("Directory"));
  arrayStringFor_InputChoice.Add(wxT("Burned DVD"));
  InputChoice = new wxChoice(this, ID_INPUTCHOICE, wxPoint(-1, -1), wxSize(-1, -1), arrayStringFor_InputChoice, 0, wxDefaultValidator, wxT("InputChoice"));
//    InputChoice->SetFont(ABGX360GUI_FONT);
  InputChoice->SetSelection(0);
  InputSub1Sizer->Add(InputChoice, 0, wxALL, 5);

  WriteDisable = new wxCheckBox(this, ID_WRITEDISABLE, wxT("Disable Writes"), wxPoint(-1, -1), wxSize(-1, -1), 0, wxDefaultValidator, wxT("WriteDisable"));
//    WriteDisable->SetFont(ABGX360GUI_FONT);
  InputSub1Sizer->Add(WriteDisable, 0, wxALL, 5);

  MatchOnly = new wxCheckBox(this, ID_MATCHONLY, wxT("Match Only:"), wxPoint(-1, -1), wxSize(-1, -1), 0, wxDefaultValidator, wxT("MatchOnly"));
  MatchOnly->Show(false);
//    MatchOnly->SetFont(ABGX360GUI_FONT);
  InputSub1Sizer->Add(MatchOnly, 0, wxALL, 5);

  MatchOnlyEditBox = new wxTextCtrl(this, ID_MATCHONLYEDITBOX, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0, wxDefaultValidator, wxT("MatchOnlyEditBox"));
  MatchOnlyEditBox->Show(false);
//    MatchOnlyEditBox->SetFont(ABGX360GUI_FONT);
  InputSub1Sizer->Add(MatchOnlyEditBox, 1, wxALL, 5);

  MatchOnlyButton = new wxButton(this, ID_MATCHONLYBUTTON, wxT("Select..."), wxPoint(-1, -1), wxSize(-1, -1), 0, wxDefaultValidator, wxT("MatchOnlyButton"));
  MatchOnlyButton->Show(false);
//    MatchOnlyButton->SetFont(ABGX360GUI_FONT);
  InputSub1Sizer->Add(MatchOnlyButton, 0, wxALL, 5);

  InputFileEditBox = new wxTextCtrl(this, ID_INPUTFILEEDITBOX, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0, wxDefaultValidator, wxT("InputFileEditBox"));
//    InputFileEditBox->SetFont(ABGX360GUI_FONT);
  InputSub2Sizer->Add(InputFileEditBox, 1, wxALL, 5);

  wxBitmap OpenButton_BITMAP(abgx360gui_OpenButton_XPM);
  OpenButton = new wxBitmapButton(this, ID_OPENBUTTON, OpenButton_BITMAP, wxPoint(-1, -1), wxSize(-1, -1), wxBU_AUTODRAW, wxDefaultValidator, wxT("OpenButton"));
//    OpenButton->SetFont(ABGX360GUI_FONT);
  InputSub2Sizer->Add(OpenButton, 0, wxALL, 5);

  Notebook = this->generate_notebook(this);
  MainSizer->Add(Notebook, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  wxStaticBox *OutputSizer_StaticBoxObj = new wxStaticBox(Panel, wxID_ANY, wxT("Program Output"));
  OutputSizer = new wxStaticBoxSizer(OutputSizer_StaticBoxObj, wxHORIZONTAL);
  MainSizer->Add(OutputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  OutputPanel = new wxPanel(Panel, ID_OUTPUTPANEL, wxPoint(9, 15), wxSize(485, 64));
  OutputPanel->SetFont(ABGX360GUI_FONT);
  OutputSizer->Add(OutputPanel, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 4);

  OpenFileWhenDone = new wxCheckBox(OutputPanel, ID_OPENFILEWHENDONE, wxT("Open file when done"), wxPoint(114, 5), wxSize(132, 17), 0, wxDefaultValidator, wxT("OpenFileWhenDone"));
  OpenFileWhenDone->Show(false);
  OpenFileWhenDone->SetFont(ABGX360GUI_FONT);

  wxArrayString arrayStringFor_ProgramOutput;
  arrayStringFor_ProgramOutput.Add(wxT("CLI Window"));
  arrayStringFor_ProgramOutput.Add(wxT("HTML File"));
  arrayStringFor_ProgramOutput.Add(wxT("Text File"));
  ProgramOutput = new wxChoice(OutputPanel, ID_PROGRAMOUTPUT, wxPoint(2, 2), wxSize(100, 23), arrayStringFor_ProgramOutput, 0, wxDefaultValidator, wxT("ProgramOutput"));
  ProgramOutput->SetFont(ABGX360GUI_FONT);
  ProgramOutput->SetSelection(0);

  OutputFileEditBox = new wxTextCtrl(OutputPanel, ID_OUTPUTFILEEDITBOX, wxEmptyString, wxPoint(2, 32), wxSize(434, 19), 0, wxDefaultValidator, wxT("OutputFileEditBox"));
  OutputFileEditBox->Enable(false);
  OutputFileEditBox->SetFont(ABGX360GUI_FONT);

  wxBitmap SaveButton_BITMAP(abgx360gui_SaveButton_XPM);
  SaveButton = new wxBitmapButton(OutputPanel, ID_SAVEBUTTON, SaveButton_BITMAP, wxPoint(446, 29), wxSize(32, 25), wxBU_AUTODRAW, wxDefaultValidator, wxT("SaveButton"));
  SaveButton->SetToolTip(wxT("Choose the file where program output will be saved"));
  SaveButton->Enable(false);
  SaveButton->SetFont(ABGX360GUI_FONT);

  TerminalFont = new wxCheckBox(OutputPanel, ID_TERMINALFONT, wxT("Use Terminal font characters"), wxPoint(250, 5), wxSize(174, 17), 0, wxDefaultValidator, wxT("TerminalFont"));
  TerminalFont->SetValue(true);
  TerminalFont->SetFont(ABGX360GUI_FONT);

  Maximize = new wxCheckBox(OutputPanel, ID_MAXIMIZE, wxT("Maximize it"), wxPoint(114, 5), wxSize(82, 17), 0, wxDefaultValidator, wxT("Maximize"));
  Maximize->SetValue(true);
  Maximize->SetFont(ABGX360GUI_FONT);

  LaunchPanel = new wxPanel(Panel, ID_LAUNCHPANEL, wxPoint(0, 467), wxSize(515, 63));
  LaunchPanel->SetFont(ABGX360GUI_FONT);
  MainSizer->Add(LaunchPanel, 0, wxEXPAND, 0);

  wxBitmap dottedOpenButtonDisabled_BITMAP(abgx360gui_dottedOpenButtonDisabled_XPM);
  dottedOpenButtonDisabled = new wxBitmapButton(Panel,
                                                ID_DOTTEDOPENBUTTONDISABLED,
                                                dottedOpenButtonDisabled_BITMAP,
                                                wxPoint(535, 333),
                                                wxSize(24, 16),
                                                wxBU_AUTODRAW,
                                                wxDefaultValidator,
                                                wxT("dottedOpenButtonDisabled"));
  dottedOpenButtonDisabled->Show(false);
  dottedOpenButtonDisabled->SetFont(ABGX360GUI_FONT);

  wxBitmap SaveButtonDisabled_BITMAP(abgx360gui_SaveButtonDisabled_XPM);
  SaveButtonDisabled =
      new wxBitmapButton(Panel, ID_SAVEBUTTONDISABLED, SaveButtonDisabled_BITMAP, wxPoint(552, 424), wxSize(32, 25), wxBU_AUTODRAW, wxDefaultValidator, wxT("SaveButtonDisabled"));
  SaveButtonDisabled->Show(false);
  SaveButtonDisabled->SetFont(ABGX360GUI_FONT);

  StatusBar = new wxStatusBar(this, ID_STATUSBAR);

  WxMenuBar1 = new wxMenuBar();
  wxMenu *ID_MNU_FILE_1667_Mnu_Obj = new wxMenu(0);
  wxMenuItem *ID_MNU_OPENINPUTFILE_1748_mnuItem_obj = new wxMenuItem(ID_MNU_FILE_1667_Mnu_Obj, ID_MNU_OPENINPUTFILE_1748, wxT("&Open Input...\tCtrl+O"), wxEmptyString, wxITEM_NORMAL);
  wxBitmap ID_MNU_OPENINPUTFILE_1748_mnuItem_obj_BMP(abgx360gui_ID_MNU_OPENINPUTFILE_1748_XPM);
  ID_MNU_OPENINPUTFILE_1748_mnuItem_obj->SetBitmap(ID_MNU_OPENINPUTFILE_1748_mnuItem_obj_BMP);
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_OPENINPUTFILE_1748_mnuItem_obj);

  wxMenu *ID_MNU_REOPEN_1745_Mnu_Obj = new wxMenu(0);
  ID_MNU_REOPEN_1745_Mnu_Obj->Append(ID_MNU_CLEARHISTORY_1749, wxT("&Clear History"), wxEmptyString, wxITEM_NORMAL);

  m_fileHistory = new wxFileHistory(9, wxID_FILE1);
  m_fileHistory->UseMenu(ID_MNU_REOPEN_1745_Mnu_Obj);
  m_fileConfig = new wxConfig(wxT("abgx360gui"));
  wxConfigBase::Set(m_fileConfig);
  m_fileConfig->SetPath(wxT("/RecentFiles"));
  m_fileHistory->Load(*m_fileConfig);
  m_fileConfig->SetPath(wxT(".."));
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_REOPEN_1745, wxT("&Reopen File(s)"), ID_MNU_REOPEN_1745_Mnu_Obj);

  wxMenu *ID_MNU_REOPENDIRECTORY_1772_Mnu_Obj = new wxMenu(0);
  ID_MNU_REOPENDIRECTORY_1772_Mnu_Obj->Append(ID_MNU_CLEARDIRHISTORY_1773, wxT("&Clear History"), wxEmptyString, wxITEM_NORMAL);
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_REOPENDIRECTORY_1772, wxT("R&eopen Directory"), ID_MNU_REOPENDIRECTORY_1772_Mnu_Obj);
  ID_MNU_FILE_1667_Mnu_Obj->AppendSeparator();
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_LOADSETTINGS_1743, wxT("&Load Settings...\tCtrl+L"), wxT("Load saved settings"), wxITEM_NORMAL);
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_SAVESETTINGS_1668, wxT("&Save Settings...\tCtrl+S"), wxT("Save current settings"), wxITEM_NORMAL);
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_DELETESETTINGS_1750, wxT("&Delete Settings...\tCtrl+D"), wxT("Delete saved settings"), wxITEM_NORMAL);
  ID_MNU_FILE_1667_Mnu_Obj->AppendSeparator();
  ID_MNU_FILE_1667_Mnu_Obj->Append(ID_MNU_EXIT_1671, wxT("E&xit\tCtrl+Q"), wxT("Quit abgx360 GUI"), wxITEM_NORMAL);
  WxMenuBar1->Append(ID_MNU_FILE_1667_Mnu_Obj, wxT("&File"));

  wxMenu *ID_MNU_HELP_1669_Mnu_Obj = new wxMenu(0);
  wxMenuItem *ID_MNU_GOTOABGX360_NET_1869_mnuItem_obj = new wxMenuItem(ID_MNU_HELP_1669_Mnu_Obj,
                                                                       ID_MNU_GOTOABGX360_NET_1869,
                                                                       wxT("&http://abgx360.net"),
                                                                       wxT("Go to the official home of abgx360 to get the latest news and updates or join the chat for help"),
                                                                       wxITEM_NORMAL);
  wxBitmap ID_MNU_GOTOABGX360_NET_1869_mnuItem_obj_BMP(abgx360gui_ID_MNU_GOTOABGX360_NET_1869_XPM);
  ID_MNU_GOTOABGX360_NET_1869_mnuItem_obj->SetBitmap(ID_MNU_GOTOABGX360_NET_1869_mnuItem_obj_BMP);
  ID_MNU_HELP_1669_Mnu_Obj->Append(ID_MNU_GOTOABGX360_NET_1869_mnuItem_obj);
  ID_MNU_HELP_1669_Mnu_Obj->AppendSeparator();
  ID_MNU_HELP_1669_Mnu_Obj->Append(ID_MNU_WHERE_STEALTHFILES_1872,
                                   wxT("Where is my Stealth&Files folder...\tCtrl+F"),
                                   wxT("Find out where your StealthFiles are stored and automatically open the folder if you want."),
                                   wxITEM_NORMAL);
  ID_MNU_HELP_1669_Mnu_Obj->Append(ID_MNU_WHERE_IMAGES_1875,
                                   wxT("Where is my &Images folder...\tCtrl+I"),
                                   wxT("Find out where your Images are stored and automatically open the folder if you want."),
                                   wxITEM_NORMAL);
  ID_MNU_HELP_1669_Mnu_Obj->AppendSeparator();
  ID_MNU_HELP_1669_Mnu_Obj->Append(ID_MNU_ABOUT_1678, wxT("&About"), wxT("About abgx360 GUI"), wxITEM_NORMAL);
  WxMenuBar1->Append(ID_MNU_HELP_1669_Mnu_Obj, wxT("&Help"));
  SetMenuBar(WxMenuBar1);

  SaveVideoFileDialog = new wxFileDialog(this,
                                         wxT("Choose a file"),
                                         wxEmptyString,
                                         wxEmptyString,
                                         wxT("Video Files (*video*.iso)|*video*.iso|ISO Files (*.iso)|*.iso|All Files (*.*)|*.*"),
                                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  OpenVideoFileDialog = new wxFileDialog(this,
                                         wxT("Choose a file"),
                                         wxEmptyString,
                                         wxEmptyString,
                                         wxT("Video Files (*video*.iso)|*video*.iso|ISO Files (*.iso)|*.iso|All Files (*.*)|*.*"),
                                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  OpenFileDialog = new wxFileDialog(this,
                                    wxT("Choose a file"),
                                    wxEmptyString,
                                    wxEmptyString,
                                    wxT("ISO/Stealth/Xex files (*.360, *.000, *.iso, *.bin, *.xex)|*.360;*.000;*.iso;*.bin;*.xex|ISO files (*.360, *.000, *.iso)|*.360;*.000;*.iso|Stealth Files (*.bin, *video*.iso)|*.bin;*video*.iso|Xex Files (*.xex)|*.xex|Video Files (*video*.iso)|*video*.iso|PFI Files (*pfi*.bin)|*pfi*.bin|DMI Files (*dmi*.bin)|*dmi*.bin|SS Files (*ss*.bin)|*ss*.bin|All Files (*)|*"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

  SavePFIFileDialogClobber =
      new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("PFI Files (*pfi*.bin)|*pfi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"), wxFD_SAVE);

  OpenSSFileDialog = new wxFileDialog(this,
                                      wxT("Choose a file"),
                                      wxEmptyString,
                                      wxEmptyString,
                                      wxT("SS Files (*ss*.bin)|*ss*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  OpenPFIFileDialog = new wxFileDialog(this,
                                       wxT("Choose a file"),
                                       wxEmptyString,
                                       wxEmptyString,
                                       wxT("PFI Files (*pfi*.bin)|*pfi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                       wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  SaveDMIFileDialog = new wxFileDialog(this,
                                       wxT("Choose a file"),
                                       wxEmptyString,
                                       wxEmptyString,
                                       wxT("DMI Files (*dmi*.bin)|*dmi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                       wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  SaveVideoFileDialogClobber =
      new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("Video Files (*video*.iso)|*video*.iso|ISO Files (*.iso)|*.iso|All Files (*.*)|*.*"), wxFD_SAVE);

  OpenDMIFileDialog = new wxFileDialog(this,
                                       wxT("Choose a file"),
                                       wxEmptyString,
                                       wxEmptyString,
                                       wxT("DMI Files (*dmi*.bin)|*dmi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                       wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  SavePFIFileDialog = new wxFileDialog(this,
                                       wxT("Choose a file"),
                                       wxEmptyString,
                                       wxEmptyString,
                                       wxT("PFI Files (*pfi*.bin)|*pfi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                       wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  SaveHTMLFileDialog =
      new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("HTML Files (*.html, *.htm)|*.html;*.htm|All Files (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  SaveTextFileDialog = new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  InputDirDialog = new wxDirDialog(this, wxT("Choose a directory"), wxEmptyString);

  SaveDMIFileDialogClobber =
      new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("DMI Files (*dmi*.bin)|*dmi*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"), wxFD_SAVE);

  SaveSSFileDialogClobber =
      new wxFileDialog(this, wxT("Choose a file"), wxEmptyString, wxEmptyString, wxT("SS Files (*ss*.bin)|*ss*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"), wxFD_SAVE);

  SaveSSFileDialog = new wxFileDialog(this,
                                      wxT("Choose a file"),
                                      wxEmptyString,
                                      wxEmptyString,
                                      wxT("SS Files (*ss*.bin)|*ss*.bin|.bin Files (*.bin)|*.bin|All Files (*.*)|*.*"),
                                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  SetStatusBar(StatusBar);
  SetTitle(wxT("abgx360 GUI"));
  SetIcon(Self_abgx360gui_XPM);

  ////GUI Items Creation End

#if wxUSE_DRAG_AND_DROP

  // enable drag and drop for input box
  InputPanel->SetDropTarget(new DnDInput(InputFileEditBox, InputChoice, this, ID_INPUTCHOICE));

#endif

  InputChoice->SetSelection(0);
  QuickstartChoice->SetSelection(0);

  wxBitmap LaunchNormal_BITMAP(LaunchNormal_xpm);
  wxBitmap LaunchOver_BITMAP(LaunchOver_xpm);
  wxBitmap LaunchClick_BITMAP(LaunchClick_xpm);

  LaunchButton = new PrettyButton(LaunchPanel, LaunchNormal_BITMAP, LaunchOver_BITMAP, LaunchClick_BITMAP, wxPoint(0, 0), wxSize(515, 63), ID_RUNBUTTON);

  AutoFixTip = new InfoTip(WxNoteBookPage_AutoFix,
                           InfoTip_BITMAP,
                           wxT("Choose the threshold at which abgx360 will attempt to find verified stealth files and automatically patch them if found. Level 1 or 2 is recommended for your own rips and Level 3 is recommended for someone else's rips that you don't fully trust."),
                           wxPoint(463, 15),
                           wxSize(16, 16));
  //LocalOnlyTip = new InfoTip(WxNoteBookPage_AutoFix, InfoTip_BITMAP, wxT("The default behavior is to check the online database first for updated inis and get new stealth files from it when needed. Enabling this option is not recommended because you won't be using the most recent files. If you have no internet connection or wish to stay offline (or the database is down/unreachable for some reason), you can simply check the box in options labeled \"Disable all online functions\"."), wxPoint(463,46), wxSize(16,16));
  TrustSSv2Tip = new InfoTip(WxNoteBookPage_AutoFix, InfoTip_BITMAP, wxT("See \"What is SS v2?\" under the Quickstart tab."), wxPoint(131, 75), wxSize(16, 16));
  FixAngle359Tip = new InfoTip(WxNoteBookPage_AutoFix,
                               InfoTip_BITMAP,
                               wxT("iXtreme versions previous to v1.4 have a bug that will cause a valid angle of 359 degrees to be jittered to an invalid angle of 360 degrees (NOT safe for XBL). It is recommended to upgrade your firmware to the latest iXtreme version to fix this issue instead of relying on this option, but enabling it will adjust 359 to 0 for compatibility with older firmwares. Also note that enabling this option is harmless on newer firmwares (iXtreme v1.4 and later) as it merely changes the SS v1 angle deviation by 1 degree."),
                               wxPoint(251, 95),
                               wxSize(16, 16));
  FixDRTTip = new InfoTip(WxNoteBookPage_AutoFix,
                          InfoTip_BITMAP,
                          wxT("abgx360 decrypts the host's CCRT (Crypted Challenge Response Table) and combines it with the drive's deobfuscated table in order to validate or invalidate the data stored in an additional table used by hacked firmwares to replay responses to challenges issued by the host. Some invalid conditions can't or shouldn't be fixed, but enabling this option will fix the most common conditions like missing C/R data caused by old buggy ripping firmwares or worn out drives. This data is always checked whether this option is enabled or not and an error message will appear if any data is missing or invalid. This option applies to SS v1 only and has no effect on SS v2."),
                          wxPoint(193, 115),
                          wxSize(16, 16));
  FixBadAnglesTip = new InfoTip(WxNoteBookPage_AutoFix,
                                InfoTip_BITMAP,
                                wxT("Enabling this option will adjust the replay angles for Challenge Types 24 and 25 (Response Types 7 and 5) back to their CCRT targets if they deviate more than X degrees (default = 3). See \"Angle Deviation and You\" under the Quickstart tab for more on this. Deviation is always checked whether this option is enabled or not and a warning message will appear if any angle deviates more than 3 degrees (message is yellow - stealth is uncertain) or 9 degrees (message is red - stealth failed). This option applies to SS v1 only and has no effect on SS v2."),
                                wxPoint(455, 135),
                                wxSize(16, 16));

  RebuildDefaultTip = new InfoTip(WxNoteBookPage_Rebuilding, InfoTip_BITMAP, wxT("Requires 7 to 7.5 GB free space on the partition your ISO is located."), wxPoint(117, 50), wxSize(16, 16));
  RebuildLowSpaceTip = new InfoTip(WxNoteBookPage_Rebuilding,
                                   InfoTip_BITMAP,
                                   wxT("Only requires 253 MB free space but will corrupt your ISO if it fails during the rebuilding process."),
                                   wxPoint(159, 70),
                                   wxSize(16, 16));

  //AutoUploadTip = new InfoTip(WxNoteBookPage_Options, InfoTip_BITMAP, wxT("AutoUpload ini and stealth files to the online database if stealth passes but verification fails, and there isn't an exact match already waiting to be verified."), wxPoint(290,76), wxSize(16,16));

  /*
  Tip = new InfoTip(WxNoteBookPage_, InfoTip_BITMAP, wxEmptyString, wxPoint(,), wxSize(16,16));
  Tip = new InfoTip(WxNoteBookPage_, InfoTip_BITMAP, wxEmptyString, wxPoint(,), wxSize(16,16));
  */

  // wxT("abgx360gui")
  m_folderHistory = new wxFileHistory(9, ID_DIR1);
  m_folderHistory->UseMenu(ID_MNU_REOPENDIRECTORY_1772_Mnu_Obj);
  m_fileConfig->SetPath(wxT("/RecentFolders"));
  m_folderHistory->Load(*m_fileConfig);
  m_fileConfig->SetPath(wxT(".."));

#ifndef WIN32
  TerminalFont->SetValue(false);
#ifdef __APPLE__
  OpenFileWhenDone->Show(false);
  OpenFileWhenDone->SetValue(false);
  TerminalFont->Show(false);
#endif
#else
  CHAR lpszDrive[4];
  CHAR lpszDriveForCreateFile[7];
  CHAR cDrive;
  HANDLE hDevice;
  wxString volumestuff;
  CHAR lpDeviceNameBuffer[1024];
  STORAGE_PROPERTY_QUERY query;
  UCHAR outBuf[1024];
  ULONG returnedLength;
  PSTORAGE_DEVICE_DESCRIPTOR devDesc;
  PUCHAR p;
  ULONG i, j;
  for (cDrive='A';cDrive<='Z';cDrive++) {
      memset(outBuf, 0, 1024);
      memset(lpDeviceNameBuffer, 0, 1024);
      sprintf(lpszDrive, "%c:\\", cDrive);
      // get the drive type, if it's a CD-ROM insert it into the list
      if (GetDriveType(lpszDrive) == DRIVE_CDROM) {
          sprintf(lpszDriveForCreateFile, "\\\\.\\%c:", cDrive);
          hDevice = CreateFile(lpszDriveForCreateFile, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
          if (hDevice != INVALID_HANDLE_VALUE) {
              // try to get the device name
              query.PropertyId = StorageDeviceProperty;
              query.QueryType = PropertyStandardQuery;
              if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query,
                                  sizeof(STORAGE_PROPERTY_QUERY), &outBuf, 1024, &returnedLength, NULL)) {
                  devDesc = (PSTORAGE_DEVICE_DESCRIPTOR) outBuf;
                  p = (PUCHAR) outBuf;
                  j = 0;
                  if (devDesc->VendorIdOffset && p[devDesc->VendorIdOffset]) {
                      for (i = devDesc->VendorIdOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++) {
                          lpDeviceNameBuffer[j] = (CHAR) p[i];
                          j++;
                      }
                      // backspace over trailing spaces
                      for (i=j-1;i>0;i--) {
                          if (lpDeviceNameBuffer[i] == ' ') j--;
                          else break;
                      }
                      lpDeviceNameBuffer[j] = ' ';
                      j++;
                  }
                  if (devDesc->ProductIdOffset && p[devDesc->ProductIdOffset]) {
                      for (i = devDesc->ProductIdOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++) {
                          lpDeviceNameBuffer[j] = (CHAR) p[i];
                          j++;
                      }
                      for (i=j-1;i>0;i--) {
                          if (lpDeviceNameBuffer[i] == ' ') j--;
                          else break;
                      }
                      lpDeviceNameBuffer[j] = ' ';
                      j++;
                  }
                  if (devDesc->ProductRevisionOffset && p[devDesc->ProductRevisionOffset]) {
                      for (i = devDesc->ProductRevisionOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++) {
                          lpDeviceNameBuffer[j] = (CHAR) p[i];
                          j++;
                      }
                      for (i=j-1;i>0;i--) {
                          if (lpDeviceNameBuffer[i] == ' ') j--;
                          else break;
                      }
                  }
                  lpDeviceNameBuffer[j] = 0x0;
              }
              CloseHandle(hDevice);
          }
          if (strlen(lpDeviceNameBuffer)) volumestuff.Printf(wxT("%s (%c:)"), lpDeviceNameBuffer, cDrive);
          else volumestuff.Printf(wxT("(%c:)"), cDrive);
          arrayStringFor_DriveChoice.Add(wxT(volumestuff));
      }
  }
  DriveChoice = new wxChoice(InputPanel, ID_DRIVECHOICE, wxPoint(2,30), wxSize(475,23), arrayStringFor_DriveChoice, 0, wxDefaultValidator, wxT("DriveChoice"));
  DriveChoice->Show(false);
  DriveChoice->SetFont(ABGX360GUI_FONT);
  DriveChoice->SetSelection(0);
#endif

  SaveButton->SetBitmapDisabled(SaveButtonDisabled_BITMAP);
  PatchVideoOpenButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  PatchPFIOpenButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  PatchDMIOpenButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  PatchSSOpenButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  ExtractVideoSaveButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  ExtractPFISaveButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  ExtractDMISaveButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);
  ExtractSSSaveButton->SetBitmapDisabled(dottedOpenButtonDisabled_BITMAP);

  // form designer will randomly disable bitmaps...
  TopBitmap->Enable(true);

  // load default settings if they exist
  m_fileConfig->SetPath(wxT("/Settings"));
  if (m_fileConfig->HasGroup(wxT("Default"))) {
    m_fileConfig->SetPath(wxT("Default"));
    doLoadSettings();
    m_fileConfig->SetPath(wxT(".."));
  } else {
    wxString str;
    long dummy;
    if (!m_fileConfig->GetFirstGroup(str, dummy)) {
      // no settings to load/delete
      WxMenuBar1->Enable(ID_MNU_LOADSETTINGS_1743, false);
      WxMenuBar1->Enable(ID_MNU_DELETESETTINGS_1750, false);
    }
  }
  m_fileConfig->SetPath(wxT(".."));

  this->SetSizer(MainSizer);
  this->Layout();

  this->Centre(wxBOTH);

  this->assign_events();
}

abgx360gui::~abgx360gui() {
  delete m_fileConfig;
}

void abgx360gui::OnClose(wxCloseEvent &WXUNUSED(event)) {
  Destroy();
}

wxNotebook *abgx360gui::generate_notebook(wxWindow *parent) {
  Notebook = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
  Notebook->SetMinSize(wxSize(-1, 200));

  Notebook->AddPage(this->generate_page_quickstart(Notebook), wxT("Quickstart"));
  Notebook->AddPage(this->generate_page_options(Notebook), wxT("Options"));
  Notebook->AddPage(this->generate_page_manually_patch(Notebook), wxT("Manually Patch or Extract Files"));
  Notebook->AddPage(this->generate_page_misc(Notebook), wxT("Misc"));

  WxNoteBookPage_AutoFix = new wxPanel(Notebook, ID_WXNOTEBOOKPAGE_AUTOFIX, wxPoint(4, 26), wxSize(490, 168));
  WxNoteBookPage_AutoFix->SetFont(ABGX360GUI_FONT);
  Notebook->AddPage(WxNoteBookPage_AutoFix, wxT("AutoFix"));

  wxArrayString arrayStringFor_AutoFix;
  arrayStringFor_AutoFix.Add(wxT("Level 3 - AutoFix if stealth passes but fails verification"));
  arrayStringFor_AutoFix.Add(wxT("Level 2 - AutoFix if stealth is uncertain and fails verification"));
  arrayStringFor_AutoFix.Add(wxT("Level 1 - AutoFix if stealth fails"));
  arrayStringFor_AutoFix.Add(wxT("Level 0 - Do not AutoFix"));
  AutoFix = new wxChoice(WxNoteBookPage_AutoFix, ID_AUTOFIX, wxPoint(118, 12), wxSize(336, 23), arrayStringFor_AutoFix, 0, wxDefaultValidator, wxT("AutoFix"));
  AutoFix->SetFont(ABGX360GUI_FONT);
  AutoFix->SetSelection(1);

  AutoFixText = new wxStaticText(WxNoteBookPage_AutoFix, ID_AUTOFIXTEXT, wxT("AutoFix Threshold:"), wxPoint(10, 16), wxDefaultSize, 0, wxT("AutoFixText"));
  AutoFixText->SetFont(ABGX360GUI_FONT);

  CRBox = new wxStaticBox(WxNoteBookPage_AutoFix, ID_CRBOX, wxT("SS Challenge / Response Data"), wxPoint(5, 55), wxSize(478, 106));
  CRBox->SetFont(ABGX360GUI_FONT);

  FixDRT = new wxCheckBox(WxNoteBookPage_AutoFix, ID_FIXDRT, wxT("Fix C/R Table if data is invalid"), wxPoint(15, 115), wxSize(175, 17), 0, wxDefaultValidator, wxT("FixDRT"));
  FixDRT->SetValue(true);
  FixDRT->SetFont(ABGX360GUI_FONT);

  FixAngle359 =
      new wxCheckBox(WxNoteBookPage_AutoFix, ID_FIXANGLE359, wxT("Adjust SS v1 angle 359 for iXtreme < v1.4"), wxPoint(15, 95), wxSize(235, 17), 0, wxDefaultValidator, wxT("FixAngle359"));
  FixAngle359->SetFont(ABGX360GUI_FONT);

  FixBadAngles =
      new wxCheckBox(WxNoteBookPage_AutoFix, ID_FIXBADANGLES, wxT("Adjust angles that deviate more than"), wxPoint(15, 135), wxSize(213, 17), 0, wxDefaultValidator, wxT("FixBadAngles"));
  FixBadAngles->SetValue(true);
  FixBadAngles->SetFont(ABGX360GUI_FONT);

  wxArrayString arrayStringFor_FixBadAnglesValue;
  arrayStringFor_FixBadAnglesValue.Add(wxT("0"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("1"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("2"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("3"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("4"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("5"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("6"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("7"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("8"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("9"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("10"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("11"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("12"));
  arrayStringFor_FixBadAnglesValue.Add(wxT("13"));
  FixBadAnglesValue =
      new wxChoice(WxNoteBookPage_AutoFix, ID_FIXBADANGLESVALUE, wxPoint(229, 132), wxSize(46, 23), arrayStringFor_FixBadAnglesValue, 0, wxDefaultValidator, wxT("FixBadAnglesValue"));
  FixBadAnglesValue->SetFont(ABGX360GUI_FONT);
  FixBadAnglesValue->SetSelection(3);

  WxStaticText5 = new wxStaticText(WxNoteBookPage_AutoFix, ID_WXSTATICTEXT5, wxT("degrees from their CCRT targets"), wxPoint(279, 136), wxDefaultSize, 0, wxT("WxStaticText5"));
  WxStaticText5->SetFont(ABGX360GUI_FONT);

  TrustSSv2 = new wxCheckBox(WxNoteBookPage_AutoFix, ID_TRUSTSSV2, wxT("Trust SS v2 angles"), wxPoint(15, 75), wxSize(115, 17), 0, wxDefaultValidator, wxT("TrustSSv2"));
  TrustSSv2->SetValue(true);
  TrustSSv2->SetFont(ABGX360GUI_FONT);

  WxNoteBookPage_Rebuilding = new wxPanel(Notebook, ID_WXNOTEBOOKPAGE_REBUILDING, wxPoint(4, 26), wxSize(490, 168));
  WxNoteBookPage_Rebuilding->SetFont(ABGX360GUI_FONT);
  Notebook->AddPage(WxNoteBookPage_Rebuilding, wxT("Rebuilding"));

  WxStaticText9 = new wxStaticText(WxNoteBookPage_Rebuilding,
                                   ID_WXSTATICTEXT9,
                                   wxT("Choose the method for rebuilding an ISO if it's missing space for a video partition."),
                                   wxPoint(10, 16),
                                   wxDefaultSize,
                                   0,
                                   wxT("WxStaticText9"));
  WxStaticText9->SetFont(ABGX360GUI_FONT);

  RebuildDefault = new wxRadioButton(WxNoteBookPage_Rebuilding, ID_REBUILDDEFAULT, wxT("Default Method"), wxPoint(10, 50), wxSize(104, 17), 0, wxDefaultValidator, wxT("RebuildDefault"));
  RebuildDefault->SetValue(true);
  RebuildDefault->SetFont(ABGX360GUI_FONT);

  RebuildLowSpace =
      new wxRadioButton(WxNoteBookPage_Rebuilding, ID_REBUILDLOWSPACE, wxT("Low Disk Space Method"), wxPoint(10, 70), wxSize(147, 17), 0, wxDefaultValidator, wxT("RebuildLowSpace"));
  RebuildLowSpace->SetFont(ABGX360GUI_FONT);

  NoRebuild = new wxRadioButton(WxNoteBookPage_Rebuilding, ID_NOREBUILD, wxT("Don't rebuild"), wxPoint(10, 90), wxSize(98, 17), 0, wxDefaultValidator, wxT("NoRebuild"));
  NoRebuild->SetFont(ABGX360GUI_FONT);

  KeepOriginalISO = new wxCheckBox(WxNoteBookPage_Rebuilding,
                                   ID_KEEPORIGINALISO,
                                   wxT("Don't delete the original ISO after rebuilding"),
                                   wxPoint(10, 126),
                                   wxSize(254, 17),
                                   0,
                                   wxDefaultValidator,
                                   wxT("KeepOriginalISO"));
  KeepOriginalISO->SetFont(ABGX360GUI_FONT);

  return Notebook;
}

wxPanel *abgx360gui::generate_page_misc(wxWindow *parent) {
  auto *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  auto *root_sizer = new wxBoxSizer(wxVERTICAL);
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);
  auto *sizer_left = new wxBoxSizer(wxVERTICAL);

  MyRegionTip = new InfoTip(panel,
                            InfoTip_BITMAP,
                            wxT("Optionally enter your console's region so abgx360 can display your game's region code in the appropriate color. If a game won't work on your console, the region code will be displayed in red so you won't overlook it and accidentally burn it. You can select multiple regions if you have multiple consoles."));
  MyRegion = new wxStaticText(panel, wxID_ANY, wxT("Your console's region code:"), wxDefaultPosition, wxDefaultSize);
  MyRegionEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  MyRegionEditBox->SetMaxLength(8);
  MyRegionButton = new wxButton(panel, wxID_ANY, wxT("Select..."), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(generate_box_sizer_with_controls({MyRegion, MyRegionEditBox, MyRegionButton, MyRegionTip}), wxSizerFlags().Expand());

  PreferredLangTip = new InfoTip(panel,
                                 InfoTip_BITMAP,
                                 wxT("This will be the preferred language used when abgx360 displays strings from the Xex title resource such as the game description, achievements, avatar awards, etc. Languages that rely heavily on unicode characters are best viewed using HTML output. If the game does not support your preferred language, abgx360 will use the English language strings. Set the Verbosity Level under the Options tab to High if you want to see which languages are supported by an Xex."));
  PreferredLangText = new wxStaticText(panel, wxID_ANY, wxT("Preferred Language:"), wxDefaultPosition, wxDefaultSize);
  wxArrayString arrayStringFor_PreferredLangChoice;
  arrayStringFor_PreferredLangChoice.Add(wxT("English"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Japanese"));
  arrayStringFor_PreferredLangChoice.Add(wxT("German"));
  arrayStringFor_PreferredLangChoice.Add(wxT("French"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Spanish"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Italian"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Korean"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Traditional Chinese"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Portuguese"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Simplified Chinese"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Polish"));
  arrayStringFor_PreferredLangChoice.Add(wxT("Russian"));
  PreferredLangChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, arrayStringFor_PreferredLangChoice);
  PreferredLangChoice->SetSelection(0);
  sizer_left->Add(generate_box_sizer_with_controls({PreferredLangText, PreferredLangChoice, PreferredLangTip}), wxSizerFlags().Expand());

  SplitVidTip = new InfoTip(panel, InfoTip_BITMAP, wxT("See \"To SplitVid or not to SplitVid\" under the Quickstart tab."));
  SplitVidText = new wxStaticText(panel, wxID_ANY, wxT("SplitVid:"), wxDefaultPosition, wxDefaultSize);
  wxArrayString arrayStringFor_SplitVid;
  arrayStringFor_SplitVid.Add(wxT("Add it if it doesn't exist or isn't valid"));
  arrayStringFor_SplitVid.Add(wxT("Remove it if it exists"));
  arrayStringFor_SplitVid.Add(wxT("Leave it the way it is"));
  SplitVid = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, arrayStringFor_SplitVid);
  SplitVid->SetSelection(2);
  sizer_left->Add(generate_box_sizer_with_controls({SplitVid, SplitVidText, SplitVidTip}), wxSizerFlags().Expand());

  XexImagesText = new wxStaticText(panel, wxID_ANY, wxT("Images:"), wxDefaultPosition, wxDefaultSize);
  wxArrayString arrayStringFor_XexImages;
  arrayStringFor_XexImages.Add(wxT("Extract images when using HTML output"));
  arrayStringFor_XexImages.Add(wxT("Extract images always"));
  arrayStringFor_XexImages.Add(wxT("Don't extract images"));
  XexImages = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, arrayStringFor_XexImages);
  XexImages->SetSelection(0);
  EmbedImages = new wxCheckBox(panel, wxID_ANY, wxT("Embed them in HTML"), wxDefaultPosition, wxDefaultSize);
  ImagesTip = new InfoTip(panel,
                          InfoTip_BITMAP,
                          wxT("This refers to the game icon, achievement icons and gamer pictures located within the Xex title resource. Images are extracted to your \"Images\" folder (Press Ctrl+I to find it) with filename \"Title ID-Image ID.png\" (the Title ID will always be displayed when extracting images so that you can find them easily; also note that the game icon will always be Image ID 00008000). Images are only displayed by abgx360 when using HTML output, but you can choose to extract them always if you're using them for your own purpose. Note that not every Xex has a title resource (most bonus discs don't) and therefore images (as well as various strings and achievements) are not always available." NEWLINE NEWLINE "You can optionally choose to embed them in the HTML source code as data URIs for better portability (when using HTML output), otherwise the HTML source will contain absolute links to the images in your local \"Images\" folder."));
  sizer_left->Add(generate_box_sizer_with_controls({XexImagesText, XexImages, EmbedImages, ImagesTip}), wxSizerFlags().Expand());

  WxStaticText10 = new wxStaticText(panel, wxID_ANY, wxT("Add extra command line options here (for advanced users only):"), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(WxStaticText10, wxSizerFlags().Expand());

  ExtraOptionsEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(ExtraOptionsEditBox, wxSizerFlags().Expand());

  // Add sub sizers to panel sizer
  sizer->Add(sizer_left, wxSizerFlags(0).Border(wxALL, 5));

  root_sizer->Add(sizer, wxSizerFlags().Center());
  panel->SetSizer(root_sizer);
  panel->Layout();
  root_sizer->Fit(panel);
  return panel;
}

wxPanel *abgx360gui::generate_page_options(wxWindow *parent) {
  auto *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  auto *root_sizer = new wxBoxSizer(wxVERTICAL);
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);
  auto *sizer_left = new wxBoxSizer(wxVERTICAL);
  auto *sizer_right = new wxBoxSizer(wxVERTICAL);

  // //////
  // Left
  // //////

  CheckDVDFileTip = new InfoTip(panel,
                                InfoTip_BITMAP,
                                wxT("Checks that your ISO has a matching .dvd file with the correct layerbreak and iso filename and corrects it if not. If the .dvd file does not exist, it is created."));
  CheckDVDFile = new wxCheckBox(panel, wxID_ANY, wxT("Check/Create .dvd file"), wxDefaultPosition, wxDefaultSize);
  CheckDVDFile->SetValue(true);
  sizer_left->Add(generate_box_sizer_with_controls({CheckDVDFile, CheckDVDFileTip}), wxSizerFlags().Expand());

  CheckStealthTip = new InfoTip(panel,
                                InfoTip_BITMAP,
                                wxT("Checks that your game appears to have a valid SS (which is required for it to boot on ANY firmware) and appears to have valid stealth files which match the Xex (which are required for it to boot on iXtreme firmware)."));
  CheckStealth = new wxCheckBox(panel, wxID_ANY, wxT("Check Stealth"), wxDefaultPosition, wxDefaultSize);
  CheckStealth->SetValue(true);
  sizer_left->Add(generate_box_sizer_with_controls({CheckStealth, CheckStealthTip}), wxSizerFlags().Expand());

  VerifyTip = new InfoTip(panel,
                          InfoTip_BITMAP,
                          wxT("Verifies that your SS, stealth files and game data match a verified rip in the abgx360 online database. (Do not confuse this with the abgx.net database; that's just a database of scene releases and has nothing to do with verification.) In order for a rip to become verified in the abgx360 database there must be two different uploads from rips which match in every way except for the RawSS CRC (which changes every time you rip a disc due to angle deviation). Verification will fail if no one has uploaded rips which share your Xex/SS/DMI combination (which changes for discs pressed on different equipment and from different masters). It shouldn't worry you if verification fails on your own rip; in that case you should be fine as long as stealth passes, AnyDVD corruption is not detected and your system is stable. However, if you're checking someone else's rip that you don't fully trust then it's recommended to set AutoFix to Level 3, and if verification fails, abgx360 will attempt to find a new set of verified stealth files that match your Xex (and automatically patch them if found)."));
  Verify = new wxCheckBox(panel, wxID_ANY, wxT("Verify if Stealth passes"), wxDefaultPosition, wxDefaultSize);
  Verify->SetValue(true);
  sizer_left->Add(generate_box_sizer_with_controls({Verify, VerifyTip}), wxSizerFlags().Expand());

  CheckVideoPaddingTip = new InfoTip(panel,
                                     InfoTip_BITMAP,
                                     wxT("Checks that Video partitions are zero padded and pads them with zeros if not. This does not affect stealth and is most definitely a waste of time because whatever data is in the padding area is not even visible to the Xbox 360 host. So it doesn't matter if you enable this option or not, but you'll save some time if you leave it disabled. This should not be confused with the random padding check which is always performed on the game partition; that data IS visible to the Xbox 360 host and an ISO without random padding in the game partition will probably not even boot."));
  CheckVideoPadding = new wxCheckBox(panel, wxID_ANY, wxT("Check/Fix Video padding"), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(generate_box_sizer_with_controls({CheckVideoPadding, CheckVideoPaddingTip}), wxSizerFlags().Expand());

  ShowAvatarAwards = new wxCheckBox(panel, wxID_ANY, wxT("Show Avatar Awards"), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(ShowAvatarAwards, wxSizerFlags().Expand());

  UseColors = new wxCheckBox(panel, wxID_ANY, wxT("Use colors"), wxDefaultPosition, wxDefaultSize);
  UseColors->SetValue(true);
  sizer_left->Add(UseColors, wxSizerFlags().Expand());

  VerbosityText = new wxStaticText(panel, wxID_ANY, wxT("Verbosity Level:"), wxDefaultPosition, wxDefaultSize);
  wxArrayString arrayStringFor_Verbosity;
  arrayStringFor_Verbosity.Add(wxT("Low"));
  arrayStringFor_Verbosity.Add(wxT("Medium"));
  arrayStringFor_Verbosity.Add(wxT("High"));
  Verbosity = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, arrayStringFor_Verbosity);
  Verbosity->SetSelection(1);
  sizer_left->Add(generate_box_sizer_with_controls({VerbosityText, Verbosity}), wxSizerFlags().Expand());

  // //////
  // Right
  // //////

  CheckCorruptionTip = new InfoTip(panel,
                                   InfoTip_BITMAP,
                                   wxT("Searches game data sectors for video files inserted by AnyDVD or any other application that does on-the-fly DVD copy protection removal. This check is performed automatically whenever the game CRC is checked so the only purpose of this option is to cause a game CRC check if nothing else did. See \"What is AnyDVD style corruption\" under the Quickstart tab for more info on game data corruption."));
  CheckCorruption = new wxCheckBox(panel, wxID_ANY, wxT("Check for AnyDVD style game data corruption"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(generate_box_sizer_with_controls({CheckCorruption, CheckCorruptionTip}), wxSizerFlags().Expand());

  DisplayFilesystem = new wxCheckBox(panel, wxID_ANY, wxT("Display ISO Filesystem"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(DisplayFilesystem, wxSizerFlags().Expand());

  CheckForUpdatesTip = new InfoTip(panel,
                                   InfoTip_BITMAP,
                                   wxT("\"Check for updates\" will check for updates to abgx360.dat, which contains things like the latest version available and the most current known PFI/Video wave exceptions. Disabling this option is not recommended because you won't be notified when a new version of abgx360 is available and abgx360 might not be using the latest list of PFI/Video exceptions." NEWLINE NEWLINE "The \"Include the CSV\" option will check for updates to GameNameLookup.csv, which is used to look up the full verbose game name entered in the online database. This isn't really necessary since abgx360 can retrieve the game name directly from the default.xex, but you may still find it useful when the Xex's game name is heavily abbreviated (developers can only use a maximum of 22 characters) or there's no title resource present, or maybe you're just checking SS/DMI files and would like to know which game they're from."));
  OnlineUpdate = new wxCheckBox(panel, wxID_ANY, wxT("Check for updates"), wxDefaultPosition, wxDefaultSize);
  OnlineUpdate->SetHelpText(wxT("Download updated versions of GameNameLookup.csv and abgx360.dat from the online database if newer than existing files"));
  OnlineUpdate->SetValue(true);
  CSVUpdate = new wxCheckBox(panel, wxID_ANY, wxT("Include the CSV"), wxDefaultPosition, wxDefaultSize);
  CSVUpdate->SetHelpText(wxT("Download updated versions of GameNameLookup.csv and abgx360.dat from the online database if newer than existing files"));
  sizer_right->Add(generate_box_sizer_with_controls({OnlineUpdate, CSVUpdate, CheckForUpdatesTip}), wxSizerFlags().Expand());

  StayOffline = new wxCheckBox(panel, wxID_ANY, wxT("Disable all online functions"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(StayOffline, wxSizerFlags().Expand());

  GameCRCTip = new InfoTip(panel,
                           InfoTip_BITMAP,
                           wxT("Checks the game data CRC if a verified ini is found to make sure that your game data isn't corrupt. Disabling this option is not recommended. If you want the game CRC to be checked always, you should also enable the option \"Check for AnyDVD style game data corruption\", which will cause a game CRC check if nothing else did and will search for the obvious signs of AnyDVD style corruption. This is the only good reason why you'd want to check the game CRC always, but note that in order for other types of corruption to be detected (for example: single bit errors commonly caused by unstable CPU/RAM), a verified ini must be found."));
  CheckGameCRCWhenNeeded = new wxCheckBox(panel, wxID_ANY, wxT("Check Game Partition CRC when needed"), wxDefaultPosition, wxDefaultSize);
  CheckGameCRCWhenNeeded->SetValue(true);
  sizer_right->Add(generate_box_sizer_with_controls({CheckGameCRCWhenNeeded, GameCRCTip}), wxSizerFlags().Expand());

  RecurseSubdirs = new wxCheckBox(panel, wxID_ANY, wxT("Recurse subdirectories when using Directory Input"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(RecurseSubdirs, wxSizerFlags().Expand());

  ShowAchievements = new wxCheckBox(panel, wxID_ANY, wxT("Show Achievements"), wxDefaultPosition, wxDefaultSize);
  HideSecretAchievements = new wxCheckBox(panel, wxID_ANY, wxT("Hide secret ones"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(generate_box_sizer_with_controls({ShowAchievements, HideSecretAchievements}), wxSizerFlags().Expand());


  // Add sub sizers to panel sizer
  sizer->Add(sizer_left, wxSizerFlags(0).Border(wxALL, 5));
  sizer->Add(sizer_right, wxSizerFlags(0).Border(wxALL, 5));

  root_sizer->Add(sizer, wxSizerFlags().Center());
  panel->SetSizer(root_sizer);
  panel->Layout();
  root_sizer->Fit(panel);
  return panel;
}

wxPanel *abgx360gui::generate_page_quickstart(wxWindow *parent) {
  auto *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  panel->SetMinSize(wxSize(-1, 400));
  auto *sizer = new wxBoxSizer(wxVERTICAL);

  wxArrayString arrayStringFor_QuickstartChoice;
  arrayStringFor_QuickstartChoice.Add(wxT("Welcome!"));
  arrayStringFor_QuickstartChoice.Add(wxT("How to change the default settings or create preset settings"));
  arrayStringFor_QuickstartChoice.Add(wxT("How to check/fix an Xbox 360 ISO or create a .dvd file"));
  arrayStringFor_QuickstartChoice.Add(wxT("Angle Deviation and You"));
  arrayStringFor_QuickstartChoice.Add(wxT("To SplitVid or not to SplitVid"));
  arrayStringFor_QuickstartChoice.Add(wxT("What is AnyDVD style corruption?"));
  arrayStringFor_QuickstartChoice.Add(wxT("Is there such a thing as a \"bad kreon rip\"?"));
  arrayStringFor_QuickstartChoice.Add(wxT("What is SS v2?"));
  arrayStringFor_QuickstartChoice.Add(wxT("Where are my StealthFiles and Images stored?"));
  QuickstartChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, arrayStringFor_QuickstartChoice, 0, wxDefaultValidator, wxT("QuickstartChoice"));
  QuickstartChoice->SetSelection(0);
  sizer->Add(QuickstartChoice, wxSizerFlags().Expand().Border(wxALL, 5));

  QuickstartMemo = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
  QuickstartMemo->SetMinSize(wxSize(-1, 300));
  QuickstartMemo->AppendText(wxT("Welcome to abgx360, the ultimate tool for Xbox 360 ISOs and Stealth files!\n\n"));
  QuickstartMemo->AppendText(wxEmptyString);
  QuickstartMemo->AppendText(wxT("Pick an item from the dropdown list above to get detailed information or instructions "));
  QuickstartMemo->AppendText(wxT("displayed here."));
  QuickstartMemo->SetFocus();
  QuickstartMemo->SetInsertionPointEnd();
  sizer->Add(QuickstartMemo, wxSizerFlags(1).Expand().Border(wxALL, 5));

  panel->SetSizer(sizer);
  panel->Layout();
  sizer->Fit(panel);
  return panel;
}

wxPanel *abgx360gui::generate_page_manually_patch(wxWindow *parent) {
  auto *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  auto *root_sizer = new wxBoxSizer(wxVERTICAL);
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);
  auto *sizer_left = new wxBoxSizer(wxVERTICAL);
  auto *sizer_right = new wxBoxSizer(wxVERTICAL);

  // //////
  // Left
  // //////

  PatchIfStealthPassesTip =
      new InfoTip(panel, InfoTip_BITMAP, wxT("The default behavior is to patch only if stealth fails and isn't AutoFixed, or stealth is uncertain and isn't verified/AutoFixed"));
  PatchIfStealthPasses = new wxCheckBox(panel, wxID_ANY, wxT("Patch even if stealth passes"), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(generate_box_sizer_with_controls({PatchIfStealthPasses, PatchIfStealthPassesTip}), wxSizerFlags().Expand());

  PatchIfInvalid = new wxCheckBox(panel, wxID_ANY, wxT("Patch even if these files are invalid"), wxDefaultPosition, wxDefaultSize);
  sizer_left->Add(PatchIfInvalid, wxSizerFlags().Expand());

  PatchVideo = new wxCheckBox(panel, wxID_ANY, wxT("Patch Video"), wxDefaultPosition, wxDefaultSize);
  PatchVideoEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  PatchVideoEditBox->Enable(false);
  wxBitmap PatchVideoOpenButton_BITMAP(abgx360gui_PatchVideoOpenButton_XPM);
  PatchVideoOpenButton = new wxBitmapButton(panel, wxID_ANY, PatchVideoOpenButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  PatchVideoOpenButton->Enable(false);
  sizer_left->Add(generate_box_sizer_with_controls({PatchVideo, PatchVideoEditBox, PatchVideoOpenButton}), wxSizerFlags().Expand());

  PatchPFI = new wxCheckBox(panel, wxID_ANY, wxT("Patch PFI"), wxDefaultPosition, wxDefaultSize);
  PatchPFIEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  PatchPFIEditBox->Enable(false);
  wxBitmap PatchPFIOpenButton_BITMAP(abgx360gui_PatchPFIOpenButton_XPM);
  PatchPFIOpenButton = new wxBitmapButton(panel, wxID_ANY, PatchPFIOpenButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  PatchPFIOpenButton->Enable(false);
  sizer_left->Add(generate_box_sizer_with_controls({PatchPFI, PatchPFIEditBox, PatchPFIOpenButton}), wxSizerFlags().Expand());

  PatchDMI = new wxCheckBox(panel, wxID_ANY, wxT("Patch DMI"), wxDefaultPosition, wxDefaultSize);
  PatchDMIEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  PatchDMIEditBox->Enable(false);
  wxBitmap PatchDMIOpenButton_BITMAP(abgx360gui_PatchDMIOpenButton_XPM);
  PatchDMIOpenButton = new wxBitmapButton(panel, wxID_ANY, PatchDMIOpenButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  PatchDMIOpenButton->Enable(false);
  sizer_left->Add(generate_box_sizer_with_controls({PatchDMI, PatchDMIEditBox, PatchDMIOpenButton}), wxSizerFlags().Expand());

  PatchSS = new wxCheckBox(panel, wxID_ANY, wxT("Patch SS"), wxDefaultPosition, wxDefaultSize);
  PatchSSEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  PatchSSEditBox->Enable(false);
  wxBitmap PatchSSOpenButton_BITMAP(abgx360gui_PatchSSOpenButton_XPM);
  PatchSSOpenButton = new wxBitmapButton(panel, wxID_ANY, PatchSSOpenButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator);
  PatchSSOpenButton->Enable(false);
  sizer_left->Add(generate_box_sizer_with_controls({PatchSS, PatchSSEditBox, PatchSSOpenButton}), wxSizerFlags().Expand());

  // //////
  // Right
  // //////

  Clobber = new wxCheckBox(panel, wxID_ANY, wxT("Overwrite these files without prompting"), wxDefaultPosition, wxDefaultSize);
  sizer_right->Add(Clobber, wxSizerFlags().Expand());

  ExtractVideo = new wxCheckBox(panel, wxID_ANY, wxT("Extract Video"), wxDefaultPosition, wxDefaultSize);
  ExtractVideoEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  ExtractVideoEditBox->Enable(false);
  wxBitmap ExtractVideoSaveButton_BITMAP(abgx360gui_ExtractVideoSaveButton_XPM);
  ExtractVideoSaveButton = new wxBitmapButton(panel, wxID_ANY, ExtractVideoSaveButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  ExtractVideoSaveButton->Enable(false);
  sizer_right->Add(generate_box_sizer_with_controls({ExtractVideo, ExtractVideoEditBox, ExtractVideoSaveButton}), wxSizerFlags().Expand());

  ExtractEntireVideoPartition = new wxCheckBox(panel, wxID_ANY, wxT("Extract entire video partition (253 MB)"), wxDefaultPosition, wxDefaultSize);
  ExtractEntireVideoPartition->Enable(false);
  sizer_right->Add(ExtractEntireVideoPartition, wxSizerFlags().Expand());

  ExtractPFI = new wxCheckBox(panel, wxID_ANY, wxT("Extract PFI"), wxDefaultPosition, wxDefaultSize);
  ExtractPFIEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  ExtractPFIEditBox->Enable(false);
  wxBitmap ExtractPFISaveButton_BITMAP(abgx360gui_ExtractPFISaveButton_XPM);
  ExtractPFISaveButton = new wxBitmapButton(panel, wxID_ANY, ExtractPFISaveButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  ExtractPFISaveButton->Enable(false);
  sizer_right->Add(generate_box_sizer_with_controls({ExtractPFI, ExtractPFIEditBox, ExtractPFISaveButton}), wxSizerFlags().Expand());

  ExtractDMI = new wxCheckBox(panel, wxID_ANY, wxT("Extract DMI"), wxDefaultPosition, wxDefaultSize);
  ExtractDMIEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  ExtractDMIEditBox->Enable(false);
  wxBitmap ExtractDMISaveButton_BITMAP(abgx360gui_ExtractDMISaveButton_XPM);
  ExtractDMISaveButton = new wxBitmapButton(panel, wxID_ANY, ExtractDMISaveButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  ExtractDMISaveButton->Enable(false);
  sizer_right->Add(generate_box_sizer_with_controls({ExtractDMI, ExtractDMIEditBox, ExtractDMISaveButton}), wxSizerFlags().Expand());

  ExtractSS = new wxCheckBox(panel, wxID_ANY, wxT("Extract SS"), wxDefaultPosition, wxDefaultSize);
  ExtractSSEditBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  ExtractSSEditBox->Enable(false);
  wxBitmap ExtractSSSaveButton_BITMAP(abgx360gui_ExtractSSSaveButton_XPM);
  ExtractSSSaveButton = new wxBitmapButton(panel, wxID_ANY, ExtractSSSaveButton_BITMAP, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  ExtractSSSaveButton->Enable(false);
  sizer_right->Add(generate_box_sizer_with_controls({ExtractSS, ExtractSSEditBox, ExtractSSSaveButton}), wxSizerFlags().Expand());

  // Add sub sizers to panel sizer
  sizer->Add(sizer_left, wxSizerFlags(0).Border(wxALL, 5));
  sizer->Add(sizer_right, wxSizerFlags(0).Border(wxALL, 5));

  root_sizer->Add(sizer, wxSizerFlags().Center());

  WxStaticText2 = new wxStaticText(panel, wxID_ANY, wxT("Note: if you choose to both patch and extract files, extraction will be done first"), wxDefaultPosition, wxDefaultSize);
  root_sizer->Add(WxStaticText2, wxSizerFlags().Center());

  panel->SetSizer(root_sizer);
  panel->Layout();
  root_sizer->Fit(panel);
  return panel;
}

/*
 * MnuSaveSettingsClick
 */
void abgx360gui::MnuSaveSettingsClick(wxCommandEvent &WXUNUSED(event)) {
  wxString name = wxT("/Settings/");
  name += wxGetTextFromUser(wxT("Enter a name for these settings. Saving them as \"Default\" will" NEWLINE"cause them to be loaded every time abgx360 GUI is launched."),
                            wxT("Save Settings"),
                            wxT("Default"));

  if (name.Cmp(wxT("/Settings/")) == 0) return;

  m_fileConfig->SetPath(name);

  // input
  m_fileConfig->Write(wxT("InputChoice"), InputChoice->GetCurrentSelection());
  m_fileConfig->Write(wxT("InputFileEditBox"), InputFileEditBox->GetValue());
  m_fileConfig->Write(wxT("MatchOnly"), MatchOnly->IsChecked());
  m_fileConfig->Write(wxT("MatchOnlyEditBox"), MatchOnlyEditBox->GetValue());
#ifdef WIN32
  if (arrayStringFor_DriveChoice.GetCount())
      m_fileConfig->Write(wxT("DriveChoice"), DriveChoice->GetCurrentSelection());
#endif
  m_fileConfig->Write(wxT("WriteDisable"), WriteDisable->IsChecked());

  // output
  m_fileConfig->Write(wxT("ProgramOutput"), ProgramOutput->GetCurrentSelection());
  m_fileConfig->Write(wxT("Maximize"), Maximize->IsChecked());
  m_fileConfig->Write(wxT("OpenFileWhenDone"), OpenFileWhenDone->IsChecked());
  m_fileConfig->Write(wxT("TerminalFont"), TerminalFont->IsChecked());
  m_fileConfig->Write(wxT("OutputFileEditBox"), OutputFileEditBox->GetValue());

  // options
  m_fileConfig->Write(wxT("CheckDVDFile"), CheckDVDFile->IsChecked());
  m_fileConfig->Write(wxT("OnlineUpdate"), OnlineUpdate->IsChecked());
  m_fileConfig->Write(wxT("CSVUpdate"), CSVUpdate->IsChecked());
  m_fileConfig->Write(wxT("Verify"), Verify->IsChecked());
  //m_fileConfig->Write(wxT("AutoUpload"), AutoUpload->IsChecked());
  m_fileConfig->Write(wxT("CheckCorruption"), CheckCorruption->IsChecked());
  m_fileConfig->Write(wxT("UseColors"), UseColors->IsChecked());
  m_fileConfig->Write(wxT("StayOffline"), StayOffline->IsChecked());
  m_fileConfig->Write(wxT("RecurseSubdirs"), RecurseSubdirs->IsChecked());
  m_fileConfig->Write(wxT("CheckStealth"), CheckStealth->IsChecked());
  m_fileConfig->Write(wxT("Verbosity"), Verbosity->GetCurrentSelection());
  m_fileConfig->Write(wxT("CheckGameCRCWhenNeeded"), CheckGameCRCWhenNeeded->IsChecked());
  m_fileConfig->Write(wxT("CheckVideoPadding"), CheckVideoPadding->IsChecked());
  m_fileConfig->Write(wxT("DisplayFilesystem"), DisplayFilesystem->IsChecked());
  m_fileConfig->Write(wxT("ShowAchievements"), ShowAchievements->IsChecked());
  m_fileConfig->Write(wxT("HideSecretAchievements"), HideSecretAchievements->IsChecked());
  m_fileConfig->Write(wxT("ShowAvatarAwards"), ShowAvatarAwards->IsChecked());

  // manually patch or extract files
  m_fileConfig->Write(wxT("PatchIfStealthPasses"), PatchIfStealthPasses->IsChecked());
  m_fileConfig->Write(wxT("PatchIfInvalid"), PatchIfInvalid->IsChecked());
  m_fileConfig->Write(wxT("PatchVideo"), PatchVideo->IsChecked());
  m_fileConfig->Write(wxT("PatchPFI"), PatchPFI->IsChecked());
  m_fileConfig->Write(wxT("PatchDMI"), PatchDMI->IsChecked());
  m_fileConfig->Write(wxT("PatchSS"), PatchSS->IsChecked());
  m_fileConfig->Write(wxT("Clobber"), Clobber->IsChecked());
  m_fileConfig->Write(wxT("ExtractVideo"), ExtractVideo->IsChecked());
  m_fileConfig->Write(wxT("ExtractEntireVideoPartition"), ExtractEntireVideoPartition->IsChecked());
  m_fileConfig->Write(wxT("ExtractPFI"), ExtractPFI->IsChecked());
  m_fileConfig->Write(wxT("ExtractDMI"), ExtractDMI->IsChecked());
  m_fileConfig->Write(wxT("ExtractSS"), ExtractSS->IsChecked());
  m_fileConfig->Write(wxT("PatchVideoEditBox"), PatchVideoEditBox->GetValue());
  m_fileConfig->Write(wxT("PatchPFIEditBox"), PatchPFIEditBox->GetValue());
  m_fileConfig->Write(wxT("PatchDMIEditBox"), PatchDMIEditBox->GetValue());
  m_fileConfig->Write(wxT("PatchSSEditBox"), PatchSSEditBox->GetValue());
  m_fileConfig->Write(wxT("ExtractVideoEditBox"), ExtractVideoEditBox->GetValue());
  m_fileConfig->Write(wxT("ExtractPFIEditBox"), ExtractPFIEditBox->GetValue());
  m_fileConfig->Write(wxT("ExtractDMIEditBox"), ExtractDMIEditBox->GetValue());
  m_fileConfig->Write(wxT("ExtractSSEditBox"), ExtractSSEditBox->GetValue());

  // autofix
  m_fileConfig->Write(wxT("AutoFix"), AutoFix->GetCurrentSelection());
  m_fileConfig->Write(wxT("FixDRT"), FixDRT->IsChecked());
  m_fileConfig->Write(wxT("FixAngle359"), FixAngle359->IsChecked());
  m_fileConfig->Write(wxT("FixBadAngles"), FixBadAngles->IsChecked());
  m_fileConfig->Write(wxT("FixBadAnglesValue"), FixBadAnglesValue->GetCurrentSelection());
  m_fileConfig->Write(wxT("TrustSSv2"), TrustSSv2->IsChecked());

  // rebuilding method
  m_fileConfig->Write(wxT("RebuildDefault"), RebuildDefault->GetValue());
  m_fileConfig->Write(wxT("RebuildLowSpace"), RebuildLowSpace->GetValue());
  m_fileConfig->Write(wxT("NoRebuild"), NoRebuild->GetValue());
  m_fileConfig->Write(wxT("KeepOriginalISO"), KeepOriginalISO->IsChecked());

  // misc
  m_fileConfig->Write(wxT("ExtraOptionsEditBox"), ExtraOptionsEditBox->GetValue());
  m_fileConfig->Write(wxT("MyRegionEditBox"), MyRegionEditBox->GetValue());
  m_fileConfig->Write(wxT("PreferredLangChoice"), PreferredLangChoice->GetCurrentSelection());
  m_fileConfig->Write(wxT("SplitVid"), SplitVid->GetCurrentSelection());
  m_fileConfig->Write(wxT("XexImages"), XexImages->GetCurrentSelection());
  m_fileConfig->Write(wxT("EmbedImages"), EmbedImages->IsChecked());

  m_fileConfig->SetPath(wxT("../.."));

  WxMenuBar1->Enable(ID_MNU_LOADSETTINGS_1743, true);
  WxMenuBar1->Enable(ID_MNU_DELETESETTINGS_1750, true);
}

/*
 * MnuDeleteSettingsClick
 */
void abgx360gui::MnuDeleteSettingsClick(wxCommandEvent &WXUNUSED(event)) {
  wxArrayString aNames;
  wxString str;
  long dummy;
  wxArrayInt aDelete;
  size_t i;

  m_fileConfig->SetPath(wxT("/Settings"));

  // enumerate all groups
  bool bCont = m_fileConfig->GetFirstGroup(str, dummy);
  while (bCont) {
    aNames.Add(str);
    bCont = m_fileConfig->GetNextGroup(str, dummy);
  }

  wxGetMultipleChoices(aDelete, wxT("Check the box next to any saved settings you want to delete"), wxT("Delete Settings"), aNames);

  for (i = 0; i < aDelete.GetCount(); i++) {
    m_fileConfig->DeleteGroup(aNames[aDelete[i]]);
  }

  if (!m_fileConfig->GetFirstGroup(str, dummy)) {
    // no more settings to load/delete
    WxMenuBar1->Enable(ID_MNU_LOADSETTINGS_1743, false);
    WxMenuBar1->Enable(ID_MNU_DELETESETTINGS_1750, false);
  }

  m_fileConfig->SetPath(wxT(".."));
}

void abgx360gui::doLoadSettings() {
  long l;
  bool b;

  // input
  if (m_fileConfig->Read(wxT("InputChoice"), &l)) InputChoice->SetSelection(l);
  InputFileEditBox->SetValue(m_fileConfig->Read(wxT("InputFileEditBox")));
  if (m_fileConfig->Read(wxT("MatchOnly"), &b)) MatchOnly->SetValue(b);
  MatchOnlyEditBox->SetValue(m_fileConfig->Read(wxT("MatchOnlyEditBox")));
#ifdef WIN32
  if (m_fileConfig->Read(wxT("DriveChoice"), &l) &&
      arrayStringFor_DriveChoice.GetCount() > (size_t) l) DriveChoice->SetSelection(l);
#endif
  if (m_fileConfig->Read(wxT("WriteDisable"), &b)) WriteDisable->SetValue(b);

  // output
  if (m_fileConfig->Read(wxT("ProgramOutput"), &l)) ProgramOutput->SetSelection(l);
  if (m_fileConfig->Read(wxT("Maximize"), &b)) Maximize->SetValue(b);
  if (m_fileConfig->Read(wxT("OpenFileWhenDone"), &b)) OpenFileWhenDone->SetValue(b);
  if (m_fileConfig->Read(wxT("TerminalFont"), &b)) TerminalFont->SetValue(b);
  OutputFileEditBox->SetValue(m_fileConfig->Read(wxT("OutputFileEditBox")));

  // options
  if (m_fileConfig->Read(wxT("CheckDVDFile"), &b)) CheckDVDFile->SetValue(b);
  if (m_fileConfig->Read(wxT("OnlineUpdate"), &b)) OnlineUpdate->SetValue(b);
  if (m_fileConfig->Read(wxT("CSVUpdate"), &b)) CSVUpdate->SetValue(b);
  if (m_fileConfig->Read(wxT("Verify"), &b)) Verify->SetValue(b);
  //if (m_fileConfig->Read("AutoUpload", &b)) AutoUpload->SetValue(b);
  if (m_fileConfig->Read(wxT("CheckCorruption"), &b)) CheckCorruption->SetValue(b);
  if (m_fileConfig->Read(wxT("UseColors"), &b)) UseColors->SetValue(b);
  if (m_fileConfig->Read(wxT("StayOffline"), &b)) StayOffline->SetValue(b);
  if (m_fileConfig->Read(wxT("RecurseSubdirs"), &b)) RecurseSubdirs->SetValue(b);
  if (m_fileConfig->Read(wxT("CheckStealth"), &b)) CheckStealth->SetValue(b);
  if (m_fileConfig->Read(wxT("Verbosity"), &l)) Verbosity->SetSelection(l);
  if (m_fileConfig->Read(wxT("CheckGameCRCWhenNeeded"), &b)) CheckGameCRCWhenNeeded->SetValue(b);
  if (m_fileConfig->Read(wxT("CheckVideoPadding"), &b)) CheckVideoPadding->SetValue(b);
  if (m_fileConfig->Read(wxT("DisplayFilesystem"), &b)) DisplayFilesystem->SetValue(b);
  if (m_fileConfig->Read(wxT("ShowAchievements"), &b)) ShowAchievements->SetValue(b);
  if (m_fileConfig->Read(wxT("HideSecretAchievements"), &b)) HideSecretAchievements->SetValue(b);
  if (m_fileConfig->Read(wxT("ShowAvatarAwards"), &b)) ShowAvatarAwards->SetValue(b);

  // manually patch or extract files
  if (m_fileConfig->Read(wxT("PatchIfStealthPasses"), &b)) PatchIfStealthPasses->SetValue(b);
  if (m_fileConfig->Read(wxT("PatchIfInvalid"), &b)) PatchIfInvalid->SetValue(b);
  if (m_fileConfig->Read(wxT("PatchVideo"), &b)) PatchVideo->SetValue(b);
  if (m_fileConfig->Read(wxT("PatchPFI"), &b)) PatchPFI->SetValue(b);
  if (m_fileConfig->Read(wxT("PatchDMI"), &b)) PatchDMI->SetValue(b);
  if (m_fileConfig->Read(wxT("PatchSS"), &b)) PatchSS->SetValue(b);
  if (m_fileConfig->Read(wxT("Clobber"), &b)) Clobber->SetValue(b);
  if (m_fileConfig->Read(wxT("ExtractVideo"), &b)) ExtractVideo->SetValue(b);
  if (m_fileConfig->Read(wxT("ExtractEntireVideoPartition"), &b)) ExtractEntireVideoPartition->SetValue(b);
  if (m_fileConfig->Read(wxT("ExtractPFI"), &b)) ExtractPFI->SetValue(b);
  if (m_fileConfig->Read(wxT("ExtractDMI"), &b)) ExtractDMI->SetValue(b);
  if (m_fileConfig->Read(wxT("ExtractSS"), &b)) ExtractSS->SetValue(b);
  PatchVideoEditBox->SetValue(m_fileConfig->Read(wxT("PatchVideoEditBox")));
  PatchPFIEditBox->SetValue(m_fileConfig->Read(wxT("PatchPFIEditBox")));
  PatchDMIEditBox->SetValue(m_fileConfig->Read(wxT("PatchDMIEditBox")));
  PatchSSEditBox->SetValue(m_fileConfig->Read(wxT("PatchSSEditBox")));
  ExtractVideoEditBox->SetValue(m_fileConfig->Read(wxT("ExtractVideoEditBox")));
  ExtractPFIEditBox->SetValue(m_fileConfig->Read(wxT("ExtractPFIEditBox")));
  ExtractDMIEditBox->SetValue(m_fileConfig->Read(wxT("ExtractDMIEditBox")));
  ExtractSSEditBox->SetValue(m_fileConfig->Read(wxT("ExtractSSEditBox")));

  // autofix
  if (m_fileConfig->Read(wxT("AutoFix"), &l)) AutoFix->SetSelection(l);
  if (m_fileConfig->Read(wxT("FixDRT"), &b)) FixDRT->SetValue(b);
  if (m_fileConfig->Read(wxT("FixAngle359"), &b)) FixAngle359->SetValue(b);
  if (m_fileConfig->Read(wxT("FixBadAngles"), &b)) FixBadAngles->SetValue(b);
  if (m_fileConfig->Read(wxT("FixBadAnglesValue"), &l)) FixBadAnglesValue->SetSelection(l);
  if (m_fileConfig->Read(wxT("TrustSSv2"), &b)) TrustSSv2->SetValue(b);

  // rebuilding method
  if (m_fileConfig->Read(wxT("RebuildDefault"), &b)) RebuildDefault->SetValue(b);
  if (m_fileConfig->Read(wxT("RebuildLowSpace"), &b)) RebuildLowSpace->SetValue(b);
  if (m_fileConfig->Read(wxT("NoRebuild"), &b)) NoRebuild->SetValue(b);
  if (m_fileConfig->Read(wxT("KeepOriginalISO"), &b)) KeepOriginalISO->SetValue(b);

  // misc
  ExtraOptionsEditBox->SetValue(m_fileConfig->Read(wxT("ExtraOptionsEditBox")));
  MyRegionEditBox->SetValue(m_fileConfig->Read(wxT("MyRegionEditBox")));
  if (m_fileConfig->Read(wxT("PreferredLangChoice"), &l)) PreferredLangChoice->SetSelection(l);
  if (m_fileConfig->Read(wxT("SplitVid"), &l)) SplitVid->SetSelection(l);
  if (m_fileConfig->Read(wxT("XexImages"), &l)) XexImages->SetSelection(l);
  if (m_fileConfig->Read(wxT("EmbedImages"), &b)) EmbedImages->SetValue(b);

  // show/hide output options that would normally only be shown/hidden when ProgramOutput is clicked on
  if (ProgramOutput->GetCurrentSelection() == 0) {  // command prompt
    Maximize->Show();
    OpenFileWhenDone->Hide();
    OutputFileEditBox->Enable(false);
    SaveButton->Enable(false);
  } else {
    Maximize->Hide();
#ifndef __APPLE__
    OpenFileWhenDone->Show();
#endif
    OutputFileEditBox->Enable(true);
    SaveButton->Enable(true);
  }

  if (InputChoice->GetCurrentSelection() == 1) {  // directory
    OpenButton->SetToolTip(wxT("Open Directory"));
    MatchOnly->Show();
    MatchOnlyEditBox->Show();
    MatchOnlyButton->Show();
    WriteDisable->Show();
#ifdef WIN32
    DriveChoice->Hide();
    InputFileEditBox->Show();
    OpenButton->Show();
#endif
  }
#ifdef WIN32
    else if (InputChoice->GetCurrentSelection() == 2) {  // burned dvd
        MatchOnly->Hide();
        MatchOnlyEditBox->Hide();
        MatchOnlyButton->Hide();
        DriveChoice->Show();
        InputFileEditBox->Hide();
        OpenButton->Hide();
        WriteDisable->Hide();
    }
#endif
    /*
    else if (InputChoice->GetCurrentSelection() == 3) {  // original disc
        WriteDisable->Hide();
        MatchOnly->Hide();
        MatchOnlyEditBox->Hide();
        MatchOnlyButton->Hide();
        #ifdef WIN32
            DriveChoice->Show();
            InputFileEditBox->Hide();
            OpenButton->Hide();
        #else
            OpenButton->SetToolTip(wxT("Open Ripper Device"));
        #endif
    }
    */
  else {  // file(s)
    OpenButton->SetToolTip(wxT("Open File(s)"));
    MatchOnly->Hide();
    MatchOnlyEditBox->Hide();
    MatchOnlyButton->Hide();
    WriteDisable->Show();
#ifdef WIN32
    DriveChoice->Hide();
    InputFileEditBox->Show();
    OpenButton->Show();
#endif
  }
}

/*
 * MnuLoadSettingsClick
 */
void abgx360gui::MnuLoadSettingsClick(wxCommandEvent &WXUNUSED(event)) {
  wxArrayString aNames;
  wxString str;
  long dummy;
  int choice;

  m_fileConfig->SetPath(wxT("/Settings"));

  // enumerate all groups
  bool bCont = m_fileConfig->GetFirstGroup(str, dummy);
  while (bCont) {
    aNames.Add(str);
    bCont = m_fileConfig->GetNextGroup(str, dummy);
  }

  choice = wxGetSingleChoiceIndex(wxT("Choose settings to load"), wxT("Load Settings"), aNames);

  if (choice == -1) {
    m_fileConfig->SetPath(wxT(".."));
    return;
  }

  m_fileConfig->SetPath(aNames[choice]);
  doLoadSettings();
  m_fileConfig->SetPath(wxT("../.."));
}

void abgx360gui::UIUpdate(wxUpdateUIEvent &WXUNUSED(event)) {
  if (m_fileHistory->GetCount()) WxMenuBar1->Enable(ID_MNU_REOPEN_1745, true);
  else WxMenuBar1->Enable(ID_MNU_REOPEN_1745, false);
  if (m_folderHistory->GetCount()) WxMenuBar1->Enable(ID_MNU_REOPENDIRECTORY_1772, true);
  else WxMenuBar1->Enable(ID_MNU_REOPENDIRECTORY_1772, false);

  if (StayOffline->IsChecked()) {
    OnlineUpdate->SetValue(false);
    OnlineUpdate->Enable(false);
  } else OnlineUpdate->Enable(true);
  if (!OnlineUpdate->IsChecked()) {
    CSVUpdate->SetValue(false);
    CSVUpdate->Enable(false);
  } else CSVUpdate->Enable(true);
  if (!CheckStealth->IsChecked()) {
    Verify->Enable(false);
    Verify->SetValue(false);
  } else Verify->Enable(true);

  if (XexImages->GetSelection() == 2) { // Don't extract images
    EmbedImages->Enable(false);
    EmbedImages->SetValue(false);
  } else EmbedImages->Enable(true);

  cmd.Empty();

#if !defined(WIN32) && !defined(__APPLE__)
  cmd += wxT("xterm ");
  if (ProgramOutput->GetCurrentSelection() == 0) cmd += wxT("-bg black ");
  if (Maximize->IsChecked()) cmd += wxT("-geometry 80x400+0+0 ");
  cmd += wxT("-e '");
#endif

  // locate abgx360 binary within an OSX .app bundle
#if defined(__APPLE__)
  CFURLRef abgx360url = CFBundleCopyAuxiliaryExecutableURL(CFBundleGetMainBundle(), CFSTR("abgx360"));
      char abgx360path[512]; // large size, just in case
      abgx360path[0] = '\"'; // double quote at start, to escape all spaces in binary path
      CFURLGetFileSystemRepresentation(abgx360url, true, reinterpret_cast<UInt8*>(&abgx360path[1]), 511);
      if(abgx360path[1] != '\0')
          cmd = cmd + abgx360path + "\" -"; // full path to abgx360 binary, plus end double quote, plus hyphen
      else // string is empty (e.g. not started from an app bundle), use default value. abgx360 binary must be in PATH
#endif

  cmd += wxT("abgx360 -");
  if (Verbosity->GetCurrentSelection() == 0) cmd += wxT("n"); // low verbosity
  else if (Verbosity->GetCurrentSelection() == 2) cmd += wxT("v"); // high verbosity
  if (!CheckStealth->IsChecked()) cmd += wxT("r");
  if (CheckVideoPadding->IsChecked()) cmd += wxT("p");
  if (CheckCorruption->IsChecked()) cmd += wxT("c");
  if (!UseColors->IsChecked() || ProgramOutput->GetCurrentSelection() == 2) cmd += wxT("s"); // ProgramOutput 2 = text file
  if (!CheckGameCRCWhenNeeded->IsChecked()) {
    cmd += wxT("g"); // check game crc never
    CheckCorruption->Enable(false);
    CheckCorruption->SetValue(false);
  } else CheckCorruption->Enable(true);
  if (RebuildLowSpace->GetValue()) {
    cmd += wxT("l");
    KeepOriginalISO->Enable(false);
    KeepOriginalISO->SetValue(false);
  } else if (RebuildDefault->GetValue()) {
    KeepOriginalISO->Enable(true);
  } else if (NoRebuild->GetValue()) {
    cmd += wxT("b");
    KeepOriginalISO->Enable(false);
    KeepOriginalISO->SetValue(false);
  }
  if (TerminalFont->IsChecked()) cmd += wxT("t");
  if (StayOffline->IsChecked()) cmd += wxT("o");
  if (ProgramOutput->GetCurrentSelection() == 1) {
    // html output
    cmd += wxT("h");
    if (XexImages->GetCurrentSelection() != 2)
      cmd += wxT("i"); // selection 0 = extract images when doing html output, selection 1 = extract images always, selection 2 = extract images never
  } else if (XexImages->GetCurrentSelection() == 1) cmd += wxT("i");
  if (CheckDVDFile->IsChecked() == false) cmd += wxT("d");
  if (FixAngle359->IsChecked()) cmd += wxT("x");
  if (KeepOriginalISO->IsChecked()) cmd += wxT("k");
  if (WriteDisable->IsChecked() || InputChoice->GetCurrentSelection() == 2) cmd += wxT("w"); // InputChoice 2 = dvd input

  if (DisplayFilesystem->IsChecked()) cmd += wxT(" --showfiles");
  if (ShowAvatarAwards->IsChecked()) cmd += wxT(" --aa");
  if (ShowAchievements->IsChecked()) {
    if (HideSecretAchievements->IsChecked()) cmd += wxT(" --achs");
    else cmd += wxT(" --ach");
  }
  if (AutoFix->GetCurrentSelection() != 1) {  // level 2 (default)
    if (AutoFix->GetCurrentSelection() == 0) // level 3
      cmd += wxT(" --af3");
    else if (AutoFix->GetCurrentSelection() == 2) // level 1
      cmd += wxT(" --af1");
    else if (AutoFix->GetCurrentSelection() == 3) // level 0
      cmd += wxT(" --af0");
  }
  //if (LocalOnly->IsChecked()) cmd += wxT(" --localonly");
  if (!FixDRT->IsChecked()) cmd += wxT(" --nofixdrt");
  if (!FixBadAngles->IsChecked()) cmd += wxT(" --nofixdev");
  else if (FixBadAnglesValue->GetCurrentSelection() != 3) {
    cmd += wxT(" --dev ");
    wxString dev;
    dev.Printf(wxT("%d"), FixBadAnglesValue->GetCurrentSelection());
    cmd += dev;
  }
  if (!TrustSSv2->IsChecked()) cmd += wxT(" --notrust");

  if (!Verify->IsChecked()) cmd += wxT(" --noverify");
  if (!OnlineUpdate->IsChecked()) cmd += wxT(" --noupdate");
  if (CSVUpdate->IsChecked()) cmd += wxT(" --csv");

  if (MatchOnly->IsChecked()) {
    MatchOnlyEditBox->Enable(true);
    MatchOnlyButton->Enable(true);
    if (InputChoice->GetCurrentSelection() == 1) { // dir
      cmd += wxT(" --match \"");
      if (MatchOnlyEditBox->GetValue().Find(',') == wxNOT_FOUND)
        cmd += wxT(","); // helps to keep the shell from automatically expanding match args based on the cwd
      cmd += MatchOnlyEditBox->GetValue();
      cmd += wxT("\"");
    }
  } else {
    MatchOnlyEditBox->Enable(false);
    MatchOnlyButton->Enable(false);
  }

  if (!MyRegionEditBox->IsEmpty()) {
    cmd += wxT(" --rgn ");
    cmd += MyRegionEditBox->GetValue();
  }
  if (PreferredLangChoice->GetCurrentSelection() != 0) {
    cmd += wxT(" --lang ");
    wxString lang;
    lang.Printf(wxT("%d"), PreferredLangChoice->GetCurrentSelection() + 1);
    cmd += lang;
  }
  if (SplitVid->GetCurrentSelection() == 0) cmd += wxT(" --splitvid"); // want splitvid
  else if (SplitVid->GetCurrentSelection() == 1) cmd += wxT(" --removesplitvid"); // don't want splitvid
  if (EmbedImages->IsChecked()) cmd += wxT(" --embed");

  if (PatchIfStealthPasses->IsChecked()) cmd += wxT(" --patchitanyway");
  if (PatchIfInvalid->IsChecked()) cmd += wxT(" --patchgarbage");

  if (PatchVideo->IsChecked()) {
    PatchVideoEditBox->Enable(true);
    PatchVideoOpenButton->Enable(true);
    cmd += wxT(" --p-video \"");
    cmd += PatchVideoEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    PatchVideoEditBox->Enable(false);
    PatchVideoOpenButton->Enable(false);
  }

  if (PatchPFI->IsChecked()) {
    PatchPFIEditBox->Enable(true);
    PatchPFIOpenButton->Enable(true);
    cmd += wxT(" --p-pfi \"");
    cmd += PatchPFIEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    PatchPFIEditBox->Enable(false);
    PatchPFIOpenButton->Enable(false);
  }

  if (PatchDMI->IsChecked()) {
    PatchDMIEditBox->Enable(true);
    PatchDMIOpenButton->Enable(true);
    cmd += wxT(" --p-dmi \"");
    cmd += PatchDMIEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    PatchDMIEditBox->Enable(false);
    PatchDMIOpenButton->Enable(false);
  }

  if (PatchSS->IsChecked()) {
    PatchSSEditBox->Enable(true);
    PatchSSOpenButton->Enable(true);
    cmd += wxT(" --p-ss \"");
    cmd += PatchSSEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    PatchSSEditBox->Enable(false);
    PatchSSOpenButton->Enable(false);
  }

  if (Clobber->IsChecked()) cmd += wxT(" --clobber");

  if (ExtractVideo->IsChecked()) {
    ExtractVideoEditBox->Enable(true);
    ExtractVideoSaveButton->Enable(true);
    ExtractEntireVideoPartition->Enable(true);
    if (ExtractEntireVideoPartition->IsChecked()) cmd += wxT(" --e-videopartition \"");
    else cmd += wxT(" --e-video \"");
    cmd += ExtractVideoEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    ExtractVideoEditBox->Enable(false);
    ExtractVideoSaveButton->Enable(false);
    ExtractEntireVideoPartition->Enable(false);
    ExtractEntireVideoPartition->SetValue(false);
  }

  if (ExtractPFI->IsChecked()) {
    ExtractPFIEditBox->Enable(true);
    ExtractPFISaveButton->Enable(true);
    cmd += wxT(" --e-pfi \"");
    cmd += ExtractPFIEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    ExtractPFIEditBox->Enable(false);
    ExtractPFISaveButton->Enable(false);
  }

  if (ExtractDMI->IsChecked()) {
    ExtractDMIEditBox->Enable(true);
    ExtractDMISaveButton->Enable(true);
    cmd += wxT(" --e-dmi \"");
    cmd += ExtractDMIEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    ExtractDMIEditBox->Enable(false);
    ExtractDMISaveButton->Enable(false);
  }

  if (ExtractSS->IsChecked()) {
    ExtractSSEditBox->Enable(true);
    ExtractSSSaveButton->Enable(true);
    cmd += wxT(" --e-ss \"");
    cmd += ExtractSSEditBox->GetValue();
    cmd += wxT("\"");
  } else {
    ExtractSSEditBox->Enable(false);
    ExtractSSSaveButton->Enable(false);
  }

  if (!ExtraOptionsEditBox->IsEmpty()) {
    cmd += wxT(" ");
    cmd += ExtraOptionsEditBox->GetValue();
  }

#ifndef __APPLE__
#ifdef WIN32
  if (Maximize->IsChecked()) cmd += wxT(" --max");
#endif
  if (ProgramOutput->GetCurrentSelection() == 0) cmd += wxT(" --pause"); // pause shell atexit
#endif

  if (InputChoice->GetCurrentSelection() == 0) {  // file(s)
    cmd += wxT(" -- "); // -- is a special argument that will end option parsing and all following arguments will be treated as filenames in case a filename begins with a hyphen
    if (!InputFileEditBox->GetValue().StartsWith(wxT("\""), NULL)) cmd += wxT("\"");
    cmd += InputFileEditBox->GetValue();
    if (!InputFileEditBox->GetValue().EndsWith(wxT("\""), NULL)) cmd += wxT("\"");
  } else if (InputChoice->GetCurrentSelection() == 1) {  // dir
    if (RecurseSubdirs->IsChecked()) cmd += wxT(" --rec --dir ");
    else cmd += wxT(" --dir ");
    if (!InputFileEditBox->GetValue().StartsWith(wxT("\""), NULL)) cmd += wxT("\"");
    cmd += InputFileEditBox->GetValue();
    if (InputFileEditBox->GetValue().EndsWith(wxT("\\"), NULL))
      cmd += wxT("\\\""); // asdf\ will be changed to asdf\\ so the shell sees it as asdf\ instead of asdf" ("asdf\" vs "asdf\\" when quoted by app)
    else if (InputFileEditBox->GetValue().EndsWith(wxT("\\\""), NULL)) {
      cmd.RemoveLast();
      cmd += wxT("\\\""); // "asdf\" will be changed to "asdf\\" if already quoted by user
    } else if (!InputFileEditBox->GetValue().EndsWith(wxT("\""), NULL)) cmd += wxT("\"");
  }
#ifdef WIN32
  else if (InputChoice->GetCurrentSelection() == 2) {  // burned dvd
      if (arrayStringFor_DriveChoice.GetCount() > 0) {
          cmd += wxT(" --dvd ");
          cmd += arrayStringFor_DriveChoice.Item((size_t) DriveChoice->GetCurrentSelection()).Mid(arrayStringFor_DriveChoice.Item((size_t) DriveChoice->GetCurrentSelection()).Len() - 3, 1);
      }
  }
#endif
  /*
  else if (InputChoice->GetCurrentSelection() == 3) {  // original disc
      #ifdef WIN32
          if (arrayStringFor_DriveChoice.GetCount() > 0) {
              cmd += wxT(" --rip ");
              cmd += arrayStringFor_DriveChoice.Item((size_t) DriveChoice->GetCurrentSelection()).Mid(arrayStringFor_DriveChoice.Item((size_t) DriveChoice->GetCurrentSelection()).Len() - 3, 1);
              cmd += wxT(" --dest ");
              if (!RipDestEditBox->GetValue().StartsWith(wxT("\""), NULL)) cmd += wxT("\"");
              cmd += RipDestEditBox->GetValue();
              if (!RipDestEditBox->GetValue().EndsWith(wxT("\""), NULL)) cmd += wxT("\"");
          }
      #else
          cmd += wxT(" --rip ");
          if (!InputFileEditBox->GetValue().StartsWith(wxT("\""), NULL)) cmd += wxT("\"");
          cmd += InputFileEditBox->GetValue();
          if (!InputFileEditBox->GetValue().EndsWith(wxT("\""), NULL)) cmd += wxT("\"");
          cmd += wxT(" --dest ");
          if (!RipDestEditBox->GetValue().StartsWith(wxT("\""), NULL)) cmd += wxT("\"");
          cmd += RipDestEditBox->GetValue();
          if (!RipDestEditBox->GetValue().EndsWith(wxT("\""), NULL)) cmd += wxT("\"");
      #endif
  }
  */
  if (ProgramOutput->GetCurrentSelection() != 0) {
    cmd += wxT(" > \"");
    cmd += OutputFileEditBox->GetValue();
    cmd += wxT("\"");
  }
#if (!defined(WIN32) && !defined(__APPLE__))
  // change single quotes/apostrophes to \' but change the first one back for xterm -e ... 'abgx360 ...
  cmd.Replace(wxT("'"), wxT("\\'"), true);
  cmd.Replace(wxT("\\'"), wxT("'"), false);
  // ending single quote
  cmd += wxT("'");
#endif

  //wxLogStatus(wxT("%s"), cmd);
  if (cmd.Find(wxT(" --user ")) == wxNOT_FOUND && cmd.Find(wxT(" --pass ")) == wxNOT_FOUND) StatusBar->SetStatusText(cmd);
  else StatusBar->SetStatusText(wxEmptyString);
}

/*
 * RunButtonClick
 */
void abgx360gui::RunButtonClick(wxCommandEvent &WXUNUSED(event)) {
  long returnvalue;
  wxString returnstring;
#ifndef WIN32
  if (InputChoice->GetCurrentSelection() == 2) {
#ifdef __APPLE__
    wxMessageBox(wxT("Mac OS X Burned DVD Input is not currently supported."),
                     wxT("Mac OS X DVD Input"), wxOK);
#else
    wxMessageBox(wxT("For Unix based operating systems: Devices are files!\n"
                     "Select \"File(s)\" as Input and enter the device name.\n"
                     "Linux Example: /dev/cdrom (requires read permissions).\n"
                     "Writes are automatically disabled for block devices."), wxT("Unix DVD Input"), wxOK);
#endif
    return;
  }
#endif
  if (InputChoice->GetCurrentSelection() == 0 && InputFileEditBox->IsEmpty()) {
    wxMessageBox(wxT("You forgot to choose the file(s) for input!"), wxT("WTF"), wxICON_EXCLAMATION);
    return;
  }
  if (InputChoice->GetCurrentSelection() == 1 && InputFileEditBox->IsEmpty()) {
    wxMessageBox(wxT("You forgot to choose the directory for input!"), wxT("WTF"), wxICON_EXCLAMATION);
    return;
  }
  /*
  if (InputChoice->GetCurrentSelection() == 3) {  // original disc
      #ifndef WIN32
          if (InputFileEditBox->IsEmpty()) {
              wxMessageBox("You forgot to choose the ripper device that contains an original disc for input!", "WTF", wxICON_EXCLAMATION);
              return;
          }
      #endif
      if (RipDestEditBox->IsEmpty()) {
          wxMessageBox("You forgot to choose the destination ISO file for ripping an original disc! Select it under the \"Ripping\" tab.", "WTF", wxICON_EXCLAMATION);
          return;
      }
  }
  */
  if (ProgramOutput->GetCurrentSelection() != 0 && OutputFileEditBox->IsEmpty()) {
    wxMessageBox(wxT("You forgot to choose an output file!"), wxT("WTF"), wxICON_EXCLAMATION);
    return;
  }
  if (ProgramOutput->GetCurrentSelection() != 0 && (OutputFileEditBox->GetValue().Find('/') == wxNOT_FOUND && OutputFileEditBox->GetValue().Find('\\') == wxNOT_FOUND)) {
    wxMessageBox(wxT("You forgot to choose a directory for the output file!"), wxT("WTF"), wxICON_EXCLAMATION);
    return;
  }
  if (PatchVideo->IsChecked()) {
    if (PatchVideoEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch video from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (PatchVideoEditBox->GetValue().EndsWith(wxT("\\")) || PatchVideoEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch video from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (PatchPFI->IsChecked()) {
    if (PatchPFIEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch PFI from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (PatchPFIEditBox->GetValue().EndsWith(wxT("\\")) || PatchPFIEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch PFI from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (PatchDMI->IsChecked()) {
    if (PatchDMIEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch DMI from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (PatchDMIEditBox->GetValue().EndsWith(wxT("\\")) || PatchDMIEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch DMI from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (PatchSS->IsChecked()) {
    if (PatchSSEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch SS from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (PatchSSEditBox->GetValue().EndsWith(wxT("\\")) || PatchSSEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a file to manually patch SS from!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (ExtractVideo->IsChecked()) {
    if (ExtractVideoEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract video!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (ExtractVideoEditBox->GetValue().EndsWith(wxT("\\")) || ExtractVideoEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract video!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (ExtractPFI->IsChecked()) {
    if (ExtractPFIEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract PFI!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (ExtractPFIEditBox->GetValue().EndsWith(wxT("\\")) || ExtractPFIEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract PFI!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (ExtractDMI->IsChecked()) {
    if (ExtractDMIEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract DMI!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (ExtractDMIEditBox->GetValue().EndsWith(wxT("\\")) || ExtractDMIEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract DMI!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  if (ExtractSS->IsChecked()) {
    if (ExtractSSEditBox->IsEmpty()) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract SS!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    } else if (ExtractSSEditBox->GetValue().EndsWith(wxT("\\")) || ExtractSSEditBox->GetValue().EndsWith(wxT("/"))) {
      wxMessageBox(wxT("You forgot to choose a filename to manually extract SS!"), wxT("WTF"), wxICON_EXCLAMATION);
      return;
    }
  }
  size_t i, count;
  if (InputChoice->GetCurrentSelection() == 0) {
    m_fileHistory->AddFileToHistory(InputFileEditBox->GetValue());
    m_fileConfig->SetPath(wxT("/RecentFiles"));
    m_fileHistory->Save(*m_fileConfig);
    count = m_fileHistory->GetCount();
    for (i = 0; i < count; i++) {
      m_fileHistory->RemoveFileFromHistory(0);
    }
    m_fileHistory->Load(*m_fileConfig);
    m_fileConfig->SetPath(wxT(".."));
  } else if (InputChoice->GetCurrentSelection() == 1) {
    m_folderHistory->AddFileToHistory(InputFileEditBox->GetValue());
    m_fileConfig->SetPath(wxT("/RecentFolders"));
    m_folderHistory->Save(*m_fileConfig);
    count = m_folderHistory->GetCount();
    for (i = 0; i < count; i++) {
      m_folderHistory->RemoveFileFromHistory(0);
    }
    m_folderHistory->Load(*m_fileConfig);
    m_fileConfig->SetPath(wxT(".."));
  }

#ifdef __APPLE__
  /*
  // create command for OS X using osascript and replacing " quotes in cmd with \'
  cmd.Replace(wxT("\""), wxT("\\'"), true);  // 3 levels of quoting: '1"2\'3\'2"1'
  cmd.Prepend(wxT("osascript -e 'tell application \"Terminal\"' -e 'do script \""));
  cmd += wxT("\"' -e 'set background color of window 1 to \"black\"' -e 'set normal text color of window 1 to \"white\"' -e 'set bold text color of window 1 to \"white\"' -e 'set cursor color of window 1 to \"white\"' ");
  if (Maximize->IsChecked()) cmd += wxT("-e 'set the position of window 1 to {0, 20}' -e 'set the number of rows of window 1 to 300' -e 'set the number of columns of window 1 to 100' ");
  cmd += wxT("-e 'end tell'");
  //wxMessageBox(cmd, wxT("mac cmd"), wxOK);
  */

  // write script to a file to avoid inability to escape ' (single quote/apostrophe)
  // still need to replace " with \"
  //cmd.Replace(wxT("\\"), wxT("\\\\"), true);
  cmd.Replace(wxT("\""), wxT("\\\""), true);
  wxFile *abgx360script = new wxFile("/tmp/abgx360.scpt", wxFile::write);
  if (abgx360script->IsOpened()) {
      cmd.Prepend(wxT("tell application \"Terminal\"\n"
                      "do script \""));
      cmd +=  wxT("\"\nset background color of window 1 to \"black\"\n"
                      "set normal text color of window 1 to \"white\"\n"
                      "set bold text color of window 1 to \"white\"\n"
                      "set cursor color of window 1 to \"white\"\n");
      if (Maximize->IsChecked())
           cmd += wxT("set the position of window 1 to {0, 20}\n"
                      "set the number of rows of window 1 to 300\n"
                      "set the number of columns of window 1 to 100\n");
      cmd +=      wxT("end tell\n");
      if (abgx360script->Write(cmd)) {
          abgx360script->Flush();
          abgx360script->Close();
          cmd = wxT("osascript /tmp/abgx360.scpt");
      }
      else {
          wxMessageBox(wxT("ERROR: Failed to write to '/tmp/abgx360.scpt'! Unable to launch abgx360."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
        return;
      }
  }
  else {
      wxMessageBox(wxT("ERROR: Failed to open '/tmp/abgx360.scpt' for writing! Unable to launch abgx360."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
    return;
  }
#endif

  //wxLogStatus(wxT("'%s' launched..."), cmd);
  StatusBar->SetStatusText(wxT("and away we go..."));

  if (ProgramOutput->GetCurrentSelection() == 0) {  // CLI Window
    returnvalue = wxExecute(cmd, wxEXEC_ASYNC);
    if (returnvalue == 0) {  // couldn't start the process
#ifdef __APPLE__
      wxMessageBox(wxT("ERROR: The command could not be executed! You're probably missing osascript"), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
#else
#ifdef WIN32
      wxMessageBox(wxT("ERROR: The command could not be executed! Most likely the abgx360 command line app isn't in your PATH... reinstalling abgx360 will fix this."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
#else
      wxMessageBox(wxT("ERROR: The command could not be executed! You probably don't have xterm installed."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
#endif
#endif
      return;
    }
  } else {
#ifdef __APPLE__
    // wxMac does not support wxEXEC_SYNC
    returnvalue = wxExecute(cmd, wxEXEC_ASYNC);
    if (returnvalue == 0) {  // couldn't start the process
        wxMessageBox(wxT("ERROR: The command could not be executed! You're probably missing osascript"), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
      return;
    }
#else
#ifdef WIN32
    wxString shellcmd;
#ifdef __WXWINCE__
        shellcmd = cmd;
#else
        wxChar *shell = wxGetenv(wxT("COMSPEC"));
        if (!shell) shell = (wxChar*) wxT("\\COMMAND.COM");
        if (!cmd) shellcmd = shell;
        else shellcmd.Printf(wxT("%s /c %s"), shell, cmd.c_str());
#endif
    returnvalue = wxExecute(shellcmd, wxEXEC_SYNC);
    if (returnvalue == -1) {  // couldn't start the process
        wxMessageBox(wxT("ERROR: The command could not be executed! Most likely the abgx360 command line app isn't in your PATH... reinstalling abgx360 will fix this."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
      return;
    }
#else
    returnvalue = wxExecute(cmd, wxEXEC_SYNC);
    if (returnvalue == -1) {  // couldn't start the process
      wxMessageBox(wxT("ERROR: The command could not be executed! You probably don't have xterm installed."), wxT("abgx360 GUI ERROR"), wxICON_ERROR);
      return;
    }
#endif
    if (OpenFileWhenDone->IsChecked()) wxLaunchDefaultBrowser(OutputFileEditBox->GetValue());
#endif
  }
}

/*
 * OpenButtonClick
 */
void abgx360gui::OpenButtonClick(wxCommandEvent &WXUNUSED(event)) {
  size_t i, count;
  if (InputChoice->GetCurrentSelection() == 0) {
    if (OpenFileDialog->ShowModal() == wxID_OK) {
      wxArrayString arrayStringFor_OpenFileDialog;
      OpenFileDialog->GetPaths(arrayStringFor_OpenFileDialog);
      //openfilename = OpenFileDialog->GetPath();
      //InputFileEditBox->ChangeValue(openfilename);
      //m_fileHistory->AddFileToHistory(openfilename);
      count = arrayStringFor_OpenFileDialog.GetCount();
      if (count > 100) {
        wxMessageBox(wxT("You have selected over 100 files!" NEWLINE"This will probably exceed maximum command line length limits so try using directory input instead."),
                     wxT("Doh!"),
                     wxICON_EXCLAMATION);
        return;
      }
      if (count) {
        InputFileEditBox->Clear();
        for (i = 0; i < count; i++) {
          InputFileEditBox->AppendText(wxT("\""));
          InputFileEditBox->AppendText(arrayStringFor_OpenFileDialog.Item(i));
          InputFileEditBox->AppendText(wxT("\""));
          if (i + 1 < count) InputFileEditBox->AppendText(wxT(" "));
        }
        m_fileHistory->AddFileToHistory(InputFileEditBox->GetValue());
        m_fileConfig->SetPath(wxT("/RecentFiles"));
        m_fileHistory->Save(*m_fileConfig);
        count = m_fileHistory->GetCount();
        for (i = 0; i < count; i++) {
          m_fileHistory->RemoveFileFromHistory(0);
        }
        m_fileHistory->Load(*m_fileConfig);
        m_fileConfig->SetPath(wxT(".."));
      }
    }
  } else if (InputChoice->GetCurrentSelection() == 1) {
    if (InputDirDialog->ShowModal() == wxID_OK) {
      openfilename = InputDirDialog->GetPath();
      InputFileEditBox->ChangeValue(openfilename);
      m_folderHistory->AddFileToHistory(openfilename);
      m_fileConfig->SetPath(wxT("/RecentFolders"));
      m_folderHistory->Save(*m_fileConfig);
      count = m_folderHistory->GetCount();
      for (i = 0; i < count; i++) {
        m_folderHistory->RemoveFileFromHistory(0);
      }
      m_folderHistory->Load(*m_fileConfig);
      m_fileConfig->SetPath(wxT(".."));
    }
  }
}

/*
 * SaveButtonClick
 */
void abgx360gui::SaveButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (ProgramOutput->GetCurrentSelection() == 1) {
    if (SaveHTMLFileDialog->ShowModal() == wxID_OK) {
      savefilename = SaveHTMLFileDialog->GetPath();
      OutputFileEditBox->ChangeValue(savefilename);
    }
  } else if (ProgramOutput->GetCurrentSelection() == 2) {
    if (SaveTextFileDialog->ShowModal() == wxID_OK) {
      savefilename = SaveTextFileDialog->GetPath();
      OutputFileEditBox->ChangeValue(savefilename);
    }
  }
}

/*
 * ProgramOutputSelected
 */
void abgx360gui::ProgramOutputSelected(wxCommandEvent &WXUNUSED(event)) {
  if (ProgramOutput->GetCurrentSelection() == 0) {  // command prompt
    Maximize->Show();
    Maximize->SetValue(true);
    OpenFileWhenDone->Hide();
    OpenFileWhenDone->SetValue(false);
    OutputFileEditBox->Enable(false);
    SaveButton->Enable(false);
#ifdef WIN32
    TerminalFont->SetValue(true);
#else
    TerminalFont->SetValue(false);
#endif
  } else {
    Maximize->Hide();
    Maximize->SetValue(false);
#ifndef __APPLE__
    OpenFileWhenDone->Show();
    OpenFileWhenDone->SetValue(true);
#endif
    OutputFileEditBox->Enable(true);
    SaveButton->Enable(true);
    TerminalFont->SetValue(false);
  }
}

/*
 * PatchVideoOpenButtonClick
 */
void abgx360gui::PatchVideoOpenButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (OpenVideoFileDialog->ShowModal() == wxID_OK) {
    openfilename = OpenVideoFileDialog->GetPath();
    PatchVideoEditBox->ChangeValue(openfilename);
  }
}

/*
 * PatchPFIOpenButtonClick
 */
void abgx360gui::PatchPFIOpenButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (OpenPFIFileDialog->ShowModal() == wxID_OK) {
    openfilename = OpenPFIFileDialog->GetPath();
    PatchPFIEditBox->ChangeValue(openfilename);
  }
}

/*
 * PatchDMIOpenButtonClick
 */
void abgx360gui::PatchDMIOpenButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (OpenDMIFileDialog->ShowModal() == wxID_OK) {
    openfilename = OpenDMIFileDialog->GetPath();
    PatchDMIEditBox->ChangeValue(openfilename);
  }
}

/*
 * PatchSSOpenButtonClick
 */
void abgx360gui::PatchSSOpenButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (OpenSSFileDialog->ShowModal() == wxID_OK) {
    openfilename = OpenSSFileDialog->GetPath();
    PatchSSEditBox->ChangeValue(openfilename);
  }
}

/*
 * ExtractVideoSaveButtonClick
 */
void abgx360gui::ExtractVideoSaveButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (Clobber->IsChecked()) {
    if (SaveVideoFileDialogClobber->ShowModal() == wxID_OK) {
      savefilename = SaveVideoFileDialogClobber->GetPath();
      ExtractVideoEditBox->ChangeValue(savefilename);
    }
  } else {
    if (SaveVideoFileDialog->ShowModal() == wxID_OK) {
      savefilename = SaveVideoFileDialog->GetPath();
      ExtractVideoEditBox->ChangeValue(savefilename);
    }
  }
}

/*
 * ExtractPFISaveButtonClick
 */
void abgx360gui::ExtractPFISaveButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (Clobber->IsChecked()) {
    if (SavePFIFileDialogClobber->ShowModal() == wxID_OK) {
      savefilename = SavePFIFileDialogClobber->GetPath();
      ExtractPFIEditBox->ChangeValue(savefilename);
    }
  } else {
    if (SavePFIFileDialog->ShowModal() == wxID_OK) {
      savefilename = SavePFIFileDialog->GetPath();
      ExtractPFIEditBox->ChangeValue(savefilename);
    }
  }
}

/*
 * ExtractDMISaveButtonClick
 */
void abgx360gui::ExtractDMISaveButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (Clobber->IsChecked()) {
    if (SaveDMIFileDialogClobber->ShowModal() == wxID_OK) {
      savefilename = SaveDMIFileDialogClobber->GetPath();
      ExtractDMIEditBox->ChangeValue(savefilename);
    }
  } else {
    if (SaveDMIFileDialog->ShowModal() == wxID_OK) {
      savefilename = SaveDMIFileDialog->GetPath();
      ExtractDMIEditBox->ChangeValue(savefilename);
    }
  }
}

/*
 * ExtractSSSaveButtonClick
 */
void abgx360gui::ExtractSSSaveButtonClick(wxCommandEvent &WXUNUSED(event)) {
  if (Clobber->IsChecked()) {
    if (SaveSSFileDialogClobber->ShowModal() == wxID_OK) {
      savefilename = SaveSSFileDialogClobber->GetPath();
      ExtractSSEditBox->ChangeValue(savefilename);
    }
  } else {
    if (SaveSSFileDialog->ShowModal() == wxID_OK) {
      savefilename = SaveSSFileDialog->GetPath();
      ExtractSSEditBox->ChangeValue(savefilename);
    }
  }
}

// returns a random number between x and y
int randomnumber(int x, int y) {
  // initialize random number generator
  srand(time(NULL));
  return (rand() % (y - x + 1) + x);
}

/*
 * MnuAboutClick
 */
void abgx360gui::MnuAboutClick(wxCommandEvent &WXUNUSED(event)) {
  wxArrayString aDescriptions;
  aDescriptions.Add(wxT("Don't tase me bro!"));
  aDescriptions.Add(wxT("The cake is a lie!"));
  aDescriptions.Add(wxT("Tell your mom thanks!"));
  aDescriptions.Add(wxT("Keep fucking that chicken!"));

  int random = randomnumber(0, aDescriptions.GetCount() - 1);

  wxAboutDialogInfo info;
  info.SetName(wxT("abgx360 GUI"));
  info.SetVersion(wxT("1.0.2"));
  info.SetCopyright(wxT("\xA9 2008-2009 by Seacrest"));
  info.SetDescription(aDescriptions.Item(random));
  wxAboutBox(info);
}

/*
 * MnuExitClick
 */
void abgx360gui::MnuExitClick(wxCommandEvent &WXUNUSED(event)) {
  Destroy();
}

/*
 * MyRegionButtonClick
 */
void abgx360gui::MyRegionButtonClick(wxCommandEvent &WXUNUSED(event)) {
  wxArrayString aRegions;
  wxArrayInt aRegionSelections;
  wxString str, regioncode;
  unsigned long ntscu = 0, pal = 0, ntscj = 0;

  aRegions.Add(wxT("NTSC/U"));
  aRegions.Add(wxT("PAL (Australia or New Zealand)"));
  aRegions.Add(wxT("PAL (Europe)"));
  aRegions.Add(wxT("NTSC/J (Japan)"));
  aRegions.Add(wxT("NTSC/J (China)"));
  aRegions.Add(wxT("NTSC/J (Other)"));

  str = MyRegionEditBox->GetValue();
  if (str.Len() == 8) {
    str.Mid(2, 2).ToULong(&pal, 16);
    str.Mid(4, 2).ToULong(&ntscj, 16);
    str.Mid(6, 2).ToULong(&ntscu, 16);
    if (ntscu == 0xFF) aRegionSelections.Add(0);
    if (pal & 0x01) aRegionSelections.Add(1);
    if ((pal & 0xFE) == 0xFE) aRegionSelections.Add(2);
    if (ntscj & 0x01) aRegionSelections.Add(3);
    if (ntscj & 0x02) aRegionSelections.Add(4);
    if ((ntscj & 0xFC) == 0xFC) aRegionSelections.Add(5);
  }

  wxGetMultipleChoices(aRegionSelections, wxT("Check the box next to the region of any and all consoles you own"), wxT("Select regions"), aRegions);

  if (aRegionSelections.GetCount()) {
    ntscu = 0;
    pal = 0;
    ntscj = 0;
    if (aRegionSelections.Index(0) != wxNOT_FOUND) ntscu = 0xFF;
    if (aRegionSelections.Index(1) != wxNOT_FOUND) pal |= 0x01;
    if (aRegionSelections.Index(2) != wxNOT_FOUND) pal |= 0xFE;
    if (aRegionSelections.Index(3) != wxNOT_FOUND) ntscj |= 0x01;
    if (aRegionSelections.Index(4) != wxNOT_FOUND) ntscj |= 0x02;
    if (aRegionSelections.Index(5) != wxNOT_FOUND) ntscj |= 0xFC;
    regioncode.Printf(wxT("00%02lX%02lX%02lX"), pal, ntscj, ntscu);
    MyRegionEditBox->SetValue(regioncode);
  }
}

/*
 * MatchOnlyButtonClick
 */
void abgx360gui::MatchOnlyButtonClick(wxCommandEvent &WXUNUSED(event)) {
  wxArrayString aMatches;
  wxArrayInt aMatchSelections;
  wxString str;
  wxString Matches;
  Matches.Empty();
  size_t i;

  aMatches.Add(wxT("*.iso"));
  aMatches.Add(wxT("*.000"));
  aMatches.Add(wxT("*.360"));
  aMatches.Add(wxT("*.img"));
  aMatches.Add(wxT("*.bin"));
  aMatches.Add(wxT("*.xex"));
  aMatches.Add(wxT("*pfi*.bin"));
  aMatches.Add(wxT("*dmi*.bin"));
  aMatches.Add(wxT("*ss*.bin"));
  aMatches.Add(wxT("*video*.iso"));

  str = MatchOnlyEditBox->GetValue();
  if (str.Len()) {
    for (i = 0; i < aMatches.GetCount(); i++) {
      if (str.Find(aMatches.Item(i)) == 0 || str.Find(wxT(",") + aMatches.Item(i)) != wxNOT_FOUND || str.Find(wxT(", ") + aMatches.Item(i)) != wxNOT_FOUND)
        aMatchSelections.Add(i);
    }
  }

  wxGetMultipleChoices(aMatchSelections, wxT("Check the box next to the type of files you want to match"), wxT("Select matches"), aMatches);

  if (aMatchSelections.GetCount()) {
    for (i = 0; i < aMatches.GetCount(); i++) {
      if (aMatchSelections.Index(i) != wxNOT_FOUND) {
        if (Matches.Len()) Matches += wxT(", ");
        Matches += aMatches.Item(i);
      }
    }
    MatchOnlyEditBox->SetValue(Matches);
    MatchOnly->SetValue(true);
  }
}

/*
 * InputChoiceSelected
 */
void abgx360gui::InputChoiceSelected(wxCommandEvent &WXUNUSED(event)) {
  if (InputChoice->GetCurrentSelection() == 1) {  // directory
    OpenButton->SetToolTip(wxT("Open Directory"));
    WriteDisable->Show();
    MatchOnly->Show();
    MatchOnlyEditBox->Show();
    MatchOnlyButton->Show();
#ifdef WIN32
    DriveChoice->Hide();
    InputFileEditBox->Show();
    OpenButton->Show();
#endif
  } else if (InputChoice->GetCurrentSelection() == 2) {  // burned dvd
#ifdef WIN32
    MatchOnly->Hide();
    MatchOnlyEditBox->Hide();
    MatchOnlyButton->Hide();
    DriveChoice->Show();
    InputFileEditBox->Hide();
    OpenButton->Hide();
    WriteDisable->Hide();
#else
    wxMessageBox(wxT("For Unix based operating systems: Devices are files!\n"
                     "Select \"File(s)\" as Input and enter the device name.\n"
                     "Linux Example: /dev/cdrom (requires read permissions).\n"
                     "Writes are automatically disabled for block devices."), wxT("Unix Burned DVD Input"), wxOK);
    return;
#endif
  }
    /*
    else if (InputChoice->GetCurrentSelection() == 3) {  // original disc
        WriteDisable->Hide();
        MatchOnly->Hide();
        MatchOnlyEditBox->Hide();
        MatchOnlyButton->Hide();
        #ifdef WIN32
            DriveChoice->Show();
            InputFileEditBox->Hide();
            OpenButton->Hide();
        #else
            OpenButton->SetToolTip(wxT("Open Ripper Device"));
        #endif
    }
    */
  else {  // file(s)
    OpenButton->SetToolTip(wxT("Open File(s)"));
    WriteDisable->Show();
    MatchOnly->Hide();
    MatchOnlyEditBox->Hide();
    MatchOnlyButton->Hide();
#ifdef WIN32
    DriveChoice->Hide();
    InputFileEditBox->Show();
    OpenButton->Show();
#endif
  }
}

/*
 * MnuClearHistoryClick
 */
void abgx360gui::MnuClearHistoryClick(wxCommandEvent &WXUNUSED(event)) {
  size_t i;
  size_t count = m_fileHistory->GetCount();
  for (i = 0; i < count; i++) {
    m_fileHistory->RemoveFileFromHistory(0);
  }
  m_fileConfig->SetPath(wxT("/RecentFiles"));
  m_fileHistory->Save(*m_fileConfig);
  m_fileConfig->SetPath(wxT(".."));
}

/*
 * MnuFileHistoryClick
 */
void abgx360gui::MnuFileHistoryClick(wxCommandEvent &event) {
  size_t i = 0;
  if (event.GetId() == wxID_FILE2) i = 1;
  else if (event.GetId() == wxID_FILE3) i = 2;
  else if (event.GetId() == wxID_FILE4) i = 3;
  else if (event.GetId() == wxID_FILE5) i = 4;
  else if (event.GetId() == wxID_FILE6) i = 5;
  else if (event.GetId() == wxID_FILE7) i = 6;
  else if (event.GetId() == wxID_FILE8) i = 7;
  else if (event.GetId() == wxID_FILE9) i = 8;
  InputChoice->SetSelection(0);
  InputChoiceSelected(event);
  InputFileEditBox->ChangeValue(m_fileHistory->GetHistoryFile(i));
}

/*
 * MnuClearDirHistoryClick
 */
void abgx360gui::MnuClearDirHistoryClick(wxCommandEvent &WXUNUSED(event)) {
  size_t i;
  size_t count = m_folderHistory->GetCount();
  for (i = 0; i < count; i++) {
    m_folderHistory->RemoveFileFromHistory(0);
  }
  m_fileConfig->SetPath(wxT("/RecentFolders"));
  m_folderHistory->Save(*m_fileConfig);
  m_fileConfig->SetPath(wxT(".."));
}

/*
 * MnuDirHistoryClick
 */
void abgx360gui::MnuDirHistoryClick(wxCommandEvent &event) {
  size_t i = 0;
  if (event.GetId() == ID_DIR2) i = 1;
  else if (event.GetId() == ID_DIR3) i = 2;
  else if (event.GetId() == ID_DIR4) i = 3;
  else if (event.GetId() == ID_DIR5) i = 4;
  else if (event.GetId() == ID_DIR6) i = 5;
  else if (event.GetId() == ID_DIR7) i = 6;
  else if (event.GetId() == ID_DIR8) i = 7;
  else if (event.GetId() == ID_DIR9) i = 8;
  InputChoice->SetSelection(1);
  InputChoiceSelected(event);
  InputFileEditBox->ChangeValue(m_folderHistory->GetHistoryFile(i));
}

/*
 * QuickstartChoiceSelected
 */
void abgx360gui::QuickstartChoiceSelected(wxCommandEvent &WXUNUSED(event)) {
  QuickstartMemo->Clear();

  if (QuickstartChoice->GetCurrentSelection() == 0) {
    QuickstartMemo->AppendText(wxT("Welcome to abgx360, the ultimate tool for Xbox 360 ISOs and Stealth files!" NEWLINE NEWLINE));
    QuickstartMemo->AppendText(wxT("Pick an item from the dropdown list above to get detailed information or instructions displayed here."));
  } else if (QuickstartChoice->GetCurrentSelection() == 1) {
    QuickstartMemo->AppendText(wxT("The default settings are recommended, although you may change any settings and press Ctrl+S to save them as the new defaults or save them with a different name so that they can be loaded at any time by pressing Ctrl+L and selecting them. To delete any of your saved settings press Ctrl+D and select the ones you want to delete. These commands are also available on the menu bar under File."));
  } else if (QuickstartChoice->GetCurrentSelection() == 2) {
    QuickstartMemo->AppendText(wxT("Step 1: Press Ctrl+O to browse for the file you want to check/fix." NEWLINE));
    QuickstartMemo->AppendText(wxT("Step 2: Select \"Disable Writes\" if you just want to check the ISO without automatically fixing it, or go to the AutoFix tab and set the threshold to Level 3 if you want your ISO to be patched with verified stealth files even if the current files look valid." NEWLINE));
    QuickstartMemo->AppendText(wxT("Step 3: Click the Launch button and read the output. Red or yellow text is important!" NEWLINE));
    QuickstartMemo->AppendText(wxT("Note: The .dvd file is automatically created if it doesn't exist or fixed if it's invalid unless you disabled that setting under the options tab or selected \"Disable Writes\""));
  } else if (QuickstartChoice->GetCurrentSelection() == 3) {
    QuickstartMemo->AppendText(wxT("Part of the response to Challenge Types 24 and 25 (Response Types 7 and 5) is a rotational angle between 2 sectors on an original disc. The host's CCRT (which abgx360 decrypts) contains the target angles that would occur on a perfectly mastered disc when read by a perfect drive. There is also a value in the CCRT which only occurs for Challenge Types 24 and 25 that appears to specify the maximum deviation for a given angle. Currently all Xbox 360 games have the same angles (1, 91, 181, 271) and maximum deviation (+/-15). This is a copy protection system based on the fact that we cannot control the location of physical sectors on our backups, and our angles would be hugely different compared to an original. This is a fine protection as long as you can trust the drive firmware to make the actual measurement instead of simply replaying a previous response, which is what hacked firmware does. The default setting in abgx360 (for SS v1) is to adjust the replay angles (which only hacked firmwares can see and use) that deviate more than 3 degrees back to their CCRT target values because in real world testing on a normal drive, up to 2 degrees of deviation is quite normal and deviation greater than 3 on any game is rare and could be suspicous (although it's very common to have high deviation on a dying or worn out drive). Alternatively, (paranoid scenario) a game could be intentionally mastered with a deviation far from its CCRT target and adjusting it to the target value would also be suspicious... although this would potentially cause problems for disc authentication on legitimate unmodified drives. See \"What is SS v2?\" for the solution to this scenario."));
  } else if (QuickstartChoice->GetCurrentSelection() == 4) {
    QuickstartMemo->AppendText(wxT("If you put an Xbox 360 disc in a normal DVD video player you will see a screen that says, \"To play this disc, put it in an Xbox 360 console.\" This happens because there's a DVD video partition on the disc which is split over both layers on original discs. On normal backups the video partition is all on layer 0, and hacked firmwares are supposed to redirect reads to the layer 1 video back to layer 0. However, it was discovered that some hacked firmware versions prior to the release of iXtreme were not doing this properly, and if the Xbox 360 host tried to read the entire video partition, the second part of it would be blank because the firmware started reading from layer 1 instead of continuing to read from layer 0 like it was supposed to. One solution to this problem was called SplitVid, and it works by appending the layer 1 video to the end of an iso (padded up to the proper position based on PFI) so that when these buggy firmwares start reading from layer 1 they will actually read the layer 1 video instead of blank data. The better solution is to simply upgrade your firmware to the latest iXtreme version because these old firmwares have even more security holes, even if you have a perfectly stealthed disc. iXtreme versions prior to v1.4 also have a bug in disc jitter (see 'Is there such a thing as a \"bad kreon rip\"?' for more on this). If you still want to add SplitVid (or even if you want to remove it) you can select your preference in the Misc tab.  The default behavior is to not check for it and just leave it the way it is, because if you're using iXtreme firmware it doesn't matter if a backup has SplitVid or not; data past the game partition will not be readable by the Xbox 360 host."));
  } else if (QuickstartChoice->GetCurrentSelection() == 5) {
    QuickstartMemo->AppendText(wxT("AnyDVD is an app that runs in the background to remove DVD video protections on the fly. This is a nice feature when playing or ripping protected DVD movies but it causes problems when ripping Xbox 360 games because the video data (and possibly game data) will be altered in a way that is detectable on Xbox Live. Altered video will be easily detected and fixed because abgx360 knows which video partition (and matching PFI) your game should have based on the timestamp of authoring in the SS, and it knows what the CRC of that data should be. However, AnyDVD has also been observed to cause corruption in the game partition. If corruption occurs in an actual file in the game data, it could result in the game crashing or reporting an error when playing that backup. If it occurs in the random padding in between files, the corruption will not affect gameplay but could be detectable on Xbox Live. abgx360 looks for AnyDVD style corruption in the game data whenever the game CRC is checked by simply searching for \"DVDVIDEO-\" at the start of every sector (it was observed that video files inserted by AnyDVD always begin with either \"DVDVIDEO-VTS\" or \"DVDVIDEO-VMG\"). abgx360 is unable to fix game data corruption because it has no way to know what the data should be; it only knows what the resulting CRC should be (assuming a verified ini was found for that game) and the only way to fix bad game data is with a PPF. Other forms of game data corruption unrelated to AnyDVD (for example: single bit errors commonly caused by unstable CPU/RAM) will only be detected if abgx360 can find a verified ini and your game CRC doesn't match."));
  } else if (QuickstartChoice->GetCurrentSelection() == 6) {
    QuickstartMemo->AppendText(wxT("Short Answer: No, at least not yet, and if MS changes the angles in the future, abgx360 will know (by decrypting the CCRT) and will be able to adjust bad angles." NEWLINE NEWLINE));
    QuickstartMemo->AppendText(wxT("Long Answer: Current versions of Kreon's ripping firmware use preset values for the angle responses to Challenge Types 24 and 25 (Response Types 7 and 5) which isn't a problem because currently all Xbox 360 games use the same target angles. Kreon applies a small jitter to the preset angles, and sometimes it will give an angle of 359 degrees for CCRT target angle 1 (deviation of -2). When stored in the SS in LSB format this angle is also known as 6701, and back when the now useless KreonChecker was introduced these angles were thought to be timing values, and a value of 6701 was thought to be radically different from values like 0000 and 0100 observed when ripping the same disc with a Samsung drive. Although original firmwares may use timing to calculate the angle measurement, the difference between 6701 and 0000 is actually only 1 degree and definitely not a huge difference as originally thought. All was not in vain however, since this discussion brought about the realization that iXtreme would improperly jitter 6701 to 6801 (359 to 360), which is completely invalid and not an angle that original firmware will ever return. The proper behavior is to rollover from 359 to 0, which was implemented in iXtreme v1.4 and is a very important security update if you use or plan to use Xbox Live, even if you only play backups ripped with a Samsung drive because it can also return an angle of 359, and for some discs it's actually the most likely measurement. And yes, this means that you can stop using KreonChecker! abgx360 has an option (not enabled by default) under the AutoFix tab that will adjust 359 degree angles to 0 so that older versions of iXtreme will not exhibit this bug, but the best solution is to simply update your firmware to the newest iXtreme release for your drive, and there will be no need to reburn any of your backups."));
  } else if (QuickstartChoice->GetCurrentSelection() == 7) {
    QuickstartMemo->AppendText(wxT("SS v2 very simply consists of a duplicate angle value for response types 7 and 5 stored in c4eva's C/R replay table and was added in iXtreme v1.6. Older 0800 ripping firmwares and Kreon's firmware will record the angle just once, which results in SS v1. The problem is that Kreon's firmware does not do an actual measurement to get the angle, but simply uses preset angles that all games currently use and applies a small jitter to them. Storing the angle twice allows us to be fairly certain that this is a real angle measurement made by an actual Xbox 360 drive. If a game was intentionally or accidentally mastered with one or more angles that deviated far from their CCRT targets, it would need to be ripped with a drive that does actual angle measurements in order to mimic the original disc's responses. If that drive also supports SS v2 and you have the default option enabled to \"Trust SS v2 angles\", abgx360 will accept them instead of complaining about deviation from the (obviously incorrect) CCRT target. It will still check and alert you when appropriate about the v1 angle deviation from the v2 angle, which should be zero unless the v1 angle has been adjusted to its CCRT target by an older version of abgx360, or if a 359 degree v1 angle was adjusted to 0 for compatibility with iXtreme < v1.4. If you have decided not to trust SS v2 angles by unchecking that option, abgx360 will compare both the v1 and v2 angle to the CCRT target as if each was a v1 angle and alert you when appropriate; this is useful if you're paranoid and think someone has maliciously tampered with the angles. Note, however, that abgx360 v1.0.2 or later will never fix angle deviation or C/R data for an SS v2 without a verified ini and SS file (although it will still adjust any 359 degree v1 angles if you have that option enabled). If your SS v2 is fucked up, something is seriously wrong and you need to rip again or use the SS provided by AutoFix, as there is no reliable way to fix an SS v2 using decrypted CCRT data."));
  } else if (QuickstartChoice->GetCurrentSelection() == 8) {
    QuickstartMemo->AppendText(wxT("You can find out where your StealthFiles are stored by pressing Ctrl+F" NEWLINE));
    QuickstartMemo->AppendText(wxT("To find out where your Images are stored, press Ctrl+I" NEWLINE));
    QuickstartMemo->AppendText(wxT("These commands are also available on the menu bar under Help."));
  }
  QuickstartMemo->ShowPosition(0);
}

/*
 * abgx360_netClick
 */
void abgx360gui::abgx360_netClick(wxCommandEvent &event) {
  wxLaunchDefaultBrowser(wxT("http://abgx360.net"));
}

/*
 * WhereStealthFilesClick
 */
void abgx360gui::WhereStealthFilesClick(wxCommandEvent &event) {
  wxString homedir, stealthdir, str;

#ifdef WIN32
  if (wxGetEnv(wxT("APPDATA"), &homedir) || wxGetEnv(wxT("ProgramData"), &homedir) || wxGetEnv(wxT("ALLUSERSPROFILE"), &homedir)) {
      str = wxT("Your StealthFiles folder is located here:"NEWLINE);
  }
  else {
      wxMessageBox(wxT("ERROR: Unable to find your StealthFiles folder!"), wxT("Where is my StealthFiles folder?"), wxOK);
    return;
  }
  homedir += wxT("\\abgx360\\");
#else
  if (wxGetEnv(wxT("ABGX360_DIR"), &homedir) || wxGetEnv(wxT("HOME"), &homedir)) {
    str = wxT("Your StealthFiles folder is located here:" NEWLINE);
  } else {
    wxMessageBox(wxT("ERROR: Unable to find your StealthFiles folder!"), wxT("Where is my StealthFiles folder?"), wxOK);
    return;
  }
  homedir += wxT("/.abgx360/");
#endif
  // create it if it doesn't exist (user probably hasn't run the cli app yet)
  if (!wxDirExists(homedir)) wxMkdir(homedir, 0755);
  stealthdir = homedir + wxT("StealthFiles");
  if (!wxDirExists(stealthdir)) wxMkdir(stealthdir, 0755);

  str += stealthdir + wxT(NEWLINE NEWLINE"Would you like me to open it for you?");

  if (wxMessageBox(str, wxT("Where is my StealthFiles folder?"), wxYES_NO) == wxYES) {
#ifdef WIN32
    wxLaunchDefaultBrowser(stealthdir);
#else
#ifdef __APPLE__
    wxExecute(wxT("open ") + stealthdir, wxEXEC_ASYNC, NULL);
#else
    wxExecute(wxT("xdg-open ") + stealthdir, wxEXEC_ASYNC, NULL);
#endif
#endif
  }
  return;
}

/*
 * WhereImagesClick
 */
void abgx360gui::WhereImagesClick(wxCommandEvent &event) {
  wxString homedir, imagesdir, str;

#ifdef WIN32
  if (wxGetEnv(wxT("APPDATA"), &homedir) || wxGetEnv(wxT("ProgramData"), &homedir) || wxGetEnv(wxT("ALLUSERSPROFILE"), &homedir)) {
      str = wxT("Your Images folder is located here:"NEWLINE);
  }
  else {
      wxMessageBox(wxT("ERROR: Unable to find your Images folder!"), wxT("Where is my Images folder?"), wxOK);
    return;
  }
  homedir += wxT("\\abgx360\\");
#else
  if (wxGetEnv(wxT("ABGX360_DIR"), &homedir) || wxGetEnv(wxT("HOME"), &homedir)) {
    str = wxT("Your Images folder is located here:" NEWLINE);
  } else {
    wxMessageBox(wxT("ERROR: Unable to find your Images folder!"), wxT("Where is my Images folder?"), wxOK);
    return;
  }
  homedir += wxT("/.abgx360/");
#endif
  // create it if it doesn't exist (user probably hasn't run the cli app yet)
  if (!wxDirExists(homedir)) wxMkdir(homedir, 0755);
  imagesdir = homedir + wxT("Images");
  if (!wxDirExists(imagesdir)) wxMkdir(imagesdir, 0755);

  str += imagesdir + wxT(NEWLINE NEWLINE"Would you like me to open it for you?");

  if (wxMessageBox(str, wxT("Where is my Images folder?"), wxYES_NO) == wxYES) {
#ifdef WIN32
    wxLaunchDefaultBrowser(imagesdir);
#else
#ifdef __APPLE__
    wxExecute(wxT("open ") + imagesdir, wxEXEC_ASYNC, NULL);
#else
    wxExecute(wxT("xdg-open ") + imagesdir, wxEXEC_ASYNC, NULL);
#endif
#endif
  }
  return;
}

#if wxUSE_DRAG_AND_DROP

/*
 * input box drag n drop
 */
bool DnDInput::OnDropFiles(wxCoord, wxCoord, const wxArrayString &filenames) {
  size_t i, count = filenames.GetCount(), realcount = 0;
  wxCommandEvent event(wxEVT_COMMAND_CHOICE_SELECTED, dndselectid);
  event.SetEventObject(dndselect);

  /*
  wxString str;
  str.Printf(wxT("%d files/folders dropped:"), (int) count);
  for (i=0;i<count;i++) {
      str += wxT(NEWLINE);
      str += filenames[i];
  }
  wxMessageBox(str, wxT("drag n drop"), wxICON_EXCLAMATION);
  */

  if (count) {
    for (i = 0; i < count; i++) {
      if (wxDirExists(filenames[i])) {
        dndeditbox->Clear();
        dndeditbox->AppendText(filenames[i]);
        dndselect->SetSelection(1);  // Directory
        event.SetInt(1);
        dndframe->GetEventHandler()->AddPendingEvent(event);
        return true;
      } else if (wxFileExists(filenames[i])) {
        if (realcount) {
          dndeditbox->AppendText(wxT(" "));
          if (realcount >= 100) {
            wxMessageBox(wxT("You have selected over 100 files!" NEWLINE"This will probably exceed maximum command line length limits so try using directory input instead."),
                         wxT("Doh!"),
                         wxICON_EXCLAMATION);
            dndeditbox->Clear();
            return false;
          }
        } else dndeditbox->Clear();
        dndeditbox->AppendText(wxT("\""));
        dndeditbox->AppendText(filenames[i]);
        dndeditbox->AppendText(wxT("\""));
        realcount++;
      }
    }
    if (realcount) {
      dndselect->SetSelection(0);  // File(s)
      event.SetInt(0);
      dndframe->GetEventHandler()->AddPendingEvent(event);
      return true;
    }
  }
  return false;
}

#endif
