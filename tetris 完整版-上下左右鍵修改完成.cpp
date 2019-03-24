#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

char tetris[10][20][2]={0};
int score=0,picture[7][4][6]={0},line[20]={0},up=72,down=80,left=75,right=77,space=32,sec=500,row=0,cache_block[3],cache_change[3];
int root[4];

void show();//��ܹC���e��
void move_show();//��ܲ��ʵe�� 
void pic();//�غc��� 
void initialization();//��l�Ƥ������ 
void mark(int point_x,int point_y,int block,int change,int clean);//��L����i�}�C 
void line_check(int point_y,int block,int change);//���p�� 
void move(int y);//���ʥؼЦ�W��Ҧ�������U�@�� 
void gotoxy(int xpos, int ypos);//�y�Ц^�k��̥��W�� 
void game();//�C���D�{�� 
void homepage(int option);//��ܥD���� 
void homepage_control();//�D�������� 
void setting(int option,int chose);//�]�w���� 
void setting_control();//�]�w�������� 
void pause(int option);//�C�����Ȱ����� 
void gameover();//�C������ 
void initialization(int *point_x,int *point_y,int *block,int *change);//��l�Ƥ�� 
void shadow(int x2,int y2,int block,int change2,int clean);//�v�l�p��
void mark_shadow(int point_x,int point_y,int block,int change);//��L�v�l�i�}�C 
void SetColor(int ForgC);//�]�w�C�� 
void ready_block(int *block,int *change,int a);//��ܹw�Ƥ�� 
void move_block(int x1,int y1,int x2,int y2,int block,int change1,int change2,int clean);//���ʤ���u�� 
void set_color(int block);//�]�w�C��
void set_root(int point_x,int point_y,int block,int change);
void shadow_new_block(int point_x,int point_y,int block,int change);//�s����v�l 
int shadow_drop_point(int point_x,int point_y,int block,int change);//�p��v�l���I 
int coordinate(int a,int xy);
int pause_control();//�Ȱ���������
int che(int point_x,int point_y,int block,int change);//�T�{������L���| 

int main(){
	srand(time(NULL));//�]�w�üƺؤl 
	homepage_control();
	return 0;
}


