
#pragma  once

#define PATH_WORK "InterAdtive\\EMMOA\\3D\\"
#define _IGA_DATA _T("F43436CA-F337-410a-8BDB-F470040A81C2")

#define D3DRM_XTEMPLATE_BYTES 3278
#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }


/*
�����ǹ��߻��ǿͻ���
0:��ʾ�ǿͻ��ˣ�������������Ѱ�ҡ����¼��ص��ֶ����ܣ�ͬʱҲ��������κ���Ϣ��
1:��ʾ�ǹ���ģʽ�����Խ�������Ѱ�ҡ�����3Dģ�ͣ����������ص���Ϣ��
*/
#define IS_3D_TOOL	1

//button״̬
enum BUTTON_MOUSE_FLAG
{
	BUTTON_MOUSE_NORMAL = 0,
	BUTTON_MOUSE_ON,
	BUTTON_MOUSE_DOWN,
};

enum GAMEUI_TEXTURE_ID
{
	GAMEUI_TEX_ITEM_STATIC_BACK,
	GAMEUI_TEX_ITEM_STATIC_0,
	GAMEUI_TEX_ITEM_STATIC_1,
	GAMEUI_TEX_ITEM_STATIC_2,
	GAMEUI_TEX_ITEM_STATIC_3,
	GAMEUI_TEX_ITEM_STATIC_4,
	GAMEUI_TEX_ITEM_STATIC_5,
	GAMEUI_TEX_ITEM_STATIC_6,

	GAMEUI_TEX_PLAYER_STATIC_BACK,
	GAMEUI_TEX_PLAYER_STATIC_PLAYER,
	GAMEUI_TEX_PLAYER_STATIC_HP,
	GAMEUI_TEX_PLAYER_STATIC_MP,

	GAMEUI_TEX_SYSTEM_BACK,

	GAMEUI_TEX_SYSTEM_BUTTON_NORMAL,
	GAMEUI_TEX_SYSTEM_BUTTON_DOWN,
	GAMEUI_TEX_SYSTEM_BUTTON_ON,

	GAMEUI_TEX_ITEM_BUTTON_NORMAL,
	GAMEUI_TEX_ITEM_BUTTON_DOWN,
	GAMEUI_TEX_ITEM_BUTTON_ON,

	GAMEUI_TEX_SHOP_STATIC_BACK,

	GAMEUI_TEX_FLX_STATIC_ITEM,

	GAMEUI_TEX_SKILL_STATIC_BACK,
	GAMEUI_TEX_SKILL_STATIC_ALPHASKILL,

	GAMEUI_TEX_FLX_STATIC_LOAD,
	GAMEUI_TEX_FLX_STATIC_LOADING,

	GAMEUI_TEX_SKILL_STATIC_1,
	GAMEUI_TEX_SKILLBOOKUI_STATIC_BACK,
	GAMEUI_TEX_MOUSEMESSAGE_STATIC_BACK,

	GAMEUI_TEX_SKILL_STATIC_2,

	GAMEUI_TEX_MINIMAP_STATIC_BACK,
	GAMEUI_TEX_MINIMAP_STATIC_MAP,
	GAMEUI_TEX_MINIMAP_STATIC_ROLE,

	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_NORMAL,
	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN,
	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_ON,

	GAMEUI_TEX_SKILL_STATIC_BUTTONRECT,

	GAMEUI_TEX_SKILL_BUTTON_FIXDOWN,
	GAMEUI_TEX_SKILL_BUTTON_NULLDOWN,
	GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN,
	GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL,


	MAX_GAMEUI_TEX_NUM,
};


//UI��Ϣ     
struct UI_MSG
{
	DWORD dwDialogId;
	DWORD dwControlId;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
};

enum IGA3D_MSG_ENUM
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};

struct SIGA3DMSG
{
	bool	bIsProc;	//�Ƿ񱻴����
	DWORD	dwType;		//��Ϣ������
	int     iKey;	    //��ֵ
	int		ixPoint;		//����λ��
	int		iyPoint;

	SIGA3DMSG()
	{
		bIsProc = false;
		dwType	= IGA3D_MSG_UNKNOW;
		iKey		= IGA3D_MSG_UNKNOW;
		ixPoint		= 0;
		iyPoint       = 0;
	}

