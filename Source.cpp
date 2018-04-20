#include <Windows.h>
#include <iostream>
#include "SDK.h"
#include "D2DOverlay.h"
#include "Menu.h"

void Drawer(Direct2DOverlay* dis);
HINSTANCE asdd;
Menu menu;


#pragma region Settings
D3DXCOLOR Friend = D3DXCOLOR(0, 0, 1, 0);
D3DXCOLOR Enemy = D3DXCOLOR(1, 0, 0, 0);
D3DXCOLOR Visible = D3DXCOLOR(1, 1, 0, 0);

int enemyboxes = true;
int teamboxes = true;
int enemysnaplines = true;
int teamsnaplines = false;
int enemyvehicles = true;
int teamvehicles = true;
int enemyhealth = true;
int teamhealth = false;
int enemynames = true;
int teamnames = false;
int vsync = true;
#pragma endregion


int ticks = 0;
int beforeclock = 0;
int FPS = 0;


void Drawer(Direct2DOverlay* ov)
{
	int vsyncA = clock();

	ticks += 1;
	if (beforeclock == 0) {
		beforeclock = clock();
	}
	ov->BeginDraw();
	ov->ClearScreen();
	
	menu.Render();

	RECT r = ov->GetOverlayRect();
	Manager gm(r);

	ov->DrawString(std::to_wstring(FPS), 30, (r.right - r.left) - 60, 20, .9, 1, 0);

	for (Player &player : gm.Players){

		D3DXCOLOR Chosencolor = D3DXCOLOR();
		if (player.Team == gm.LocalPlayer.Team) {
			Chosencolor = Friend;
		}
		else {
			player.Visible ? Chosencolor = Visible : Chosencolor = Enemy;
		}

		D3DXVECTOR3 head = player.Location;
		D3DXVECTOR3 foot = player.Location;
		head.y += player.SoldierHeight + .25;
		if ((gm.WorldToScreen(foot)) && (gm.WorldToScreen(head))) {
			float heightoffset = Distance3D(foot, head);

			if ((player.Team == gm.LocalPlayer.Team && teamboxes) || (player.Team != gm.LocalPlayer.Team && enemyboxes)) {
					float factor = (heightoffset / 5);

					D3DXVECTOR3 m2 = D3DXVECTOR3(head.x - factor, head.y, 0);
					D3DXVECTOR3 m1 = D3DXVECTOR3(head.x + factor, head.y, 0);
					D3DXVECTOR3 m3 = D3DXVECTOR3(foot.x - factor, foot.y, 0);
					D3DXVECTOR3 m4 = D3DXVECTOR3(foot.x + factor, foot.y, 0);

					ov->DrawLine(m1.x, m1.y, m2.x, m2.y, 3, Chosencolor.r, Chosencolor.g, Chosencolor.b);
					ov->DrawLine(m2.x, m2.y, m3.x, m3.y, 3, Chosencolor.r, Chosencolor.g, Chosencolor.b);
					ov->DrawLine(m3.x, m3.y, m4.x, m4.y, 3, Chosencolor.r, Chosencolor.g, Chosencolor.b);
					ov->DrawLine(m4.x, m4.y, m1.x, m1.y, 3, Chosencolor.r, Chosencolor.g, Chosencolor.b);
			}

			if ((player.Team == gm.LocalPlayer.Team && teamhealth) || (player.Team != gm.LocalPlayer.Team && enemyhealth)) {
				float factor = (heightoffset / 3);
				D3DXVECTOR3 m1 = D3DXVECTOR3(head.x + factor, head.y, 0);
				D3DXVECTOR3 m2 = D3DXVECTOR3(foot.x + factor, foot.y, 0);

				D3DXCOLOR LowColor = D3DXCOLOR(1, 0, 0, 1);
				D3DXCOLOR HighColor = D3DXCOLOR(0, 1, 0, 1);
				float perc = (player.Health / player.MaxHealth);

				float thicc = heightoffset/8;
				if (thicc < 4) thicc = 4;
				D3DXCOLOR HealthColor = D3DXCOLOR(1 - perc, perc, 0, 1);

				ov->DrawLine(m1.x + thicc / 2, m1.y - 2, m2.x + thicc / 2, m2.y + 2, thicc + 4, 0, 0, 0);
				ov->DrawLine(m1.x + thicc / 2, m1.y + ((m2.y - m1.y)*(1 - perc)), m2.x + thicc / 2, m2.y, thicc, HealthColor.r, HealthColor.g, HealthColor.b);

			}
			if ((player.Team == gm.LocalPlayer.Team && teamnames) || (player.Team != gm.LocalPlayer.Team && enemynames)) {
				int fontsize = 20;
				float offset = (player.Name.length() * fontsize) / 5;
				ov->DrawString(player.Name, fontsize, head.x - offset, head.y - (heightoffset/4) - fontsize, Chosencolor.r, Chosencolor.g, Chosencolor.b);
			}
			if ((player.Team == gm.LocalPlayer.Team && teamsnaplines) || (player.Team != gm.LocalPlayer.Team && enemysnaplines)) {
				ov->DrawLine(foot.x, foot.y, (gm.rect.right - gm.rect.left)/2, gm.rect.bottom - gm.rect.top, 3, Chosencolor.r, Chosencolor.g, Chosencolor.b);
			}
		}
	}


	ov->EndDraw();
	if (vsync) {
		int pausetime = 15 - (clock() - vsyncA);
		if (pausetime > 0 && pausetime < 30) {
			Sleep(pausetime);
		}
	}

	int afterclock = clock();

	if (afterclock - beforeclock > 1000) {
		FPS = ticks;
		beforeclock = 0;
		ticks = 0;
	}
}


