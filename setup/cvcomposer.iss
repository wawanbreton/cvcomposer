
[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{051E987E-CDAA-49CB-A5EF-014FDEBAF6FE}}
AppName=CvComposer
AppVersion=0.1
DefaultDirName={pf}\CvComposer
DisableProgramGroupPage=yes
OutputBaseFilename=CvComposer-setup
Compression=lzma

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "..\release\cvcomposer.exe"; DestDir: "{app}"; Flags: ignoreversion

; Qt and its dependancies
Source: "C:\Qt\5.7\mingw53_32\bin\Qt5Core.dll";                DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.7\mingw53_32\bin\Qt5Gui.dll";                 DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.7\mingw53_32\bin\Qt5Widgets.dll";             DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.7\mingw53_32\bin\Qt5Xml.dll";                 DestDir: "{app}"; Flags: ignoreversion
source: "C:\Qt\5.7\mingw53_32\plugins\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
source: "C:\Qt\5.7\mingw53_32\plugins\imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "c:\qt\5.7\5.7\mingw53_32\bin\LIBWINPTHREAD-1.DLL";    DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\5.7\5.7\mingw53_32\bin\LIBSTDC++-6.DLL";        DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\5.7\5.7\mingw53_32\bin\LIBGCC_S_DW2-1.DLL";     DestDir: "{app}"; Flags: ignoreversion

; OpenCV
Source: "C:\Program Files\opencv2.4.6\build-mingw\bin\libopencv_core249.dll";    DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv2.4.6\build-mingw\bin\libopencv_highgui249.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv2.4.6\build-mingw\bin\libopencv_imgproc249.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commonprograms}\CvComposer"; Filename: "{app}\cvcomposer.exe"
Name: "{commondesktop}\CvComposer";  Filename: "{app}\cvcomposer.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\cvcomposer.exe"; Description: "{cm:LaunchProgram,CvComposer}"; Flags: nowait postinstall skipifsilent
