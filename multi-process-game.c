#include <stdio.h>
#include <stdlib.h> // atoi, srandom, random
// add here other necessary include directives
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


#define DEAD  '.'
#define ALIVE 'o'

#define BLINKER 0
#define TOAD 1
#define BEACON 2
#define PULSAR 3
#define PENTADECATHLON 4
#define RANDOM 5

#define NLINES_RANDOM 20
#define NCOLUMNS_RANDOM 100

#define NWORKERS 5

unsigned int GLOBAL_nlines, GLOBAL_ncolumns, GLOBAL_totalcells;
char *GLOBAL_grid_A=NULL, *GLOBAL_grid_B=NULL;

// -1 for INFINITE
//#define NITERS -1
#define NITERS 2000

//////////////////////////////////////////////////////////////////////
#include <termcap.h>
void clearScreen() 
{
 char buf[1024]; char *str;

 tgetent(buf, getenv("TERM"));
 str = tgetstr("cl", NULL);
 fputs(str, stdout);
}

//////////////////////////////////////////////////////////////////////
void gridShow(char *grid)
{
 // shows the grid in the screen, one grid line per screen line;
 // do not forget to call clearScreen before calling gridShow
 clearScreen();
 
 for(int i=0; i<GLOBAL_totalcells; i++)
 {
	if(i%GLOBAL_ncolumns==0)
		printf("\n");
	printf("%c",grid[i]);
 }
 printf("\n");
}

////////////////////////////////////////////////////////////////////
void defineDimensions(int pattern) 
{ 
 // define number of lines (GLOBAL_nlines) and columns (GLOBAL_coluns)
 // of the grids, depending on the pattern (BLINKER, TOAD, ..., RANDOM);
 // for the specific number of lines and columns of each pattern see 
 // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life ;
 // if pattern is RANDOM you must use NLINES_RANDOM and NCOLUMNS_RANDOM
 switch(pattern)
 {
	 case BLINKER:
		GLOBAL_nlines = 5;
		GLOBAL_ncolumns = 5;
		break;
	 case TOAD:
		GLOBAL_nlines = 6;
		GLOBAL_ncolumns = 6;
		break;
	 case BEACON:
		GLOBAL_nlines = 6;
		GLOBAL_ncolumns = 6;
		break;
	 case PULSAR:
		GLOBAL_nlines = 17;
		GLOBAL_ncolumns = 17;
		break;
	 case PENTADECATHLON:
		GLOBAL_nlines = 18;
		GLOBAL_ncolumns = 11;
		break;
	 case RANDOM:
		GLOBAL_nlines = NLINES_RANDOM;
		GLOBAL_ncolumns = NCOLUMNS_RANDOM;
		break;
 }
 GLOBAL_totalcells = GLOBAL_nlines*GLOBAL_ncolumns;
}

