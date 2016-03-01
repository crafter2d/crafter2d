;NSIS Modern User Interface
;Crafter 2D installer script
;Written by Jeroen Broekhuizen

;--------------------------------
;Include extensions

  !include "MUI2.nsh"
  
;--------------------------------
;General

  ;Name and file
  Name "Crafter 2D"  
  OutFile "setup.exe"
  
  ;Default installation folder
  InstallDir "$PROGRAMFILES\Crafter2D"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Crafter2D" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user
  
;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE English
  !insertmacro MUI_LANGUAGE Dutch

;--------------------------------
;Installer Sections

InstType "Full"
InstType "Normal"

Section "Engine" SecEngine

  SectionIn 1 2
  
  SetOutPath "$INSTDIR"
  File license.txt
  File readme.md

  SetOutPath "$INSTDIR\bin"
  File /r bin\*
  
  SetOutPath "$INSTDIR\demo"
  File /r demo\*
  
  SetOutPath "$INSTDIR\scripts"
  File /r scripts\*
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
SectionEnd

Section "Source" SecSource

  SectionIn 1
  
  SetOutPath "$INSTDIR"
  File .gitignore
  File Doxyfile
  File gen.bat
  File installer.nsi
  File premake5.exe
  File premake5.lua
  File coverity_model.c
  File basic-dx.fx
  File basic-gl.fx
  File particle-dx.fx
  File particle-gl.fx
  
  SetOutPath "$INSTDIR\crafter"
  File /r crafter\*.*
  
  SetOutPath "$INSTDIR\contrib"
  File /r contrib\*.*
  
  SetOutPath "$INSTDIR\src"
  File /r src\*.*
  
  SetOutPath "$INSTDIR\tools"
  File /r tools\*.*
  
  SetOutPath "$INSTDIR\projects"
  File /r projects\*.*
  
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecEngine ${LANG_ENGLISH} "Required engine files needed to run Crafter 2D based games. Includes demo game showing the capabilities of the engine."
  LangString DESC_SecSource ${LANG_ENGLISH} "Full source code of the engine and demo."
  
  LangString DESC_SecEngine ${LANG_DUTCH} "Vereiste bestanden voor de engine inclusief een demo spel."
  LangString DESC_SecSource ${LANG_DUTCH} "Volledige source code van de engine en het demo spel."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecEngine} $(DESC_SecEngine)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSource} $(DESC_SecSource)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
  
  RMDir /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Crafter2D"

SectionEnd

;--------------------------------
;Installer functions
Function .onInit
    InitPluginsDir
    !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd