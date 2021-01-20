// Shutdownsound.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#pragma comment(lib, "winmm.lib" )
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// アクセス権の定数
#define OPEN_PROCESS_TOKEN      (TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY)

// プロセスハンドルから特権名を有効/無効
BOOL ProcessPrivilegeName(HANDLE hProcess, LPCTSTR lpPrivilegeName, BOOL bEnable)
{
    BOOL bSuccess = FALSE;          // 戻り値(成功/失敗)
    HANDLE              hToken;     // アクセストークンのハンドル
    LUID                Luid;       // LUID(ローカル・ユニークID)
    DWORD               dwSize;     // 特権トークン容量(変更前の特権)
    TOKEN_PRIVILEGES    OldPriv;    // 特権トークン情報(変更前の特権)
    TOKEN_PRIVILEGES    NewPriv;    // 特権トークン情報(新しい特権)

    // アクセストークンのハンドルを取得
    if (OpenProcessToken(hProcess, OPEN_PROCESS_TOKEN, &hToken)) {
        if (LookupPrivilegeValue(NULL, lpPrivilegeName, &Luid)) {    // 特権名のLUIDを取得
            NewPriv.PrivilegeCount = 1;                // 特権数
            NewPriv.Privileges[0].Luid = Luid;             // 識別子
            NewPriv.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

            // 特権トークン状態の有効/無効
            if (AdjustTokenPrivileges(hToken, FALSE, &NewPriv, sizeof(TOKEN_PRIVILEGES), &OldPriv, &dwSize)) {
                if (GetLastError() == ERROR_SUCCESS) {
                    bSuccess = TRUE;
                }
            }
        }
        CloseHandle(hToken);
    }
    return bSuccess;
}

int main(void)
{
    PlaySound(_TEXT("Shutdown.wav"), NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
    // Windows NT 系のために SE_SHUTDOWN_NAME 特権を有効にする
    ProcessPrivilegeName(GetCurrentProcess(), SE_SHUTDOWN_NAME, TRUE);
    ExitWindowsEx(EWX_SHUTDOWN, 0);
    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