void game(){
	int point_x=5,point_y=-2,block[4]={rand()%7,rand()%7,rand()%7,rand()%7},change[4]={rand()%4,rand()%4,rand()%4,rand()%4},control=0,check,game_control=0,clean=0;
	set_root(point_x,point_y,block[0],change[0]);
	int i=0;
	for(i=0;i<3;i++){
		cache_change[i]=change[i+1];
		cache_block[i]=block[i+1];
	}
	pic();//�غc��� 
	clock_t t1,t2,t3,t4;
	gotoxy(0,0);
	show();

	for(;;){//�C���}�l
		check=0;
		switch(game_control){//������w
			case 0:
				shadow_new_block(point_x,point_y,block[0],change[0]);
				gotoxy(34,1);
				printf("SCORE");
				gotoxy(34,2);
				printf("%08d",score);
				for(;point_y+picture[block[0]][change[0]][1]<0 && point_y+picture[block[0]][change[0]][3]<0 && point_y+picture[block[0]][change[0]][5]<0;point_y++);//�N������ʦ��S�X�@�� 
				check=che(point_x,point_y,block[0],change[0]);//�˴����L������|
				ready_block(block,change,0);
				if(check==1)break;//����L����C������
				t1=clock();	
			case 1://���ʧ��^�즹
				//shadow(point_x,point_y,block[0],change[0],0);//��W�v�l 
				//mark(point_x,point_y,block[0],change[0],0);//��W���U���
				move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],clean);
				shadow(point_x,point_y,block[0],change[0],clean);
				clean=1;
				set_root(point_x,point_y,block[0],change[0]);
			case 2://�L�k���ʮ�
				control=0;
		}
		if(check==1)break;//switch�̧P�_����쳻 
		game_control=0;
		do{//���o��J 
			t2=clock();
			if(kbhit())control=getch();
		}while((t2-t1)<sec && control!=224 && control!=space && control!=27);
		if(control==224)control=getch();
		if(control==27){
			t3=clock();
			pause_control();
			ready_block(block,change,0);
			t4=clock();
			t1=t1+t4-t3;
			game_control=1;
			continue;	
		}
		mark(point_x,point_y,block[0],change[0],1);//�M�������������K�B�� 
		if((t2-t1)>=sec){//�ɶ��쩹�U�@�� 
			if(point_y>=19){//�W�X�d�� 
				move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
				clean=0;
				line_check(point_y,block[0],change[0]);
				initialization(&point_x,&point_y,block,change);//��l�� 
				continue;
			}
			point_y++;//���U�@��}�l�P�_
			if(point_y+picture[block[0]][change[0]][1]>19 || point_y+picture[block[0]][change[0]][3]>19 || point_y+picture[block[0]][change[0]][5]>19){//�U����m�W�L�̩���
				point_y--;//�L�k���U�A�˰h�@�� 
				move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
				clean=0;
				line_check(point_y,block[0],change[0]);
				initialization(&point_x,&point_y,block,change);//��l�� 
				continue;
			}
			else{
				check=che(point_x,point_y,block[0],change[0]);//�T�{�U����m���L��� 
				if(check==1){
					point_y--;//�L�k���U�A�˰h�@�� 
					move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
					clean=0;
					line_check(point_y,block[0],change[0]);
					initialization(&point_x,&point_y,block,change);//��l�� 
					continue;
				}
			}
		}
		if(control==left){//���� 
			point_x--;//�����@��}�l�P�_ 
			if(point_x+picture[block[0]][change[0]][0]>=0 && point_x+picture[block[0]][change[0]][2]>=0 && point_x+picture[block[0]][change[0]][4]>=0){//������S�W�L�d��i�J 
				check=che(point_x,point_y,block[0],change[0]);
				if(check==1){
					point_x++;//�����b�d�򤺡A��������A���k�^�h��� 
					game_control=2;
					continue;
				}
			}
			else{
				point_x++;//�W�L�d��A�h�^�h 
				game_control=2;
				continue;
			}
			game_control=1;
			continue;
		}
		else if(control==down){//�U���� 
			point_y++;//�U���@��}�l�P�_ 
			if(point_y+picture[block[0]][change[0]][1]<=19 && point_y+picture[block[0]][change[0]][3]<=19 && point_y+picture[block[0]][change[0]][5]<=19){//���U��S�W�L�d��i�J 
				check=che(point_x,point_y,block[0],change[0]);
				if(check==1){
					point_y--;////���U�b�d�򤺡A��������A���W�^�h��� 
					move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
					clean=0;
					line_check(point_y,block[0],change[0]);//�p���W����� 
					initialization(&point_x,&point_y,block,change);//��l�� 
					continue;
				}
			}
			else{
				point_y--;
				move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
				clean=0;
				line_check(point_y,block[0],change[0]);
				initialization(&point_x,&point_y,block,change);//��l�� 
				continue;
			}
			game_control=1;
			continue;
		}
		else if(control==right){//�k�� 
			point_x++;
			if(point_x+picture[block[0]][change[0]][0]<=9 && point_x+picture[block[0]][change[0]][2]<=9 && point_x+picture[block[0]][change[0]][4]<=9){//�S�W�X��� 
				check=che(point_x,point_y,block[0],change[0]);//�ˬd���ʫᦳ�L���| 
				if(check==1){//�����|�h�^�A���s���ݿ�J 
					point_x--;
					game_control=2;
					continue;
				}
			}
			else{//�W�X��� 
				point_x--;//�h�^�A���ݷs��J 
				game_control=2;
				continue;
			}
			game_control=1;
			continue;
		}
		else if(control==up){//�W��(����) 
			if(change[0]<=2)change[0]++; 
			else change[0]=0;
			if(point_x+picture[block[0]][change[0]][0]>9 || point_x+picture[block[0]][change[0]][2]>9 || point_x+picture[block[0]][change[0]][4]>9){//�����W�X�k����� 
				point_x--;//�����@�� 
				check=che(point_x,point_y,block[0],change[0]);//�T�{���L���| 
				if(check==1){//���| 
					point_x++;//�^�k��� 
					if(change[0]>0)change[0]--;
					else change[0]=3;
					game_control=2;
					continue;
				}
			}else if(point_x+picture[block[0]][change[0]][0]<0 || point_x+picture[block[0]][change[0]][2]<0 || point_x+picture[block[0]][change[0]][4]<0){//�W�X������� 
				point_x++;//�k���@�� 
				int a=0;
				if(point_x+picture[block[0]][change[0]][0]<0 || point_x+picture[block[0]][change[0]][2]<0 || point_x+picture[block[0]][change[0]][4]<0){//�A�׶W�X������� 
					point_x++;//�A�ץk�� 
					a++;
				}
				check=che(point_x,point_y,block[0],change[0]);//�T�{���L���| 
				if(check==1){
					point_x--;
					if(a==1)point_x--; //�P�_�k�����Ʀ^�_��� 
					if(change[0]>0)change[0]--;
					else change[0]=3;
					game_control=2;
					continue;
				}
				if(a==0){
				}
				else if(a==1){
				}
			}
			if(point_y+picture[block[0]][change[0]][1]>19 || point_y+picture[block[0]][change[0]][3]>19 || point_y+picture[block[0]][change[0]][5]>19){//�W�X�̤U�� 
				point_y--;//�W�� 
				int a=0;
				if(point_y+picture[block[0]][change[0]][1]>19 || point_y+picture[block[0]][change[0]][3]>19 || point_y+picture[block[0]][change[0]][5]>19){//�A�׶W�X�̤U�� 
					point_y--;// 
					a++;
				}
				check=che(point_x,point_y,block[0],change[0]);//�T�{���L���| 
				if(check==1){//���|�A�^�_��� 
					point_y++;
					if(a==1)point_y++; 
					if(change[0]>0)change[0]--;
					else change[0]=3;
					game_control=2;
					continue;
				}
			}
			check=che(point_x,point_y,block[0],change[0]);//�S�W�X��ɡA�T�{����ᦳ�L���| 
			if(check==1){
				if(change[0]>0)change[0]--;
				else change[0]=3;
				game_control=2;
				continue;
			}
			game_control=1;
			continue;
		}	
		else if(control==space){//�Ů�������
			check=0;
			do{
				point_y++;
				if(point_y+picture[block[0]][change[0]][1]<=19 && point_y+picture[block[0]][change[0]][3]<=19 && point_y+picture[block[0]][change[0]][5]<=19)
					check=che(point_x,point_y,block[0],change[0]);
			}while(check==0 && point_y+picture[block[0]][change[0]][1]<=19 && point_y+picture[block[0]][change[0]][3]<=19 && point_y+picture[block[0]][change[0]][5]<=19);
			point_y--;
			move_block(root[0],root[1],point_x,point_y,block[0],root[3],change[0],1);
			clean=0;
			line_check(point_y,block[0],change[0]);
			initialization(&point_x,&point_y,block,change);
			continue;
		}
		t1=clock();
		game_control=1;
		continue;
	}
	gameover();
}


