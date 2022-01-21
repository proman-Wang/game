#include "RPGgame.h"
#include ".\\TinyEngine\\T_Util.h"

//���¶���֡����
int RPGgame::PROP_FRAME[9] = { 0,0,0,1,1,1,2,2,2 };
int RPGgame::PERSON_FRAME_LEFT[20] = { 4,4,5,5,5,6,6,6,7,7,7,4,4,5,5,6,6,7,7,7 };
int RPGgame::PERSON_FRAME_RIGHT[20] = { 8,8,9,9,9,10,10,10,11,11,11,8,8,9,9,10,10,11,11,11 };;
int RPGgame::PERSON_FRAME_UP[20] = { 12,12,13,13,13,14,14,14,15,15,15,12,12,13,13,14,14,15,15,15 };
int RPGgame::PERSON_FRAME_DOWN[20] = { 0,0,1,1,1,2,2,2,3,3,3,0,0,1,1,2,2,3,3,3 };

int RPGgame::FRAME_CLOSE_ATTACK_LEFT[20] = { 20,20,21,21,21,22,22,22,23,23,23,20,20,21,21,22,22,23,23,23 };
int RPGgame::FRAME_CLOSE_ATTACK_RIGHT[20] = { 24,24,25,25,25,26,26,26,27,27,27,24,24,25,25,26,26,27,27,27 };
int RPGgame::FRAME_CLOSE_ATTACK_UP[20] = { 28,28,29,29,29,30,30,30,31,31,31,28,28,29,29,30,30,31,31,31 };
int RPGgame::FRAME_CLOSE_ATTACK_DOWN[20] = { 16,16,17,17,17,18,18,18,19,19,19,16,16,17,17,18,18,19,19,19 };

int RPGgame::FRAME_CLOSE_SHOOT_LEFT[20] = { 36,36,37,37,37,38,38,38,39,39,39,36,36,37,37,38,38,39,39,39 };
int RPGgame::FRAME_CLOSE_SHOOT_RIGHT[20] = { 40,40,41,41,41,42,42,42,43,43,43,40,40,41,41,42,42,43,43,43 };
int RPGgame::FRAME_CLOSE_SHOOT_UP[20] = { 44,44,45,45,45,46,46,46,47,47,47,44,44,45,45,46,46,47,47,47 };
int RPGgame::FRAME_CLOSE_SHOOT_DOWN[20] = { 32,32,33,33,33,34,34,34,35,35,35,32,32,33,33,34,34,35,35,35 };
int RPGgame::EXPLOSION_FRAME[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
bool clickState;

LEVELINFO RPGgame::LevelSprites[TOTAL_LEVEL + 1] =
{
	//imgName					frameWidth		frameHeight 	  Speed	   Score	    Ratio     level
	//------------------------------------------------------------------------
	{L"snack.png",				48,					64,				3,		15,		     1.0f,        1},
	{L"lion.png",			    48,					64,				5,		20,		     1.0f,        1},
	{L"ghost.png",		        32,					32,				10,		25,		     2.0f,        2},
	{L"skull.png",	            32,					32,				6,		30,		     2.0f,        2},
	{L"dragon.png",		        48,					64,				6,		35,		     2.0f,        3},
	{L"chicken.png",			32,					32,				3,		10,		     1.0f,        2}
};

//�๹�캯��
RPGgame::RPGgame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	wnd_width = winwidth;
	wnd_height = winheight;
}

// WinMain����
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"T_Engine���Գ���";
	RPGgame* test = new RPGgame(h_instance, WIN_CLASS, WinTitle, NULL, NULL, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = test;
	test->SetFrame(30);
	test->StartEngine();
	return TRUE;
}

// �����������
RPGgame::~RPGgame(void)
{
	ClearGameLevel();
}

//������Ϸ��ʼ���ӿں���
//��Ϸ��ʼ������
void RPGgame::GameInit()
{
	GameLevel = 1;//��Ϸ�ؿ���ʼ��
	LoadGameLevel(GameLevel);
	GameState == GAME_START;
	if (!ds.CreateDS(m_hWnd)) return;
	backmusic_buffer.LoadWave(ds, L".\\sound\\backmusic.wav");
	mousedown_buffer.LoadWave(ds, L".\\sound\\click.wav");
	mousemove_buffer.LoadWave(ds, L".\\sound\\move.wav");
	shoot_buffer.LoadWave(ds, L".\\sound\\shoot.wav");
	walk_buffer.LoadWave(ds, L".\\sound\\walk.wav");
	demaged_buffer.LoadWave(ds, L".\\sound\\demaged.wav");
	backmusic_buffer.Play(true); //��ʼѭ�����ű�������
}

//��Ϸ�߼�����
void RPGgame::GameLogic()
{
	GameKeyAction();//���������Ϸ����
	if (GameState == GAME_RUN)
	{
		player->SetEndTime(GetTickCount()); //��¼��ɫ���ɺ��ʱ��

		//��ɫ���ɺ��ܱ�����ʱ��
		if (player->GetEndTime() - player->GetStartTime() >= SUPER_TIME)
		{
			player->SetStartTime(player->GetEndTime());
			isDelayCollision = false;
		}
		//if (isDelayCollision == true) player->SetImage(superPlayerImg);
		if (isDelayCollision == false) player->ResetImage();

		UpdateNpcPos();//�ƶ�����ҽ�ɫ(NPC)
		UpdatePlayerPos();//�ƶ���ҽ�ɫ
		UpdateFirePos(&npc_fire_set);//���¾�������
		UpdateArrowPos(&player_arrow_set);
		UpdateAnimation();//���¶�����

		if (updateLifeCount == true) UpdatePlayerLife();
		if (upgradeGameLevel == true) UpgradePlayerLevel();

		LoadProp();
		UpdateProp(arrow_set);
		UpdateProp(life_set);
		UpdateProp(speed_set);
	}
}

//��Ϸ��ʾ
void RPGgame::GamePaint(HDC hdc)
{
	if (GameState == GAME_RUN)
	{
		t_scene->Draw(hdc, 0, 0);
	}
	PaintMenu(hdc, GameState);
}

//��Ϸ�˳�
void RPGgame::GameEnd()
{
	ClearGameLevel();
}

