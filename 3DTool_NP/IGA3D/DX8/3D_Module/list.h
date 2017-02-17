//ͨ������
#pragma once
template <typename T>
struct Node
{
	T Node_Struct;
	Node *next,*pre;
	Node(T &s):next(0),pre(0),Node_Struct(s){}
};
/////////////////
template <typename T>
class List
{
	Node<T>  *head,*end;
	static int sum;
public:
	List():head(0),end(0){}
	static int GetSum(){return sum;}
	Node<T>* Get_List_Head(){return head;}//ʼ��ָ������ӽ����Ľڵ�
	Node<T>* Get_List_end(){return end;}//ʼ��ָ���һ����ӽ����Ľڵ�
	void AddNode(T& n);
	void DelNode(Node<T> *p);
	void DelAllNode();
	void DelAllNode_point();
	~List()
	{
		for(Node<T> *p;p=head;delete p)
		{
			head=head->next;
		}
		sum = 0;
	}
};
//////
template<typename T>
int List<T>::sum =0;
///
template<typename T>
void List<T>::AddNode(T& n)
{
	Node<T> *p=new Node<T>(n);
	p->next =head;
	head=p;
	(end ? p->next ->pre : end)=p;
	sum++;
}
///
template<typename T>
void List<T>::DelNode (Node<T> *p)
{
	(p->next ? p->next ->pre :end)=p->pre ;
	(p->pre ? p->pre ->next :head)=p->next ;
	delete p;
	sum--;
}

template<typename T>
void List<T>::DelAllNode ()
{
	for(Node<T> *p;p=head;delete p)
	{
		head=head->next;
	}
	sum = 0;
}

template<typename T>
void List<T>::DelAllNode_point()
{
	for(Node<T> *p;p=head;delete p)
	{
		if( p->Node_Struct != NULL )
		{
			delete ( p->Node_Struct );
		}
		head=head->next;
	}
	sum = 0;
}