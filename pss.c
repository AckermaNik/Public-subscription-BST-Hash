/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "pss.h"
#include<time.h>
#include<math.h>

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */

#define MG 64

struct Group G[MG];
struct SubInfo** hash;
int hashlength,pnum,validgroups=0,allsubs=0;

int initialize(int m, int p){
	int i;
	hash= malloc(m*sizeof(struct SubInfo));

	if(p<=0 || m<=0){
		exit(1);
	}
	pnum=p;
	hashlength=m;
   for(i=0;i<m;i++){
	   hash[i]=NULL;
   }

	 for(i=0;i<MG;i++){
		    G[i].gId=i;            /* number of each team*/
	        G[i].gr=NULL;      /* initialization of pointers for each team*/
	        G[i].gsub=NULL;
	     }
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */

void freeinfolist(struct Info* root){ /* h diagrafh tou dendrou me ta infos twn group diasxizontas to tree me metadiatetagmenh diataxh*/
	if(root==NULL){
		return;
	}
	freeinfolist(root->ilc);
	freeinfolist(root->irc);
	free(root);

}
int free_all(void){

	struct Info* tempi=malloc(sizeof(struct Info));
	struct SubInfo* temps=malloc(sizeof(struct SubInfo));
	struct Subscription* tempggs=malloc(sizeof(struct Subscription));
	int i;
	for(i=0;i<hashlength;i++){
		temps=hash[i];
		while(hash[i]!=NULL){        /*katastrofh SubInfo list*/
			temps=hash[i];
			hash[i]=hash[i]->snext;
			free(temps);
		}
	}

	for(i=0;i<MG;i++){       /* gia kathe mia apo tis omades diagrafh twn liston tous*/
		 freeinfolist(G[i].gr);

	  
	  while(G[i].gsub!=NULL){ /* katastrofi subscription list*/
			tempggs=G[i].gsub;
			 G[i].gsub=G[i].gsub->snext;
			 free(tempggs);

		 }
	}
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */


/* SYNARTHSEIS GIA TO DIPLA SYNDEDEMENO DENDRO TVN INFO KATHE OMADAS */




void insertstart(int iTM,int iId,struct Info** root,int array[64],int arnum){
	int j;
	struct Info* newnode =malloc(sizeof(struct Info)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
	if(!newnode){
		 printf("Adunamia desmeuseis mnimis1");
		 exit(1) ;
	}
	newnode->iId=iId;    /*arxikopoihsh ton pedion tou neo komvou */
	newnode->itm = iTM;

	for(j=0;j<64;j++){
	 	 newnode->igp[j]=0;
	  }
	for(j=0;j<arnum-1;j++){
		 newnode->igp[array[j]]=1; /* bazo 1 stis omades pou exoun thn idia plhroforia me aythn*/
	 }
	newnode->ip = *root;   /* NULL thn proth fora*/
	newnode->irc=NULL;
	newnode->ilc=NULL;
	(*root)= newnode;        /* o head ginetai o 2os komvos tora*/
}

void insertafter(int iTM,int iId,struct Info* prev,int array[64],int arnum){
	struct Info* newnode =malloc(sizeof(struct Info));   /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
	int j;
			if(!newnode){
				 printf("Adunamia desmeuseis mnimis1");
				  return ;
			}
			newnode->iId=iId;          /*arxikopoihsh ton pedion tou neo komvou */
			newnode->itm = iTM;
			for(j=0;j<64;j++){
			 	 newnode->igp[j]=0;
			  }
			for(j=0;j<arnum-1;j++){
				 newnode->igp[array[j]]=1;
			 }


			if(iId>prev->iId){    /*eisagwgh komvou sta dexia */
				prev->irc=newnode;
				newnode->irc=NULL;
				newnode->ilc=NULL;
				newnode->ip=prev;
			}
			else{                /*eisagwgh komvou sta aristera */

				prev->ilc=newnode;
				newnode->irc=NULL;
				newnode->ilc=NULL;
				newnode->ip=prev;

			}

}

void BST_insert(int iTM,int iId,struct Info** root,int array[64],int arnum){
	        struct Info* prev=malloc(sizeof(struct Info));
			struct Info* current=malloc(sizeof(struct Info));
		    if(!prev || !current ){
					 printf("Adunamia desmeuseis mnimis1");
					 exit(1);
			}


			if(*root==NULL){  /* proth fora insert*/
				insertstart(iTM,iId,root,array,arnum);
				return;
			}

			current=*root;
			prev=current;

			while(current!=NULL){     /* traversing the tree*/
				if(current->iId<iId){
					prev=current;
					current=current->irc;
				}
				else{
				     prev=current;
				     current=current->ilc;
			   }
		   }

		   insertafter(iTM,iId,prev,array,arnum); /*eisagwgh komvou ws fullo*/

}

void BST_print(struct Info* root){  /* print with endodiatetagmeni diataxh*/

            if(root==NULL){
            	return;
            }
		    BST_print(root->ilc);
			printf("  ITM: %d",root->itm);
			printf("  Id: %d",root->iId);
			printf(",  ");
			BST_print(root->irc);

}

int BST_LookUp(int iId,struct Info* root){
	struct Info* current=malloc(sizeof(struct Info));
    current=root;
    while(current!=NULL){
    	if(current->iId==iId){
    		return 1;
    	}
    	else if(current->iId<iId){
    		current=current->irc;
    	}
        else{
    	   current=current->ilc;
    	 }
    }
    return 0;
}
void deletestart(struct Info** root){ /* gia thn root mono */

	int i=0;
	struct Info* replace=malloc(sizeof(struct Info));
	struct Info* newnode=malloc(sizeof(struct Info));
	struct Info* current=malloc(sizeof(struct Info));



	if((*root)->ilc!=NULL && (*root)->irc!=NULL){

		current=(*root)->irc;

		while((current->ilc)!=NULL){
			current=current->ilc; /* pao olo left*/
	    }

	   newnode->iId=current->iId;
	   for(i=0;i<MG;i++){
		   newnode->igp[i]=current->igp[i];
	   }
	   newnode->itm=current->itm;

	   newnode->ip=(*root)->ip; /*NULL*/
	   newnode->ilc=(*root)->ilc;
	   (*root)->ilc->ip=newnode;
	   newnode->irc=(*root)->irc;
	   (*root)->irc->ip=newnode;


	   if(current->irc!=NULL){ /* an o komvos pou tha adikatasthsei auton pou tha diagrapsw exei paidi(momo dexi tha exei an exei)*/

	   		current->irc->ip=current->ip;

	   		if(current==current->ip->ilc){       /*syndesh tou katallhlou paidiou tou gonea tou komvou pou tha adikatasthsei auton pou tha diagrapsw me to paidi tou*/
	   			current->ip->ilc=current->irc;
	   		}
	   		else{
	   			current->ip->irc=current->irc;
	   		}

	   	  }
	   	  else{     /* an o komvos pou tha antikatastisei auton pou tha diagrapsv einai fullo*/

	   		  if(current==current->ip->ilc){
	   		  		current->ip->ilc=NULL;


	   		  	}
	   		  	else{
	   		  		current->ip->irc=NULL;
	   		  	}
	   	  }
	   free(*root);
	   (*root)=newnode;
	}
	else if((*root)->ilc==NULL && (*root)->irc==NULL){ /*xwris paidia*/
		free(*root);
		(*root)=NULL;

	}
	else{						/*me 1 paidi*/

		if((*root)->ilc!=NULL){

			(*root)->ilc->ip=(*root)->ip; /*NULL*/
			replace=(*root)->ilc;
		}
		else if((*root)->irc!=NULL){
			replace=(*root)->irc;
			(*root)->irc->ip=(*root)->ip; /*NULL*/

		}
		free(*root);
		(*root)=replace;
	}


}
void deleteafter(struct Info* deleted){ /* deleted= o komvos pros diagrafh*/
		int i=0;
		struct Info* newnode=malloc(sizeof(struct Info));
		struct Info* current=malloc(sizeof(struct Info));


		if(deleted->ilc!=NULL && deleted->irc!=NULL){  /* me 2 paidia */

			current=deleted->irc;
			while((current->ilc)!=NULL){  /* PAO NA VRO RO MIKROTERO STOIXEIO STHN ENDO*/
				current=current->ilc; /* pao olo left*/
		    }

		   newnode->iId=current->iId;
		   for(i=0;i<MG;i++){
			   newnode->igp[i]=current->igp[i];
		   }
		   newnode->itm=current->itm;

		   if(deleted==deleted->ip->ilc){  /* syndesi tou gvnea tou komvou pros diagrafi me to ena paidi pou exei autos o komvoa pros diagrafi*/
		  	    deleted->ip->ilc=newnode;
		  	}
		  	 else{

		  		deleted->ip->irc=newnode;

		   }
		   newnode->ip=deleted->ip;
		   newnode->ilc=deleted->ilc;
		   newnode->irc=deleted->irc;
		   deleted->ilc->ip=newnode;
		   deleted->irc->ip=newnode;

		   if(current->irc!=NULL){  /* ean exei right paidi */
		   			current->irc->ip=current->ip;

		   			if(current==current->ip->ilc){         /*syndesh tou katallhlou paidiou tou gonea tou komvou pou tha adikatasthsei auton pou tha diagrapsw me to paidi tou*/
		   				current->ip->ilc=current->irc;
		   			}
		   			else{
		   				current->ip->irc=current->irc;
		   			}

		   		  }
		   		  else{          /* xwris paidia*/

		   			  if(current==current->ip->ilc){
		   			  		current->ip->ilc=NULL;

		   			  }
		   			  else{
		   			  		current->ip->irc=NULL;
		   			  }
		   		  }

		   free(deleted);
		}
		else if(deleted->ilc==NULL && deleted->irc==NULL){  /* me kanena o deleted*/

			if(deleted==deleted->ip->ilc){
				deleted->ip->ilc=NULL;
			}
			else if(deleted==deleted->ip->irc){
				deleted->ip->irc=NULL;
			}
			free(deleted);
		}
		else{  /* me 1 paidi */

			if(deleted->ilc!=NULL){     /* syndesi tou left paidioy tou gvnea tou komvou pros diagrafi me to ena paidi pou exei  o komvos pros diagrafi*/
				deleted->ilc->ip=deleted->ip;

				if(deleted==deleted->ip->ilc){ /* an o komvos pros diagrafh einai to left paidi tou gonea tou*/
				deleted->ip->ilc=deleted->ilc;
				}
				else{  /* an o komvos pros diagrafh einai to right paidi tou gonea tou*/
					deleted->ip->irc=deleted->ilc;
				}

			}
			else if(deleted->irc!=NULL){     /* syndesi tou left paidioy tou gvnea tou komvou pros diagrafi me to ena paidi pou exei  o komvos pros diagrafi*/
				deleted->irc->ip=deleted->ip;

				if(deleted==deleted->ip->ilc){  /* an o komvos pros diagrafh einai to left paidi tou gonea tou*/
					deleted->ip->ilc=deleted->irc;
				}
				else{							 /* an o komvos pros diagrafh einai to right paidi tou gonea tou*/
					deleted->ip->irc=deleted->irc;
				}

			}

            free(deleted);
		}

}

void BST_delete(int tm,struct Info** root){
	struct Info* current=malloc(sizeof(struct Info));
	current=*root;


	if(current->ip==NULL && current->itm==tm){ /* an auto pou thelw na diagrapsw einai h rhza*/

		deletestart(root);
		return;

	}
	while(current!=NULL){           /* vriskw ton komvo pros diagrafh*/
	    	if(current->itm==tm){
              deleteafter(current);
              return;
	    	}

	    	else if(current->itm<tm){  /* traversing mexri na vrw ton komvo pros diagrafh*/
	    		current=current->irc;
	    	}
	        else{
	    	   current=current->ilc;
	    	 }
	}

}
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
	 int i;
	    for(i=0;i<size_of_gids_arr-1;i++){ /* gia tis omades tou gids_arr*/
	    	if(!BST_LookUp(iId,G[gids_arr[i]].gr)){ /* an den yparxei idi ayth h plhroforia se mia omada (idio Id) na eisaxthei*/

	    	  BST_insert(iTM,iId,&G[gids_arr[i]].gr,gids_arr,size_of_gids_arr);

	    	  printf("\nGROUPID=<%d> ",gids_arr[i]);
	    	  printf("INFOLIST= ");

	    	  /*if( iId==10){  douleuoun
    	       //BST_delete(2,&G[gids_arr[i]].gr);
	    	  BST_delete(1,&G[gids_arr[i]].gr);
   	          //BST_delete(7,&G[gids_arr[i]].gr);
	    	  BST_delete(9,&G[gids_arr[i]].gr);
	    	  //BST_delete(11,&G[gids_arr[i]].gr);
	    	  BST_delete(7,&G[gids_arr[i]].gr);
	    	  BST_delete(11,&G[gids_arr[i]].gr);
	    	  }*/

	    	  BST_print(G[gids_arr[i]].gr); /* ektyposh infolist kathe omadas*/

	    	 }
	    }
    return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */


/* SYNARTHSEIS GIA THN SUBSCRIPTION LIST KATHE OMADAS */



void insertstartsublist(int sid,struct Subscription** head){
	struct Subscription* newnode =malloc(sizeof(struct Subscription));
	if(!newnode){
		 printf("Adunamia desmeuseis mnimis1");
		  return ;
	}

	newnode->sId = sid;
	newnode->snext = *head; /* NULL sthn arxi*/
	*head = newnode;        /* o head ginetai o 2os komvos tora*/

}



void insertaftersublist(int sid,struct Subscription* prev){
 struct Subscription* after =malloc(sizeof(struct Subscription));
 struct Subscription* newnode =malloc(sizeof(struct Subscription));
 if(!after || !newnode){
	printf("Adunamia desmeuseis mnimis2");
		 return ;
 }

 newnode->sId = sid; /* arxikopoihsh pedivn ths*/

 after= prev->snext;  /* syndesi deiktvn*/
 prev->snext = newnode;
 newnode->snext = after;

}

void deletestartsublist(int key,struct Subscription** head) {
	struct Subscription* temp=malloc(sizeof(struct Subscription));
	 if(!temp){
		printf("Adunamia desmeuseis mnimis gia temp");
			 return ;
	 }

	temp=(*head)->snext; /* diagrafh kai allagh head pointer*/
	free(*head);
	*head = temp;

}


void deleteaftersublist(int key,struct Subscription* prev){   /* o deiktis enos dtoixeiou prin apo ayto pou thelo na diagrapso*/
	struct Subscription* temp=malloc(sizeof(struct Subscription));
	 if(!temp){
			printf("Adunamia desmeuseis mnimis gia temp");
				 return ;
		 }


		temp=(prev->snext)->snext; /* diagrafh komvou kai allagh pointer*/
		free(prev->snext);
		prev->snext=temp;

}





void L_insert(int sid,struct Subscription** head){

	struct Subscription* prev =malloc(sizeof(struct Subscription));
	struct Subscription* current =malloc(sizeof(struct Subscription));
    if(!prev || !current ){
			 printf("Adunamia desmeuseis mnimis1");
			 return ;
	}


	if(*head==NULL){  /* prvth fora insert*/

		insertstartsublist(sid,head);
		return;
	}

	current=*head;
	prev=current;

	while(current!=NULL){
		if(current==*head && (current->sId)>sid){
			insertstartsublist(sid,head);
			return;
		}
		else if((current->sId)>sid){
			insertaftersublist(sid,prev);


			return;
		}
		prev=current;
		current=current->snext;

	}

	insertaftersublist(sid,prev); /* aplo insert sto telos ths*/



}


int L_LookUp(int id,struct Subscription* head){ /* psaxno an yparxei o syndromiths me to sygkekrimeno sId sthn subscription list mias omadas*/
	struct Subscription* current ;
    current=head;

    while(current!=NULL){
    	if((current->sId)==id){

    		return 1;
        }
    	current=current->snext;
    }
    return 0;
}



void L_Delete(int key,struct Subscription** head){
	struct Subscription* prev =malloc(sizeof(struct Subscription));
		struct Subscription* current =malloc(sizeof(struct Subscription));
	    if(!prev || !current ){
				 printf("Adunamia desmeuseis mnimis1");
				 return ;
		}
	    if(*head==NULL){ /* an einai adeia h lista */
	    	return;
	    }

	    current=*head;
	    prev=current;
	    while(current!=NULL){
	    		if(current==*head && (current->sId)==key){
	    			deletestartsublist(key,head);
	    			return;
	    		}
	    		else if((current->sId)==key){
	    			deleteaftersublist(key,prev);

	    			return;
	    		}
	    		prev=current;
	    		current=current->snext;

	    	}

}

void L_print(int* subarr ,int size){
	struct  Subscription* current =malloc(sizeof(struct Subscription));
        int i;
		for(i=0;i<size-1;i++){
			current=G[subarr[i]].gsub;/* head pointer ths ekastote subscription list*/
			if(current!=NULL){
			printf("\nGROUPID=<%d>  ",subarr[i]);  /*ektiponei ola ta groups exomtas h oxi syndromiti  syndromiti*/
			}
			else{
				printf("\nGROUPID=<%d>  No Subscriber",subarr[i]);
			}
			while(current!=NULL){
		       printf("Subscriber with sId:%d  ",current->sId);
		       current=current->snext;
		    }
	    }
}




/* SYNARTHSEIS GIA THN SUBINFO*/




void insertstartsub(int stm,int sid,struct SubInfo** head,int array[64],int arrnum){
	struct SubInfo* newnode =malloc(sizeof(struct SubInfo));
	int i;
	if(!newnode){
		 printf("Adunamia desmeuseis mnimis1");
		  return ;
	}
	newnode->stm = stm;   /* arxikopoihsei ton pedion enos komvou*/
	newnode->sId = sid;
	for(i=0;i<MG;i++){
		newnode->sgp[i]=(struct TreeInfo*)1;
		newnode->tgp[i]=(struct TreeInfo*)1;
	}
	for(i=0;i<arrnum-1;i++){
        newnode->sgp[array[i]]=NULL;  /*gia thn kathe omada pou endiaferetai autos o syndromitis kano na deixnei sto proto stoixeio ths Info list ths*/
        newnode->tgp[array[i]]=NULL;
	}
	newnode->snext = *head; /* NULL*/
	*head = newnode;        /* o head ginetai o 2os komvos tora*/

}



void insertaftersub(int stm,int sid,struct SubInfo* prev,int array[64],int arrnum){
 struct SubInfo* after =malloc(sizeof(struct SubInfo));
 struct SubInfo* newnode =malloc(sizeof(struct SubInfo));
 int i;
 if(!after || !newnode){
	printf("Adunamia desmeuseis mnimis2");
		 return ;
 }
   newnode->stm = stm;   /* ta idia me thn insert start*/
   newnode->sId = sid;
   for(i=0;i<MG;i++){
   		newnode->sgp[i]=(struct TreeInfo*)1;
   		newnode->tgp[i]=(struct TreeInfo*)1;
   	}
   	for(i=0;i<arrnum-1;i++){
           newnode->sgp[array[i]]=NULL;  /*gia thn kathe omada pou endiaferetai autos o syndromitis kano na deixnei sto proto stoixeio ths Info list ths*/
           newnode->tgp[array[i]]=NULL;
   	}

	 after = prev->snext; /* syndesi deikton */
	 prev->snext = newnode;
	 newnode->snext = after;
}

void deletestartsub(int key,struct SubInfo** head) {
	struct SubInfo* temp=malloc(sizeof(struct SubInfo));
	 if(!temp){
		printf("Adunamia desmeuseis mnimis gia temp");
			 return ;
	 }

	temp=(*head)->snext; /* diagrafh tou protou komvou kai metakinish tou head ston epomeno*/
	printf("\n\nDeleted subscriber:  %d",key);
	free(*head);
	*head = temp;

}


void deleteaftersub(int key,struct SubInfo* prev){   /* o deiktis enos dtoixeiou prin apo ayto pou thelo na diagrapso*/
	struct SubInfo* temp=malloc(sizeof(struct SubInfo));
	 if(!temp){
			printf("Adunamia desmeuseis mnimis gia temp");
				 return ;
		 }

		printf("\n\nDeleted subscriber:  %d",key);

		temp=(prev->snext)->snext; /* katallhlh diagrafh tou prev->next kai syndesi deiktvn*/
		free(prev->snext);
		prev->snext=temp;

}

void SL_insert(int stm,int sid,struct SubInfo** head,int arr[64],int arrnum){

	struct SubInfo* prev =malloc(sizeof(struct SubInfo));
	struct SubInfo* current =malloc(sizeof(struct SubInfo));
    if(!prev || !current ){
			 printf("Adunamia desmeuseis mnimis1");
			 return ;
	}


	if(*head==NULL){  /* gia thn proth eisagvgh komvou*/
		insertstartsub(stm,sid,head,arr,arrnum);

		return;
	}
	else{
		current=*head;
		prev=current;
	}

	while(current!=NULL){
		if(current==*head && (current->stm)>stm){
			insertstartsub(stm,sid,head,arr,arrnum);
			return;
		}
		else if((current->stm)>stm){
			insertaftersub(stm,sid,prev,arr,arrnum);

			return;
		}
		prev=current;
		current=current->snext;

	}

	insertaftersub(stm,sid,prev,arr,arrnum); /* an thelei aplh eisagvgh sto telos ths listas*/


}
void SL_print(struct SubInfo* head){
	struct  SubInfo* current =malloc(sizeof(struct SubInfo));
	current=head;

	while(current!=NULL){  /*emfanisi stoixeion subinbfo ston sygkekrimeno index tou hash*/


		printf(" sId:%d,  ",current->sId);
		current=current->snext;
	}


}

void SL_Delete(int key,struct SubInfo** head){
	struct SubInfo* prev =malloc(sizeof(struct SubInfo));
		struct SubInfo* current =malloc(sizeof(struct SubInfo));
	    if(!prev || !current ){
				 printf("Adunamia desmeuseis mnimis1");
				 return ;
		}

	    if(*head==NULL){/* an den yparxoun stoixeiA gia diagrafh*/
	    	return;
	    }

	    current=*head;
	    prev=current;
	    while(current!=NULL){
	    		if(current==*head && (current->sId)==key){

	    			deletestartsub(key,head);
	    			return;
	    		}
	    		else if((current->sId)==key){
	    			deleteaftersub(key,prev);

	    			return;
	    		}
	    		prev=current;
	    		current=current->snext;

	    	}
}


struct SubInfo* SL_LookUp(int id){ /* psaxno an yparxei syndromitis me to sygkekrimeno sId sto systhma*/
	struct SubInfo* current ;
	int i;
	for(i=0;i<hashlength;i++){ /* psaxnw olo ton hash*/
	    current=hash[i];
	    while(current!=NULL){
	        	if((current->sId)==id){
	        		return current;
	            }
	        	current=current->snext;
	        }

	}
    return NULL;
}

int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
	int i,hashindex;
	struct SubInfo* found;
	if(sId>pnum){
		 printf("Lathos sId/sId<p");
		 return 1;
	}

    hashindex=Universal_Hash(hashlength,pnum,sId);

    SL_insert(sTM,sId,&hash[hashindex],gids_arr,size_of_gids_arr); /*insert sthn SubInfo*/

    for(i=0;i<size_of_gids_arr-1;i++){
		   L_insert(sId,&G[gids_arr[i]].gsub);        /*insert sthn Subscription kathe katallhlhs omadas*/
    }

    printf("\n");
    printf("-----------------------------------------------");
    printf("\nSUBSCRIBERSINFO LIST: \n");

    for(i=0;i<hashlength;i++){ /* den tha emfanizontai taxinomimena logo ths hash*/
    	SL_print(hash[i]); /* ektyposh ths SubInfo sthn sygkekrimenh thesh hash*/
    }



    L_print(gids_arr,size_of_gids_arr); /* ektyposh ths subscription list kathe omadas*/

    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
void insertstartleaforiented(struct TreeInfo** root, int tId, int ttm){

	struct TreeInfo* newnode =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
	struct TreeInfo* newnodeleft =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
	struct TreeInfo* newnoderight =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
		if(!newnode || !newnodeleft || !newnoderight){
			 printf("Adunamia desmeuseis mnimis1");
			 exit(1) ;
		}
		newnode->tId=tId;    /*arxikopoihsh ton pedion tou neo komvou */
		newnode->ttm = ttm;

		if(*root!=NULL){

		  newnode->tp =NULL; /* O NEOS KOMVOS root TO BE*/

		  newnoderight->tId=(*root)->tId;  /* o right komvos ths neas root*/
		  newnoderight->ttm=(*root)->ttm;
          newnode->trc=newnoderight;
          newnoderight->tp=newnode;

          newnodeleft->tId=tId;       /* o left komvos ths neas root*/
          newnodeleft->ttm=ttm;
          newnode->tlc=newnodeleft;
          newnodeleft->tp=newnode;

          newnode->prev=NULL;       /* arxikopoihsh ths neas root*/
          newnode->next=NULL;
          newnode->tlc->prev=NULL;
          newnode->trc->next=NULL;

          newnode->tlc->next=newnoderight;
          newnode->trc->prev=newnode->tlc;

          newnodeleft->tlc=NULL;  /* ta paidia twn paidiwn ths neas root*/
          newnodeleft->trc=NULL;

          newnoderight->tlc=NULL;
          newnoderight->trc=NULL;


		}

		else{  /* prwth fora insert*/
		newnode->tlc=NULL;
		newnode->trc=NULL;
		newnode->next=NULL;
		newnode->prev=NULL;
		newnode->tp=NULL;
		}
		(*root)= newnode;        /* o head ginetai o neos komvos tora*/
}

void insertright(struct TreeInfo* current, int tId, int ttm){
	struct TreeInfo* newnoderight =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
	struct TreeInfo* newnodeleft =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
			if(!newnoderight || !newnodeleft){
				 printf("Adunamia desmeuseis mnimis1");
				 exit(1) ;
			}
			newnoderight->tId=tId;    /*arxikopoihsh ton pedion tou teleiws neou right komvou */
			newnoderight->ttm=ttm;
			newnoderight->tp=current;

			newnodeleft->tId=current->tId;    /*arxikopoihsh ton pedion tou neou left komvou pou exei ta idia stoixeia me ton gonea */
			newnodeleft->ttm =current->ttm;
			newnodeleft->tp=current;

			current->trc=newnoderight;   /* current= o komvos me vash ton opoio tha ginei h insert*/
			current->tlc=newnodeleft;
			current->ttm=ttm;
		    current->tId=tId;

			if(current->prev==NULL){    /* arxikopohsh deiktwn next kai prev tou yparxontas komvou me vash ton opoio egine h insert*/
				newnodeleft->prev=NULL;
			}
			else{
				newnodeleft->prev=current->prev;
				current->prev->next=newnodeleft;
				current->prev=NULL;
			}
			if(current->next==NULL){
				newnoderight->next=NULL;
			}
			else{
				newnoderight->next=current->next;
				current->next->prev=newnoderight;
				current->next=NULL;

			}
			 newnodeleft->next=newnoderight;
			 newnoderight->prev=newnodeleft;

			 newnodeleft->tlc=NULL;   /* ta paidia twn paidiwn tou neou komvou*/
			 newnodeleft->trc=NULL;

			 newnoderight->tlc=NULL;
			 newnoderight->trc=NULL;

}

void insertleft(struct TreeInfo* current, int tId, int ttm){
	    struct TreeInfo* newnoderight =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
		struct TreeInfo* newnodeleft =malloc(sizeof(struct TreeInfo)); /* desmeusei mnimis gia ton neo komvo pou tha ginei insert*/
				if(!newnoderight || !newnodeleft){
					 printf("Adunamia desmeuseis mnimis1");
					 exit(1) ;
				}
				newnodeleft->tId=tId;    /*arxikopoihsh ton pedion tou teleivs neou left komvou */
				newnodeleft->ttm = ttm;
				newnodeleft->tp=current;

				newnoderight->tId=current->tId;    /*arxikopoihsh ton pedion tou neou right  komvou pou exei ta idia stoixeia me ton gonea */
				newnoderight->ttm =current->ttm;
				newnoderight->tp=current;

				current->trc=newnoderight;       /* current= o komvos me vash ton opoio tha ginei h insert*/
				current->tlc=newnodeleft;
                current->ttm=ttm;
                current->tId=tId;

                if(current->prev==NULL){		/* arxikopohsh deiktwn next kai prev tou yparxontas komvou me vash ton opoio egine h insert*/
                	newnodeleft->prev=NULL;
                }
                else{

                	newnodeleft->prev=current->prev;
                	current->prev->next=newnodeleft;

                	current->prev=NULL;

                }
				newnodeleft->next=newnoderight;

				if(current->next==NULL){
					newnoderight->next=NULL;
				}
				else{
					newnoderight->next=current->next;
					current->next->prev=newnoderight;
					current->next=NULL;

				}

				  newnoderight->prev=newnodeleft;

				 newnodeleft->tlc=NULL;				 /* ta paidia twn paidiwn tou neou komvou*/
				 newnodeleft->trc=NULL;

				 newnoderight->tlc=NULL;
				 newnoderight->trc=NULL;


}

void LO_BST_insert(struct TreeInfo** root, int tId, int ttm){
	struct TreeInfo* current=malloc(sizeof(struct TreeInfo));



	if((*root)==NULL){ /* proth fora insert*/
			insertstartleaforiented(root,tId,ttm);
			return;
	}
	current=*root;


	while(current->tlc!=NULL){ /*pao oloaristera sto terma aristera filo*/
		current=current->tlc;
	}
	while(current->ttm<ttm && current->next!=NULL){ /* vrisko ton proto komvo me megalutero ttm apo emena kai stamatao*/
		current=current->next;
	}


	if(ttm>current->ttm){
		insertright(current,tId, ttm);
	}
	else{
		if(current->tp==NULL){   /* gia otan exo ena mono komvo-thn root- kai pao na valo kati mikrotero apo ton aritmo ths*/
		  insertstartleaforiented(root,tId,ttm);
		}else{
		  insertleft(current,tId, ttm);
		}
	}

}

void LO_BST_print(struct TreeInfo* root){
	struct TreeInfo* current;
	current=root;
	if(current==NULL){return;}

	while(current->tlc!=NULL){				/*pao olo left sto dendro*/
		   /* printf("%d",current->tlc->tId);*/
			current=current->tlc;
	}
	while(current!=NULL){       /* ektypwsh*/
		    printf(" %d, ",current->ttm);
			current=current->next;
	}

}
int LO_BST_LookUp(int tId,struct TreeInfo* root){
	struct TreeInfo* current=malloc(sizeof(struct TreeInfo));
	current=root;
	while(current->tlc!=NULL){				/*pao olo left sto dendro*/
		   /* printf("%d",current->tlc->tId);*/
			current=current->tlc;
	}
	while(current!=NULL){
		    if(tId==current->tId){
		    	return 1;
		    }
			current=current->next;
		}
	return 0;

}
 void pruning(int omada,struct Info* root,int tm,struct SubInfo* found){
	 int iId;

	 if(root==NULL){
	   return;
	 }

	  pruning(omada,root->ilc,tm,found); /*pruning me endo*/

	  if(tm>=root->itm){
		  iId=root->iId;
		  LO_BST_insert(&found->tgp[omada],iId,root->itm); /*eisagwgh tou komvou me itm<= tm sta fulloprosanatolismena*/
	  }

	  pruning(omada,root->irc,tm,found);
 }




 void deleteafterpruning(int tm ,struct Info** root){ /* delete ola ta itm<=tm apo to dipla syndedemeno  metadiatetagmena*/

 	 if((*root)==NULL){
 	   return;
 	 }

 	deleteafterpruning(tm,&((*root)->ilc));
 	deleteafterpruning(tm,&((*root)->irc));

 	  if(tm>=(*root)->itm){
 		  BST_delete((*root)->itm ,root);
 	  }
  }





int Prune(int tm){
	struct Subscription* current ;
	struct SubInfo* subin ;
    int i,j;
    for(i=0;i<MG;i++){
    	current=G[i].gsub;
    	if(G[i].gr!=NULL){
    	     while(current!=NULL){
    	    	if((SL_LookUp(current->sId))->tgp[i]!=(struct TreeInfo*)1){

    	    	pruning(i,G[i].gr,tm,SL_LookUp(current->sId)); /*actual pruning*/

    	    	SL_LookUp(current->sId)->sgp[i]=(SL_LookUp(current->sId))->tgp[i]; /* kanw set ton sgp sygkekrimehs omadas na deixnei sth root tou consaption tree auths ths sygkekrimehs omadas*/
                }
    	    	current=current->snext;
    	     }
    	     deleteafterpruning(tm,&G[i].gr);
    	 }
    }
    for(i=0;i<MG;i++){
    	printf("\nGROUPID=<%d> ",i);
    	printf("INFOLIST= ");

    	if(G[i].gr!=NULL){		 /* ektyposh infolist kathe omadas*/
    	BST_print(G[i].gr);
    	}
    	else{
    		printf(" NULL,");
    	}

    	printf(" SUBLIST= ");
    	current=G[i].gsub;     /* head pointer ths ekastote subscription list*/

		if(current==NULL){
			printf(" No Subscriber");
		}
		while(current!=NULL){
		   printf(" sId:%d  ",current->sId);
		   current=current->snext;
		}
    }
	for(i=0;i<hashlength;i++){
			if(hash[i]!=NULL){
		       subin=hash[i];
		       while(subin!=NULL){
		       		   printf("\nSUBSCRIBERID = <%d>  ",subin->sId);

		       		   for(j=0;j<MG;j++){
		       			if(subin->tgp[j]!=(struct TreeInfo*)1){
		       				printf("\nGROUPLIST=<%d> ",j);
		       				if(subin->tgp[j]==NULL){
		       					printf(" TREELIST = NULL ");
		       				}else{
		       				printf(" TREELIST = ");
		       				LO_BST_print(subin->tgp[j]);
		       				}
		       			   }
		       		   }
		       		   subin=subin->snext;
		       	}
			}
    }
    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
struct TreeInfo* current=malloc(sizeof(struct TreeInfo));
struct SubInfo*found=malloc(sizeof(struct SubInfo));
int i;

found=SL_LookUp(sId); /* vriskw thn eggrafh tou subscriber me auto to sid ston hash table*/

if(found!=NULL){ /* KAI AN YPARXEI*/
	
	for(i=0;i<MG;i++){
	current=found->sgp[i];

	if(current!=(struct TreeInfo*)1){
		printf("\nGROUPID=<%d>, ",i);
       if(current!=NULL){ /* an yparxoun infos*/
		while(current->tlc!=NULL){				/*pao olo left sto dendro*/
				current=current->tlc;
		}
		while(current->next!=NULL){ /* diasxish pros ta dexia gia na vrw thn timh tou neou found->sgp[i]*/
				current=current->next;
		}

		 found->sgp[i]=current; /* anannaiwsh pointer*/
		 printf("NEWSGP = <%d>,",found->sgp[i]->ttm);
		 printf(" TREELIST = ");
		 LO_BST_print(found->sgp[i]);

       }
       else{
    	   printf("NO INFOS"); /* an den yparxoun infos sto treelist auths ths omadas*/
       }

	}
  }
}
    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
struct SubInfo* current=malloc(sizeof(struct SubInfo));
struct Subscription* currentsub=malloc(sizeof(struct Subscription));
int i,j,k;
if(SL_LookUp(sId)!=NULL){  /* arxika vlepo  yparxei o subscriber pou thelw na diagrapso*/
	for(i=0;i<hashlength;i++){ /* psaxnw olo ton hash gia na vrw auton ton subscriber*/
		current=hash[i];
		while(current!=NULL){
				if((current->sId)==sId){ /* ean ton vrika tha ginoun ola ta parakatw*/
					SL_Delete(sId,&hash[i]);
					printf("\n");
					printf("NEW SUBSCRIBERSINFO LIST:\n");

					for(k=0;k<hashlength;k++){ /* den tha emfanizontai taxinomimena logo ths hash*/
					    	SL_print(hash[k]); /* ektyposh ths SubInfo sthn sygkekrimenh thesh hash*/
					  }

					for(j=0;j<MG;j++){ /* kanw delete ton subscriber apo thn subscription list kathe omadas pou endiaferetai*/						if(current->tgp[j]!=(struct TreeInfo*)1){
							L_Delete(sId,&G[j].gsub);
							if(G[j].gsub!=NULL){       /* ayth h if xrisimeuei sto na typono ta katallhla mhnymata mono */
								printf("\n");
								printf("GROUPID=<%d>  ",j);  /*ektiponei ola ta groups pou exoun esto kai ena syndromiti*/
								printf("NEW SUBLIST:  ");
							}
					        else{
						        printf("\nGROUPID<%d>    No Subscriber",j);/*ektiponei kai thn pliroforia oti h omada ayth den exei subscriber an den exei kanenan*/
					         }

							currentsub=G[j].gsub;
							while(currentsub!=NULL){   /* ektuposh ths subscription list kathe omadas*/
								printf(" sId:%d,",currentsub->sId);
								currentsub=currentsub->snext;
							}
						}
					}
				}
				current=current->snext;
			}

	  }
	}

    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
	struct Subscription* current ;
	struct SubInfo* subin ;
	int i,j;
	for(i=0;i<MG;i++){
	    	printf("\nGROUPID=<%d> ",i);
	    	printf("INFOLIST= ");

	    	if(G[i].gr!=NULL){		 /* ektyposh infolist kathe omadas*/
	    	BST_print(G[i].gr);
	    	}
	    	else{
	    		printf(" NULL,");
	    	}

	    	printf(" SUBLIST= ");
	    	current=G[i].gsub;     /* head pointer ths ekastote subscription list*/

			if(current==NULL){       /* ektypwsh subscriptionlist kathe omadas*/
				printf(" No Subscriber");
			}
			while(current!=NULL){
			   printf(" sId:%d,",current->sId);
			   current=current->snext;
			}
	 }
	printf("\nSUBSCRIBERLIST = ");
	for(i=0;i<hashlength;i++){ /* den tha emfanizontai taxinomimena logo ths hash*/
			SL_print(hash[i]); /* ektyposh ths SubInfo sthn sygkekrimenh thesh hash*/
	}
  for(i=0;i<hashlength;i++){ /* den tha emfanizontai taxinomimena logo ths hash*/
		subin=hash[i];        /* ektyposh ths SubInfo sthn sygkekrimenh thesh hash*/

	while(subin!=NULL){       /* gia kathe eggrafh sub sthn sygkekrimenh thesh hash emfanizw thn treeinfolist twn omadwn pou endiaferetai autos o subscriber */
		printf("\nSUBSCRIBERID=<%d>, ",subin->sId);

	    for(j=0;j<MG;j++){
	    	if(subin->tgp[j]!=(struct TreeInfo*)1){
	    		printf(" \nGROUPLIST=<%d>",j);
	    		printf(" \nTREELIST= ");
	    		if(subin->tgp[j]==NULL){
	    			printf("NULL");
	    		}
	    		else{
	    			LO_BST_print(subin->tgp[j]);
	    		}
	    	}
	    }
	    subin=subin->snext;
	}
  }
	printf("\n");
	for(j=0;j<MG;j++){
		if(G[j].gr!=NULL){
			validgroups++;
		}
	}
	 for(i=0;i<hashlength;i++){
			subin=hash[i];
			while(subin!=NULL){
				allsubs++; /* posoi subscribers yparxoun synolika*/
				subin=subin->snext;
			}
	 }
	printf("NO_GROUPS = <%d>, NO_SUBSCRIBERS = <%d> \n",validgroups,allsubs); /* an exei ginei prune kai mia omada meta den exei alles plhrofories tote den einai valid*/
    return EXIT_SUCCESS;
}
int Universal_Hash(int m,int p,int key){ /* key =sId*/
      int x,y;
      srand(time(0));
      x=fabs(rand());  /* oi tyxaioi arithmoi sthn synarthsh*/
      y=fabs(rand());

      return fabs(((key*x+y)%p)%m);  /* se apolyth timh gia na apofeugw tis arnhtikes times*/
}
