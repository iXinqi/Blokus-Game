#include<stdio.h>      
#include<math.h>
#include <iostream>              
using namespace std;             
               
#define MAXSTATE 800               
  
#define pop( ) stack[--sp]  
#define push(node) stack[sp++] = node  
#define stack_not_empty (sp>0)       
      
int piece[21][10]={{0,0,0,0,0,0,0,0,0,0},    //1    
                    {0,0,0,1,0,0,0,0,0,0},   //2    
                    {0,0,0,1,1,1,0,0,0,0},   //3    
                    {0,0,0,1,0,2,0,0,0,0},   //4    
                    {0,0,0,1,1,0,1,1,0,0},   //5    
                    {0,0,1,0,1,1,1,-1,0,0},  //6    
                    {0,0,0,1,0,2,0,3,0,0},   //7    
                    {0,0,1,0,1,-1,1,-2,0,0}, //8    
                    {0,0,0,1,1,0,1,-1,0,0},  //9    
                    {0,0,1,0,1,1,1,2,1,3},   //10    
                    {0,0,1,0,2,0,2,1,2,-1},  //11    
                    {0,0,1,0,2,0,2,1,2,2},   //12    
                    {0,0,0,1,0,2,1,0,1,-1},  //13    
                    {0,0,1,0,1,-1,1,-2,2,-2}, //14    
                    {0,0,1,0,2,0,3,0,4,0},  //15    
                    {0,0,1,0,1,1,2,0,2,1},  //16    
                    {0,0,0,1,1,0,1,-1,2,-1},  //17    
                    {0,0,0,1,1,0,2,0,2,1},   //18    
                    {0,0,0,1,1,0,1,-1,2,0},  //19    
                    {0,0,1,0,1,1,1,-1,2,0},  //20    
                    {0,0,1,0,1,-1,1,1,1,2}};  //21    
typedef struct state  
{  
    int board[22][22];  
    int used[2][21];  
    int move[7];
	int cover;
	int center;
}state;  
  
state stack[MAXSTATE]; 
state stackcompare[MAXSTATE];  
int sp;     

void showboard(state st)
{
	int i,j;
	for(i=0;i<22;i++)
	{
		for(j=0;j<22;j++)
			printf("%d ",st.board[j][i]);
		printf("\n");
	}
}
      
void action_l(int piece[21][10],int block_id,int t)   
{      
    if(t==0)    
        return;    
    int i,j;             
    for(i=0;i<10;i=i+2)     
        piece[block_id][i]=-piece[block_id][i];      
}     
void action_v(int piece[21][10],int block_id,int d)    
{      
    if(d==0)    
        return;    
    int i,j;            
    for(i=1;i<10;i=i+2)     
        piece[block_id][i]=-piece[block_id][i];      
}    
void action_d(int piece[21][10],int block_id,int d)  
{      
    int i,j,t;      
    if(d==0)    
        return;        
	if(d==1)     //90  
    {    
        for(i=0;i<10;i=i+2)  
        {  
            t=piece[block_id][i];  
            piece[block_id][i]=piece[block_id][i+1];  
            piece[block_id][i+1]=-t;  
        } 
		return;
    }  
    if(d==3)     //270  
    {    
        for(i=0;i<10;i=i+2)  
        {  
            t=piece[block_id][i];  
            piece[block_id][i]=-piece[block_id][i+1];  
            piece[block_id][i+1]=t;  
        } 
		return;
    }  
    if(d==2)     //180  
    {    
        for(i=0;i<10;i=i+2)  
        {  
            piece[block_id][i]=-piece[block_id][i];  
            piece[block_id][i+1]=-piece[block_id][i+1];  
        }
		return;
    }                
}     

