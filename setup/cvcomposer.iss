
[Setup]
#define AppBin "cvcomposer.exe"
#define AppName "CvComposer"
#define AppVersion "1.3"
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
; Executable and automatically gathered dependancies
Source: "..\build\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

; Haar cascade resources
Source: "..\haarcascade_*.xml"; DestDir: "{app}"; Flags: ignoreversion

; OpenCV
Source: "..\artifacts\opencv\4.6.0\windows-x64\x64\vc17\bin\opencv_*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]
Name: "{group}\{#AppName}";        Filename: "{app}\{#AppBin}"; Comment: "{#AppName} {#AppVersion}"
Name: "{userdesktop}\{#AppName}";  Filename: "{app}\{#AppBin}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppBin}"; Description: "{cm:LaunchProgram,{#AppName}}"; Flags: nowait postinstall skipifsilent
