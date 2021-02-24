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
typedef struct info {int pid;int rang; int val;}info;
 
int main (void)
{
   key_t clef1; 
   key_t clef2;
   key_t clef3;
   key_t clef4;
   int p,i,err;


/* Création des sémaphores */
 
  clef1 = ftok("principl.c", 2);   
  t=semget(clef1, N, IPC_CREAT|0666);
  if(t==-1) {printf("\n Erreur de creation des sémaphores"); exit(1);}


/* Initialisation des sémaphores */

  semctl(t,0,SETVAL,0);      
  semctl(t,1,SETVAL,0);      
  semctl(t,2,SETVAL,0);     
  semctl(t,3,SETVAL,0);      
  semctl(t,4,SETVAL,0);
  semctl(t,5,SETVAL,1);
/////////////////////////////////////////////////////////////////////////////
/* Création des sémaphores */
 
  clef2 = ftok("principl.c", 3);   
  mutex=semget(clef2, 1, IPC_CREAT|0666);
  if(mutex==-1) {printf("\n Erreur de creation des sémaphores"); exit(1);}


 /* Création des segments partagés */

  clef3 = ftok("principl.c", 4);   
  
  memVar=shmget(clef3,2*sizeof(int),IPC_CREAT|0666);
  if(memVar==-1) {printf("\n Erreur de creation de la mémoire partagée"); exit(2);}
  /* Attachement */
  int *adrVar= shmat(memVar,0,0);
///////////////////////////////////////////////////////////////////////////////

 /* Création des segments partagés */

  clef4 = ftok("principl.c", 5);   
  
  Acces_V1=shmget(clef4,N*sizeof(info),IPC_CREAT|0666);
  if(Acces_V1==-1) {printf("\n Erreur de creation de la mémoire partagée"); exit(2);}
  /* Attachement */
  info *adrTab= shmat(Acces_V1,0,0);
///////////////////////////////////////////////////////////////////////////////




/* Détachement des segements */
  shmdt(adrVar); 
  shmdt(adrTab);
/*procduction, comsommation et assemblage*/
        for(i=0;i<6;i++)
	{
		i++;		
		p=fork();
 	 	if(p==-1) {printf("\n Erreur de creation du processus"); exit(3);}

  	        if(p==0) {   err=execl("./Pgme_processus","Pgme_processus",NULL);
                printf("\n Erreur de chargement du programme productA N° erreur %d\n", err); 
                exit(4);  
	}
}

  p=wait(NULL);
   while(p!=-1)
    {  printf("\n  Fils terminé %d",p);
       p=wait(NULL);}
/* Destrcution des sémaphores et des segments partagés */ 

   semctl(t,IPC_RMID,0);  
   shmctl(mutex,IPC_RMID,0);
   semctl(memVar,IPC_RMID,0);
   semctl(Acces_V1,IPC_RMID,0);
   sleep(2);
   printf("\n Fin du processus");
   return 0;
   
    
  }
  