void homepage_control(){//�D�������� 
	int option=0,control=0;
	home: 
	control=0;
	gotoxy(0,0);
	homepage(option);
	do{
		if(kbhit())control=getch();
	}while(control!=224 && control!=13);//���ݿ�J�W�U��enter��
	if(control==224){//�ܧ� 
		if(option==1)option=0;
		else option++;
		goto home;
	}
	switch(option){//���Uenter���P�_�ﶵ����ð��� 
		case 0:
			game();//�i�J�C�� 
			break;
		case 1://�����C�� 
			break;
	} 
}

void homepage(int option){//�D����� 
	int start1=0,close1=0;
	int start2=0,close2=0;
	switch(option){//��ܽb�Y 
		case 0:
			start1=62;
			start2=60;
			break;
		case 1:
			close1=62;
			close2=60;
			break;
	}
	printf("����������������������������������������������\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	//printf("������ ������  ������ �������� �� ������\n");
	//printf(" ��  ������   ��  �������� �� ��������\n");
	//printf(" ��  ������   ��  �� ��  ��  ������\n");
	printf("��                     ��\n");
	printf("��        TETRIS       ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��       %c START %c     ��\n",start1,start2);
	printf("��       %c CLOSE %c     ��\n",close1,close2);
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("����������������������������������������������\n");
}


void setting_control(){//�]�w���� 
	int option=0,chose=0,control=0;
	home:
	gotoxy(0,0);
	setting(option,chose);
	control=0;
	do{
		if(kbhit())control=getch();
	}while(control!=50 && control!=53 && control!=13 && control!=27);//���ݤW�� �U�� enter�� esc�� 
	if(control==27)return;//esc��h�X 
	if(control==50){//�U��P�_ 
		if(option==5)option=0;
		else option++;
		goto home;
	}
	else if(control==53){//�W��P�_ 
		if(option==0)option=5;
		else option--;
		goto home;
	}
	chose++;
	control=0;
	gotoxy(0,0);
	setting(option,chose);
	if(option==0){//���ɶ�(sec) 
		sec=0;
		gotoxy(0,0);
		setting(option,chose);
		do{
			if(kbhit()){
				control=getch();
				if(control==8)sec/=10;
				else if(control==13)break;
				else if(control<=57 && control>=48){
					sec=sec*10+control-48;
				if(sec>9999)sec=9999;
				}
				gotoxy(0,0);
				setting(option,chose);
			}
		}while(control!='\n');
		chose--;
		goto home;
	}
	else{
		do{//���o��J���� 
			if(kbhit())control=getch();
			if(control<=0 || control>126)control=0;
		}while(control==0);
		switch(option){//�N����]�w�s�J�W�U���k 
			case 1:
				down=control;
				break;
			case 2:
				right=control;
				break;
			case 3:
				left=control;
				break;
			case 4:
				up=control;
				break;
			case 5:
				space=control;
				break;
		}
		chose--;
		goto home;
	}		
}

void setting(int option,int chose){//�]�w������� 
	int fall_time1=0,fall_time2=0,fall_time3=0;//�ŧi�b�Y 
	int down1=0,down2=0,down3=0;
	int right1=0,right2=0,right3=0;
	int left1=0,left2=0,left3=0;
	int rotate1=0,rotate2=0,rotate3=0;
	int bottom1=0,bottom2=0,bottom3=0;
	switch(option){//��ܽb�Y 
		case 0:
			if(chose==0)fall_time1=62;
			else fall_time2=62;
			fall_time3=60;
			break;
		case 1:
			if(chose==0)down1=62;
			else down2=62;
			down3=60;
			break;
		case 2:
			if(chose==0)right1=62;
			else right2=62;
			right3=60;
			break;
		case 3:
			if(chose==0)left1=62;
			else left2=62;
			left3=60;
			break;
		case 4:
			if(chose==0)rotate1=62;
			else rotate2=62;
			rotate3=60;
			break;
		case 5:
			if(chose==0)bottom1=62;
			else bottom2=62;
			bottom3=60;
			break;
	}
	printf("����������������������������������������������\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��        TETRIS       ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("�� %c fall time %c %-4d %c��\n",fall_time1,fall_time2,sec,fall_time3);
	switch(down){
		case 8:
			printf("��     %c down %c Back %c ��\n",down1,down2,down3);
			break;
		case 9:
			printf("��     %c down %c Tab %c  ��\n",down1,down2,down3);
			break;
		case 13:
			printf("��     %c down %c Enter %c��\n",down1,down2,down3);
			break;
		case 32:
			printf("��     %c down %c Space %c��\n",down1,down2,down3);
			break;
		default:
			printf("��     %c down %c %c %c    ��\n",down1,down2,down,down3);
			break;
	}
	switch(right){
		case 8:
			printf("��    %c right %c Back %c ��\n",right1,right2,right3);
			break;
		case 9:
			printf("��    %c right %c Tab %c  ��\n",right1,right2,right3);
			break;
		case 13:
			printf("��    %c right %c Enter %c��\n",right1,right2,right3);
			break;
		case 32:
			printf("��    %c right %c Space %c��\n",right1,right2,right3);
			break;
		default:
			printf("��    %c right %c %c %c    ��\n",right1,right2,right,right3);
			break;		
	}
	switch(left){
		case 8:
			printf("��     %c left %c Back %c ��\n",left1,left2,left3);
			break;
		case 9:
			printf("��     %c left %c Tab %c  ��\n",left1,left2,left3);
			break;
		case 13:
			printf("��     %c left %c Enter %c��\n",left1,left2,left3);
			break;
		case 32:
			printf("��     %c left %c Space %c��\n",left1,left2,left3);
			break;
		default:
			printf("��     %c left %c %c %c    ��\n",left1,left2,left,left3);
			break;		
	}
	switch(up){
		case 8:
			printf("��   %c rotate %c Back %c ��\n",rotate1,rotate2,rotate3);
			break;
		case 9:
			printf("��   %c rotate %c Tab %c  ��\n",rotate1,rotate2,rotate3);
			break;
		case 13:
			printf("��   %c rotate %c Enter %c��\n",rotate1,rotate2,rotate3);
			break;
		case 32:
			printf("��   %c rotate %c Space %c��\n",rotate1,rotate2,rotate3);
			break;
		default:
			printf("��   %c rotate %c %c %c    ��\n",rotate1,rotate2,up,rotate3);
			break;		
	}
	switch(space){
		case 8:
			printf("��   %c bottom %c Back %c ��\n",bottom1,bottom2,bottom3);
			break;
		case 9:
			printf("��   %c bottom %c Tab %c  ��\n",bottom1,bottom2,bottom3);
			break;
		case 13:
			printf("��   %c bottom %c Enter %c��\n",bottom1,bottom2,bottom3);
			break;
		case 32:
			printf("��   %c bottom %c Space %c��\n",bottom1,bottom2,bottom3);
			break;
		default:
			printf("��   %c bottom %c %c %c    ��\n",bottom1,bottom2,space,bottom3);
			break;		
	}
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("��                     ��\n");
	printf("����������������������������������������������\n");
}

void show(){
	printf("������������������������������������������������������������������\n");
	int i=0,j=0;
	for(i=0;i<20;i++){
		printf("�� "); 
		for(j=0;j<10;j++){
			if(tetris[j][i][0]=='O'){
				switch(tetris[j][i][1]){
					case 0:
						SetColor(9);
						break;
					case 1:
						SetColor(3);
						break;
					case 2:
						SetColor(10);
						break;
					case 3:
						SetColor(11);
						break;
					case 4:
						SetColor(12);
						break;
					case 5:
						SetColor(14);
						break;
					case 6:
						SetColor(15);
						break;
				}
				printf("��");
				SetColor(7);
			}
			else if(tetris[j][i][0]=='.'){
				SetColor(8);
				printf("��");
				SetColor(7);
			}
			else printf("�@"); 
		}
		if(i<14 && i!=4 && i!=9 && i!=14)printf("�� �@�@�@�@��");
		else if(i==4 || i==9)printf("����������������������");
		else if(i==14)printf("����������������������");
		else printf("��");
		//if(i==0)printf("�@SCORE");
		//else if(i==1)printf("�@%d",score);
		printf("\n"); 
	} 
	printf("����������������������������������������������\n");
}

void pic(){//��J����۹�����m 
	picture[0][0][0]=-1;picture[0][0][3]=1;picture[0][0][4]=1;picture[0][0][5]=1;//Z�� 
	picture[0][1][0]=1;picture[0][1][1]=-1;picture[0][1][2]=1;picture[0][1][5]=1;
	picture[0][2][0]=-1;picture[0][2][3]=1;picture[0][2][4]=1;picture[0][2][5]=1;
	picture[0][3][0]=1;picture[0][3][1]=-1;picture[0][3][2]=1;picture[0][3][5]=1;
	 
	picture[1][0][1]=1;picture[1][0][2]=-1;picture[1][0][3]=-1;picture[1][0][4]=-1;//���� 
	picture[1][1][1]=1;picture[1][1][2]=-1;picture[1][1][3]=1;picture[1][1][4]=1;
	picture[1][2][1]=1;picture[1][2][2]=-1;picture[1][2][3]=-1;picture[1][2][4]=-1;
	picture[1][3][1]=1;picture[1][3][2]=-1;picture[1][3][3]=1;picture[1][3][4]=1;
	
	picture[2][0][0]=-1;picture[2][0][2]=-1;picture[2][0][3]=1;picture[2][0][5]=1;//�Ы� 
	picture[2][1][0]=-1;picture[2][1][2]=-1;picture[2][1][3]=1;picture[2][1][5]=1;
	picture[2][2][0]=-1;picture[2][2][2]=-1;picture[2][2][3]=1;picture[2][2][5]=1;
	picture[2][3][0]=-1;picture[2][3][2]=-1;picture[2][3][3]=1;picture[2][3][5]=1;
	
	picture[3][0][1]=-1;picture[3][0][3]=1;picture[3][0][5]=2;//1�� 
	picture[3][1][0]=-2;picture[3][1][2]=-1;picture[3][1][4]=1;
	picture[3][2][1]=-1;picture[3][2][3]=1;picture[3][2][5]=2;
	picture[3][3][0]=-2;picture[3][3][2]=-1;picture[3][3][4]=1;
	
	picture[4][0][1]=-1;picture[4][0][2]=1;picture[4][0][3]=-1;picture[4][0][5]=1;//�~�� 
	picture[4][1][0]=-1;picture[4][1][2]=1;picture[4][1][4]=1;picture[4][1][5]=1;
	picture[4][2][1]=-1;picture[4][2][3]=1;picture[4][2][4]=-1;picture[4][2][5]=1;
	picture[4][3][0]=-1;picture[4][3][1]=-1;picture[4][3][2]=-1;picture[4][3][4]=1;
	
	picture[5][0][1]=-1;picture[5][0][2]=-1;picture[5][0][3]=-1;picture[5][0][5]=1;//7�� 
	picture[5][1][0]=-1;picture[5][1][2]=1;picture[5][1][4]=1;picture[5][1][5]=-1;
	picture[5][2][1]=-1;picture[5][2][3]=1;picture[5][2][4]=1;picture[5][2][5]=1;
	picture[5][3][0]=-1;picture[5][3][2]=-1;picture[5][3][3]=1;picture[5][3][4]=1;
	
	picture[6][0][1]=-1;picture[6][0][2]=1;picture[6][0][4]=-1;//��T�� 
	picture[6][1][0]=1;picture[6][1][3]=-1;picture[6][1][5]=1;
	picture[6][2][0]=-1;picture[6][2][2]=1;picture[6][2][5]=1;
	picture[6][3][0]=-1;picture[6][3][3]=-1;picture[6][3][5]=1;
}

void mark(int point_x,int point_y,int block,int change,int clean){//�N�����m�s�J�βM��tetris�}�C�A�^�ǭ�0�L�W 1�M�� 
	char a;
	if(clean==0)a='O';
	else a=0;
	if(point_y>=0){
		tetris[point_x][point_y][0]=a;
		tetris[point_x][point_y][1]=block;
	}
	if(point_y+picture[block][change][1]>=0){
		tetris[point_x+picture[block][change][0]][point_y+picture[block][change][1]][0]=a;
		tetris[point_x+picture[block][change][0]][point_y+picture[block][change][1]][1]=block;
	}
	if(point_y+picture[block][change][3]>=0){
		tetris[point_x+picture[block][change][2]][point_y+picture[block][change][3]][0]=a;
		tetris[point_x+picture[block][change][2]][point_y+picture[block][change][3]][1]=block;
	}
	if(point_y+picture[block][change][5]>=0){
		tetris[point_x+picture[block][change][4]][point_y+picture[block][change][5]][0]=a;
		tetris[point_x+picture[block][change][4]][point_y+picture[block][change][5]][1]=block;
	}
}

int che(int point_x,int point_y,int block,int change){//�T�{�����m�O�_���_�A�^��1�����ơA0�S���� 
	if(point_y>=0){
		if(tetris[point_x][point_y][0]=='O')return 1;
	}
	if(point_y+picture[block][change][1]>=0){
		if(tetris[point_x+picture[block][change][0]][point_y+picture[block][change][1]][0]=='O')return 1;
	}
	if(point_y+picture[block][change][3]>=0){
		if(tetris[point_x+picture[block][change][2]][point_y+picture[block][change][3]][0]=='O')return 1;
	}
	if(point_y+picture[block][change][5]>=0){
		if(tetris[point_x+picture[block][change][4]][point_y+picture[block][change][5]][0]=='O')return 1;
	}
	return 0;
}


void line_check(int point_y,int block,int change){//�C�����ƭp�� 
	line[point_y]++;
	line[point_y+picture[block][change][1]]++;
	line[point_y+picture[block][change][3]]++;
	line[point_y+picture[block][change][5]]++;
	int i=0,j=0,k=0,count[20];
	for(i=0;i<20;i++)count[i]=-1;
	for(i=0;i<20;i++){
		if(line[i]==10){
			line[i]=0;
			row++;
			count[j]=i;
			j++;
			for(k=0;k<10;k++)
				tetris[i][k][0]==0;
		}
	}
	if(j>0){
		switch(j){
			case 1:
				score+=40;
				break;
			case 2:
				score+=100;
				break;
			case 3:
				score+=300;
					break;
			case 4:
				score+=1200;
				break;
		}
		for(i=0;i<5;i++){
			for(j=0;count[j]!=-1;j++){//�������ʵe 
				gotoxy(coordinate(4-i,0),coordinate(count[j],1));
				printf("�@");
				gotoxy(coordinate(5+i,0),coordinate(count[j],1));
				printf("�@");
				gotoxy(0,22); 
			}
			Sleep(50);
		}
		for(j=0;count[j]!=-1;j++){
			move(count[j]);
			gotoxy(0,22);
			Sleep(50);
		}
	}
}

void move(int y){//�N�ؼЦ�ƤW���Ҧ�������U���ʤ@�� 
	int i=0,j=0;
	for(i=y;i>0;i--){//�}�l���� 
		for(j=0;j<10;j++){
			tetris[j][i][0]=tetris[j][i-1][0];
			tetris[j][i][1]=tetris[j][i-1][1];
			gotoxy(coordinate(j,0),coordinate(i,1));
			if(tetris[j][i][0]=='O'){
				set_color(tetris[j][i][1]);
				printf("��");	
			}
			else
				printf("�@");
		}
		line[i]=line[i-1];//�C�����Ʋ��� 
	}
}

void gotoxy(int xpos, int ypos){
  COORD scrn;    
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}

void gameover(){//��ܥX���e�� 
	gotoxy(5,11);
	printf("+ Game Over +");
	gotoxy(0,22);
	system("pause");
}

int pause_control(){//�Ȱ��������� 
	int option=0,control=0;
	for(;;){
		//gotoxy(0,0);
		//show();
		control=0;
		pause(option);
		do{
			if(kbhit)control=getch();
		}while(control!=up && control!=down && control!=13 && control!=27);//���ݤW�U�� enter�� ������ 
		if(control==27){//esc��h�X
			gotoxy(0,0);
			show();
			return 0;
		} 
		if(option==2 && control==down)option=0;//�W�U��ﶵ�B�� 
		else if(option<2 && control==down)option++;
		else if(option==0 && control==up)option=2;
		else if(option>0 && control==up)option--;
		pause(option);
		if(control==13){//���Uenter�䱱�� 
			switch(option){
				case 0:
					return 1;
					break;
				case 1:
					setting_control();
					break;
				case 2:	
					return 0;		
					break;
			}
		}
	}
	
} 

void pause(int option){//�Ȱ�������� 
	int home1=0,home2=0;
	int setting1=0,setting2=0;
	int continue1=0,continue2=0;
	switch(option){
		case 0:
			home1=62;
			home2=60;
			break;
		case 1:
			setting1=62;
			setting2=60;
			break;
		case 2:
			continue1=62;
			continue2=60;
			break;
	}
	gotoxy(3,9);
	printf("����������������������������������\n");
	gotoxy(3,10);
	printf("��    %c HOME %c   ��",home1,home2);
	gotoxy(3,11);
	printf("��  %c SETTING %c  ��",setting1,setting2);
	gotoxy(3,12);
	printf("��  %c CONTINUE %c ��",continue1,continue2);
	gotoxy(3,13);
	printf("����������������������������������\n");
} 

void initialization(int *point_x,int *point_y,int *block,int *change){//��l�Ƽ��Ix y�ƭȡA���������ܡA����������� 
	*point_x=5;
	*point_y=-2;
	int i=0;
	for(i=0;i<3;i++){
		block[i]=block[i+1];
		change[i]=change[i+1];
	}
	block[3]=rand()%7;
	change[3]=rand()%4;
}

void shadow(int x2,int y2,int block,int change2,int clean){//�v�l���I�p��
	if(clean==0)return;
	int check,x1=root[0],y1=root[1],change1=root[3],temp=y2;
	mark(x2,y2,block,change2,1);//�M�������������K�B�� 
	y2=shadow_drop_point(x2,y2,block,change2);
	y1=shadow_drop_point(x1,y1,root[2],change1);
	mark(x2,temp,block,change2,0);//��_���������
	set_color(7);//��⬰�C��
	if(y2>=0 && y2<=19){//�л\�s��� 
		if(tetris[x2][y2][0]=='O' || tetris[x2][y2][0]=='.')
			tetris[x2][y2][1]=-1; 
		else{
			tetris[x2][y2][0]='.';
			tetris[x2][y2][1]=8;
			gotoxy((x2+1)*2,y2+1);
			printf("��");
		}
	}
	int i=0;
	for(i=0;i<3;i++){//�л\�s��� 
		if(y2+picture[block][change2][i*2+1]>=0 && y2+picture[block][change2][i*2+1]<=19){
			if(tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][0]=='O' || tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][0]=='.')
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][1]=-1;
			else{
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][0]='.';
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][1]=8;
				gotoxy((x2+picture[block][change2][i*2]+1)*2,y2+picture[block][change2][i*2+1]+1);
				printf("��");
			}
		}
	}
	SetColor(7);//��_���`�C�� 
	if(y1>=0 && y1<=19){//�M���¤�� 
		if(tetris[x1][y1][1]!=-1 && tetris[x1][y1][0]!='O'){
			tetris[x1][y1][0]=0; 
			gotoxy((x1+1)*2,y1+1);
			printf("�@");
		}
		else
			tetris[x1][y1][1]=block;
	}
	for(i=0;i<3;i++){//�M���¤�� 
		if(y1+picture[block][change1][i*2+1]>=0 && y1+picture[block][change1][i*2+1]<=19){
			if(tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][1]!=-1 && tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][0]!='O'){
				tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][0]=0;
				gotoxy((x1+picture[block][change1][i*2]+1)*2,y1+picture[block][change1][i*2+1]+1);
				printf("�@");
			} 		
			else 
				tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][1]=block;
		}
	}
	
	gotoxy(0,22);
}