void main() {
	std::cout << "欢迎使用星球大战：前线2透视辅助" << std::endl;
	std::cout << "1.读取内存..." << std::endl;
	mem.attach(GameName); 
	
	std::cout << "2.覆盖遮罩..." << std::endl;
	Direct2DOverlay* asd = new Direct2DOverlay(asdd, Drawer);
	asd->Initialize(GameName, "星球大战：前线2透视辅助");

	std::cout << "3.设置菜单..." << std::endl;
	menu.Initialize(asd, L"汉化：luxfun 18.04.20 v2.04");

	MenuTab esptab(L"敌方ESP");
	esptab.AddItem(MenuItem(L"敌方框体", &enemyboxes));
	esptab.AddItem(MenuItem(L"敌方视线", &enemysnaplines));
	esptab.AddItem(MenuItem(L"敌方生命", &enemyhealth));
	esptab.AddItem(MenuItem(L"敌方姓名", &enemynames));

	MenuTab teamtab(L"队友ESP");
	teamtab.AddItem(MenuItem(L"队友框体", &teamboxes));
	teamtab.AddItem(MenuItem(L"队友视线", &teamsnaplines));
	teamtab.AddItem(MenuItem(L"队友生命", &teamhealth));
	teamtab.AddItem(MenuItem(L"队友姓名", &teamnames));

	MenuTab misctab(L"其它设置");
	misctab.AddItem(MenuItem(L"遮罩垂直同步", &vsync));
	menu.AddTab(esptab);
	menu.AddTab(teamtab);
	menu.AddTab(misctab);

	std::cout << "4.开始工作！" << std::endl;
	std::cout << std::endl << std::endl << "\t控制:" << std::endl;
	std::cout << "\t\t[DELETE]\t展示/隐藏 菜单" << std::endl;
	std::cout << "\t\t[UP/DOWN]\t菜单选项 上/下" << std::endl;
	std::cout << "\t\t[LEFT/RIGHT]\t开启/关闭 功能" << std::endl;
	std::cout << "\t\t[TAB]\t\t切换菜单" << std::endl;

	asd->StartLoop();
}