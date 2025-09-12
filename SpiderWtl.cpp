// BmpView.cpp : main source file for BmpView.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcrack.h>

CAppModule _Module;


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT )
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainWindow wndMain;

    if(wndMain.CreateEx() == NULL)
    {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
    }

    wndMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(iccx);
    iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
    BOOL bRet = ::InitCommonControlsEx(&iccx);
    bRet;
    ATLASSERT(bRet);

    HRESULT hRes = _Module.Init(NULL, hInstance);
    hRes;
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    return nRet;
}
