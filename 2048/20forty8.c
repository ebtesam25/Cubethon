#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define SIZE 4



#define WW 45
#define WH 15



struct player * initnode( char * );
struct player * printnode( struct player *ptr );
void printlist( struct player * );
struct player * add( struct player * );
void insertnode( struct player * );
void load( struct player *f );
void open();
void smenu();
void display_p(struct player *info);
struct player *find(char *name);
struct player *findg(char *group);
void delete(struct player **head, struct player **end);

struct player
{
    char name[30];
    int scre;
    struct player* next;
    struct player* previous;
};


struct player *head = (struct player *) NULL;
struct player *end = (struct player *) NULL;


int Matrix[SIZE+1][SIZE+1]={0};
int last=2048;


// Global variables
bool anim;
char emptyCell[5];
char cell[4][4][5];
int score, sscore;
bool gameEnded;

// Prototypes
void gotoXY(unsigned short,unsigned short);
void setColor(char*);
void center(char*,unsigned short);
void init();
void welcomeView();
void endView();
void display();
bool isEmpty(char*);
void addRandomValue(bool);
short mergeable(short,short);
short merge(char*,char*,bool);
unsigned short getAction();
bool doProcess(unsigned short);


void gotoXY(unsigned short x,unsigned short y){
 HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
 COORD position={x,y};
 SetConsoleCursorPosition(handle,position);
}
void setColor(char *cell1){
 unsigned short cColor;
 switch(atoi(cell1)){
  case 2: cColor=112; break;
  case 4: cColor=128; break;
  case 8: cColor=48; break;
  case 16: cColor=32; break;
  case 32: cColor=72; break;
  case 64: cColor=71; break;
  case 128: cColor=96; break;
  case 256: cColor=103; break;
  case 512: cColor=103; break;
  case 1024: cColor=160; break;
  case 2048: cColor=224; break;
  case 4096: cColor=224; break;
  default: cColor=7; break;
 }
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),cColor);
}
void center(char *str, unsigned short y){
 gotoXY((WW-strlen(str))/2,y);
 printf("%s",str);
}



void init(){
 HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
 SMALL_RECT windowSize = {0,0,WW,WH};
 SetConsoleWindowInfo(handle,1,&windowSize);

 CONSOLE_SCREEN_BUFFER_INFO info;
 GetConsoleScreenBufferInfo(handle,&info);
 COORD new_size = {info.srWindow.Right-info.srWindow.Left+1,info.srWindow.Bottom-info.srWindow.Top+1};
 SetConsoleScreenBufferSize(handle,new_size);

 srand(time(NULL));

 strcpy(emptyCell,"    ");

 anim = false;
 gameEnded = false;

 unsigned short x,y;
 for(x=0; x<4; x++){
  for(y=0; y<4; y++){
   strcpy(cell[x][y], emptyCell);
  }
 }

 score = 0;

 addRandomValue(true);
 addRandomValue(true);
}


void welcomeView(){
 center("Press Enter to start",5);
 center("",WH);

 while(getch() != 13){
 }

 system("cls");
}
void endView(){
 system("cls");
 sscore=score;
 //printf("%d",sscore);
 gameEnded = true;

 center("Enter=Restart", 10);
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
 center("s=Score Records", 10);
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
 center("You lost! Better Luck next time!", 6);
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
 center("Esc=exit", 11);
 setColor("");

 display();

 gotoXY(WW,WH);

 bool checkAction = true;
 unsigned short ch;
 while(checkAction){
  ch = getch();
  if (ch == 13){
   checkAction = false;
   system("cls");
   init();
  } else if (ch == 27){
   exit(0);
  }
   else if(ch == 's'){
    score_record();
  }
 }
}

void display(){
 unsigned short x,y;
 if (!gameEnded){
  for (x=0; x<4; x++){
   gotoXY((WW-20)/2,5+x);
   for(y = 0; y < 4; y++){
    printf("%c",221);
    setColor(cell[x][y]);
    printf("%s", cell[x][y]);
    setColor("");
   }
   printf("%c",221);
  }
 }

 char str[13] = "Score : ";
 char sc[5];
 sprintf(sc,"%d",score);
 strcat(str,sc);
 center(str,2);

 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
 gotoXY(WW,0);
 printf(" ");
 gotoXY(WW-9,0);
 setColor("");

 center("",WH);
}



bool isEmpty(char *cell1){
 return strcmp(cell1,emptyCell) ? false : true;
}

