;NSIS Modern User Interface
;JEngine SSE installer script
;Written by Jeroen Broekhuizen

;--------------------------------
;Include extensions

  !include "MUI2.nsh"
  !include "VPatchLib.nsh"
  
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
InstType "Minimal"

Section "Engine" SecEngine

  SectionIn 1 2 3
  
  SetOutPath "$INSTDIR"
  File changelog.txt
  File license.txt
  File readme.txt
  File releasenotes.txt

  SetOutPath "$INSTDIR\bin"
  File /x .svn bin\*.*
  
  SetOutPath "$INSTDIR\images"
  File images\application_double.png
  File images\application_form.png
  File images\application_form_add.png
  File images\application_form_delete.png
  File images\cancel.png
  File images\cancel_disabled.png
  File images\circle.png
  File images\folder.png
  File images\go-down.png
  File images\go-up.png
  File images\splash.png
  
  SetOutPath "$INSTDIR\scripts"
  File /oname=main.lua scripts\main.lua.default
  File /oname=server.lua scripts\server.lua.default
  File /oname=client.lua scripts\client.lua.default
  File /oname=actionmap.lua scripts\actionmap.lua.default
  File /oname=keymap.lua scripts\keymap.lua.default
  
  File scripts\box2d.lua
  File scripts\config.lua
  File scripts\console.lua
  File scripts\editor.lua
  File scripts\msgbox.lua
  File scripts\splash.lua
  
  File scripts\gen_msgbox.xml
  File scripts\des_*.*
  File scripts\te_*.*
  
  SetOutPath "$INSTDIR\shaders"
  File shaders\simple_frag.cg
  File shaders\simple_vert.cg
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
SectionEnd

Section "Demo" SecDemo

  SectionIn 1 2
  
  SetOutPath "$INSTDIR\images"
  File images\blocks.png
  File images\blocks.xml
  File images\box.png
  File images\bridgepart.png
  File images\char.png
  
  SetOutPath "$INSTDIR\objects"
  File objects\*.*
  
  SetOutPath "$INSTDIR\scripts"
  File scripts\bridge.lua
  File scripts\mainmenu.lua
  File scripts\map1.lua
  File scripts\mainmenu.xml
  File scripts\ingamemenu.xml
  
  SetOutPath "$INSTDIR\shaders"
  File shaders\blocks.xml
  
  SetOutPath "$INSTDIR\worlds"
  File worlds\intro.jwl
  File worlds\map1.jwl
    
  ; patch default files
  !insertmacro VPatchFile "scripts\demopatch.pat" "$INSTDIR\scripts\main.lua" "$INSTDIR\scripts\temporaryfile"
  !insertmacro VPatchFile "scripts\demopatch.pat" "$INSTDIR\scripts\server.lua" "$INSTDIR\scripts\temporaryfile"
  !insertmacro VPatchFile "scripts\demopatch.pat" "$INSTDIR\scripts\client.lua" "$INSTDIR\scripts\temporaryfile"
  !insertmacro VPatchFile "scripts\demopatch.pat" "$INSTDIR\scripts\actionmap.lua" "$INSTDIR\scripts\temporaryfile"
  !insertmacro VPatchFile "scripts\demopatch.pat" "$INSTDIR\scripts\keymap.lua" "$INSTDIR\scripts\temporaryfile"

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
  File /x .svn src\*.*
  
  SetOutPath "$INSTDIR\docs"
  File docs\manual.chm
  
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecEngine ${LANG_ENGLISH} "Required engine files needed to run JEngine SSE based games."
  LangString DESC_SecDemo ${LANG_ENGLISH} "Simple demo game demonstrating capabilities of the engine."
  LangString DESC_SecSource ${LANG_SOURCE} "Full source code of the engine and demo."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecEngine} $(DESC_SecEngine)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDemo} $(DESC_SecDemo)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSource} $(DESC_SecSource)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\JEngine SSE"

SectionEnd
