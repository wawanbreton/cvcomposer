
[Setup]
#define AppBin "cvcomposer.exe"
#define AppName "CvComposer"
#define AppVersion "1.2"
AppId={{051E987E-CDAA-49CB-A5EF-014FDEBAF6FE}}
AppName={#AppName}
VersionInfoVersion={#AppVersion}
AppVerName={#AppName} - v{#AppVersion}     
DefaultDirName={userpf}\{#AppName}
DisableProgramGroupPage=yes
OutputBaseFilename=CvComposer-setup
Compression=lzma
PrivilegesRequired=lowest
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64


[Files]
; Main executable
Source: "..\release\{#AppBin}"; DestDir: "{app}"; Flags: ignoreversion

; Haar cascade resources
Source: "..\haarcascade_*.xml"; DestDir: "{app}"; Flags: ignoreversion

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
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_objdetect310.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_video310.dll";     DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files\opencv3.1.0\build-mingw64\bin\libopencv_ml310.dll";        DestDir: "{app}"; Flags: ignoreversion

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]
Name: "{group}\{#AppName}";        Filename: "{app}\{#AppBin}"; Comment: "{#AppName} {#AppVersion}"
Name: "{userdesktop}\{#AppName}";  Filename: "{app}\{#AppBin}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppBin}"; Description: "{cm:LaunchProgram,{#AppName}}"; Flags: nowait postinstall skipifsilent