void addRandomValue(bool add){
 unsigned short x,y;

 if (add){
  do{
   x = rand()%4;
   y = rand()%4;
  } while (!isEmpty(cell[x][y]));

  char str[5];
  sprintf(str,"%4d",(rand()%2+1)*2);
  strcpy(cell[x][y], str);

  bool end = true;
  bool fullBoard = true;
  for(x=0; x<4; x++){
   for(y=0; y<4; y++){
    if (isEmpty(cell[x][y])){
     fullBoard = false;
    }
    if (mergeable(x,y) > 0){
     end = false;
    }
   }
  }
  if (end && fullBoard){
   endView();
  }

 }
}

short mergeable(short r, short c){
 unsigned short i=0;

 if (c > -1){
  if (!isEmpty(cell[0][c])){
   if (!strcmp(cell[0][c],cell[1][c])){
    i++;
   } else if (!strcmp(cell[0][c],cell[2][c]) && isEmpty(cell[1][c])){
    i++;
   } else if (!strcmp(cell[0][c],cell[3][c]) && isEmpty(cell[1][c]) && isEmpty(cell[2][c])){
    i++;
   }
  }

  if (!isEmpty(cell[1][c]) && i == 0){
   if (!strcmp(cell[1][c],cell[2][c])){
    i++;
   } else if (!strcmp(cell[1][c],cell[3][c]) && isEmpty(cell[2][c])){
    i++;
   }
  }

  if (!strcmp(cell[2][c],cell[3][c]) && !isEmpty(cell[2][c])){
   i++;
  }
 }

 if (r > -1){
  if (!isEmpty(cell[r][0])){
   if (!strcmp(cell[r][0],cell[r][1])){
    i++;
   } else if (!strcmp(cell[r][0],cell[r][2]) && isEmpty(cell[r][1])){
    i++;
   } else if (!strcmp(cell[r][0],cell[r][3]) && isEmpty(cell[r][1]) && isEmpty(cell[r][2])){
    i++;
   }
  }

  if (!isEmpty(cell[r][1]) && i == 0){
   if (!strcmp(cell[r][1],cell[r][2])){
    i++;
   } else if (!strcmp(cell[r][1],cell[r][3]) && isEmpty(cell[r][2])){
    i++;
   }
  }

  if (!strcmp(cell[r][2],cell[r][3]) && !isEmpty(cell[r][2])){
   i++;
  }
 }

 return i;
}
short merge(char *cell1, char *cell2, bool dontMerge){
 if (anim){
  clock_t wait = clock()+0.002*CLOCKS_PER_SEC;
  while(clock()<wait){}
  display();
 }

 if (isEmpty(cell2)){ //Nothing
  return -1;
 }

 if (isEmpty(cell1)){ //Move
  strcpy(cell1,cell2);
  strcpy(cell2,emptyCell);
  return 0;
 } else if (!strcmp(cell1, cell2) && !dontMerge){ //Merge
  strcpy(cell2,emptyCell);

  char str[5];
  sprintf(str,"%4d",atoi(cell1)*2);
  strcpy(cell1,str);
  score += atoi(cell1);
  return 1;
 } else {
  return -1;
 }
}

unsigned short getAction(){
 unsigned short ch;
 do{
  ch = getch();
  if (ch == 0xE0){
   switch(getch()){
    case 72: return 0; break;
    case 77: return 1; break;
    case 80: return 2; break;
    case 75: return 3; break;
   }
  } else if (ch == 97){
   anim = !anim;
   display();
  }
 } while(1);
}

