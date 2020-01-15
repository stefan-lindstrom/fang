/******************************************************************************
 * $Id: dll.c,v 1.3 2002/11/07 04:02:02 fang Exp $
 *   
 * File: dll.c
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A general dynamical linked list lib. :)
 * 
 *   $Log: dll.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.1  2000/02/21 14:47:58  mud
 *   First commit
 *
 *
 *****************************************************************************/
#include "dll.h"
#include <stdlib.h>
#include <stdio.h>

struct node {
  struct node *prev,*next;
  ELEMENT *elm;
};

struct dll {
  struct node *first,*last,*current;
  int nr;
};

typedef struct node NODE;
typedef NODE *NODEP;

void * createDll() {
  DLL foo; 
  if(!(foo=calloc(sizeof(struct dll),1))) {
    fprintf(stderr,"Out of memory");
    exit(EXIT_FAILURE);
  }
  return foo;
}

int peekDll(DLL list) {
  return (int)(list->current->next);
}

int next(DLL list) {
  if(list->current&&(list->current!=list->last)) {
    list->current=list->current->next;
    return 1;
  } else {
    return 0;
  }
}

int prev(DLL list) {
  if(list->current&&(list->current!=list->first)) {
    list->current=list->current->prev;
    return 1;
  } else {
    return 0;
  }
}

int first(DLL list) {
  list->current=list->first;
  return list->current?1:0;
}

int last(DLL list) {
  list->current=list->last;
  return list->current?1:0;
}

ELEMENT elementAt(DLL list,int pos) {
  NODEP cur;
  int nr=0;
  if(pos>0) {
    cur=list->first;
    while(cur) {
      nr++;
      if(nr==pos) {
        return cur->elm;
      }
      cur=cur->next;
    }
  } else if(pos<0) {
    cur=list->last;
    while(cur) {
      nr--;
      if(nr==pos) {
        return cur->elm;
      }
      cur=cur->prev;
    }
  }
  printf("%d: Event not found.",pos);
  return NULL;
}

ELEMENT this(DLL list) {
  if(list->current) {
    return list->current->elm;
  } else {
    return NULL;
  }
}

int insert(DLL list,ELEMENT elm, int where) {
  NODEP nd=(NODEP) calloc(sizeof(NODE),1);
  if(!list) {
    *(&list)=createDll();
  }
  if(!nd||!list)
    return 0;
  nd->elm=elm;
  if(list->first==NULL) {
    list->current=list->first=list->last=nd;
  } else if(!list->current) {
    free(nd);
    return 0;
  } else if(where&&(list->current!=list->first)) {
    nd->prev=list->current->prev;
    nd->next=list->current;
    list->current->prev->next=nd;
    list->current->prev=nd;
  } else if(!where&&(list->current!=list->last)){
    nd->prev=list->current;
    nd->next=list->current->next;
    list->current->next->prev=nd;
    list->current->next=nd;
  } else if(where) {
    nd->prev=NULL;
    nd->next=list->current;
    list->current->prev=nd;
    list->first=nd;
  } else {
    nd->next=NULL;
    nd->prev=list->current;
    list->current->next=nd;
    list->last=nd;
  }
  list->current=nd;
  list->nr++;
  return 1;
}

void append(DLL list,ELEMENT elm) {
  last(list);
  insert(list,elm,0);
  last(list);
}

int map(DLL list, int (*f)(ELEMENT)) {
  NODEP tmp;
  int retval=0;
  for(tmp=list->first;tmp;tmp=tmp->next) {
    retval=(*f)(tmp->elm)||retval;
  }
  return retval;
}

void deleteAll(DLL list,void (*f)(ELEMENT)) {
  NODEP tmp=list->first,tmp2;
  while(tmp) {
    (*f)(tmp->elm);
    tmp2=tmp;
    tmp=tmp->next;
    free(tmp2);
  }
  free(list);
}

void deleteCurrentElement(DLL list,void (*f)(ELEMENT)) {
  NODEP oldc=list->current;
  if(!oldc) {
    return;
  } else if((oldc==list->last)&&(list->last==list->first)) {
    list->first=list->last=list->current=NULL;
  } else if(oldc==list->last) {
    oldc->prev->next=list->current=NULL;
    list->last=oldc->prev;
  } else if(oldc==list->first) {
    list->current=list->first=oldc->next;
    oldc->next->prev=NULL;
  } else {
    oldc->prev->next=oldc->next;
    oldc->next->prev=oldc->prev;
    list->current=oldc->next;
  }
  (*f)(oldc->elm);
  free(oldc);
  list->nr--;
}

int search(DLL list,ELEMENT elm, int (*equal)(ELEMENT,ELEMENT)) {
  NODEP tmp;
  for(tmp=list->first;tmp&&!(*equal)(tmp->elm,elm);tmp=tmp->next)
    ;
  if(tmp) {
    list->current=tmp;
    return 0;
  } else {
    return 1;
  }
}

int getNumber(DLL list){
  if(!list)
    return 0;
  return list->nr;
}
