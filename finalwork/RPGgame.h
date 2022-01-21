#pragma once
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Map.h"
#include ".\\TinyEngine\\T_Sprite.h"
#include ".\\TinyEngine\\T_AI.h"
#include ".\\TinyEngine\\T_Audio.h"
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_Menu.h"
#include ".\\TinyEngine\\T_Graph.h"
#include "myMenu.h"

//����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

//���屣�������Դ��Ϣ�Ľṹ��
typedef struct
{
	LPCTSTR imgName;//ͼ������
	int		frameWidth;		// ֡��
	int		frameHeight;	// ֡��
	int		Speed;			// ��ɫ��ʼ���ƶ��ٶ�
	int		Score;			// ��ɫ��ɫ��ֵ
	float	Ratio;			// ֡ͼƬ�Ŵ����С��
	int     level;          //�ȼ�
}LEVELINFO;

class RPGgame:public T_Engine
{
private:
	//�˵��滭
	T_Menu t_menu;
	T_Menu t_menuAbout;
	T_Menu t_menuHelp;
	T_Menu t_menuFail;
	T_Menu t_menuNext;
	T_Menu t_menuWin;
	myMenu t_menuRun; // ��Ϸ����ʱ�Ĳ˵�
	T_Graph navMenu;
	void PaintMenu(HDC hdc, int GameState);//��˵�


	//����֡��������
	static int PERSON_FRAME_LEFT[20];//��������֡����
	static int PERSON_FRAME_RIGHT[20];//�����ҷ���֡����
	static int PERSON_FRAME_UP[20];//���ﳯ��֡����
	static int PERSON_FRAME_DOWN[20];//���ﳯ��֡����

	static int FRAME_CLOSE_ATTACK_LEFT[20];//��������֡����
	static int FRAME_CLOSE_ATTACK_RIGHT[20];//�����ҷ���֡����
	static int FRAME_CLOSE_ATTACK_UP[20];//���ﳯ��֡����
	static int FRAME_CLOSE_ATTACK_DOWN[20];//���ﳯ��֡����

	static int FRAME_CLOSE_SHOOT_LEFT[20];//��������Զ��
	static int FRAME_CLOSE_SHOOT_RIGHT[20];//�����ҷ���Զ��
	static int FRAME_CLOSE_SHOOT_UP[20];//���ﳯ��Զ��
	static int FRAME_CLOSE_SHOOT_DOWN[20];//���ﳯ��Զ��

	static int EXPLOSION_FRAME[8];//��ը֡����

	static int PROP_FRAME[9];//����֡����

	//��Ϸ�е�����
	static const int TOTAL_LEVEL = 5;			// ��Ϸ�ܹ���
	static LEVELINFO LevelSprites[TOTAL_LEVEL + 1];//ÿ�ؽ�ɫ��Ϣ

	static const int NPC_NUM = 2;//npc����

	static const int SUPER_TIME = 3000;//����޵�ʱ��
	static const int PLAYER_LIFE = 5;//��ҳ�ʼ����
	static const int PLAYER_CHARGE = 30;//��ҹ�����

	static const int MORE_CHARGE = 10;//����������
	static const int PROP_TIME = 15000;//��������ʱ��


	// ������ر���
	int scn_width, scn_height;//��ͼ�������
	int wnd_width, wnd_height;//��Ϸ���ڿ��

	//��Ϸ�߼����Ʊ���
	int speedup;//�ٶ�
	int blood;//ʧѪ��
	long ChargeCount;//����ڵ�������
	int LifeCount;//�������������
	bool updateLifeCount;//�Ƿ�������������

	int GameLevel;// ��Ϸ����������

	bool isDelayCollision;//�Ƿ���ʱ��ײ
	bool upgradeGameLevel;// �Ƿ��Ѿ�����

	long PropLoadTime;//���߼��ؼ�ʱ

	int NpcNumber;//npc������������

	//��������Դ��ر���
	AudioDX	ds;			// DirectSound����
	AudioDXBuffer backmusic_buffer;  //��������
	AudioDXBuffer robot_buffer;   // ��������
	AudioDXBuffer mousedown_buffer; //�������Ч
	AudioDXBuffer mousemove_buffer;  //�������Ч
	AudioDXBuffer shoot_buffer;	     //�����Ч
	AudioDXBuffer walk_buffer;		 //��·��Ч
	AudioDXBuffer demaged_buffer;	 //��·��Ч
	AudioDXBuffer explosionSound;    //��ը��Ч

	//��ͼƬ��Դ��ر���
	T_Map* map;
	T_Graph* lifeImg;// ״̬���������СͼƬ
	T_Graph* bmbImg;// ״̬���ڵ���СͼƬ
	T_Graph* npcNumImg;// ״̬���з�СͼƬ

	//����Ϸ��ɫ����ر���
	T_Sprite* player;//��Ϸ���
	T_Scene* t_scene;//��Ϸ��ͼ����

	//����Ϸ��ɫ��ؼ���
	vSpriteSet npc_set;//npc����
	vSpriteSet explosion_set;//��ըЧ������
	vSpriteSet xdead_set;//������Ч������
	vSpriteSet pdead_set;//������˼���

	vSpriteSet speed_set;//���ٵ��߼���
	vSpriteSet arrow_set;//����ҩ����
	vSpriteSet life_set;//�������߼���

	vSpriteSet player_arrow_set;//��Ҽ�����

	vSpriteSet npc_fire_set;//������򼯺�

private:
	//����Դ������ع��ܺ���
	void LoadPlayer();//������ҽ�ɫ
	void LoadNpc(int total,int number);//����npc
	void LoadMap();//������Ϸ��ͼ
	void LoadImageRes();//������ϷͼƬ��Դ
	void LoadMenu();
	void LoadProp();//���ص���
	void LoadExplosion(int x, int y);//���ر�ըЧ��
	void LoadxDead(int x, int y);//���ؼ�����Ч��
	void LoadpDead(int x, int y);//�����������Ч��
	void LoadFire(//������
		T_Sprite* sp,
		vSpriteSet& fireSet,
		int time = 0);
	void LoadArrow(//������
		T_Sprite* sp,
		vSpriteSet& arrowSet,
		int time = 0);

	//��Ϸ״̬������ع��ܺ���
	void UpdatePlayerPos();//�������λ��
	void UpdatePlayerLife();//�����������

	void UpdateNpcPos();//����npcλ��

	void UpdateFirePos(vSpriteSet* fireSet);//���»���λ��
	void UpdateArrowPos(vSpriteSet* arrowSet);//���¼�λ��

	void UpdateProp(vSpriteSet& propSet);//���µ���״̬

	void UpdateAnimation();//���½�ɫ֡�������к�

	//��Ϸ�ؿ�������
	void LoadGameLevel(int level);//���عؿ�����
	void ClearGameLevel();//����ؿ�����
	bool UpgradePlayerLevel();// �ؿ���������

	//��Ϸϸ�ڴ���
	void Collide(T_Sprite* arrow);//��ײ

public:
	//�๹�캯������������
	RPGgame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 1024, int winheight = 768);
	virtual ~RPGgame(void);

	// ����T_Engine���е��麯��ʵ����Ϸ����
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};

