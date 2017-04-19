!include "WordFunc.nsh"
#!include "nsWindows.nsh"
!include "MUI.nsh"
!include 'StdUtils.nsh'

SetCompressor /SOLID lzma


!define FILE_VERSION  "1.0.0.2"
!define PRODUCT_VERSION  "1.0.0.2"
!define PRODUCT_NAME  "SunPlayer"
!define DESKTOP_ICON  "SunPlayer"        ; �����ݷ�ʽ����
!define INTERNAL_NAME "${PRODUCT_NAME}.exe"
!define FOLDERNAME "${PRODUCT_NAME}"	; �ļ�������
!define MainExe "${PRODUCT_NAME}.exe"     ; ����������
!define Company_Name "xxoo.com"


OutFile "${PRODUCT_NAME}Setup.exe"     ;���ɵİ�װ������


!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

!define MUI_ICON "logo.ico"		;��װ����ͼ��ico�ļ�
!define MUI_UNICON "logo.ico"		;ж�س����ͼ��ico�ļ�

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES
; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"
; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS


VIProductVersion ${PRODUCT_VERSION}
VIAddVersionKey /LANG=${LANG_SimpChinese} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SimpChinese} "LegalCopyright" "Copyright (C) 2015-2016 ${Company_Name}"
VIAddVersionKey /LANG=${LANG_SimpChinese} "FileDescription" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SimpChinese} "FileVersion" ${FILE_VERSION}
VIAddVersionKey /LANG=${LANG_SimpChinese} "ProductVersion" ${PRODUCT_VERSION}
VIAddVersionKey /LANG=${LANG_SimpChinese} "OriginalFilename" ${INTERNAL_NAME}
VIAddVersionKey /LANG=${LANG_SimpChinese} "InternalName" ${INTERNAL_NAME}
VIAddVersionKey /LANG=${LANG_SimpChinese} "CompanyName" "${Company_Name}"

RequestExecutionLevel admin

Function AddIcon
      ;д�����ݷ�ʽ
      CreateShortCut "$DESKTOP\${DESKTOP_ICON}.lnk" "$INSTDIR\${MainExe}" "$INSTDIR\${MainExe}"
      ;д��ʼ�˵���ݷ�ʽ
      CreateDirectory "$SMPROGRAMS\${FOLDERNAME}"
      CreateShortCut "$SMPROGRAMS\${FOLDERNAME}\ж��${DESKTOP_ICON}.lnk" "$INSTDIR\Uninstall.exe"
      CreateShortCut "$SMPROGRAMS\${FOLDERNAME}\${DESKTOP_ICON}.lnk" "$INSTDIR\${MainExe}" "$INSTDIR\${MainExe}"
       ;����������
      ${StdUtils.InvokeShellVerb} $0 "$INSTDIR" "${DESKTOP_ICON}.lnk" ${StdUtils.Const.ShellVerb.PinToTaskbar}
      ;��ʼ�˵�����
      ${StdUtils.InvokeShellVerb} $0 "$INSTDIR" "${DESKTOP_ICON}.lnk" ${StdUtils.Const.ShellVerb.PinToStart}
FunctionEnd

;��ӿ������ж��
Function AddControlPannel
      WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}" "DisplayName" ${PRODUCT_NAME}
      WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}" "DisplayVersion" ${PRODUCT_VERSION}
      WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}" "Publisher" ${PRODUCT_NAME}
      WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}" "DisplayIcon" "$INSTDIR\${MainExe}"
      WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FOLDERNAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
FunctionEnd


Function .onInit
      SetSilent silent

      StrCpy $INSTDIR "$PROGRAMFILES\${FOLDERNAME}" ;���ð�װĿ¼
      SetOutPath "$INSTDIR"

      SetOverwrite try
      File /r "BIN\*.*"   ;BIN �ļ�����Ŀ¼

      Call AddControlPannel         ;дע���ж����
      WriteUninstaller "$INSTDIR\Uninstall.exe" ;����ж��exe
      Call AddIcon                  ;д��ݷ�ʽ

      Exec '"$INSTDIR\${MainExe}"'

      Quit
FunctionEnd

Section Main
SectionEnd



#�����ǰ�װ�����ж�ز���
Section Uninstall

  ;����������
  ${StdUtils.InvokeShellVerb} $0 "$INSTDIR" "${DESKTOP_ICON}.lnk" ${StdUtils.Const.ShellVerb.UnpinFromTaskbar}
  ;��ʼ�˵�����
  ${StdUtils.InvokeShellVerb} $0 "$INSTDIR" "${DESKTOP_ICON}.lnk" ${StdUtils.Const.ShellVerb.UnpinFromStart}
  Delete "$INSTDIR\${DESKTOP_ICON}.lnk"
  Delete "$DESKTOP\${DESKTOP_ICON}.lnk"
  RMDir /r "$SMPROGRAMS\${FOLDERNAME}"

  Delete "$INSTDIR\plugins\platforms\*.*"
  RMDir "$INSTDIR\plugins\platforms\"
  Delete "$INSTDIR\plugins\*.*"
  RMDir "$INSTDIR\plugins\"
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKCU "Software\BestDRM"

  SetAutoClose true
SectionEnd


#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#
Function un.onInit
      SetSilent silent

  FindProcDLL::FindProc ${MainExe}
    IntCmp $R0 1 0 norun
    MessageBox MB_ICONSTOP "${PRODUCT_NAME}��������,���ȹرճ����ٽ���ж�أ�"
    Quit
  norun:
        MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷ��Ҫ��ȫж��${PRODUCT_NAME}��" IDYES +2
        Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME}�Ѿ�ж����ɣ�"
FunctionEnd


