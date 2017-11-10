
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

#define HomePath GetEnv('HOMEPATH')

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "..\release\cvcomposer.exe"; DestDir: "{app}"; Flags: ignoreversion

; Qt and its dependancies
Source: "C:\Qt\Qt5.7.0-MinGW64\bin\Qt5Core.dll";                DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.7.0-MinGW64\bin\Qt5Gui.dll";                 DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.7.0-MinGW64\bin\Qt5Widgets.dll";             DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.7.0-MinGW64\bin\Qt5Xml.dll";                 DestDir: "{app}"; Flags: ignoreversion
source: "C:\Qt\Qt5.7.0-MinGW64\plugins\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
source: "C:\Qt\Qt5.7.0-MinGW64\plugins\imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Qt\Qt5.7.0-MinGW64\mingw-deps\libgcc_s_seh-1.dll";  DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.7.0-MinGW64\mingw-deps\libstdc++-6.dll";     DestDir: "{app}"; Flags: ignoreversion

; OpenCV
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_core310.dll";      DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_highgui310.dll";   DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_imgproc310.dll";   DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_imgcodecs310.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_videoio310.dll";   DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commonprograms}\CvComposer"; Filename: "{app}\cvcomposer.exe"
Name: "{commondesktop}\CvComposer";  Filename: "{app}\cvcomposer.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\cvcomposer.exe"; Description: "{cm:LaunchProgram,CvComposer}"; Flags: nowait postinstall skipifsilent
