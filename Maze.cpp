
#include <iostream>
using namespace std;

/*
OOP实现一个迷宫路径的寻找问题
软件运行要求如下：

请输入迷宫路径的行列数：5 5
请输入迷宫的具体路径信息(0表示能走，1表示不能走)：
0 0 1 0 0 
0 0 0 0 0
1 0 0 1 1
0 1 0 1 0
1 0 0 0 0
开始寻找迷宫路径...

没找到，打印提示信息“迷宫不存在可通行的路径”
找到了，把迷宫路径打印一遍
* * 1 0 0
0 * * 1 0
1 0 * 0 1
0 1 * 1 0
1 0 * * *

git创建远程分支
git创建本地分支
git在本地切换分支
git提交代码的流程 
*/
// 常量定义
// 定义方向
const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;
const int WAY_NUM = 4;
// 定义行走状态用的
const int YES = 5;
const int NO = 6;

static int flag=0;
class Maze;

// 抽象迷宫节点类型
class MazeNode
{
public:
	MazeNode():lt_x(-2),lt_y(-2),mflag(0)
	{
		for (int i = 0; i < WAY_NUM; ++i)
		{
			mstate[i] = YES;
		}
	}
	friend class CStack;
	friend class Maze;
private:
	int mval; // 节点的值
	int mx; // 节点坐标
	int my; // 节点坐标
	int lt_x;//上一个节点的坐标x
	int lt_y;//上一个节点的坐标y
	int mflag;
	int mstate[WAY_NUM]; // 节点四个方向的状态
};
// 抽象顺序栈结构
class CStack
{
public:
	friend class Maze;
	// CStack() CStack(int)
	CStack(int size = 100) // 构造函数，给对象成员变量进行初始化
	{
		//cout << this << " CStack(int)" << endl;
		mtop = 0;
		msize = size;
		mpstack = new MazeNode[msize];
	}
	~CStack() // 析构函数，释放对象
	{
		//cout << this << " ~CStack()" << endl;
		delete[]mpstack;
		mpstack = nullptr;
	}
	void push( MazeNode &val) // 入栈
	{
		if (full())
			resize();
		val.mflag=1;
		mpstack[mtop++] = val;
		
	
	}
	void pop() // 出栈
	{
		if (empty())
			return;
	//	pmaze[mpstack[mtop].mx][mpstack[mtop].my].mflag=0;
		--mtop;

	}
	MazeNode top()
	{
		if (empty())
			throw "stack is empty!";
		return mpstack[mtop-1];
	}
	bool full() { return mtop == msize; }
	bool empty() { return mtop == 0; }
	static CStack *getPstack()
	{
		if(NULL==pstack)
			pstack=new CStack();
		return pstack;
	}
private:
	MazeNode *mpstack; // 存储栈的数据
	int mtop; // 表示栈顶下标
	int msize; // 表示栈的总长度
	int s_x;
	int s_y;
	static CStack* pstack;
	void resize()
	{
		MazeNode *ptmp = new MazeNode[msize * 2];
		//memcpy(ptmp, mpstack, sizeof(int)*msize);
		//realloc  对象扩容禁止使用realloc/memcpy这些函数!!!
		for (int i = 0; i < msize; ++i)
		{
			ptmp[i] = mpstack[i];
		}
		delete []mpstack;
		mpstack = ptmp;
		msize *= 2;
	}
};

CStack* CStack::pstack=nullptr;
// 抽象迷宫类型
class Maze
{
public:
	Maze(int row=3, int col=3): mcol(col),mrow(row)
	{
		mpMaze=new MazeNode* [row];
		for(int i=0;i<row;++i)
		{
			mpMaze[i]=new MazeNode [col];
		}
	}

	~Maze()
	{
		for(int i=0;i<mrow;i++)
		{
			delete[]mpMaze[i];
		}
		delete[]mpMaze;
	}

	void initMazeNode(int data,int  i,int  j)
	{
		mpMaze[i][j].mx=i;
		mpMaze[i][j].my=j;
		mpMaze[i][j].mval=data;
	}

	void adjustMaze()
	{
		for(int i=0;i<mrow;++i)
		{
			for(int j=0;j<mcol;++j)
			{
				if(i-1<0 || mpMaze[i-1][j].mval==1)
				{
					mpMaze[i][j].mstate[UP]=NO;
				}

				if(j-1<0 || mpMaze[i][j-1].mval==1)
				{
					mpMaze[i][j].mstate[LEFT]=NO;
				}

				if(j+1>mcol-1 || mpMaze[i][j+1].mval==1)
				{
					mpMaze[i][j].mstate[RIGHT]=NO;
				}

				if(i+1>mrow-1 || mpMaze[i+1][j].mval==1)
				{
					mpMaze[i][j].mstate[DOWN]=NO;
				}
			}
		}
	}