	SIGA3DMSG( const SIGA3DMSG& obj )
	{
		bIsProc = obj.bIsProc;
		dwType	= obj.dwType;
		iKey		= obj.iKey;
		ixPoint		= obj.ixPoint;
		iyPoint       = obj.iyPoint;
	}
	~SIGA3DMSG() {}
};

//-----------------------------------------------------
//
struct SUIElement
{
	DWORD	m_dwID;
	int		m_iTexID;	//ʹ�õ�����ID
	RECT	m_rect;

	SUIElement*  pFirstChildNode; //�ӽڵ�
	SUIElement*  pSiblingNode;    //�ֵܽڵ�

	SUIElement()
	{
		m_dwID		= 0;
		m_iTexID	= 0;
		m_rect.top		= 0;
		m_rect.left		= 0;
		m_rect.right		= 30;
		m_rect.bottom		= 50;

		pFirstChildNode = NULL;
		pSiblingNode	= NULL;
	}

};


struct _S3DXML
{
	int		id;
	int		Close;
};


struct STDrawElement
{
	int		m_iObjID;	 //����id
	bool	m_bIsShow;
	int		m_iMeshID;  //ģ��ID
	int		scene_id;   //����ID
	int		tex_Num;	//��������
	int		tex_Index;  //��������
	int		tex[100];	//�����б�
	int		tex_id;
	//2008-10-13
	//�����飬���ڱȽϸ���ģ�ͣ�ʹ�����������ʽ��������
	int     mesh_sub_num;
	int     mesh_array[100];

	float	x, y, z;
	float	s_xyz;
	float	r_x, r_y, r_z;
	char	szURL[MAX_PATH];
	UINT	ShowTime;		//�ܹ���ʾ��ʱ��
	UINT	StartShowTime;	//��ʼ��ʾ��ʱ��
	bool    bStartShow;		//�Ƿ�ʼ��ʾ
	UINT	PickCount;		//����Ĵ���

	STDrawElement()
	{
		m_iObjID  = -1;
		m_iMeshID = -1;
		scene_id = -1;
		tex_id   = -1;

		m_bIsShow = false;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szURL[0] = '\0';

		tex_Num		= 0;
		tex_Index	= 0;
		tex[0]		= -1;

		mesh_sub_num = 0;
		mesh_array[0] = -1;

		ShowTime		= 0;		
		StartShowTime	= 0;	
		bStartShow		= false;
		PickCount		= 0;

	}

	STDrawElement( const STDrawElement& obj )
	{
		m_iObjID = obj.m_iObjID;

		m_iMeshID = obj.m_iMeshID;
		scene_id = obj.scene_id;
		tex_id   = obj.tex_id;

		m_bIsShow = obj.m_bIsShow;

		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;

		strcpy( szURL, obj.szURL );

		tex_Num		= obj.tex_Num;
		tex_Index	= obj.tex_Index;
		for( int i = 0; i < tex_Num; i++ )
		{
			tex[i]		= obj.tex[i];
		}

		mesh_sub_num = obj.mesh_sub_num;
		for( int i = 0; i < mesh_sub_num; i++ )
		{
			mesh_array[i] = obj.mesh_array[i];
		}

		ShowTime		= obj.ShowTime;		
		StartShowTime	= obj.StartShowTime;	
		bStartShow		= obj.bStartShow;
		PickCount		= obj.PickCount;
	}

	void Clear()
	{
		m_iObjID = -1;

		m_iMeshID = -1;
		scene_id = -1;
		tex_id   = -1;

		m_bIsShow = false;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szURL[0] = '\0';

		tex_Num		= 0;
		tex_Index	= 0;
		tex[0]		= -1;

		mesh_sub_num = 0;
		mesh_array[0] = -1;

		ShowTime		= 0;		
		StartShowTime	= 0;	
		bStartShow		= false;
		PickCount		= 0;
	}
};

//���ڱ���Ľṹ��
struct STShowInfoHead
{
	UINT	year;
	char	month;
	char	day;

	char	hour;
	char	minute;
	char	sec;

	UINT	number;	//�ļ��м�¼������
	struct ShowInfo
	{
		UINT	id;
		UINT	PickCount;
		float	ShowTime;
	};
};