////////////////////////////////////////////////////////////////////
void gridInit(char *grid, int pattern)
{
 // initializes the starting grid depending on pattern (BLINKER,...);
 // for the initial state (dead/alive) of each cell see 
 // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life ;
 // if pattern is RANDOM you must define the state of the grid cells
 // in a random way, ensuring only a fraction of 10% are alive cells
 srand(time(NULL));
 switch(pattern)
 {
	 case BLINKER:
		for(int i=0;i<25;i++)
			grid[i]=DEAD;
		grid[11] = ALIVE;
		grid[12] = ALIVE;
		grid[13] = ALIVE;
		break;
	 case TOAD:
		for(int i=0;i<36;i++)
			grid[i]=DEAD;
		grid[9] = ALIVE;
		grid[13] = ALIVE;
		grid[16] = ALIVE;
		grid[19] = ALIVE;
		grid[22] = ALIVE;
		grid[26] = ALIVE;
		break;
	 case BEACON:
		for(int i=0;i<36;i++)
			grid[i]=DEAD;
		grid[7] = ALIVE;
		grid[8] = ALIVE;
		grid[13] = ALIVE;
		grid[22] = ALIVE;
		grid[27] = ALIVE;
		grid[28] = ALIVE;
		break;
	 case PULSAR:
		for(int i=0;i<289;i++)
			grid[i]=DEAD;
		grid[38] = ALIVE;
		grid[39] = ALIVE;
		grid[40] = ALIVE;
		grid[44] = ALIVE;
		grid[45] = ALIVE;
		grid[46] = ALIVE;
		
		grid[70] = ALIVE;
		grid[75] = ALIVE;
		grid[77] = ALIVE;
		grid[82] = ALIVE;
		
		grid[87] = ALIVE;
		grid[92] = ALIVE;
		grid[94] = ALIVE;
		grid[99] = ALIVE;
		
		grid[104] = ALIVE;
		grid[109] = ALIVE;
		grid[111] = ALIVE;
		grid[116] = ALIVE;
		
		grid[123] = ALIVE;
		grid[124] = ALIVE;
		grid[125] = ALIVE;
		grid[129] = ALIVE;
		grid[130] = ALIVE;
		grid[131] = ALIVE;
		
		grid[157] = ALIVE;
		grid[158] = ALIVE;
		grid[159] = ALIVE;
		grid[163] = ALIVE;
		grid[164] = ALIVE;
		grid[165] = ALIVE;
		
		grid[172] = ALIVE;
		grid[177] = ALIVE;
		grid[179] = ALIVE;
		grid[184] = ALIVE;
		
		grid[189] = ALIVE;
		grid[194] = ALIVE;
		grid[196] = ALIVE;
		grid[201] = ALIVE;
		
		grid[206] = ALIVE;
		grid[211] = ALIVE;
		grid[213] = ALIVE;
		grid[218] = ALIVE;
		
		grid[242] = ALIVE;
		grid[243] = ALIVE;
		grid[244] = ALIVE;
		grid[248] = ALIVE;
		grid[249] = ALIVE;
		grid[250] = ALIVE;
		break;
	 case PENTADECATHLON:
		for(int i=0;i<198;i++)
			grid[i]=DEAD;
		for(int i=0;i<=7;i++)
		{
			int x=59+i*11;
			grid[x] = ALIVE;
			grid[x+1] = ALIVE;
			grid[x+2] = ALIVE;
		}
		grid[71] = DEAD;
		grid[126] = DEAD;
		break;
	 case RANDOM:
		int random_number, random_len, random_10;
		random_len = NLINES_RANDOM*NCOLUMNS_RANDOM;
		random_10 = random_len/10;
		
		for(int i=0;i<random_len;i++)
			grid[i]=DEAD;
			
		for(int i=0;i<=random_10;i++)
		{
			do{
				random_number = rand()%random_len;
			}while(grid[random_number]==ALIVE);
			grid[random_number]=ALIVE;
		}
		break;
 }
}

////////////////////////////////////////////////////////////////////////
void gridEvolve(char *current, char *next)
{
 // generates the next grid depending on the content of the current grid,
 // by applying the game of life rules to each cell of the current grid
	int i, j, k, r, c, count;
	for(i=0;i<(GLOBAL_totalcells);i++)
	{
		count=0;
		r=i/GLOBAL_ncolumns;
		c=i%GLOBAL_ncolumns;
		for(j=r-1; j<=r+1; j++){
			for(k=c-1;k<=c+1;k++){
				if((j==r && k==c) || (j<0 || k<0) || (j>=GLOBAL_nlines || k>=GLOBAL_ncolumns)){
					continue;
				}
				int index = (j * GLOBAL_ncolumns) + k;
				if(current[index]==ALIVE){
					count++;
				}
			}
		}
		
		if((current[i]==ALIVE) && (count<2 || count>3))
		{
			next[i]=DEAD;
		}		
		else if((current[i]==DEAD) && (count==3))
			next[i]=ALIVE;
		else
			next[i]=current[i];
	}
}

//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
void gridEvolveTrimmed(char *current, char *next, int start, int end)
{
 // generates the next grid depending on the content of the current grid,
 // by applying the game of life rules to each cell of the current grid
	int i, j, k, r, c, count;
	for(i=start;i<end;i++)
	{
		count=0;
		r=i/GLOBAL_ncolumns;
		c=i%GLOBAL_ncolumns;
		for(j=r-1; j<=r+1; j++){
			for(k=c-1;k<=c+1;k++){
				if((j==r && k==c) || (j<0 || k<0) || (j>=GLOBAL_nlines || k>=GLOBAL_ncolumns)){
					continue;
				}
				int index = (j * GLOBAL_ncolumns) + k;
				if(current[index]==ALIVE){
					count++;
				}
			}
		}
		
		if((current[i]==ALIVE) && (count<2 || count>3))
		{
			next[i]=DEAD;
		}		
		else if((current[i]==DEAD) && (count==3))
			next[i]=ALIVE;
		else
			next[i]=current[i];
	}
}