//��Ϸ��������
void RPGgame::GameKeyAction(int Action)
{
	if (GameState == GAME_RUN)
	{
		if (CheckKey(VK_LEFT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
		{
			player->SetActive(true);
			player->SetSequence(PERSON_FRAME_LEFT, 20);
			player->SetDir(DIR_LEFT);
			walk_buffer.Restore();
			walk_buffer.Play(false);
		}
		if (CheckKey(VK_RIGHT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
		{
			player->SetActive(true);
			player->SetSequence(PERSON_FRAME_RIGHT, 20);
			player->SetDir(DIR_RIGHT);
			walk_buffer.Restore();
			walk_buffer.Play(false);
		}
		if (CheckKey(VK_DOWN) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
		{
			player->SetActive(true);
			player->SetSequence(PERSON_FRAME_DOWN, 20);
			player->SetDir(DIR_DOWN);
			walk_buffer.Restore();
			walk_buffer.Play(false);
		}
		if (CheckKey(VK_UP) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
		{
			player->SetActive(true);
			player->SetSequence(PERSON_FRAME_UP, 20);
			player->SetDir(DIR_UP);
			walk_buffer.Restore();
			walk_buffer.Play(false);
		}

		if (CheckKey(VK_SHIFT))
		{
			player->SetSpeed(speedup);
		}
		if (CheckKey(VK_SHIFT) == false)
		{
			player->SetSpeed(4);
		}
		if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_UP) == false && CheckKey(VK_DOWN) == false)
		{
			player->SetActive(false);
		}
		if (Action == KEY_DOWN) {
			if (GetAsyncKeyState(VK_SPACE) < 0)
			{
				if (player->IsDead() == false && player->IsVisible() == true)
				{
					if (ChargeCount > 0)
					{
						LoadArrow(player, player_arrow_set, 500);
						switch (player->GetDir())
						{
						case DIR_UP:
							player->SetSequence(FRAME_CLOSE_SHOOT_UP, 20);
							player->LoopFrame();

							break;
						case DIR_DOWN:
							player->SetSequence(FRAME_CLOSE_SHOOT_DOWN, 20);
							player->LoopFrame();
							break;
						case DIR_LEFT:
							player->SetSequence(FRAME_CLOSE_SHOOT_LEFT, 20);
							player->LoopFrame();
							break;
						case DIR_RIGHT:
							player->SetSequence(FRAME_CLOSE_SHOOT_RIGHT, 20);
							player->LoopFrame();
							break;
						}
					}
					if (ChargeCount == 0)
					{
						switch (player->GetDir())
						{
						case DIR_UP:
							player->SetSequence(FRAME_CLOSE_ATTACK_UP, 20);
							player->LoopFrame();

							break;
						case DIR_DOWN:
							player->SetSequence(FRAME_CLOSE_ATTACK_DOWN, 20);
							player->LoopFrame();
							break;
						case DIR_LEFT:
							player->SetSequence(FRAME_CLOSE_ATTACK_LEFT, 20);
							player->LoopFrame();
							break;
						case DIR_RIGHT:
							player->SetSequence(FRAME_CLOSE_ATTACK_RIGHT, 20);
							player->LoopFrame();
							break;
						}
					}
				}
			}
		}
	}
}


//��Ϸ�����Ϊ
void RPGgame::GameMouseAction(int x, int y, int Action)
{
	if (Action == MOUSE_MOVE && GameState != GAME_RUN) {
		t_menu.MenuMouseMove(x, y);
		t_menuAbout.MenuMouseMove(x, y);
		t_menuHelp.MenuMouseMove(x, y);
		t_menuFail.MenuMouseMove(x, y);
		t_menuNext.MenuMouseMove(x, y);
		t_menuWin.MenuMouseMove(x, y);
	}
	else
	{
		t_menuRun.MenuMouseMove(x, y);
	}

	if (Action == MOUSE_LCLICK)
	{
		if (GameState == GAME_START) {
			int index = t_menu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_RUN;
					break;
				case 1:
					GameState = GAME_ABOUT;
					break;
				case 2:
					GameState = GAME_HELP;
					break;
				case 3:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_ABOUT) {
			int index = t_menuAbout.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_START;
					break;
				case 1:
					GameState = GAME_RUN;
					break;
				case 2:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_HELP) {
			int index = t_menuHelp.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_START;
					break;
				case 1:
					GameState = GAME_RUN;
					break;
				case 2:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_UPGRADE) {
			int index = t_menuAbout.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_RUN;
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_OVER) {
			int index = t_menuAbout.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_START;
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_WIN) {
			int index = t_menuWin.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_START;
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
		else if (GameState == GAME_RUN)
		{

			RECT menuAreaRec;
			menuAreaRec.left = (wnd_width - navMenu.GetImageWidth());
			menuAreaRec.top = 0;
			menuAreaRec.right = (wnd_width);
			menuAreaRec.bottom = navMenu.GetImageHeight();
			POINT mousePt;
			mousePt.x = x;
			mousePt.y = y;

			if (PtInRect(&menuAreaRec, mousePt))
			{
				clickState = TRUE;
			}
			else
			{
				clickState = FALSE;
			}

			int index = t_menuRun.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index) {
				case 0:
					GameState = GAME_START;
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}

	}
}




//��Դ������غ���
// ���ؽ���˵�

void RPGgame::LoadMenu()
{
	Color normalClr, focusClr;

	int x = 0, y = 0;

	//��ʼҳ��˵�����-----------------------------------------------------------------------------
	wstring menuItems[] = { L"��ʼ",L"����" ,L"����",L"�˳�" };
	t_menu.SetMenuBkg(L".\\res\\game\\background.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	int btn_width = 247;   //�˵���ͼƬ��
	int btn_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::Yellow;   //����״̬������ɫ
	focusClr = Color::Red;  //ѡ��״̬
	t_menu.SetBtnBmp(L".\\res\\game\\blueButton.png", btn_width, btn_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;  //���䷽ʽ����
	menuInfo.space = MENU_SPACE;
	menuInfo.width = btn_width;
	menuInfo.height = btn_height;
	menuInfo.fontName = L"����";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);
	for (int i = 0; i < 4; i++)
	{
		x = (wndWidth - btn_width) / 2;
		y = i * (btn_height + MENU_SPACE) + (wnd_height - 3 * btn_height - 2 * MENU_SPACE) / 2;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);

	}
	t_menu.SetClickSound(&mousedown_buffer);
	t_menu.SetMoveSound(&mousemove_buffer);


	//���ڲ˵�����------------------------------------------------------------------------------------
	wstring menuAboutItems[] = { L"����",L"��ʼ" ,L"�˳�" };
	t_menuAbout.SetMenuBkg(L".\\res\\game\\background2.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	int btnA_width = 100;   //�˵���ͼƬ��
	int btnA_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::White;   //����״̬������ɫ
	focusClr = Color::Green;  //ѡ��״̬
	t_menuAbout.SetBtnBmp(L".\\res\\game\\circle_button.png", btnA_width, btnA_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuAboutInfo;
	menuAboutInfo.align = 1;  //���䷽ʽ����
	menuAboutInfo.space = 3 * MENU_SPACE;
	menuAboutInfo.width = btnA_width;
	menuAboutInfo.height = btnA_height;
	menuAboutInfo.fontName = L"����";
	menuAboutInfo.isBold = true;
	menuAboutInfo.normalTextColor = normalClr;
	menuAboutInfo.focusTextColor = focusClr;
	t_menuAbout.SetMenuInfo(menuAboutInfo);
	for (int i = 0; i < 3; i++)
	{
		y = wnd_height - 2 * btnA_height;
		x = i * (btnA_width + MENU_SPACE) + (wnd_width - 3 * btnA_width - 2 * MENU_SPACE) / 2;
		MENUITEM mItemA;
		mItemA.pos.x = x;
		mItemA.pos.y = y;
		mItemA.ItemName = menuAboutItems[i];
		t_menuAbout.AddMenuItem(mItemA);

	}
	t_menuAbout.SetClickSound(&mousedown_buffer);
	t_menuAbout.SetMoveSound(&mousemove_buffer);



	//����ҳ��˵�����----------------------------------------------------------------------------------
	wstring menuHelpItems[] = { L"����",L"��ʼ" ,L"�˳�" };
	t_menuHelp.SetMenuBkg(L".\\res\\game\\background.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	//int btn_width = 247;   //�˵���ͼƬ��
	//int btn_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::Yellow;   //����״̬������ɫ
	focusClr = Color::Red;  //ѡ��״̬
	t_menuHelp.SetBtnBmp(L".\\res\\game\\circle_button.png", btnA_width, btnA_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuHelpInfo;
	menuHelpInfo.align = 1;  //���䷽ʽ����
	menuHelpInfo.space = MENU_SPACE;
	menuHelpInfo.width = btnA_width;
	menuHelpInfo.height = btnA_height;
	menuHelpInfo.fontName = L"����";
	menuHelpInfo.isBold = true;
	menuHelpInfo.normalTextColor = normalClr;
	menuHelpInfo.focusTextColor = focusClr;
	t_menuHelp.SetMenuInfo(menuHelpInfo);
	for (int i = 0; i < 3; i++)
	{
		y = wnd_height - 2 * btnA_width;
		x = i * (btnA_height + MENU_SPACE) + (wnd_width - 3 * btnA_height - 2 * MENU_SPACE) / 2;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuHelpItems[i];
		t_menuHelp.AddMenuItem(mItem);
	}
	t_menuHelp.SetClickSound(&mousedown_buffer);
	t_menuHelp.SetMoveSound(&mousemove_buffer);


	//����ʱ�˵�-------------------------------------------------------------------------------
	wstring menunextItems[] = { L"��һ��",L"�˳�" };
	t_menuNext.SetMenuBkg(L".\\res\\game\\background.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	int btnN_width = 100;   //�˵���ͼƬ��
	int btnN_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::White;   //����״̬������ɫ
	focusClr = Color::Green;  //ѡ��״̬
	t_menuNext.SetBtnBmp(L".\\res\\game\\circle_button.png", btnN_width, btnN_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menunextInfo;
	menunextInfo.align = 1;  //���䷽ʽ����
	menunextInfo.space = 3 * MENU_SPACE;
	menunextInfo.width = btnN_width;
	menunextInfo.height = btnN_height;
	menunextInfo.fontName = L"����";
	menunextInfo.isBold = true;
	menunextInfo.normalTextColor = normalClr;
	menunextInfo.focusTextColor = focusClr;
	t_menuNext.SetMenuInfo(menunextInfo);
	for (int i = 0; i < 2; i++)
	{
		y = wnd_height - 2 * btnN_height;
		x = i * (btnN_width + MENU_SPACE) + (wnd_width - 3 * btnN_width - 2 * MENU_SPACE) / 2;
		MENUITEM mItemN;
		mItemN.pos.x = x;
		mItemN.pos.y = y;
		mItemN.ItemName = menunextItems[i];
		t_menuNext.AddMenuItem(mItemN);

	}
	t_menuNext.SetClickSound(&mousedown_buffer);
	t_menuNext.SetMoveSound(&mousemove_buffer);

	//ʧ��ʱ�˵�-----------------------------------------------------------------------------------------
	wstring menufailItems[] = { L"���¿�ʼ",L"�˳�" };
	t_menuFail.SetMenuBkg(L".\\res\\game\\background.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	int btnF_width = 100;   //�˵���ͼƬ��
	int btnF_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::White;   //����״̬������ɫ
	focusClr = Color::Green;  //ѡ��״̬
	t_menuFail.SetBtnBmp(L".\\res\\game\\circle_button.png", btnF_width, btnF_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menufailInfo;
	menufailInfo.align = 1;  //���䷽ʽ����
	menufailInfo.space = 3 * MENU_SPACE;
	menufailInfo.width = btnF_width;
	menufailInfo.height = btnF_height;
	menufailInfo.fontName = L"����";
	menufailInfo.isBold = true;
	menufailInfo.normalTextColor = normalClr;
	menufailInfo.focusTextColor = focusClr;
	t_menuFail.SetMenuInfo(menufailInfo);
	for (int i = 0; i < 2; i++)
	{
		y = wnd_height - 2 * btnF_height;
		x = (i * (btnF_width + MENU_SPACE) + (wnd_width - 3 * btnF_width - 2 * MENU_SPACE) / 2) + 50;
		MENUITEM mItemF;
		mItemF.pos.x = x;
		mItemF.pos.y = y;
		mItemF.ItemName = menufailItems[i];
		t_menuFail.AddMenuItem(mItemF);

	}
	t_menuFail.SetClickSound(&mousedown_buffer);
	t_menuFail.SetMoveSound(&mousemove_buffer);

	//����ʱ�˵�-------------------------------------------------------------------------------
	wstring menuwinItems[] = { L"��һ��",L"�˳�" };
	t_menuWin.SetMenuBkg(L".\\res\\game\\background.png"); //���ò˵�����ͼƬ
	//�����ΰ�ť�˵���
	int btnW_width = 100;   //�˵���ͼƬ��
	int btnW_height = 100;   //�˵���ͼƬ�� һ��
	normalClr = Color::White;   //����״̬������ɫ
	focusClr = Color::Green;  //ѡ��״̬
	t_menuWin.SetBtnBmp(L".\\res\\game\\circle_button.png", btnW_width, btnW_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuwinInfo;
	menuwinInfo.align = 1;  //���䷽ʽ����
	menuwinInfo.space = 3 * MENU_SPACE;
	menuwinInfo.width = btnW_width;
	menuwinInfo.height = btnW_height;
	menuwinInfo.fontName = L"����";
	menuwinInfo.isBold = true;
	menuwinInfo.normalTextColor = normalClr;
	menuwinInfo.focusTextColor = focusClr;
	t_menuWin.SetMenuInfo(menunextInfo);
	for (int i = 0; i < 2; i++)
	{
		y = wnd_height - 2 * btnW_height;
		x = (i * (btnW_width + MENU_SPACE) + (wnd_width - 3 * btnW_width - 2 * MENU_SPACE) / 2)+50;
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuwinItems[i];
		t_menuWin.AddMenuItem(mItem);

	}
	t_menuWin.SetClickSound(&mousedown_buffer);
	t_menuWin.SetMoveSound(&mousemove_buffer);

	//����ʱ�Ĳ˵�------------------------------------------------------------------------------------------
	navMenu.LoadImageFile(L".\\res\\game\\area_menu.png");
	wstring menuRunItems[] = { L"����" ,L"�˳�" };
	//�����ΰ�ť�˵���
	int btnR_width = 180;   //�˵���ͼƬ��
	int btnR_height = 60;   //�˵���ͼƬ�� һ��
	normalClr = Color::Red;   //����״̬������ɫ
	focusClr = Color::White;  //ѡ��״̬
	t_menuRun.SetBtnBmp(L".\\res\\game\\rect_button.png", btnR_width, btnR_height); //�˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuRunInfo;
	menuRunInfo.align = 1;  //���䷽ʽ����
	menuRunInfo.space = MENU_SPACE;
	menuRunInfo.width = btnR_width;
	menuRunInfo.height = btnR_height;
	menuRunInfo.fontName = L"����";
	menuRunInfo.isBold = true;
	menuRunInfo.normalTextColor = normalClr;
	menuRunInfo.focusTextColor = focusClr;
	t_menuRun.SetMenuInfo(menuRunInfo);
	for (int i = 0; i < 2; i++)
	{
		x = wndWidth - btnR_width;
		y = i * (btnR_height + 10) + 20;
		MENUITEM mItemR;
		mItemR.pos.x = x;
		mItemR.pos.y = y;
		mItemR.ItemName = menuRunItems[i];
		t_menuRun.AddMenuItem(mItemR);
	}
	t_menuRun.SetClickSound(&mousedown_buffer);
	t_menuRun.SetMoveSound(&mousemove_buffer);

}

void RPGgame::PaintMenu(HDC hdc, int GameState)
{
	if (GameState == GAME_START)
	{
		t_menu.DrawMenu(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 0.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height / 4;
		T_Graph::PaintText(hdc, textRec, L"��Ϸ��ƴ���ҵ", 36, L"����",Color::Black);
	}
	else  if (GameState == GAME_ABOUT)
	{
		t_menuAbout.DrawMenu(hdc);
		int w = 640;
		int h = 320;
		int x = 200;
		int y = (wnd_height) / 2 - 200;
		T_Graph::PaintBlank(hdc, x, y, w, h, Color::Blue, 100);
		RectF titleRec;
		titleRec.X = (REAL)x;
		titleRec.Y = (REAL)y + 20.0f;
		titleRec.Width = (REAL)w;
		titleRec.Height = 50.0f;
		T_Graph::PaintText(hdc, titleRec, L"����", 24, L"����");

		RectF textRec;
		textRec.X = (REAL)x + 100;
		textRec.Y = (REAL)y + 50.0f;
		textRec.Width = (REAL)w;
		textRec.Height = 250.0f;
		T_Graph::PaintText(hdc, textRec, L"����:��Ϸ��ƴ���ҵ\
                                         �༶:ѡ��2��  \
                                         ԭʼ�༶����1901   ��1901\
			                             ѧ��:    8002119011 8002119008\
                                         ������    ���      �����\
                                         ʱ��:2022.1.13", 24, L"����", Color::White, FontStyleBold, StringAlignmentNear);
	}
	else  if (GameState == GAME_HELP)
	{
		t_menuHelp.DrawMenu(hdc);
		int w = 750;
		int h = 320;
		int x = 150;
		int y = 100;
		T_Graph::PaintBlank(hdc, x, y, w, h, Color::Blue, 100);
		RectF titleRec;
		titleRec.X = (REAL)x;
		titleRec.Y = (REAL)y + 20.0f;
		titleRec.Width = (REAL)w;
		titleRec.Height = 50.0f;
		T_Graph::PaintText(hdc, titleRec, L"����", 24, L"����");

		RectF textRec;
		textRec.X = (REAL)x + 100;
		textRec.Y = (REAL)y + 50.0f;
		textRec.Width = (REAL)w;
		textRec.Height = 250.0f;
		T_Graph::PaintText(hdc, textRec, L"����:��Ϸ��ƴ���ҵ\
			                              ", 20, L"����", Color::White, FontStyleBold, StringAlignmentNear);

	}
	else  if (GameState == GAME_RUN)
	{
		navMenu.PaintImage(hdc, wnd_width - 180, 0, 180, 18, 255);
		if (clickState == TRUE)
		{
			t_menuRun.DrawMenu(hdc);
		}
		int FontHeight = 0;//�ֺ�
		Gdiplus::RectF rect;
		wstring Content = L"";
		int i = 0;
		int x = 25;
		int y = 12;
		int fy = 15;
		int iconW = 34;
		int textW = 150;
		int textH = 34;
		FontHeight = 18; // �ֺ�

		npcNumImg->PaintRegion(
			npcNumImg->GetBmpHandle(), hdc, x, y, 0, 0,
			npcNumImg->GetImageWidth(), npcNumImg->GetImageHeight(), 1);

		x = x + iconW;
		wstring EnemyNum = L"����: ";
		EnemyNum.append(T_Util::int_to_wstring(NpcNumber));
		rect.X = (float)x;
		rect.Y = (float)fy;
		rect.Width = (float)textW;
		rect.Height = (float)textH;
		T_Graph::PaintText(hdc, rect, EnemyNum.c_str(), (REAL)FontHeight,
			L"����", Color(255, 255, 255, 0),
			FontStyleBold, StringAlignmentNear);

		x = x + textW;
		lifeImg->PaintRegion(
			lifeImg->GetBmpHandle(), hdc, x, y, 0, 0,
			lifeImg->GetImageWidth(), lifeImg->GetImageHeight(), 1);
		wstring lifeleft = L"����: ";
		lifeleft.append(T_Util::int_to_wstring(LifeCount));
		x = x + iconW;
		rect.X = (float)x;
		rect.Y = (float)fy;
		rect.Width = (float)textW;
		rect.Height = (float)textH;
		T_Graph::PaintText(
			hdc, rect, lifeleft.c_str(), (REAL)FontHeight,
			L"����", Color(255, 255, 255, 0),
			FontStyleBold, StringAlignmentNear);

		x = x + textW;
		bmbImg->PaintRegion(
			bmbImg->GetBmpHandle(), hdc, x, y, 0, 0,
			bmbImg->GetImageWidth(), bmbImg->GetImageHeight(), 1);
		wstring ChargeNum = L"��ҩ: ";
		ChargeNum.append(T_Util::int_to_wstring(ChargeCount));
		x = x + iconW;
		rect.X = (float)x;
		rect.Y = (float)fy;
		rect.Width = (float)textW;
		rect.Height = (float)textH;
		T_Graph::PaintText(
			hdc, rect, ChargeNum.c_str(), (REAL)FontHeight,
			L"����", Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);
	}
	else if (GameState == GAME_UPGRADE)
	{
	t_menuNext.DrawMenu(hdc);
	int w = 750;
	int h = 320;
	int x = 150;
	int y = 100;
	T_Graph::PaintBlank(hdc, x, y, w, h, Color::Blue, 100);
	RectF titleRec;
	titleRec.X = (REAL)x;
	titleRec.Y = (REAL)y + 20.0f;
	titleRec.Width = (REAL)w;
	titleRec.Height = 50.0f;
	T_Graph::PaintText(hdc, titleRec, L"��ϲͨ�أ�", 36, L"����", Color::Yellow);
	}
	else if (GameState == GAME_OVER)
	{
	t_menuFail.DrawMenu(hdc);
	int w = 750;
	int h = 320;
	int x = 150;
	int y = 100;
	T_Graph::PaintBlank(hdc, x, y, w, h, Color::Blue, 100);
	RectF titleRec;
	titleRec.X = (REAL)x;
	titleRec.Y = (REAL)y + 20.0f;
	titleRec.Width = (REAL)w;
	titleRec.Height = 50.0f;
	T_Graph::PaintText(hdc, titleRec, L"ͨ��ʧ�ܣ�", 36, L"����", Color::Yellow);
	}
	else if (GameState == GAME_WIN)
	{
	t_menuWin.DrawMenu(hdc);
	int w = 750;
	int h = 320;
	int x = 150;
	int y = 100;
	T_Graph::PaintBlank(hdc, x, y, w, h, Color::Blue, 100);
	RectF titleRec;
	titleRec.X = (REAL)x;
	titleRec.Y = (REAL)y + 20.0f;
	titleRec.Width = (REAL)w;
	titleRec.Height = 50.0f;
	T_Graph::PaintText(hdc, titleRec, L"ȫ��ͨ�أ�", 36, L"����", Color::Yellow);
	}
}

// ������ϷͼƬ��Դ
void RPGgame::LoadImageRes()
{
	if (npcNumImg == NULL)	npcNumImg = new T_Graph(L".\\res\\sprite\\npc.png");
	if (lifeImg == NULL)		lifeImg = new T_Graph(L".\\res\\sprite\\life.png");
	if (bmbImg == NULL)		bmbImg = new T_Graph(L".\\res\\sprite\\shot.png");
}

//������ҽ�ɫ
void RPGgame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO tank_Info;
	player = new T_Sprite(L".\\res\\sprite\\player.png", 60, 60);

	tank_Info.Active = false;
	tank_Info.Dead = false;
	tank_Info.Dir = DIR_UP;
	tank_Info.Rotation = TRANS_NONE;
	tank_Info.Ratio = 1.0f;
	tank_Info.Level = 0;
	tank_Info.Score = 0;
	tank_Info.Speed = 3;
	tank_Info.Alpha = 220;
	tank_Info.X = (wnd_width ) / 2;
	tank_Info.Y = (wnd_height ) / 2 - player->GetHeight();
	tank_Info.Visible = true;
	player->Initiate(tank_Info);
	player->SetSequence(PERSON_FRAME_UP, 20);
	player->SetLayerTypeID(LAYER_PLY);

	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}

//����npc��ɫ
void RPGgame::LoadNpc(int total,int number)
{
	for (int i = 0; i < total; i++)
	{
		
			GAMELAYER gameLayer;
			SPRITEINFO npc_Info;

			npc_Info.Active = true;
			npc_Info.Dead = false;
			npc_Info.Rotation = TRANS_NONE;
			npc_Info.Ratio = LevelSprites[number].Ratio;
			npc_Info.Speed = LevelSprites[number].Speed;
			npc_Info.Alpha = 230;
			npc_Info.Visible = true;
			npc_Info.Level = LevelSprites[number].level;
			npc_Info.Score = LevelSprites[number].Score;

			int sp_width = LevelSprites[number].frameWidth;
			int sp_height = LevelSprites[number].frameHeight;
			LPCTSTR img_name = LevelSprites[number].imgName;
			//��ȡ�������(NPC��ɫ�ڵ�ͼ���Ľ�����)
			int sdr = rand() % 4;
			int d = rand() % 2;
			switch (sdr)
			{
			case 0://���Ͻ�
				if (d == 0)  npc_Info.Dir = DIR_RIGHT;
				if (d == 1)  npc_Info.Dir = DIR_DOWN;
				npc_Info.X = t_scene->getSceneX();
				npc_Info.Y = t_scene->getSceneY();
				break;
			case 1://���Ͻ�
				if (d == 0)  npc_Info.Dir = DIR_LEFT;
				if (d == 1)  npc_Info.Dir = DIR_DOWN;
				npc_Info.X = t_scene->getSceneX() + scn_width - sp_width;
				npc_Info.Y = t_scene->getSceneY();
				break;
			case 2://���½�
				if (d == 0)  npc_Info.Dir = DIR_RIGHT;
				if (d == 1)  npc_Info.Dir = DIR_UP;
				npc_Info.X = t_scene->getSceneX();
				npc_Info.Y = t_scene->getSceneY() + scn_height - sp_height;
				break;
			case 3://���½�
				if (d == 0)  npc_Info.Dir = DIR_LEFT;
				if (d == 1)  npc_Info.Dir = DIR_UP;
				npc_Info.X = t_scene->getSceneX() + scn_width - sp_width;
				npc_Info.Y = t_scene->getSceneY() + scn_height - sp_height;
				break;
			}
			//��NPC�б��������µ���Ŀ
			wstring path = L".\\res\\sprite\\";
			path.append(img_name);
			npc_set.push_back(new T_Sprite(path.c_str(), sp_width, sp_height));

			//��ʼ�������ӵ���Ŀ
			T_Sprite* sp = npc_set.back();
			sp->Initiate(npc_Info);
			switch (npc_Info.Dir)
			{
			case DIR_LEFT:
				sp->SetSequence(PERSON_FRAME_LEFT, 20);
				break;

			case DIR_RIGHT:
				sp->SetSequence(PERSON_FRAME_RIGHT, 20);
				break;
			case DIR_UP:
				sp->SetSequence(PERSON_FRAME_UP, 20);
				break;
			case DIR_DOWN:
				sp->SetSequence(PERSON_FRAME_DOWN, 20);
				break;
			}
			sp->SetLayerTypeID(LAYER_NPC);

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_NPC;
			gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;		
	}
}

//���ص�ͼ����
void RPGgame::LoadMap()
{
	if (GameLevel <= 2)
	{
		t_scene->LoadTxtMap(".\\res\\map2.txt");
		scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
		scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();
		//��ͼ��ʼ��λ���Ե�ͼ��Ϊ����
		int scn_x = (wnd_width - scn_width) / 2;
		int scn_y = (wnd_height - scn_height) / 2;
		//����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ�á�
		t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
		//�����е�ͼͼ�㶨λ����Ļ����
		SCENE_LAYERS::iterator p;
		for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
		{
			if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
		}
	}
	if (GameLevel < 4&& GameLevel > 2)
	{
		t_scene->LoadTxtMap(".\\res\\bigmap.txt");
		scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
		scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();
		//��ͼ��ʼ��λ���Ե�ͼ��Ϊ����
		int scn_x = (wnd_width - scn_width) / 2;
		int scn_y = (wnd_height - scn_height) / 2;
		//����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ�á�
		t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
		//�����е�ͼͼ�㶨λ����Ļ����
		SCENE_LAYERS::iterator p;
		for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
		{
			if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
		}
	}
	if ( GameLevel == 4)
	{
		t_scene->LoadTxtMap(".\\res\\map3.txt");
		scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
		scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();
		//��ͼ��ʼ��λ���Ե�ͼ��Ϊ����
		int scn_x = (wnd_width - scn_width) / 2;
		int scn_y = (wnd_height - scn_height) / 2;
		//����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ�á�
		t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
		//�����е�ͼͼ�㶨λ����Ļ����
		SCENE_LAYERS::iterator p;
		for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
		{
			if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
		}
	}
	
}

//���ص���
void RPGgame::LoadProp()
{
	long iCount = GetTickCount();
	if (iCount - PropLoadTime > PROP_TIME)
	{
		PropLoadTime = iCount;

		GAMELAYER gameLayer;
		SPRITEINFO propInfo;

		propInfo.Active = true;
		propInfo.Dead = false;
		propInfo.Visible = true;
		propInfo.Rotation = TRANS_NONE;
		propInfo.Ratio = 2.0f;
		propInfo.Speed = 0;
		propInfo.Alpha = 255;
		propInfo.Level = 0;
		propInfo.Score = 0;
		propInfo.Dir = 0;

		// �ڵ�ͼ�ϲ��ҷ��ϰ����λ��
		POINT bPT = t_scene->getRandomFreeCell();
		propInfo.X = bPT.x;
		propInfo.Y = bPT.y;

		int type = rand() % 3;

		if (type == 0)
		{
			//��NPC�б��������µ���Ŀ
			arrow_set.push_back(new T_Sprite(L".\\res\\sprite\\shootthing.png", 34, 34));

			//��ʼ�������ӵ���Ŀ
			T_Sprite* sp = arrow_set.back();
			sp->Initiate(propInfo);
			sp->SetSequence(PROP_FRAME, 9);
			sp->SetLayerTypeID(LAYER_BONUS);
			sp->SetStartTime(GetTickCount());

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_BONUS;
			gameLayer.z_order = t_scene->getBarrier()->getZorder() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;
		}

		if (type == 1)
		{
			//��NPC�б��������µ���Ŀ
			life_set.push_back(new T_Sprite(L".\\res\\sprite\\blood.png", 34, 34));

			//��ʼ�������ӵ���Ŀ
			T_Sprite* sp = life_set.back();
			sp->Initiate(propInfo);
			sp->SetSequence(PROP_FRAME, 9);
			sp->SetLayerTypeID(LAYER_BONUS);
			sp->SetStartTime(GetTickCount());

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_BONUS;
			gameLayer.z_order = t_scene->getBarrier()->getZorder() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;
		}

		if (type == 2)
		{
			//��NPC�б��������µ���Ŀ
			speed_set.push_back(new T_Sprite(L".\\res\\sprite\\boost.png", 34, 34));

			//��ʼ�������ӵ���Ŀ
			T_Sprite* sp = speed_set.back();
			sp->Initiate(propInfo);
			sp->SetSequence(PROP_FRAME, 9);
			sp->SetLayerTypeID(LAYER_BONUS);
			sp->SetStartTime(GetTickCount());

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_BONUS;
			gameLayer.z_order = t_scene->getBarrier()->getZorder() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;
		}
	}
}

//������Ϸ��ը
void RPGgame::LoadExplosion(int x, int y)
{
	SPRITEINFO explInfo;
	GAMELAYER gameLayer;

	explosion_set.push_back(new T_Sprite(L".\\res\\sprite\\dead1.png", 58, 58));
	// ��ʼ�������ӵ���Ŀ
	T_Sprite* sp_explosion = explosion_set.back();

	explInfo.Active = true;
	explInfo.Dead = false;
	explInfo.Dir = 0;
	explInfo.Rotation = TRANS_NONE;
	explInfo.Ratio = 1;
	explInfo.Level = 0;
	explInfo.Score = 0;
	explInfo.Speed = 5;
	explInfo.Alpha = 255;
	explInfo.X = x;
	explInfo.Y = y;
	explInfo.Visible = true;

	sp_explosion->Initiate(explInfo);
	sp_explosion->SetSequence(EXPLOSION_FRAME, 8);
	sp_explosion->SetLayerTypeID(LAYER_EXPLOSION);

	gameLayer.layer = sp_explosion;
	gameLayer.type_id = LAYER_EXPLOSION;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}
//���ؼ�����Ч��
void RPGgame::LoadxDead(int x, int y)
{
	SPRITEINFO xInfo;
	GAMELAYER gameLayer;

	xdead_set.push_back(new T_Sprite(L".\\res\\sprite\\xdamage.png", 66, 66));
	// ��ʼ�������ӵ���Ŀ
	T_Sprite* sp_explosion = xdead_set.back();

	xInfo.Active = true;
	xInfo.Dead = false;
	xInfo.Dir = 0;
	xInfo.Rotation = TRANS_NONE;
	xInfo.Ratio = 1;
	xInfo.Level = 0;
	xInfo.Score = 0;
	xInfo.Speed = 5;
	xInfo.Alpha = 255;
	xInfo.X = x;
	xInfo.Y = y;
	xInfo.Visible = true;

	sp_explosion->Initiate(xInfo);
	sp_explosion->SetSequence(EXPLOSION_FRAME, 8);
	sp_explosion->SetLayerTypeID(LAYER_EXPLOSION);

	gameLayer.layer = sp_explosion;
	gameLayer.type_id = LAYER_EXPLOSION;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}
//�����������Ч��
void RPGgame::LoadpDead(int x, int y)
{
	SPRITEINFO pInfo;
	GAMELAYER gameLayer;

	pdead_set.push_back(new T_Sprite(L".\\res\\sprite\\demon.png", 66, 66));
	// ��ʼ�������ӵ���Ŀ
	T_Sprite* sp_explosion = pdead_set.back();

	pInfo.Active = true;
	pInfo.Dead = false;
	pInfo.Dir = 0;
	pInfo.Rotation = TRANS_NONE;
	pInfo.Ratio = 1;
	pInfo.Level = 0;
	pInfo.Score = 0;
	pInfo.Speed = 5;
	pInfo.Alpha = 255;
	pInfo.X = x;
	pInfo.Y = y;
	pInfo.Visible = true;

	sp_explosion->Initiate(pInfo);
	sp_explosion->SetSequence(EXPLOSION_FRAME, 8);
	sp_explosion->SetLayerTypeID(LAYER_EXPLOSION);

	gameLayer.layer = sp_explosion;
	gameLayer.type_id = LAYER_EXPLOSION;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}


//���������(sp:�����ɫ,arrowSet��������,iTime:�����ʱ��)
void RPGgame::LoadArrow(T_Sprite* sp, vSpriteSet& arrowSet, int time)
{
	sp->SetEndTime(GetTickCount());

	if (sp->GetEndTime() - sp->GetStartTime() >= (DWORD)time)
	{
		shoot_buffer.Play();
		ChargeCount = ChargeCount - 1;
		sp->SetStartTime(sp->GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO bombInfo;
		int m_dir = sp->GetDir();

		wchar_t path[256];
		wsprintf(path, L".\\res\\sprite\\arrow%d.png", m_dir);
		arrowSet.push_back(new T_Sprite(path));

		//��ʼ�������ӵ���Ŀ
		T_Sprite* bomb = arrowSet.back();
		bombInfo.Active = true;
		bombInfo.Dead = false;
		bombInfo.Dir = m_dir;
		bombInfo.Rotation = TRANS_NONE;
		bombInfo.Ratio = 1;
		bombInfo.Level = 0;
		bombInfo.Score = 0;
		bombInfo.Speed = 15;
		bombInfo.Alpha = 255;

		switch (m_dir)
		{
		case DIR_LEFT:
			bombInfo.X = sp->GetX() - 16;
			bombInfo.Y = sp->GetY() + sp->GetRatioSize().cy / 2 - 12;
			break;
		case DIR_RIGHT:
			bombInfo.X = sp->GetX() + sp->GetRatioSize().cx;
			bombInfo.Y = sp->GetY() + sp->GetRatioSize().cy / 2 - 12;
			break;
		case DIR_UP:
			bombInfo.X = sp->GetX() + sp->GetRatioSize().cx / 2 - 2;
			bombInfo.Y = sp->GetY() - 16;
			break;
		case DIR_DOWN:
			bombInfo.X = sp->GetX() + sp->GetRatioSize().cx / 2 - 2;
			bombInfo.Y = sp->GetY() + sp->GetRatioSize().cy - 25;
			break;
		}

		bombInfo.Visible = true;
		bomb->Initiate(bombInfo);

		gameLayer.layer = bomb;
		if (arrowSet == player_arrow_set)
		{
			gameLayer.type_id = LAYER_PLY_BOMB;
			gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB);
		}
		gameLayer.z_order = t_scene->GetTotalLayers() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
	}
}
void RPGgame::LoadFire(T_Sprite* sp, vSpriteSet& fireSet, int time)
{
	sp->SetEndTime(GetTickCount());
	if (sp->GetEndTime() - sp->GetStartTime() >= (DWORD)time)
	{
		sp->SetStartTime(sp->GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO fireInfo;
		int m_dir = sp->GetDir();

		wchar_t path[256];
		wsprintf(path, L".\\res\\sprite\\fireball%d.png", m_dir);
		fireSet.push_back(new T_Sprite(path));

		//��ʼ�������ӵ���Ŀ
		T_Sprite* fire = fireSet.back();
		fireInfo.Active = true;
		fireInfo.Dead = false;
		fireInfo.Dir = m_dir;
		fireInfo.Rotation = TRANS_NONE;
		fireInfo.Ratio = 2;
		fireInfo.Level = 0;
		fireInfo.Score = 0;
		fireInfo.Speed = 13;
		fireInfo.Alpha = 255;

		switch (m_dir)
		{
		case DIR_LEFT:
			fireInfo.X = sp->GetX() - 16;
			fireInfo.Y = sp->GetY() + sp->GetRatioSize().cy / 2 - 12;
			break;
		case DIR_RIGHT:
			fireInfo.X = sp->GetX() + sp->GetRatioSize().cx;
			fireInfo.Y = sp->GetY() + sp->GetRatioSize().cy / 2 - 12;
			break;
		case DIR_UP:
			fireInfo.X = sp->GetX() + sp->GetRatioSize().cx / 2 - 16;
			fireInfo.Y = sp->GetY() - 16;
			break;
		case DIR_DOWN:
			fireInfo.X = sp->GetX() + sp->GetRatioSize().cx / 2 - 16;
			fireInfo.Y = sp->GetY() + sp->GetRatioSize().cy - 25;
			break;
		}

		fireInfo.Visible = true;
		fire->Initiate(fireInfo);

		gameLayer.layer = fire;
		if (fireSet == npc_fire_set)
		{
			gameLayer.type_id = LAYER_NPC_BOMB;
			gameLayer.layer->SetLayerTypeID(LAYER_NPC_BOMB);
		}
		gameLayer.z_order = t_scene->GetTotalLayers() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
	}
}


//�������λ��
void RPGgame::UpdatePlayerPos()
{
	if (player == NULL) return;
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;

	if (player->IsDead() == false && player->IsVisible() == true && player->IsActive() == true)
	{
		switch (player->GetDir())
		{
		case DIR_LEFT:
			SpeedX = -player->GetSpeed();
			SpeedY = 0;
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepX = player->GetX() - player->GetSpeed();
			if (nextStepX <= 0) SpeedX = 0 - player->GetX();
			break;
		case DIR_RIGHT:
			SpeedX = player->GetSpeed();
			SpeedY = 0;
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepX = player->GetX() + player->GetRatioSize().cx + player->GetSpeed();
			if (nextStepX >= wnd_width)
				SpeedX = wnd_width - player->GetRatioSize().cx - player->GetX();
			break;
		case DIR_UP:
			SpeedX = 0;
			SpeedY = -player->GetSpeed();
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepY = player->GetY() - player->GetSpeed();
			if (nextStepY <= 0) SpeedY = 0 - player->GetY();
			break;
		case DIR_DOWN:
			SpeedX = 0;
			SpeedY = player->GetSpeed();
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepY = player->GetY() + player->GetRatioSize().cy + player->GetSpeed();
			if (nextStepY >= wnd_height)
				SpeedY = wnd_height - player->GetRatioSize().cy - player->GetY();
			break;

		}
		int x = player->GetX();    //����ƶ�ǰ��xλ��
		int y = player->GetY();    //����ƶ�ǰ��yλ��
		if (!player->CollideWith(t_scene->getBarrier())) player->Move(SpeedX, SpeedY);
		if (player->CollideWith(t_scene->getBarrier()))   player->SetPosition(x, y);  //��ԭ�ƶ�ǰ��λ��

		//��npc��ײ
		vSpriteSet::iterator p;
		for (p = npc_set.begin(); p != npc_set.end(); p++) 
		{
			if(player->CollideWith((*p)) && !(*p)->IsDead() && (*p)->IsVisible())
			{
				if ((*p)->GetScore() > 10 &&(*p)->GetScore() <= 15)
				{
					blood = 1;
				}
				if ((*p)->GetScore() > 15)
				{
					blood = 2;
				}
				LoadpDead(player->GetX(), player->GetY());
				if(isDelayCollision == false)
				{
					updateLifeCount = true;
					return;  //�˴����뷵��
				}
			}
		}
		//��Ϸ������������
		if (NpcNumber == 0)
		{
			GameState = GAME_UPGRADE;
			upgradeGameLevel = true;
		}
	}
	t_scene->ScrollScene(player);
}

//�����������ֵ
void RPGgame::UpdatePlayerLife()
{

	LifeCount = LifeCount - blood;
	player->SetStartTime(GetTickCount());
	isDelayCollision = true;
	updateLifeCount = false;
	if (LifeCount <= 0)
	{
		player->SetActive(false);
		player->SetVisible(false);
		player->SetDead(true);
		GameState = GAME_OVER;
		ClearGameLevel();
		GameLevel = 1;
		LoadGameLevel(GameLevel);		
	}
}

//����NPCλ��
void RPGgame::UpdateNpcPos()
{
	if (npc_set.size() == 0) return;

	vSpriteSet::iterator p;
	T_AI* spAi = new T_AI(4);
	for (p = npc_set.begin(); p != npc_set.end(); p++)
	{
		if ((*p)->IsActive() == true && (*p)->IsVisible() == true)
		{
			int SpeedX = 0, SpeedY = 0;
			int npc_dir = 0;
			if ((*p)->GetScore() <= 20)
			{
				spAi->Evade((*p), player);
			}
			spAi->CheckOverlay((*p), npc_set);

			npc_dir = (*p)->GetDir();
			switch (npc_dir)
			{
			case DIR_LEFT:
				(*p)->SetSequence(PERSON_FRAME_LEFT, 20);
				break;

			case DIR_RIGHT:
				(*p)->SetSequence(PERSON_FRAME_RIGHT, 20);
				break;

			case DIR_UP:
				(*p)->SetSequence(PERSON_FRAME_UP, 20);
				break;

			case DIR_DOWN:
				(*p)->SetSequence(PERSON_FRAME_DOWN, 20);
				break;
			}
			spAi->Wander((*p), t_scene->getBarrier());
			//�����ڵ�
			if ((*p)->GetScore() >= 30)
			{
				LoadFire((*p), npc_fire_set, 2000);
			}
		}
	}
	delete spAi;
}

void RPGgame::UpdateProp(vSpriteSet& propSet)
{
	if (propSet.size() == 0) return;

	long tm = GetTickCount();
	vSpriteSet::iterator itp;
	if (propSet.size() > 0)
	{
		itp = propSet.begin();
		for (; itp != propSet.end();)
		{
			if ((tm - (*itp)->GetStartTime()) > (PROP_TIME - 2000))
			{
				(*itp)->LoopFrame(); // ������ʧ���涯��
			}

			bool isPicked = player->CollideWith(*itp);
			if (isPicked || ((tm - (*itp)->GetStartTime()) > PROP_TIME))
			{
				if (isPicked)
				{
					if (propSet == arrow_set)
					{
						ChargeCount = ChargeCount + MORE_CHARGE;
					}
					if (propSet == life_set)
					{
						LifeCount = LifeCount + 1;
					}
					if (propSet == speed_set)
					{
						speedup = 20;
					}
				}
				//ɾ�������еĶ���	
				SCENE_LAYERS::iterator p;
				for (p = t_scene->getSceneLayers()->begin();
					p != t_scene->getSceneLayers()->end(); p++)
				{
					if ((*p).layer == (*itp))
					{
						p = t_scene->getSceneLayers()->erase(p);
						break;
					}
				}
				delete (*itp);
				itp = propSet.erase(itp);
			}

			if (itp == propSet.end())
			{
				break;
			}
			itp++;
		}
	}
}
//���½�ɫ֡����
void RPGgame::UpdateAnimation()
{
	vSpriteSet::iterator p;
	if (npc_set.size() > 0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); p++)
		{
			(*p)->LoopFrame();
		}
	}
	if (player != NULL)
	{
		if (player->IsVisible() == true && player->IsActive() == true)
		{
			player->LoopFrame();
		}
	}
	if (explosion_set.size() > 0)
	{
		vSpriteSet::iterator e = explosion_set.begin();
		for (; e != explosion_set.end();)
		{
			if ((*e)->LoopFrameOnce() == true)
			{
				// ɾ���������Ѿ�ʧЧ�ı�ը����	
				SCENE_LAYERS::iterator lp;
				for (lp = t_scene->getSceneLayers()->begin(); lp != t_scene->getSceneLayers()->end(); lp++)
				{
					if ((*lp).layer == (*e))
					{
						lp = t_scene->getSceneLayers()->erase(lp);
						break;
					}
				}
				delete (*e);
				e = explosion_set.erase(e);
				continue;
			}
			else
			{
				++e;
			}
		}
	}
	if (xdead_set.size() > 0)
	{
		vSpriteSet::iterator e = xdead_set.begin();
		for (; e != xdead_set.end();)
		{
			if ((*e)->LoopFrameOnce() == true)
			{
				// ɾ���������Ѿ�ʧЧ�ı�ը����	
				SCENE_LAYERS::iterator lp;
				for (lp = t_scene->getSceneLayers()->begin(); lp != t_scene->getSceneLayers()->end(); lp++)
				{
					if ((*lp).layer == (*e))
					{
						lp = t_scene->getSceneLayers()->erase(lp);
						break;
					}
				}
				delete (*e);
				e = xdead_set.erase(e);
				continue;
			}
			else
			{
				++e;
			}
		}
	}
	if (pdead_set.size() > 0)
	{
		vSpriteSet::iterator e = pdead_set.begin();
		for (; e != pdead_set.end();)
		{
			if ((*e)->LoopFrameOnce() == true)
			{
				// ɾ���������Ѿ�ʧЧ�ı�ը����	
				SCENE_LAYERS::iterator lp;
				for (lp = t_scene->getSceneLayers()->begin(); lp != t_scene->getSceneLayers()->end(); lp++)
				{
					if ((*lp).layer == (*e))
					{
						lp = t_scene->getSceneLayers()->erase(lp);
						break;
					}
				}
				delete (*e);
				e = pdead_set.erase(e);
				continue;
			}
			else
			{
				++e;
			}
		}
	}
}

//���¼�λ��
void RPGgame::UpdateArrowPos(vSpriteSet* arrowSet)
{
	if (arrowSet->size() == 0) return;

	vSpriteSet::iterator itp = arrowSet->begin();
	for (; itp != arrowSet->end();)
	{
		if ((*itp)->IsVisible() == true && (*itp)->IsActive() == true)
		{
			int SpeedX = 0, SpeedY = 0;
			switch ((*itp)->GetDir())
			{
			case DIR_LEFT:
				SpeedX = -(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_RIGHT:
				SpeedX = (*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_UP:
				SpeedX = 0;
				SpeedY = -(*itp)->GetSpeed();
				break;

			case DIR_DOWN:
				SpeedX = 0;
				SpeedY = (*itp)->GetSpeed();
				break;
			}
			(*itp)->Move(SpeedX, SpeedY);

			//����ڵ��Ƿ����Ŀ��
			Collide((*itp));
			if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width)
		{
			//ɾ�������еĶ���	
			SCENE_LAYERS::iterator p;
			for (p = t_scene->getSceneLayers()->begin();
				p != t_scene->getSceneLayers()->end(); p++)
			{
				if ((*p).layer == (*itp))
				{
					p = t_scene->getSceneLayers()->erase(p);
					break;
				}
			}
			delete (*itp);
			itp = arrowSet->erase(itp);
		}

		if (itp == arrowSet->end())
		{
			break;
		}

		itp++;
	}
}
//���»���λ��
void RPGgame::UpdateFirePos(vSpriteSet* fireSet)
{
	if (fireSet->size() == 0) return;

	vSpriteSet::iterator itp = fireSet->begin();
	for (; itp != fireSet->end();)
	{
		if ((*itp)->IsVisible() == true && (*itp)->IsActive() == true)
		{
			int SpeedX = 0, SpeedY = 0;
			switch ((*itp)->GetDir())
			{
			case DIR_LEFT:
				SpeedX = -(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_RIGHT:
				SpeedX = (*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_UP:
				SpeedX = 0;
				SpeedY = -(*itp)->GetSpeed();
				break;

			case DIR_DOWN:
				SpeedX = 0;
				SpeedY = (*itp)->GetSpeed();
				break;
			}
			(*itp)->Move(SpeedX, SpeedY);

			//����ڵ��Ƿ����Ŀ��
			Collide((*itp));
			if (GameState != GAME_RUN) return;
		}

		if ((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width)
		{
			//ɾ�������еĶ���	
			SCENE_LAYERS::iterator p;
			for (p = t_scene->getSceneLayers()->begin();
				p != t_scene->getSceneLayers()->end(); p++)
			{
				if ((*p).layer == (*itp))
				{
					p = t_scene->getSceneLayers()->erase(p);
					break;
				}
			}
			delete (*itp);
			itp = fireSet->erase(itp);
		}

		if (itp == fireSet->end())
		{
			break;
		}

		itp++;
	}
}

//���عؿ�����
void RPGgame::LoadGameLevel(int level)
{
	isDelayCollision = true;
	upgradeGameLevel = false;
	updateLifeCount = false;
	PropLoadTime = GetTickCount();
	ChargeCount = PLAYER_CHARGE;
	LifeCount = PLAYER_LIFE;
	NpcNumber = NPC_NUM*(GameLevel+1);
	npcNumImg = NULL;
	lifeImg = NULL;
	bmbImg = NULL;
	t_scene = NULL;
	speedup = 10;

	if (t_scene == NULL) t_scene = new T_Scene();
	LoadMenu();
	LoadPlayer();
	LoadMap();
	LoadImageRes();
	for (int i = 0; i <= (GameLevel); i++)
	{
		LoadNpc(NPC_NUM, i);
	}
}
// ����ؿ�����
void RPGgame::ClearGameLevel()
{
	delete npcNumImg;		npcNumImg = NULL;
	delete lifeImg;			lifeImg = NULL;
	delete bmbImg;			bmbImg = NULL;
	delete t_scene;			t_scene = NULL;

	npc_set.clear();			npc_set.swap(vector<T_Sprite*>());
	player_arrow_set.clear();	player_arrow_set.swap(vector<T_Sprite*>());
	npc_fire_set.clear();		npc_fire_set.swap(vector<T_Sprite*>());
	explosion_set.clear();		explosion_set.swap(vector<T_Sprite*>());
	xdead_set.clear();		    xdead_set.swap(vector<T_Sprite*>());
	pdead_set.clear();		    pdead_set.swap(vector<T_Sprite*>());
	life_set.clear();		    life_set.swap(vector<T_Sprite*>());
	arrow_set.clear();		    arrow_set.swap(vector<T_Sprite*>());
	speed_set.clear();		    speed_set.swap(vector<T_Sprite*>());
}

// �����������
bool RPGgame::UpgradePlayerLevel()
{
	bool breakThrough = false;

	if (upgradeGameLevel)
	{
		if (GameLevel < TOTAL_LEVEL )
		{
			player->SetLevel(player->GetLevel() + 1);
			breakThrough = false;
			GameState = GAME_UPGRADE;
			GameLevel = GameLevel + 1; 
			ClearGameLevel();
			LoadGameLevel(GameLevel);
		}

		if (GameLevel >= TOTAL_LEVEL)
		{
			GameLevel = 1;
			upgradeGameLevel = false;
			breakThrough = true;
			GameState = GAME_WIN;
			ClearGameLevel();
			LoadGameLevel(GameLevel);
		}
	}

	return breakThrough;
}

void RPGgame::Collide(T_Sprite* arrow)
{
	if (arrow->IsActive() == true && arrow->IsVisible() == true)
	{
		// ��������˵�ͼ�ϰ�

		bool collideBarrier = arrow->CollideWith(t_scene->getBarrier());
		if (collideBarrier)
		{
			if (collideBarrier) map = t_scene->getBarrier();
			map->setTile(arrow->GetMapBlockPT().x, arrow->GetMapBlockPT().y, 0);
			arrow->SetVisible(false);
			arrow->SetActive(false);
			arrow->SetVisible(false);
			arrow->SetActive(false);
			int x = arrow->GetX() - 33;
			int y = arrow->GetY() - 33;
			LoadExplosion(x, y);
		}
		//����ڵ�����npc
		if (arrow->GetLayerTypeID() == LAYER_PLY_BOMB)
		{
			for (vSpriteSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++)
			{
				if (arrow->CollideWith((*sp)) && !((*sp)->IsDead()) &&
					((*sp)->IsVisible()) && ((*sp)->IsActive()))
				{
					if ((*sp)->GetLevel() == 1)
					{
					(*sp)->SetActive(false);
					(*sp)->SetVisible(false);
					(*sp)->SetDead(true);
					arrow->SetActive(false);
					arrow->SetVisible(false);
					//NPC��Ŀ����
						NpcNumber = NpcNumber -1;
						demaged_buffer.Play();
					}
					if ((*sp)->GetLevel() >= 2) {
						(*sp)->SetLevel(1);
						arrow->SetActive(false);
						arrow->SetVisible(false);
					}
					LoadxDead((*sp)->GetX(), (*sp)->GetY());
					//��Ϸ������������
					if (NpcNumber == 0)
					{
						GameState = GAME_UPGRADE;
						upgradeGameLevel = true;
					}
					break;
				}
			}
		}
		// ���NPC�ڵ����������
		if (arrow->GetLayerTypeID() == LAYER_NPC_BOMB &&
			arrow->CollideWith(player) &&
			player->IsDead() == false && player->IsVisible() == true)
		{
			blood = 1;
			arrow->SetVisible(false);
			arrow->SetActive(false);

			int x = arrow->GetX() - player->GetWidth() / 2;
			int y = arrow->GetY() - player->GetHeight() / 2;
			LoadpDead(x, y);

			if (isDelayCollision == false)
			{
				updateLifeCount = true;
			}
		}
	}
}