void init(state &st)  
{  
    int i,j;  
    for(i=0;i<22;i++)  
    {  
        st.board[i][0]=5;  
        st.board[i][21]=5;  
    }  
    for(i=0;i<22;i++)  
    {  
        st.board[0][i]=5;  
        st.board[21][i]=5;  
    }  
    for(i=1;i<21;i++)  
    {  
        for(j=1;j<21;j++)  
            st.board[i][j]=0;  
    }  
    for(i=0;i<2;i++)  
    {  
        for(j=0;j<21;j++)  
            st.used[i][j]=0;  
    }     
}
void read(int ID,int x,int y,int block_id,int l,int v,int d,state &st)
{
	int i,j,x0,y0;
	action_l(piece,block_id-1,l);  
    action_v(piece,block_id-1,v);  
    action_d(piece,block_id-1,d);  
    x0=piece[block_id-1][0]+x;  
	y0=piece[block_id-1][1]+y;  
    st.board[x][y]=ID;  
    for(i=2;i<10;i=i+2)  
    {  
        x=x0+piece[block_id-1][i];  
        y=y0+piece[block_id-1][1+i];  
        st.board[x][y]=ID;  
    }   
    if(d==0||d==2)
        action_d(piece,block_id-1,d);
    if(d==1)
        action_d(piece,block_id-1,3);
    if(d==3)
       action_d(piece,block_id-1,1);
	action_v(piece,block_id-1,v);
    action_l(piece,block_id-1,l);
}
  
state start(int ID,int piece[21][10],int x,int y,int block_id,int l,int v,int d)  
{  
    state st;  
	init(st);
    int i,x0,y0;  
    if(ID==2)   
    {  
        read(1,x,y,block_id,l,v,d,st);
        x0=piece[16][0]+1;  
        y0=piece[16][1]+19;  
        st.board[1][19]=2;  
        for(i=2;i<10;i=i+2)  
        {  
            x=x0+piece[16][i];  
            y=y0+piece[16][1+i];  
            st.board[x][y]=2;  
        }  
        printf("2 1 19 17 0 0 0\n");  
        fflush(stdout);  
        scanf("%d%d%d%d%d%d%d",&ID,&x,&y,&block_id,&l,&v,&d); 
		read(3,x,y,block_id,l,v,d,st);
        x0=piece[16][0]+18;  
        y0=piece[16][1]+2;  
        st.board[18][2]=4;  
        for(i=2;i<10;i=i+2)  
        {  
            x=x0+piece[16][i];  
            y=y0+piece[16][1+i];  
            st.board[x][y]=4;  
        }  
        printf("4 18 2 17 0 0 0\n");  
        fflush(stdout);  
        st.used[0][16]=1;  
        st.used[1][16]=1; 
    }      
    if(ID==1)  
    {       
        action_l(piece,16,1);  
        x0=piece[16][0]+3;  
        y0=piece[16][1]+2;  
        st.board[3][2]=1;  
        for(i=2;i<10;i=i+2)  
        {  
            x=x0+piece[16][i];  
            y=y0+piece[16][1+i];  
            st.board[x][y]=1;  
        }   
        x0=piece[16][0]+20;  
        y0=piece[16][1]+19;  
        st.board[20][19]=3;  
        for(i=2;i<10;i=i+2)  
        {  
            x=x0+piece[16][i];  
            y=y0+piece[16][1+i];  
            st.board[x][y]=3;  
        }  
        printf("1 3 2 17 1 0 0\n");   
        fflush(stdout);  
        action_l(piece,16,1); 
        scanf("%d%d%d%d%d%d%d",&ID,&x,&y,&block_id,&l,&v,&d);  
		read(2,x,y,block_id,l,v,d,st);
        printf("3 20 19 17 1 0 0\n");  
        fflush(stdout);  
        st.used[0][16]=1;  
        st.used[1][16]=1;   
         
    }      
    return st;  
}    

int have_one_point(int box[10],state st,int ID) 
{
	int i;
	for(i=0;i<10;i=i+2)
		if(st.board[box[i]-1][box[i+1]-1]==ID||st.board[box[i]-1][box[i+1]+1]==ID||st.board[box[i]+1][box[i+1]-1]==ID||st.board[box[i]+1][box[i+1]+1]==ID)
			return 1; 
	return 0;	
}

