#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define N 6

int mutex, t, memVar,Acces_V1;

 struct sembuf Pmutex={0,-1,0};
 struct sembuf Vmutex={0,1,0};
 
 struct sembuf P={0,-1,0};
 struct sembuf V={0,1,0};
 typedef struct info {int pid;int rang; int val;}info;

int main (void)
{
  info element;
  int i=0; 
  int j=0;
  int memVar;
  int Acces_V1;
 int *adrVar;
 info *adrTab;
///////////////////////////////////////////////////////
key_t clef1 = ftok("principl.c", 2);   
  t=semget(clef1, N, IPC_CREAT|0666);

  
 key_t clef2 = ftok("principl.c", 3);   
  mutex=semget(clef2, 1, IPC_CREAT|0666);
////////////////////////////////////////////////////

  key_t clef3 = ftok("principl.c", 4);   
  
  memVar=shmget(clef3,2*sizeof(int),IPC_CREAT|0666);

  
  key_t clef4 = ftok("principl.c", 5);   
  
  Acces_V1=shmget(clef4,N*sizeof(info),IPC_CREAT|0666);


  /* Attachement des segments partagés  */
  adrVar= shmat(memVar,0,0);
  adrTab= shmat(Acces_V1,0,0);
///////////////////////////////////////////////////////////////////////
 
  semop(mutex,&Pmutex,1);
  j=adrVar[0]; //adrVar[0] c'est le cpt
  adrVar[0]=adrVar[0]+1;
  printf("processuc n°%d de PID = %d",j,getpid());
  semop(mutex,&Vmutex,1);
  //struct sembuf P.sem_num=j;
  semop(t,&P,1);
  adrVar[1]=adrVar[1]+j;  //v1
  element.pid= getpid();
  element.rang=j;
  element.val=adrVar[1];
  adrTab[j]=element;
  
 semop(t,&V,1);
  if(j==0){  
   printf("le resultat d'acces à var V1");
  for(i=N-1;i>=0;i--){
    printf("access n°%d par le processuc de PID = %d de rang = %d v1= %d",N-i+1, adrTab[i].pid, adrTab[i].rang, adrTab[i].val);

          }

     }

////////////////////////////////////////////////////////////////////////

/* Déttachement des segments partagés  */ 
shmdt(adrVar);
shmdt(adrTab);
return 0; 
    }