enum 
{
	b_ESC, b_F1, b_F2, b_F3, b_F4, b_F5, b_F6, b_F7, b_F8, b_F9, b_F10, b_F11, b_F12, //���ܼ�
	b_L, b_W, b_H, b_S, b_R, b_T, b_M, b_A, b_N,//��ĸ��
	b_UP, b_DOWN, b_LEFT, b_RIGHT, //�����
	b_LBUTTON, b_RBUTTON, //������
	b_CTRL, b_SHIFT, //CTRL��SHIFT��
	b_ADD, b_SUB, //��+������-����
	b_DELETE, //DELETE��
	b_MAX, b_MIN, //��,������.����<������>����
};

enum GAME_STATE
{
	STATE_START,	//��ʼ
	STATE_IN_SCENE, //�����˳���
	STATE_AFTER_DLG,	//���ѡ����һ��������ڣ�����IGA�Ի���,��ʾ��ҿ���IGA�˵��
	STATE_CHOICE_IGA,	//���ѡ����IGA�
	STATE_IGA_START,	//IGA���ʼ
	STATE_IGA_COMPLETE, //���IGA�
	STATE_IGA_END 		//�˳�IGA������ܵ��������ɺ��˳�����;�˳���һ��ʼ���˳�
};

enum IGA_STATE
{
	IGA_START,	//IGA����ʼ
	IGA_END,	//IGA�������
	IGA_1,	//��ɵ�һ������
	IGA_2,	//
	IGA_3,
	IGA_OK,	//���ȫ������
	IGA_DESIGN //���ģʽ
};

enum RenderPos
{
	Per_SetTransform,	//���þ���ǰ
	Post_SetTransform,	//���þ����
	Per_Present,		//��תǰ
	Post_Present		//��ת��
};

enum ENUM_IGA_GAME_LIST
{
	IGA_NO_GAME = 0,
	IGA_FENGYUN,			//����
	IGA_GONGFUSHIJIE,		//��������
	IGA_GUANGZHIGUODU,		//��֮����
	IGA_SHENQI,				//����
	IGA_TEZHONGBUDUI,		//���ֲ���
	IGA_HANGHAISHIJI,		//��������
	IGA_JIJIASHIJI,			//��������
	IGA_WUJIEQU	,			//�����
	IGA_HANGHAISHIJI_EX	,	//�������͸��°�
	IGA_JINGTIANDONGDI,		//���춯��
	IGA_LONGHUMEN,			//������
	IGA_HAIZEIWANG	,		//������
	IGA_TIANSHANGRENJIAN,	//�����˼�
	IGA_JUREN,				//����
	IGA_KANGZHAN,			//��ս
	IGA_LUOQI,				//����
	IGA_PAOPAOKADINGCHE,		//���ܿ�����
	IGA_REXUEJIANGHU,		//��Ѫ����


	IGA_SILUCHUANSHUO,		//˿·��˵
	IGA_TIANJI,				//���
	IGA_TIANLONGBABU,		//�����˲�  
	IGA_WANMEISHIJIE,		//��������	
	IGA_ZHUXIAN	,			//����	
	IGA_WEIWUDUZUN,			//Ψ�����	
	IGA_ZHUOYUEZHIJIAN,		//׿Խ֮��	
	IGA_CHUNQIU_Q_ZHUAN, 	//����Q��	
	IGA_FENGHUOZHILV,		//���֮��	
	IGA_MENGJIANG,			//�ͽ�	
	IGA_ZHENGFU	,			//����
	IGA_MOYU,				//����
	IGA_XINYANG,				//����--����ս��	
	IGA_ZHANHUO,				//ս��	
	IGA_JIETOULANQIU,		//��ͷ����	
	IGA_TIANTANG2,			//����2
	IGA_SUN,					//�漣����
	IGA_TIANZUN,				//����OnLine
	IGA_WLOL,				//����Ⱥ���� 
	IGA_GV,					//�󺽺�ʱ��Online
	IGA_RELIPAIQIU,			//��������	

	IGA_WUJIEQU_MLXY	=155,	//�����(��������)
	IGA_WUJIEQU_XJP,				//�����(�¼���)
	IGA_WUJIEQU_FLB = 158,				//�����(���ɱ�)
};

