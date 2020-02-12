#ifdef __LINKEDLIST_H
#define __LINKEDLIST_H


#include "Member.h"


typedef struct node_s{
	struct node_s* next;
	member data;
}Node;

typedef struct{
	struct Node head;
}Linkedlist_s;

Node getNext(node p){

}

void Reverse(Linkedlist_s l){
	if(l.head==NULL)
		return;
	node p=l.head,q=p.getNext();
	while(p!=NULL&&q!=NULL){
		p.
	}
}

#endif
