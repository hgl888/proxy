#pragma once

typedef unsigned char UCHAR;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};


//��������
bool InitDrawing();
//�ͷ�����
bool UnInitDrawing();

//��lock ��unlock֮����ã����ڻ��ƴӻ�����
bool Draw_Video();

bool Draw_Flash();

bool ZoomBmpBuffer(UCHAR * pInData, int nSrcWidth, int nSrcHeigth,int bpp, UCHAR * pOutData, int nDstWidth, int nDstHeight);

bool ShowDialog();


