; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServiceControlWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ServiceControl.h"

ClassCount=5
Class1=CServiceControlApp

ResourceCount=8
Resource2=IDD_COMPUTER_NAME_DLG (English (U.S.))
Resource3=IDD_SERVICECONTROL_DIALOG (English (U.S.))
Class2=CServiceControlWnd
Resource1=IDR_MAINFRAME
Class3=CColumnsDlg
Resource4=IDR_MAINFRAME (English (U.S.))
Class4=CComputerNameDlg
Resource5=IDD_SERVICE_CONTROL (English (U.S.))
Resource6=IDD_COLUMNS_DLG (English (U.S.))
Class5=CConfigServiceDlg
Resource7=IDD_ABOUT_BOX (English (U.S.))
Resource8=IDD_CONFIG_SERVICE (English (U.S.))

[CLS:CServiceControlApp]
Type=0
HeaderFile=ServiceControl.h
ImplementationFile=ServiceControl.cpp
Filter=N

[DLG:IDD_SERVICECONTROL_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=14
Control1=IDC_COMPUTER,edit,1350631552
Control2=IDC_CONNECT,button,1342242816
Control3=IDC_SERVICES,SysListView32,1350631445
Control4=IDC_START,button,1342242816
Control5=IDC_STOP,button,1342242816
Control6=IDC_PAUSE,button,1342242816
Control7=IDC_CONTINUE,button,1342242816
Control8=IDC_ADD,button,1342242816
Control9=IDC_REMOVE,button,1342242816
Control10=IDC_CONFIGURE,button,1342242817
Control11=IDC_REFRESH,button,1342242816
Control12=IDOK,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=IDC_SELECT_COMPUTER
Command2=IDC_SERVICE_START
Command3=IDC_SERVICE_STOP
Command4=IDC_SERVICE_PAUSE
Command5=IDC_SERVICE_CONTINUE
Command6=IDC_SERVICE_CONFIGURE
Command7=IDC_SERVICE_ADD
Command8=IDC_SERVICE_REMOVE
Command9=IDC_OPTIONS_FONT
Command10=IDC_OPTIONS_COLUMNS
Command11=IDC_REFRESH
Command12=IDC_HELP_ABOUT
CommandCount=12

[CLS:CServiceControlWnd]
Type=0
HeaderFile=servicecontrolwnd.h
ImplementationFile=servicecontrolwnd.cpp
BaseClass=CFrameWnd
LastObject=IDC_HELP_ABOUT
Filter=T
VirtualFilter=fWC

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=IDC_SERVICE_REMOVE
Command2=IDC_REFRESH
Command3=IDC_SERVICE_ADD
Command4=IDC_SERVICE_CONFIGURE
CommandCount=4

[CLS:CColumnsDlg]
Type=0
HeaderFile=ColumnsDlg.h
ImplementationFile=ColumnsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CColumnsDlg
VirtualFilter=dWC

[DLG:IDD_COMPUTER_NAME_DLG (English (U.S.))]
Type=1
Class=CComputerNameDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMPUTER_NAME,edit,1350631552

[DLG:IDD_COLUMNS_DLG (English (U.S.))]
Type=1
Class=CColumnsDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DISPLAYED_LIST,listbox,1352728833
Control4=IDC_STATIC,static,1342308352
Control5=IDC_HIDDEN_LIST,listbox,1352728835
Control6=IDC_STATIC,static,1342308352
Control7=IDC_HIDE,button,1342242816
Control8=IDC_SHOW,button,1342242816
Control9=IDC_UP,button,1342242816
Control10=IDC_DOWN,button,1342242816

[CLS:CComputerNameDlg]
Type=0
HeaderFile=ComputerNameDlg.h
ImplementationFile=ComputerNameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CComputerNameDlg
VirtualFilter=dWC

[DLG:IDD_SERVICE_CONTROL (English (U.S.))]
Type=1
ControlCount=2
Control1=IDC_ACTION_TEXT,static,1342308353
Control2=IDC_ACTION_PROGRESS,msctls_progress32,1350565888

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=IDC_REFRESH
Command2=IDC_SERVICE_START
Command3=IDC_SERVICE_STOP
Command4=IDC_SERVICE_PAUSE
Command5=IDC_SERVICE_CONTINUE
Command6=IDC_SELECT_COMPUTER
CommandCount=6

[DLG:IDD_CONFIG_SERVICE (English (U.S.))]
Type=1
Class=CConfigServiceDlg
ControlCount=36
Control1=IDC_SERVICE_NAME,edit,1350631552
Control2=IDC_DISPLAY_NAME,edit,1350631552
Control3=IDC_GROUP,edit,1350631552
Control4=IDC_DEPENDENCIES,edit,1350631552
Control5=IDC_EXE_PATH,edit,1350631552
Control6=IDC_TYPE_OWN,button,1342308361
Control7=IDC_TYPE_SHARED,button,1342177289
Control8=IDC_TYPE_DRIVER,button,1342177289
Control9=IDC_TYPE_FILE_SYSTEM,button,1342177289
Control10=IDC_TYPE_INTERACTIVE,button,1342242819
Control11=IDC_START_BOOT,button,1342308361
Control12=IDC_START_SYSTEM,button,1342177289
Control13=IDC_START_AUTO,button,1342177289
Control14=IDC_START_MANUAL,button,1342177289
Control15=IDC_START_DISABLED,button,1342177289
Control16=IDC_ERRORS_IGNORE,button,1342308361
Control17=IDC_ERRORS_NORMAL,button,1342177289
Control18=IDC_ERRORS_SEVERE,button,1342177289
Control19=IDC_ERRORS_CRITICAL,button,1342177289
Control20=IDC_ACCOUNT,edit,1350631552
Control21=IDC_PASSWORD,edit,1350631584
Control22=IDC_CONFIRM_PASSWORD,edit,1350631584
Control23=IDOK,button,1342242817
Control24=IDCANCEL,button,1342242816
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATIC,button,1342177287
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,button,1342177287
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,static,1342308352

[CLS:CConfigServiceDlg]
Type=0
HeaderFile=ConfigServiceDlg.h
ImplementationFile=ConfigServiceDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CConfigServiceDlg
VirtualFilter=dWC

[DLG:IDD_ABOUT_BOX (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342242817