int  shadow_drop_point(int point_x,int point_y,int block,int change){
	int check,a=0;
	for(;point_y+picture[block][change][1]<=19 && point_y+picture[block][change][3]<=19 && point_y+picture[block][change][5]<=19;point_y++){
		check=che(point_x,point_y,block,change);a++;
		if(check==1){
			break;
		}
	}
	point_y--;
	return point_y;
}

void shadow_new_block(int point_x,int point_y,int block,int change){
	mark_shadow(point_x,shadow_drop_point(point_x,point_y,block,change),block,change);
	set_color(7);
	if(shadow_drop_point(point_x,point_y,block,change)>=0 && shadow_drop_point(point_x,point_y,block,change)
	<=19){
		gotoxy((point_x+1)*2,shadow_drop_point(point_x,point_y,block,change)+1);
		printf("��");
	}
	int i;
	for(i=0;i<3;i++){
		if(shadow_drop_point(point_x,point_y,block,change)+picture[block][change][i*2+1]>=0 && shadow_drop_point(point_x,point_y,block,change)+picture[block][change][i*2+1]<=19){
			gotoxy((point_x+picture[block][change][i*2]+1)*2,shadow_drop_point(point_x,point_y,block,change)+picture[block][change][i*2+1]+1);
			printf("��");
		}
	}
	SetColor(7);
	gotoxy(0,22);
}

