--�����˲� 
--test.lua
 --ȫ��״̬�б�
State = {}; --ϵͳ״̬��
State[7] = 1;
State[8] = 0;
------------------------------
--C����
--CGetState(): ��ȡ״̬����



--CSetState(): ����״̬����
--������˵��
-- 1���������ͣ�����ֵ�����ݲ�ͬ��ֵ���в������������ģ����Ϣ
-- 2-...���������͵Ĳ��������ݾ������


---------------------------------------------
--״̬��־
SHOW_MESH  = 1;
SHOW_WINDOW = 2;

-----------------------------------------------
--������
function Run()
   --------------------------------------
   --��ȡϵͳ״̬
   State[1], State[2], State[3], State[4], State[5], State[6] = CGetState();
   
   --------------------------------
   --�߼�����
   	--		   ��ʾ����   mesh_id     x		  	 y	  			z    		s_xyz  	r_x    	r_y  r_z  tex_id
--	����(173,127)
	CSetState( SHOW_MESH,    6, 	2062.9980, 	1203.0229, 	-3250.7236, 	5.000,  	0,  	1.570,   0,	-1);
    
--	���� 
	CSetState( SHOW_MESH,    7, 	2062.9980, 	1203.0229, 	-2950.977, 		5.000,  	0,  	1.570,   0,	-1);
-- 	
-- 	���ֹ� 
	CSetState( SHOW_MESH,    8, 	1462.9980, 	1203.0229, 	-3450.7236, 	1.000,  	0,  	1.570,   0,	-1);
--	
--	���� 
	CSetState( SHOW_MESH,    9, 	1462.9980, 	1300.0229, 	-2950.977, 		1.000,  	0,  	1.570,   0,	-1);
--   
--  ���޾��� 
    CSetState( SHOW_MESH,    10, 	1862.9980, 	1203.0229, 	-3650.7236,		0.020,  	0,  	1.570,   0,	-1);
--    
--	��� 
	CSetState( SHOW_MESH,    11, 	1462.9980, 	1203.0229, 	-3650.7236,		0.050,  	0,  	1.570,   0,	-1);
--
--	����� 
    bill_board.Draw();



end

------------------------------
--��ʱ��
timer = 0;

function MyTimer()
   timer = timer + 1;

   if timer > 50 then
	  timer = 0
	  return true;
   else
	  return false;
   end
end


bill_board  =
{
	mesh_id  	= 4,   --ʹ�õ�����id
	tex_id      = 0,
	max_tex_id  = 9,

	x    	= 900.9980,
	y       = 1203.0229,
	z       = -3000.7236,

	s_xyz   = 100.000,

	r_x     = 0,
	r_y     = 0,
	r_z     = 0,

	-----------------------------
	--����
	Draw    = function()
			   --CSetState( SHOW_MESH,    4, 	112000, 163800, 86200, 650,  -1570,  0,   0,	-1);
			   bill_board.AI();
	           CSetState( SHOW_MESH, bill_board.mesh_id, bill_board.x, bill_board.y, bill_board.z, bill_board.s_xyz, bill_board.r_x, bill_board.r_y, bill_board.r_z, bill_board.tex_id );
			   end,

	---------------------------
	--AI
	AI      = function()
	            b_change = MyTimer();
	            if b_change == true then
                   bill_board.tex_id = bill_board.tex_id + 1;
				end

				if bill_board.tex_id > bill_board.max_tex_id then
				   bill_board.tex_id = -1;
				end

			  end
}

--------------------------------------------------
--ϵͳ������Ϣ
DP =
{
	Per_SetTransform	= 0;
	Post_SetTransform	= 1;
	Per_Present			= 2;
	Post_Present		= 3;
}

DrawPos	= DP.Per_Present;