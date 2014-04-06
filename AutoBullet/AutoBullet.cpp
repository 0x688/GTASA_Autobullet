#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

bool EnableDebugPrivilege()
{
    HANDLE hThis = GetCurrentProcess();
    HANDLE hToken;
    OpenProcessToken(hThis, TOKEN_ADJUST_PRIVILEGES, &hToken);
    LUID luid;
    LookupPrivilegeValue(0, TEXT("seDebugPrivilege"), &luid);
    TOKEN_PRIVILEGES priv;
    priv.PrivilegeCount = 1;
    priv.Privileges[0].Luid = luid;
    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, false, &priv, 0, 0, 0);
    CloseHandle(hToken);
    CloseHandle(hThis);
	
    return true;
} 

void SetTextCol(int iColID)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hSTDOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if(GetConsoleScreenBufferInfo(hSTDOutput, &csbi))
		SetConsoleTextAttribute(hSTDOutput, ((WORD)(csbi.wAttributes & 0xF0) + (iColID & 0x0F)));
}


HANDLE hGTA;
#define MAX_WEAPS_IT		(100)
DWORD	dwOrginalWeaponState[MAX_WEAPS_IT];
bool bStatus = false;
int _tmain(int argc, _TCHAR* argv[])
{
	EnableDebugPrivilege();
	DWORD dwID;
    GetWindowThreadProcessId(FindWindow(NULL, TEXT("GTA:SA:MP")), &dwID);
    hGTA = OpenProcess(PROCESS_ALL_ACCESS, false, dwID);

	if ( hGTA ) {
		SetTextCol(34);
		printf(" --- [ Autobullet.eXe by 0x688 ] ---\n");
		SetTextCol(4);
		printf(" Visit: youtube.com/0x688 & ugbase.eu or ugbase.net\n");
		SetTextCol(3);
		printf(" Keys: Enable & Disable (F5)\n");
		SetTextCol(7);
		printf(" Waiting for User Interactions ... \n\n");

		DWORD dwAddr;
		while ( true ) 
		{ 
			if ( GetAsyncKeyState(VK_F5) & 0x80000 ) {
				bStatus ^= 1;

				if ( bStatus ) {
					for ( int i=0; i != MAX_WEAPS_IT; i++) {
						dwAddr = 0x70;
						dwAddr *= i;
						dwAddr += 0x18; // DWORD flags
						dwAddr = 0xC8AAB8 + dwAddr;
						ReadProcessMemory(hGTA, (void *)dwAddr, &dwOrginalWeaponState[i], sizeof(DWORD), 0);

						DWORD val = 3;
						WriteProcessMemory(hGTA, (void *)dwAddr,  &val,  sizeof(DWORD), NULL);
					}
					SetTextCol(3);
					printf("- Autobullet enabled !\n");
					SetTextCol(7);
					MessageBeep(MB_OK);
				}
				else {
					for ( int i=0; i != MAX_WEAPS_IT; i++) {
						dwAddr = 0x70;
						dwAddr *= i;
						dwAddr += 0x18; // DWORD flags
						dwAddr = 0xC8AAB8 + dwAddr;
						WriteProcessMemory(hGTA, (void *)dwAddr,  &dwOrginalWeaponState[i],  sizeof(DWORD), NULL);
					}
					SetTextCol(4);
					printf("- Autobullet disabled !\n");
					SetTextCol(7);
					MessageBeep(MB_ICONERROR);
				}

				Sleep(1000);
			}
			Sleep(5); 
		}
	}
	return 0;
}