bool doProcess(unsigned short direction){
 short x,y,mergeStatus,nbMerge;
 bool redo;
 bool done = false, merged = false;

 switch (direction){
  case 0:
  for(y=0; y<4; y++){
   redo = true;
   merged = false;
   nbMerge = mergeable(-1,y);
   while(redo){
    nbMerge--;
    redo = false;
    for(x=0; x<3; x++){
     mergeStatus = merge(cell[x][y],cell[x+1][y], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 1:
  for(x=0; x<4; x++){
   redo = true;
   merged = false;
   nbMerge = mergeable(x,-1);
   while(redo){
    nbMerge--;
    redo = false;
    for(y=3; y>0; y--){
     mergeStatus = merge(cell[x][y],cell[x][y-1], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 2:
  for(y=0; y<4; y++){
   redo = true;
   merged = false;
   nbMerge = mergeable(-1,y);
   while(redo){
    nbMerge--;
    redo = false;
    for(x = 3; x>0; x--){
     mergeStatus = merge(cell[x][y],cell[x-1][y], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 3:
  for(x=0; x<4; x++){
   redo = true;
   merged = false;
   nbMerge = mergeable(x,-1);
   while(redo){
    nbMerge--;
    redo = false;
    for(y=0; y < 3; y++){
     mergeStatus = merge(cell[x][y],cell[x][y+1], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;
 }

 return done;
}



void wait()
{
    int i;
    for(i=0;i<4500000;i++);
}


void colors()
{
    int i;
    for(i=0;i<25;i++)
    {
        wait();
    }
}
void vname()
{
    int i=1;
    system("cls");
    system("color F");
    printf("\n\n\n\n\n\n   CCC");wait();printf("CCC");wait();printf("CCC");wait(); printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();
    printf("\n");wait();printf("    ");wait();printf(" ");wait();printf(" ");wait();printf(" ");wait();printf(" ");wait();printf(" ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();
    printf("\n");wait();printf("   ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();
    printf("\n");wait();printf("   ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();
    printf("\n");wait();printf("   ");wait();printf("CCC");wait();printf("   ");wait();printf("   ");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("   ");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("   ");wait();printf("CCC");wait();
    printf("\n");wait();printf("   ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf(" ");wait();printf("   ");wait();printf("   ");wait();printf("CCC");wait();printf(" ");wait();printf("CCC");wait();printf("CCC");wait();printf("CCC");wait();printf("\n\n");wait();
    getch();
    colors();
    printf("\n\t\t\t\t\t\tLOADING ");
    while(i<2)
    {
        system("color 9");
        colors();
        system("color 1");
        colors();
        system("color 9");
        colors();
        system("color A");
        colors();
        system("color 2");
        colors();
        system("color A");
        colors();
        system("color B");
        colors();
        system("color 3");
        colors();
        system("color B");
        colors();
        system("color C");
        colors();
        system("color 4");
        colors();
        system("color C");
        colors();
        system("color D");
        colors();
        system("color 5");
        colors();
        system("color D");
        colors();
        system("color E");
        colors();
        system("color 6");
        colors();
        system("color E");
        colors();
        system("color F");
        i++;
    }
    return 0;

}
void instructions(){
    system("cls");
    printf("\n\n\n\t\t\tINSTRUCTIONS\n\n\n");
    printf(" ->  Press arrow keys to move\n\n");
    printf(" ->  The objective of the game is to get the number 2048 using additions of the number 2 and its multiples. You will have a grid of 16 tiles.\n\n");
    printf(" -> You cannot move diagonally\n\n\n\n");
    printf("\t\t\t\t\t\tPRESS ANY KEY TO PLAY\n");
}

void load( struct player *f )
{
	//struct player *f;
	FILE *fp;

	fp = fopen("player.txt", "rb+");
	if(!fp)
	{
		printf("Cannot open file..!!\n");
		exit(1);
	}
	printf("\nSaving File..\n");

	f = head;
	while(f)
	{
		fwrite(f, sizeof(struct player), 1, fp);
		f = f->next;
	}
	fclose(fp);
}


void open()
{
	struct player *info;
	FILE *fp;

	fp = fopen("player.txt", "rb+");
	if(!fp)
	{
		printf("Cannot open file..!!\n");
		exit(1);
	}


	while(head)
	{
		info = head->next;
		free(info);
		head = info;
	}


	head = end = NULL;

	printf("\nLoading file..\n");
	while(!feof(fp))
	{
		info = (struct player *) malloc(sizeof(struct player));
		if(!info)
		{
			printf("Out of Memory..!!");
			return;
		}
		if(1 != fread(info, sizeof(struct player), 1, fp)) break;
		insertnode(info);
	}
	fclose(fp);
}

struct player * initnode( char *name )
{
    struct player *ptr;
    ptr = (struct player *) malloc( sizeof(struct player ) );
    if( ptr == NULL )
        return (struct player *) NULL;
    else
    {
        strcpy( ptr->name, name );
        add(ptr);
        return ptr;
    }
}
struct player * printnode( struct player *ptr )
{
    int i;
    printf("\nPlayer Details:\n");
    printf ("Player Name : %s\n", ptr -> name);
    printf ("Score: %d",ptr -> scre);

    printf("\n");
    getch();
    return ptr;
}

void printlist( struct player *ptr )
{
    //struct player *head = ptr;
    while( ptr != NULL )
    {
        printnode( ptr );
        ptr = ptr->next;
    }
}

struct player * add( struct player *ptr )
{
    struct player *current;
    int n_scre;
    int i;



    n_scre=sscore;
    ptr->scre=n_scre;
    ptr -> next = NULL;
    return ptr;
}


void insertnode( struct player *new )
{
    struct player *temp, *prev;

    if( head == NULL )
    {
        head = new;
        end = new;
        head->next = NULL;
        return;
    }

    temp = head;
    while( strcmp( temp->name, new->name) < 0 )
    {
        temp = temp->next;
        if( temp == NULL )
            break;
    }


    if( temp == head )
    {
        new->next = head;
        head = new;
    }
    else
    {
        prev = head;
        while( prev->next != temp )
        {
            prev = prev->next;
        }
        prev->next = new;
        new->next = temp;
        if( end == prev )
            end = new;
    }
}
void search(void)
{
	char name[40];
	struct player *info;

	printf("Enter name to find: ");
	gets(name);
	//strupr(name);
	info = find(name);
	if(!info)
	{
		printf("Not found\n");
	}
	else
	{
		display(info);
	}
}
struct player *find(char *name)
{
	struct player *info;

	info = head;
	while(info)
	{
		if(strcmp(name, info->name)==0)
            return info;
		info = info->next;
	}
	printf("Name not found\n");
	return NULL;
}



void display_p(struct player *info)
{
	printf("%s\n", info->name);
	printf("%d\n", info->scre);
	printf("\n\n");
}

void delete(struct player **head, struct player **end)
{
    char x;
	struct player *info;
	char name[80];
    printf("Enter a name:");
	gets(name);

	info = find(name);
	if(info)
	{
		if(*head==info)
		{
			*head=info->next;
			if(*head)
			{
				(*head)->previous = NULL;
			}
			else
			{
				*end = NULL;
			}
		}
		else
		{
			info->previous->next = info->next;
			if(info!=*end)
			{
				info->next->previous = info->previous;
			}
			else
			{
				*end = info->previous;
			}
		}
		printf("Are you sure you want to delete this player?(Y/N)");
		//fflush(stdin);
		getche(x);
		if(x=='y')
		free(info);
		else
            return;
	}
}
int score_record()
{
    struct player *ptr;
    char name[30];
    open();
    while(1)
    {

        int op;



        char menu[5][32],menuc[5][32],menul[5][32], key, key2;
        int i = 0;
        int item = 1;
        strcpy(menu[0], "   save score");
        strcpy(menu[1], "   view all players");
        strcpy(menu[2], "   play");
        strcpy(menu[3], "   search players");
        strcpy(menu[4], "   delete a player");
        strcpy(menuc[0], "   SAVE SCORE");
        strcpy(menuc[1], "   VIEW ALL PLAYERS");
        strcpy(menuc[2], "   PLAY");
        strcpy(menuc[3], "   SEARCH PLAYERS");
        strcpy(menuc[4], "   DELETE A PLAYER");
        strcpy(menul[0], "   save score");
        strcpy(menul[1], "   view all players");
        strcpy(menul[2], "   play");
        strcpy(menul[3], "   search players");
        strcpy(menul[4], "   delete a player");
        while(1)
        {
            system("cls");
            printf("%c%c%c%c%cSCORE RECORDS%c%c%c%c%c\n",220,220,220,220,220,220,220,220,220,220);
            printf("%s\n%s\n%s\n%s\n%s\n", menu[0], menu[1], menu[2],menu[3],menu[4]);
            key = getch();
            key2 = 0;
            if(key == -32)
            {
                key2 = getch();
            }
            else if(key == 13)
            {
                op=item;
                break;
            }
            else
            {
                continue;
            }
            if(key2 == 80)
            {
                item++;
            }
            else if(key2 == 72)
            {
                item--;
            }


            if(item < 1)
                item = 1;
            if(item > 5)
                item = 5;


            menu[1][0] = ' ';
            menu[2][0] = ' ';
            menu[3][0] = ' ';
            menu[4][0] = ' ';
            strcpy(menu[0],menul[0]);
            strcpy(menu[1],menul[1]);
            strcpy(menu[2],menul[2]);
            strcpy(menu[3],menul[3]);
            strcpy(menu[4],menul[4]);
            strcpy(menu[item-1],menuc[item-1]);
            menu[item-1][0]='>';

        }

        switch(op)
        {
        case 1 :
            printf("Player name:");
            fflush(stdin);
            scanf("%s",name);
            strupr(name);
            ptr = initnode( name);
            insertnode( ptr );
            break;

        case 2 :
            printlist(head);
            break;

        case 3 :
            load(head);
            return 0;
            exit(1);
            break;

        case 4:
            search();
        case 5:
            delete(&head,&end);

        }
        system("cls");
    }
}

int main(){
SetConsoleTitle("TWO-ZERO-FOUR-EIGHT || EBTESAM");
    vname();
instructions();
 init();

 welcomeView();

 while(1){
  display();
  addRandomValue(doProcess(getAction()));
 }


 return 0;

}