int can_put_box(int i,int j,state st,int ID)  
{
	if(st.board[i][j]==0)
		if(st.board[i+1][j]!=ID&&st.board[i][j+1]!=ID&&st.board[i-1][j]!=ID&&st.board[i][j-1]!=ID) //ио??oбд???б┴ио2??идиви▓ 
				return 1;    
	return 0;
}


void compare1(state stack[MAXSTATE],int ID,state &raw)
{
	int i,j,x0,y0,x,y,cover;
	double dis=0,n=0,min=0;
	state st,bestst;
	while(stack_not_empty)
	{
		st=pop();
		n=0;dis=0;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        n++;
		cover=400-n;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        dis=dis+sqrt((i-1)*(i-1)+(j-20)*(j-20));
		n=cover+dis*50;
		if(n>min)
		{
			min=n;
			bestst=st;
		}
	}
	if(ID==1||ID==2)
	   raw.used[0][bestst.move[3]-1]=1;
	if(ID==3||ID==4)
	   raw.used[1][bestst.move[3]-1]=1;
	read(bestst.move[0],bestst.move[1],bestst.move[2],bestst.move[3],bestst.move[4],bestst.move[5],bestst.move[6],raw);//?ивии?
    for(i=0;i<7;i++)
    	printf("%d ",bestst.move[i]);
    printf("\n");
    fflush(stdout);
}

void compare2(state stack[MAXSTATE],int ID,state &raw)
{
	int i,j,x0,y0,x,y,cover;
	double dis=0,n=0,min=0;
	state st,bestst;
	while(stack_not_empty)
	{
		st=pop();
		n=0;dis=0;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        n++;
		cover=400-n;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        dis=dis+sqrt((i-1)*(i-1)+(j-1)*(j-1));
		n=cover+dis*50;
		if(n>min)
		{
			min=n;
			bestst=st;
		}
	}
	if(ID==1||ID==2)
	   raw.used[0][bestst.move[3]-1]=1;
	if(ID==3||ID==4)
	   raw.used[1][bestst.move[3]-1]=1;
	read(bestst.move[0],bestst.move[1],bestst.move[2],bestst.move[3],bestst.move[4],bestst.move[5],bestst.move[6],raw);//?ивии?
    for(i=0;i<7;i++)
    	printf("%d ",bestst.move[i]);
    printf("\n");
    fflush(stdout);
}

void compare3(state stack[MAXSTATE],int ID,state &raw)
{
	int i,j,x0,y0,x,y,cover;
	double dis=0,n=0,min=0;
	state st,bestst;
	while(stack_not_empty)
	{
		st=pop();
		n=0;dis=0;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        n++;
		cover=400-n;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        dis=dis+sqrt((i-20)*(i-20)+(j-1)*(j-1));
		n=cover+dis*50;
		if(n>min)
		{
			min=n;
			bestst=st;
		}
	}
	if(ID==1||ID==2)
	   raw.used[0][bestst.move[3]-1]=1;
	if(ID==3||ID==4)
	   raw.used[1][bestst.move[3]-1]=1;
	read(bestst.move[0],bestst.move[1],bestst.move[2],bestst.move[3],bestst.move[4],bestst.move[5],bestst.move[6],raw);//?ивии?
    for(i=0;i<7;i++)
    	printf("%d ",bestst.move[i]);
    printf("\n");
    fflush(stdout);
}

void compare4(state stack[MAXSTATE],int ID,state &raw)
{
	int i,j,x0,y0,x,y,cover;
	double dis=0,n=0,min=0;
	state st,bestst;
	while(stack_not_empty)
	{
		st=pop();
		n=0;dis=0;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        n++;
		cover=400-n;
		for(i=1;i<21;i++)
			for(j=1;j<21;j++)
				if(st.board[i][j]==ID)
			        dis=dis+sqrt((i-20)*(i-20)+(j-20)*(j-20));
		n=cover+dis*50;
		if(n>min)
		{
			min=n;
			bestst=st;
		}
	}
	if(ID==1||ID==2)
	   raw.used[0][bestst.move[3]-1]=1;
	if(ID==3||ID==4)
	   raw.used[1][bestst.move[3]-1]=1;
	read(bestst.move[0],bestst.move[1],bestst.move[2],bestst.move[3],bestst.move[4],bestst.move[5],bestst.move[6],raw);//?ивии?
    for(i=0;i<7;i++)
    	printf("%d ",bestst.move[i]);
    printf("\n");
    fflush(stdout);
}

