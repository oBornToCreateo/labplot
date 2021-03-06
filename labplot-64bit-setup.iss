#define MyAppName "LabPlot2"
#define MyAppVersion "2.5.0"
#define MyAppPublisher "Stefan Gerlach"
#define MyAppURL "https://labplot.kde.org"
#define MyAppExeName "labplot2.exe"
#define ImageMagickVersion "ImageMagick-7.0.7-Q16"
#define CraftRoot "C:\CraftRoot"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EAFA7C2D-F2C4-4337-A4D3-3912BEA4F535}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf64}\{#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename=labplot-{#MyAppVersion}-64bit-setup
ArchitecturesAllowed=x64
;use "none" for testing (much faster)
Compression=lzma
SolidCompression=yes
Uninstallable=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#CraftRoot}\bin\labplot2.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\libkdewin.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Archive.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5core.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\icuin58.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\icuuc58.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\icudt58.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\mingw64\bin\libgcc_s_seh-1.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\mingw64\bin\libwinpthread-1.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\mingw64\bin\libstdc++-6.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\mingw64\bin\libssp-0.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libz.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\liblzma.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Completion.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5ConfigCore.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5ConfigGui.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Gui.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Xml.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5WidgetsAddons.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Widgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5ConfigWidgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Auth.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5CoreAddons.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Codecs.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5GuiAddons.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5I18n.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libintl-8.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\iconv.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5DBus.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5IconThemes.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5ItemViews.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Svg.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Crash.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5WindowSystem.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5GlobalAccel.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5JobWidgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5KIOCore.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5DBusAddons.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Service.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Network.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libeay32.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\ssleay32.dll"; DestDir: "{app}";Flags: ignoreversion
;Source: "{#CraftRoot}\bin\libssl-1_1-x64.dll"; DestDir: "{app}";Flags: ignoreversion
;Source: "{#CraftRoot}\bin\libcrypto-1_1-x64.dll"; DestDir: "{app}";Flags: ignoreversion
;Source: "{#CraftRoot}\bin\libdbus-1-3.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5KIOFileWidgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5KIOWidgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5XmlGui.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Attica.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5TextWidgets.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5SonnetCore.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5SonnetUi.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5PrintSupport.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5NewStuff.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5SyntaxHighlighting.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libKF5Parts.dll"; DestDir: "{app}";Flags: ignoreversion
;Source: "{#CraftRoot}\bin\libKF5Notifications.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5Sql.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\Qt5TextToSpeech.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\plugins\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#CraftRoot}\plugins\printsupport\windowsprintersupport.dll"; DestDir: "{app}\printsupport"; Flags: ignoreversion
;Source: "{#CraftRoot}\dev-utils\bin\msvcr120.dll"; DestDir: "{app}"; Flags: ignoreversion
; cantor
Source: "{#CraftRoot}\bin\libcantorlibs.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\plugins\cantor\*"; DestDir: "{app}\cantor"; Flags: recursesubdirs ignoreversion
Source: "{#CraftRoot}\bin\libKF5NewStuffCore.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "{#CraftRoot}\bin\cantor.exe"; DestDir: "{app}";Flags: ignoreversion
; misc
Source: "{#CraftRoot}\bin\libfftw3.dll"; DestDir: "{app}"; DestName: "libfftw3-3.dll"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\netcdf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\hdf5_hl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\hdf5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; TODO: craft does not install own version
;Source: "C:\Program Files\cfitsio\cfitsio.dll"; DestDir: "{app}";Flags: ignoreversion

Source: "{#CraftRoot}\bin\data\labplot2\*"; Excludes: "splash.png,\pics,\themes,\colorschemes"; DestDir: "{app}\labplot2"; Flags: recursesubdirs ignoreversion
Source: "{#CraftRoot}\bin\data\kxmlgui5\labplot2\labplot2ui.rc"; DestDir: "{app}\labplot2"; Flags: ignoreversion
Source: "{#CraftRoot}\bin\data\labplot2\pics\*"; DestDir: "{app}\data\pics"; Flags: recursesubdirs ignoreversion
Source: "{#CraftRoot}\bin\data\labplot2\themes\*"; DestDir: "{app}\data\themes"; Flags: recursesubdirs ignoreversion
Source: "{#CraftRoot}\bin\data\labplot2\color-schemes\*"; DestDir: "{app}\data\color-schemes"; Flags: recursesubdirs ignoreversion
Source: "{#CraftRoot}\bin\data\labplot2\splash.png"; DestDir: "{app}"; Flags: ignoreversion
; Source: "{#CraftRoot}\labplot\labplot2.cmd"; DestDir: "{app}";Flags: ignoreversion

; icon theme
Source: "{#CraftRoot}\bin\data\icontheme.rcc"; DestDir: "{app}\data";Flags: ignoreversion
; oxygen icons
;Source: "{#CraftRoot}\bin\data\icons\hicolor\*"; DestDir: "{app}\icons\hicolor"; Flags: recursesubdirs ignoreversion

; handbook
Source: "{#CraftRoot}\bin\data\doc\HTML\en\labplot2\*"; DestDir: "{app}\doc\HTML\en\labplot2";Flags: recursesubdirs ignoreversion
; for SVG icons
Source: "{#CraftRoot}\plugins\iconengines\qsvgicon.dll"; DestDir: "{app}\iconengines";Flags: ignoreversion
Source: "{#CraftRoot}\bin\libexpat.dll"; DestDir: "{app}";Flags: ignoreversion

; convert
; TODO
Source: "C:\Program Files\{#ImageMagickVersion}\convert.exe"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_MagickCore_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_MagickWand_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_bzlib_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_glib_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_lcms_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_lqr_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_png_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_ttf_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\CORE_RL_zlib_.dll"; DestDir: "{app}";Flags: ignoreversion
; Source: "C:\Program Files\{#ImageMagickVersion}\msvcr120.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\vcomp120.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\delegates.xml"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\magic.xml"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\modules\coders\IM_MOD_RL_pdf_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\modules\coders\IM_MOD_RL_png_.dll"; DestDir: "{app}";Flags: ignoreversion
Source: "C:\Program Files\{#ImageMagickVersion}\modules\coders\IM_MOD_RL_ps_.dll"; DestDir: "{app}";Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{app}\labplot2\labplot2.ico"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

; [Registry]
; Root: HKCU; Subkey: "Environment"; ValueType:string; ValueName:"KDEROOT"; ValueData:"{app}" ; Flags: preservestringtype ;
