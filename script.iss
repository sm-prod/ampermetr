;------------------------------------------------------------------------------
;
;       Пример установочного скрипта для Inno Setup 5.5.5
;       (c) maisvendoo, 15.04.2015
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   Определяем некоторые константы
;------------------------------------------------------------------------------

; Имя приложения
#define   Name       "Ampermetr"
; Версия приложения
#define   Version    "1.1"
; Фирма-разработчик
#define   Publisher  "sm_prod"
; Сафт фирмы разработчика
#define   URL        ""
; Имя исполняемого модуля
#define   ExeName    "ampermetr.exe"

;------------------------------------------------------------------------------
;   Параметры установки
;------------------------------------------------------------------------------
[Setup]

; Уникальный идентификатор приложения, 
;сгенерированный через Tools -> Generate GUID
AppId={{FABB4C97-316F-45EB-BDF8-F45B78FA27CC}

; Прочая информация, отображаемая при установке
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}
;AppPublisherURL={#URL}
;AppSupportURL={#URL}
;AppUpdatesURL={#URL}

; Путь установки по-умолчанию
DefaultDirName={pf}\{#Name}
; Имя группы в меню "Пуск"
DefaultGroupName={#Name}

; Каталог, куда будет записан собранный setup и имя исполняемого файла
OutputDir=C:\project\AMP
OutputBaseFileName=ampermetr

; Файл иконки
SetupIconFile=C:\project\AMP\ampermetr\icon.ico

; Параметры сжатия
Compression=lzma
SolidCompression=yes

;------------------------------------------------------------------------------
;   Устанавливаем языки для процесса установки                 LicenseFile: "License_ENG.txt"     LicenseFile: "License_RUS.txt"
;------------------------------------------------------------------------------
[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl" 
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl" 

;------------------------------------------------------------------------------
;   Опционально - некоторые задачи, которые надо выполнить при установке
;------------------------------------------------------------------------------
[Tasks]
; Создание иконки на рабочем столе
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked 


;------------------------------------------------------------------------------
;   Файлы, которые надо включить в пакет установщика
;------------------------------------------------------------------------------
[Files]

; Исполняемый файл
Source: "C:\project\AMP\ampermetr_32\ampermetr.exe"; DestDir: "{app}"; Flags: ignoreversion

; Прилагающиеся ресурсы
Source: "C:\project\AMP\ampermetr_32\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

;    Вставить установочник дров
;Source: "C:\project\MAGMID\VideoReg\evicam_demo\driver\*"; DestDir: "{tmp}"; Flags: deleteafterinstall

;------------------------------------------------------------------------------
;   Указываем установщику, где он должен взять иконки
;------------------------------------------------------------------------------ 
[Icons]

Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"

Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon

;[Run]
;------------------------------------------------------------------------------
;   Секция запуска после инсталляции
;------------------------------------------------------------------------------
;Filename: {tmp}\dpinst.exe; StatusMsg: Drivers pack is installed. Please wait...