void put_piece(int piece[21][10],state st,int ID,int i,int x0,int y0,int l,int v,int d)
{
	int box[10]={0};
	int n,x,y;
	for(n=0;n<10;n=n+2)
		box[n]=piece[i][n]+x0;
	for(n=1;n<10;n=n+2)
		box[n]=piece[i][n]+y0;  
	if(can_put_box(box[0],box[1],st,ID)&&can_put_box(box[2],box[3],st,ID)&&can_put_box(box[4],box[5],st,ID)&&can_put_box(box[6],box[7],st,ID)&&can_put_box(box[8],box[9],st,ID)&&have_one_point(box,st,ID))
	{        
		for(n=0;n<10;n=n+2)
			st.board[box[n]][box[n+1]]=ID; 
		if(ID==1||ID==2)
			st.used[0][i]==1;
		if(ID==3||ID==4)
			st.used[1][i]==1;
		st.move[0]=ID;
		st.move[1]=x0;
		st.move[2]=y0;
		st.move[3]=i+1;      
		st.move[4]=l; 
		st.move[5]=v;
		st.move[6]=d;
       	push(st);	
	}
} 

void move(state &st,int piece[21][10],int ID)  
{
	int i,n,x,y,x0,y0;
	for(x=1;x<21;x++)
	{
		for(y=1;y<21;y++)   
		{
			if(can_put_box(x,y,st,ID))  
			{
				for(i=20;i>-1;i--)    
				{
					if(ID==1||ID==2)
						if(st.used[0][i]==1)
							continue;
					if(ID==3||ID==4)
						if(st.used[1][i]==1)
							continue;      
					put_piece(piece,st,ID,i,x,y,0,0,0);  
					
					action_l(piece,i,1);          
					put_piece(piece,st,ID,i,x,y,1,0,0);	
					action_l(piece,i,1);   
					action_v(piece,i,1);          
					put_piece(piece,st,ID,i,x,y,0,1,0);
					action_v(piece,i,1);
					action_d(piece,i,1); //90
					put_piece(piece,st,ID,i,x,y,0,0,1);
					action_d(piece,i,1); //180
					put_piece(piece,st,ID,i,x,y,0,0,2);
					action_d(piece,i,1);  //270
					put_piece(piece,st,ID,i,x,y,0,0,3);
					action_d(piece,i,1);  //360
					
				}
			}
		}
	}    
	if(!stack_not_empty)
	{
		printf("%d 0 0 0 0 0 0\n",ID);
		fflush(stdout);
		return;
	}
	if(ID==1)
	compare1(stack,ID,st); 
	if(ID==2)
	compare2(stack,ID,st);
	if(ID==3)
	compare3(stack,ID,st);
	if(ID==4)
	compare4(stack,ID,st);
}
     
int main()      
{     
    int ID,x,y,block_id,l,v,d,x0,y0,i;    
	state st;     
    scanf("%d%d%d%d%d%d%d",&ID,&x,&y,&block_id,&l,&v,&d);  
    if(ID==0&&x==0&&y==0&&block_id==0&&l==0&&v==0&&d==0)      
        ID=1;                 
    else      
        ID=2;      
    st=start(ID,piece,x,y,block_id,l,v,d);    
    while(1)      
    {     
        scanf("%d%d%d%d%d%d%d",&ID,&x,&y,&block_id,&l,&v,&d);  
		if(block_id!=0)
			read(ID,x,y,block_id,l,v,d,st);
		
		int IDnext;
        if(ID==4)
        	IDnext=1;
        else
        	IDnext=ID+1; 
        move(st,piece,IDnext);          
    }            
    return 0;      
}



