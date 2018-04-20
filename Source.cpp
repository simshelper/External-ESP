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
	std::cout << "��ӭʹ�������ս��ǰ��2͸�Ӹ���" << std::endl;
	std::cout << "1.��ȡ�ڴ�..." << std::endl;
	mem.attach(GameName); 
	
	std::cout << "2.��������..." << std::endl;
	Direct2DOverlay* asd = new Direct2DOverlay(asdd, Drawer);
	asd->Initialize(GameName, "�����ս��ǰ��2͸�Ӹ���");

	std::cout << "3.���ò˵�..." << std::endl;
	menu.Initialize(asd, L"������luxfun 18.04.20 v2.04");

	MenuTab esptab(L"�з�ESP");
	esptab.AddItem(MenuItem(L"�з�����", &enemyboxes));
	esptab.AddItem(MenuItem(L"�з�����", &enemysnaplines));
	esptab.AddItem(MenuItem(L"�з�����", &enemyhealth));
	esptab.AddItem(MenuItem(L"�з�����", &enemynames));

	MenuTab teamtab(L"����ESP");
	teamtab.AddItem(MenuItem(L"���ѿ���", &teamboxes));
	teamtab.AddItem(MenuItem(L"��������", &teamsnaplines));
	teamtab.AddItem(MenuItem(L"��������", &teamhealth));
	teamtab.AddItem(MenuItem(L"��������", &teamnames));

	MenuTab misctab(L"��������");
	misctab.AddItem(MenuItem(L"���ִ�ֱͬ��", &vsync));
	menu.AddTab(esptab);
	menu.AddTab(teamtab);
	menu.AddTab(misctab);

	std::cout << "4.��ʼ������" << std::endl;
	std::cout << std::endl << std::endl << "\t����:" << std::endl;
	std::cout << "\t\t[DELETE]\tչʾ/���� �˵�" << std::endl;
	std::cout << "\t\t[UP/DOWN]\t�˵�ѡ�� ��/��" << std::endl;
	std::cout << "\t\t[LEFT/RIGHT]\t����/�ر� ����" << std::endl;
	std::cout << "\t\t[TAB]\t\t�л��˵�" << std::endl;

	asd->StartLoop();
}