	 CStack *second_handle(CStack *stack,int i,int j,int flag,int flag_t)
	 {
		 mpMaze[i][j].mstate[flag]=YES;
		 	 /*
		 if(flag_t==DOWN)
		 {
			  return handle_path(stack,i,j,i+1,j,DOWN);	
		 }

		 if(flag_t==LEFT)
		 {
			  return 	handle_path(stack,i,j,i,j-1,LEFT);	
		 }

		 if(flag_t==UP)
		 {
			 return handle_path(stack,i,j,i-1,j,UP);
		 }


		*/
		 switch(flag_t)
		 {
		 case DOWN:
					 return handle_path(stack,i,j,i+1,j,DOWN);	
					 break;
		 case LEFT:
					 return 	handle_path(stack,i,j,i,j-1,LEFT);	
					 break;
		 case UP:
					 return handle_path(stack,i,j,i-1,j,UP);
					 break;
		 default:
			         break;
		 }
	
	 }

    CStack* handle_path(CStack *stack,int i,int j,int k,int l,int flag)
	{
		        mpMaze[i][j].mstate[flag]=NO;
	
				if((mpMaze[i][j].lt_x==k&& mpMaze[i][j].lt_y==l) )
				{
					
						 for(int m=flag+1;m<4;m++)
						 {
								 if(mpMaze[i][j].mstate[m]==YES)
									 return second_handle(stack,i,j,flag,m);
					    }

						if(mpMaze[i][j].mflag==1)
						{
							stack->pop();						
						}
				}
				else
				{
						if(mpMaze[i][j].mflag!=1)
						{
							stack->push(mpMaze[i][j]);
						}		

						mpMaze[k][l].lt_x=i;
						mpMaze[k][l].lt_y=j;
							
				}
		
				return findMazePath(stack,k,l);
	}

	CStack*  findMazePath(CStack *stack,int i,int j)
	{

		if(nullptr==stack)
			return nullptr;
		
		if( mpMaze[0][0].mval!=0 && mpMaze[mrow-1][mcol-1].mval!=0)
		{
			      return new CStack();
		}		

		if(i==mrow-1 && j==mcol-1)
		{
				stack->push(mpMaze[i][j]);
					return stack;
		}

		if(mpMaze[i][j].mstate[RIGHT]==YES)
		{				  
					
			         return handle_path(stack,i,j,i,j+1,RIGHT);
		}
	
		if(mpMaze[i][j].mstate[DOWN]==YES)
		{				 
				    return handle_path(stack,i,j,i+1,j,DOWN);						 
		}

		if(mpMaze[i][j].mstate[LEFT]==YES)
		{
				    return 	handle_path(stack,i,j,i,j-1,LEFT);			
		}

		if(mpMaze[i][j].mstate[UP]==YES)
		{				
			    	return handle_path(stack,i,j,i-1,j,UP);
		}
			     
				return new CStack();     																							  
	}

	void showResult(CStack *stack)
	{
		if(nullptr==stack)
			return;
		if(stack->empty())
		{
			cout<<"there is no way!\n"<<endl;
		}
		else
		{
			int i=0,j=0;
	
			for(;i<stack->mtop;i++)
			{
				mpMaze[stack->mpstack[i].mx][stack->mpstack[i].my].mval=5;
				cout<<"("<<stack->mpstack[i].mx<<", "<<stack->mpstack[i].my<<")";
				if(i!=stack->mtop-1)
				{
					cout<<"->";
				}
			}
			cout<<endl;

			for(i=0;i<mrow;i++)
			{
				for(j=0;j<mcol;j++)
				{
					cout<<mpMaze[i][j].mval<<"  ";
				}
				cout<<endl;
			}
			
		}
	}

	static Maze* getPmaze(int row=3,int col=3)
	{
		if(NULL==Pmaze)
			Pmaze=new Maze(row,col);
		return Pmaze;
	}

	friend class CStack;
private:
	int mrow;
	int mcol;
	MazeNode **mpMaze;
	static Maze* Pmaze;
	//为了防止对象浅拷贝出现问题，私有化那两个函数
};

Maze* Maze::Pmaze=nullptr;



int main()
{
	cout << "请输入迷宫路径的行列数：";
	int row, col;
	cin >> row >> col;

	Maze *maze=Maze::getPmaze(row, col);

	cout << "请输入迷宫的具体路径信息(0表示能走，1表示不能走)：" << endl;

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			int data;
			cin >> data;
			maze->initMazeNode(data, i, j);
		}
	}

	// 调整迷宫所有节点的方向的行走状态
	maze->adjustMaze();

	// 寻找迷宫路径
	CStack *stack=CStack::getPstack();

	// 打印寻找结构
	maze->showResult(maze->findMazePath(stack,0,0));

		
	return 0;
}
