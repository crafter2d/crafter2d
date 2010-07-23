;NSIS Modern User Interface
;JEngine SSE installer script
;Written by Jeroen Broekhuizen

;--------------------------------
;Include extensions

  !include "MUI2.nsh"
  
;--------------------------------
;General

  ;Name and file
  Name "JEngine SSE"  
  OutFile "setup.exe"
  
  ;Default installation folder
  InstallDir "$PROGRAMFILES\JEngine SSE"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\JEngine SSE" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user
  
;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

InstType "Full"
InstType "Normal"

Section "Engine" SecEngine

  SectionIn 1 2
  
  SetOutPath "$INSTDIR"
  File changelog.txt
  File license.txt
  File readme.txt
  File releasenotes.txt

  SetOutPath "$INSTDIR\bin"
  File /x .svn bin\*.*
  
  SetOutPath "$INSTDIR\images"
  File /x .svn /x original images\*.*
  
  SetOutPath "$INSTDIR\objects"
  File /x .svn objects\*.*
  
  SetOutPath "$INSTDIR\scripts"
  File /x .svn scripts\*.*
  
  SetOutPath "$INSTDIR\shaders"
  File /x .svn shaders\*.*
  
  SetOutPath "$INSTDIR\worlds"
  File /x .svn worlds\*.*
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
SectionEnd

Section "Source" SecSource

  SectionIn 1
  
  SetOutPath "$INSTDIR"
  File cp.exe
  File Doxyfile
  File gen.bat
  File gen.sh
  File installer.nsi
  File premake.exe
  File premake.lua
  
  SetOutPath "$INSTDIR\src"
  File /r /x .svn src\*.*
  
  SetOutPath "$INSTDIR\docs"
  File docs\manual.chm
  
  SetOutPath "$INSTDIR\tolua"
  File /x .svn tolua\*.*
  
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecEngine ${LANG_ENGLISH} "Required engine files needed to run JEngine SSE based games. Includes demo game showing the capabilities of the engine."
  LangString DESC_SecSource ${LANG_SOURCE} "Full source code of the engine and demo."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecEngine} $(DESC_SecEngine)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSource} $(DESC_SecSource)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstaller Section

Section "Uninstall"
  
  RMDir /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\JEngine SSE"

SectionEnd