void mark_shadow(int point_x,int point_y,int block,int change){//��W�v�l
	int a='.';
	if(tetris[point_x][point_y][0]!='O')
		tetris[point_x][point_y][0]=a;
	int i=0;
	for(i=0;i<3;i++){
		if(tetris[point_x+picture[block][change][2*i]][point_y+picture[block][change][2*i+1]][0]!='O')
			tetris[point_x+picture[block][change][2*i]][point_y+picture[block][change][2*i+1]][0]=a;
	}
} 

void ready_block(int *block,int *change,int a){//��ܹw�Ƥ�� 
	if(a==3)return;
	set_color(block[a+1]); 
	int i,j,x=0,y=0,area[8][4]={0};//�T�{���I 
	if((block[a+1]==0 && (change[a+1]==1 || change[a+1]==3)) || (block[a+1]==5 && change[a+1]==2) || (block[a+1]==6 && change[a+1]==1)){
		x=2;
		y=1;
	}
	else if(block[a+1]==4 && change[a+1]==0){
		x=2;
		y=2;
	}
	else if((block[a+1]==0 && (change[a+1]==0 || change[a+1]==2)) || (block[a+1]==1 && (change[a+1]==1 || change[a+1]==3)) || (block[a+1]==3 && (change[a+1]==0 || change[a+1]==2)) || (block[a+1]==4 && change[a+1]==1) || (block[a+1]==5 && change[a+1]==3) || (block[a+1]==6 && change[a+1]==2)){
		x=3;
		y=1;
	}
	else if((block[a+1]==4 && change[a+1]==3) || (block[a+1]==5 && change[a+1]==1) || (block[a+1]==6 && change[a+1]==0)){
		x=3;
		y=2;
	}
	else if((block[a+1]==1 && (change[a+1]==0 || change[a+1]==2))  || (block[a+1]==2 && (change[a+1]==0 ||change[a+1]==1 || change[a+1]==2 || change[a+1]==3)) ||(block[a+1]==3 && (change[a+1]==1 || change[a+1]==3)) || (block[a+1]==4 && change[a+1]==2) || (block[a+1]==6 && change[a+1]==3)){
		x=4;
		y=1;
	}
	else if(block[a+1]==5 && change[a+1]==0){
		x=4;
		y=2;
	}
	area[x][y]='O';
	for(i=0;i<3;i++){
		area[x+(picture[block[a+1]][change[a+1]][i*2])*2][y+picture[block[a+1]][change[a+1]][i*2+1]]='O';
	}
	for(i=0;i<4;i++){
		for(j=0;j<8;j++){
			gotoxy(24+j,1+i+a*5);
			if(area[j][i]=='O'){
				printf("��");
				j++; 
			}
			else if(area[j][i]==0)
				printf(" "); 
		}
	}
	gotoxy(0,22);
	SetColor(7);
	ready_block(block,change,a+1);
}