//////////////////////////////////////////////////////////////////////


int gridCompare(char *gridA, char *gridB)
{
 // compares the two input grids, returning 0 if they are equal;
 // usefull to compare the current and the next grid to see if they
 // are equal, in which case the game converged to a final state
 for(int i=0;i<GLOBAL_totalcells;i++)
 {
	 if(gridA[i] == gridB[i]) continue;
	 return 1;
 }
 return 0;
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
 int pattern, semid, shmid, shmid2, trim, niters=0; 
 int *counter;
 char* shmaddr;
 struct sembuf sop;
 union semun { int val; unsigned short *array;} arg;
 
 setbuf(stdout, NULL);
 semid=semget(IPC_PRIVATE, 3, 00600);
 // semaphore 0 : to block the Parent
 // semaphore 1 : to block the Children
 // semaphore 2 : to block each Child 
 

 shmid2=shmget(IPC_PRIVATE, sizeof(int), 00600);
 counter=(int*)shmat(shmid2, (void*)0, 0);
 *counter = 0;
 
 
 arg.val=0;
 semctl(semid, 0, SETVAL, arg);
 semctl(semid, 1, SETVAL, arg);
 arg.val=1;
 semctl(semid, 2, SETVAL, arg);
 sop.sem_flg=0;
 
 // define initial pattern depending on the command line parameters
 pattern = atoi(argv[1]);
 // define grids dimensions (GLOBAL_nlines and GLOBAL_ncolumns) depending on pattern
 
 defineDimensions(pattern); 
 
 shmid=shmget(IPC_PRIVATE, GLOBAL_totalcells * sizeof(char), 00600 | IPC_CREAT);
 shmaddr=(char*)shmat(shmid, (void*)0, 0);
 strcpy(shmaddr," ");
 
 int slides = (GLOBAL_totalcells) / NWORKERS;
 int res = (GLOBAL_totalcells) % NWORKERS;
 
 // alloc the working grids (GLOBAL_grid_A and GLOBAL_grid_B) in dynamic memory
 GLOBAL_grid_A = (char*) malloc(GLOBAL_totalcells * sizeof(char));
 GLOBAL_grid_B = (char*) malloc(GLOBAL_totalcells * sizeof(char));

 // initialize the seed grid (GLOBAL_grid_A) depending on the chosen pattern
 gridInit(GLOBAL_grid_A,pattern);
 memcpy(GLOBAL_grid_B, GLOBAL_grid_A,GLOBAL_totalcells * sizeof(char));
 
 
 // main loop: WHILE (maximum number of iterations not reached AND convergence not achieved)
 //                 show current generation
 //                 define next generation
 //                 if (convergence not achieved)
 //                     swap pointers between current and next generation

 //while(1)
 
 while((niters != NITERS) && (gridCompare(shmaddr, GLOBAL_grid_B)==1))
 {
	 niters++;
	 sleep(1);
	 gridShow(GLOBAL_grid_A);
	 for(int i = 0; i<NWORKERS; i++)
	 {
	 	//child n
		if(fork()==0)
		{
			/* wait(x) */ sop.sem_num=1; sop.sem_op=-1; sop.sem_flg=0; semop(semid, &sop, 1);
			
			if(i == NWORKERS-1)
				gridEvolveTrimmed(GLOBAL_grid_A, shmaddr,i*slides,(i*slides + slides + res));
			else
				gridEvolveTrimmed(GLOBAL_grid_A, shmaddr,i*slides,(i*slides + slides));	
						
			/*signal(y)*/ sop.sem_num=0; sop.sem_op=+1; semop(semid, &sop, 1);
			exit(0);
		}
	 /*signal(x)*/ sop.sem_num=1; sop.sem_op=+1; semop(semid, &sop, 1); 
	 /* wait(y) */ sop.sem_num=0; sop.sem_op=-1; semop(semid, &sop, 1);	
	 }

	 if(gridCompare(shmaddr,GLOBAL_grid_B)==1)
		 memcpy(GLOBAL_grid_A, shmaddr,GLOBAL_totalcells * sizeof(char));	 
		 
 }
 printf("\n");
 if (niters == NITERS) {
     printf("FINISHED all iterations\n");
 }

 // free dynamic memory
 free(GLOBAL_grid_A);
 free(GLOBAL_grid_B);
 exit(0);
}

