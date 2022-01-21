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

//定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

//定义保存怪物资源信息的结构体
typedef struct
{
	LPCTSTR imgName;//图像名称
	int		frameWidth;		// 帧宽
	int		frameHeight;	// 帧高
	int		Speed;			// 角色初始的移动速度
	int		Score;			// 角色角色分值
	float	Ratio;			// 帧图片放大或缩小比
	int     level;          //等级
}LEVELINFO;

class RPGgame:public T_Engine
{
private:
	//菜单绘画
	T_Menu t_menu;
	T_Menu t_menuAbout;
	T_Menu t_menuHelp;
	T_Menu t_menuFail;
	T_Menu t_menuNext;
	T_Menu t_menuWin;
	myMenu t_menuRun; // 游戏运行时的菜单
	T_Graph navMenu;
	void PaintMenu(HDC hdc, int GameState);//绘菜单


	//定义帧动画序列
	static int PERSON_FRAME_LEFT[20];//人物左方向帧动画
	static int PERSON_FRAME_RIGHT[20];//人物右方向帧动画
	static int PERSON_FRAME_UP[20];//人物朝上帧动画
	static int PERSON_FRAME_DOWN[20];//人物朝下帧动画

	static int FRAME_CLOSE_ATTACK_LEFT[20];//人物左方向帧动画
	static int FRAME_CLOSE_ATTACK_RIGHT[20];//人物右方向帧动画
	static int FRAME_CLOSE_ATTACK_UP[20];//人物朝上帧动画
	static int FRAME_CLOSE_ATTACK_DOWN[20];//人物朝下帧动画

	static int FRAME_CLOSE_SHOOT_LEFT[20];//人物左方向远程
	static int FRAME_CLOSE_SHOOT_RIGHT[20];//人物右方向远程
	static int FRAME_CLOSE_SHOOT_UP[20];//人物朝上远程
	static int FRAME_CLOSE_SHOOT_DOWN[20];//人物朝下远程

	static int EXPLOSION_FRAME[8];//爆炸帧动画

	static int PROP_FRAME[9];//道具帧动画

	//游戏中的数量
	static const int TOTAL_LEVEL = 5;			// 游戏总关数
	static LEVELINFO LevelSprites[TOTAL_LEVEL + 1];//每关角色信息

	static const int NPC_NUM = 2;//npc数量

	static const int SUPER_TIME = 3000;//玩家无敌时间
	static const int PLAYER_LIFE = 5;//玩家初始生命
	static const int PLAYER_CHARGE = 30;//玩家弓箭数

	static const int MORE_CHARGE = 10;//奖励弓箭数
	static const int PROP_TIME = 15000;//奖励存在时间


	// 场景相关变量
	int scn_width, scn_height;//地图场景宽高
	int wnd_width, wnd_height;//游戏窗口宽高

	//游戏逻辑控制变量
	int speedup;//速度
	int blood;//失血数
	long ChargeCount;//玩家炮弹计数器
	int LifeCount;//玩家生命计数器
	bool updateLifeCount;//是否更新玩家生命数

	int GameLevel;// 游戏关数计数器

	bool isDelayCollision;//是否延时碰撞
	bool upgradeGameLevel;// 是否已经升级

	long PropLoadTime;//道具加载计时

	int NpcNumber;//npc消灭数计数器

	//与声音资源相关变量
	AudioDX	ds;			// DirectSound对象
	AudioDXBuffer backmusic_buffer;  //背景音乐
	AudioDXBuffer robot_buffer;   // 背景音乐
	AudioDXBuffer mousedown_buffer; //鼠标点击音效
	AudioDXBuffer mousemove_buffer;  //鼠标点击音效
	AudioDXBuffer shoot_buffer;	     //射箭音效
	AudioDXBuffer walk_buffer;		 //走路音效
	AudioDXBuffer demaged_buffer;	 //走路音效
	AudioDXBuffer explosionSound;    //爆炸音效

	//与图片资源相关变量
	T_Map* map;
	T_Graph* lifeImg;// 状态栏玩家生命小图片
	T_Graph* bmbImg;// 状态栏炮弹数小图片
	T_Graph* npcNumImg;// 状态栏敌方小图片

	//与游戏角色类相关变量
	T_Sprite* player;//游戏玩家
	T_Scene* t_scene;//游戏地图场景

	//与游戏角色相关集合
	vSpriteSet npc_set;//npc集合
	vSpriteSet explosion_set;//爆炸效果集合
	vSpriteSet xdead_set;//箭射中效果集合
	vSpriteSet pdead_set;//玩家受伤集合

	vSpriteSet speed_set;//加速道具集合
	vSpriteSet arrow_set;//箭弹药集合
	vSpriteSet life_set;//生命道具集合

	vSpriteSet player_arrow_set;//玩家箭集合

	vSpriteSet npc_fire_set;//怪物火球集合

private:
	//与资源加载相关功能函数
	void LoadPlayer();//加载玩家角色
	void LoadNpc(int total,int number);//加载npc
	void LoadMap();//加载游戏地图
	void LoadImageRes();//加载游戏图片资源
	void LoadMenu();
	void LoadProp();//加载道具
	void LoadExplosion(int x, int y);//加载爆炸效果
	void LoadxDead(int x, int y);//加载箭射中效果
	void LoadpDead(int x, int y);//加载玩家受伤效果
	void LoadFire(//火球发射
		T_Sprite* sp,
		vSpriteSet& fireSet,
		int time = 0);
	void LoadArrow(//箭发射
		T_Sprite* sp,
		vSpriteSet& arrowSet,
		int time = 0);

	//游戏状态更新相关功能函数
	void UpdatePlayerPos();//更新玩家位置
	void UpdatePlayerLife();//更新玩家生命

	void UpdateNpcPos();//更新npc位置

	void UpdateFirePos(vSpriteSet* fireSet);//更新火球位置
	void UpdateArrowPos(vSpriteSet* arrowSet);//更新箭位置

	void UpdateProp(vSpriteSet& propSet);//更新道具状态

	void UpdateAnimation();//更新角色帧动画序列号

	//游戏关卡处理函数
	void LoadGameLevel(int level);//加载关卡数据
	void ClearGameLevel();//清除关卡数据
	bool UpgradePlayerLevel();// 关卡升级处理

	//游戏细节处理
	void Collide(T_Sprite* arrow);//碰撞

public:
	//类构造函数和析构函数
	RPGgame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 1024, int winheight = 768);
	virtual ~RPGgame(void);

	// 重载T_Engine类中的虚函数实现游戏功能
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};