void set_color(int block){
	switch(block){
		case 0:
			SetColor(9);
			break;
		case 1:
			SetColor(3);
			break;
		case 2:
			SetColor(10);
			break;
		case 3:
			SetColor(11);
			break;
		case 4:
			SetColor(12);
			break;
		case 5:
			SetColor(14);
			break;
		case 6:
			SetColor(15);
			break;
		case 7://�v�l 
			SetColor(8);
			break;
	}
}

void move_block(int x1,int y1,int x2,int y2,int block,int change1,int change2,int clean){
	set_color(block);//��⬰����C��
	if(clean==1)mark(root[0],root[1],root[2],root[3],0);
	int a='O';
	//if(block2==7)a='.';
	if(y2>=0 && y2<=19){//�л\�s��� 
		if(tetris[x2][y2][0]=='O')
			tetris[x2][y2][1]=-1; 
		else{
			tetris[x2][y2][0]=a;
			tetris[x2][y2][1]=block;
			gotoxy((x2+1)*2,y2+1);
			printf("��");
		}
	}
	int i=0;
	for(i=0;i<3;i++){//�л\�s��� 
		if(y2+picture[block][change2][i*2+1]>=0 && y2+picture[block][change2][i*2+1]<=19){
			if(tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][0]=='O')
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][1]=-1;
			else{
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][0]=a;
				tetris[x2+picture[block][change2][i*2]][y2+picture[block][change2][i*2+1]][1]=block;
				gotoxy((x2+picture[block][change2][i*2]+1)*2,y2+picture[block][change2][i*2+1]+1);
				printf("��");
			}
		}
	}
	SetColor(7);//��_���`�C��
	if(clean==1){//����ۦP �M���¤�� 
		if(y1>=0 && y1<=19){//�M���¤�� 
			if(tetris[x1][y1][1]!=-1){
				tetris[x1][y1][0]=0; 
				gotoxy((x1+1)*2,y1+1);
				printf("�@");
			}
			else
				tetris[x1][y1][1]=block;
		}
		for(i=0;i<3;i++){//�M���¤�� 
			if(y1+picture[block][change1][i*2+1]>=0 && y1+picture[block][change1][i*2+1]<=19){
				if(tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][1]!=-1){
					tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][0]=0;
					gotoxy((x1+picture[block][change1][i*2]+1)*2,y1+picture[block][change1][i*2+1]+1);
					printf("�@");
				} 		
				else 
					tetris[x1+picture[block][change1][i*2]][y1+picture[block][change1][i*2+1]][1]=block;
			}
		}
	}
	gotoxy(0,22);
}

int coordinate(int a,int xy){//�ഫgotoxy�y��  0�ഫx 1�ഫy 
	if(xy==0)return (a+1)*2;
	else if(xy==1)return a+1;
}

void set_root(int point_x,int point_y,int block,int change){
	root[0]=point_x;
	root[1]=point_y;
	root[2]=block;
	root[3]=change;
}

void SetColor(int ForgC){
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	//We use csbi for the wAttributes word.
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
	//Mask out all but the background attribute, and add in the forgournd color
    wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
	SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
