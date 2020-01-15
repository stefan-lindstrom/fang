/******************************************************************************
 * $Id: tree.c,v 1.3 2002/11/07 04:02:04 fang Exp $
 *   
 * File: tree.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: tree.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/
#include "tree.h"
#include <stddef.h>


/* Inserts an element into the tree pointed out by head.    */
/* the function otto tells this function whether it should  */
/* continue to traverse the tree to the left or to the right*/
void t_insert(tree **head,tree *elem,insertfunc otto)
{
  tree *tmp,*par;
  int rc;

  par = NULL;
  tmp = (*head);
  
  while (NULL != tmp) {
    par = tmp;

    rc = otto(tmp,elem);

    if (GOING_LEFT == rc) {
      tmp = LEFT(tmp);
    }
    else {
      tmp = RIGHT(tmp);
    }
  }

  PARENT(elem) = par;

  if (NULL == par) {
    (*head) = elem;
  }
  else if (GOING_LEFT == otto(par,elem)) {
    LEFT(par) = elem;
  }
  else {
    RIGHT(par) = elem;
  }
}

/* Removes the node elem from the tree head. otto is a pointer to  */
/* a function that deletes data within a node (as well as the node */
/* perry is a pointer to a function that copies the data from one  */
/* node to another. 						   */
void t_delete(tree **head,tree *elem,deletefunc otto,copyfunc perry)
{
  tree *par,*tmp;

  if ((NULL == LEFT(elem)) || (NULL == RIGHT(elem))) {
    par = elem;
  }
  else {
    par = t_succ(elem);
  }

  if (NULL != LEFT(par)) {
    tmp = RIGHT(par);
  }
  else {
    tmp = RIGHT(par);
  }

  if (NULL != tmp) {
    PARENT(tmp) = PARENT(par);
  }

  if (NULL == PARENT(par)) {
    (*head) = tmp;
  }
  else if (par == LEFT(PARENT(par))) {
    LEFT(PARENT(par)) = tmp;
  }
  else {
    RIGHT(PARENT(par)) = tmp;
  }

  if (par != elem) {
    perry(elem,par);
    otto(par);
  }
  else {
    otto(elem);
  }
}

tree *t_succ(tree *elem)
{
  tree *p; 

  if (NULL == elem) {
    return NULL;
  }

  if (NULL != RIGHT(elem)) {
    return (t_min(RIGHT(elem)));
  }

  p = PARENT(elem);

  while ((NULL != p) && (elem == RIGHT(p))) {
    elem = p;
    p = PARENT(p);
  }
  return (p);
}

tree *t_min(tree *elem)
{
  if (NULL == elem) {
    return NULL;
  }

  while (NULL != LEFT(elem)) {
    elem = LEFT(elem);
  }
  return (elem);
}

tree *t_max(tree *elem)
{
  if (NULL == elem) {
    return NULL;
  }

  while (NULL != RIGHT(elem)) {
    elem = RIGHT(elem);
  }
  return (elem);
}

/* Searches the tree for an node in head, that matches elem. */
/* otto is a pointer to a func that determines whether the   */
/* elems compared are equal, if not it decides if the tree   */
/* should be walked to the 'left' or to the 'right'	     */
/* returns NULL if no match was found. 			     */ 
tree *t_search(tree *head,tree *elem,searchfunc otto)
{
  int rc;

  if ((NULL == head) || (IS_EQUAL == (rc = otto(head,elem)))) {
    return head;
  }

  if (GOING_LEFT == rc) {
    return (t_search(LEFT(head),elem,otto));
  }
  else {
    return (t_search(RIGHT(head),elem,otto));
  }
  return NULL;
}

/* Functions to traverse the tree. three different traversing orders */
/* are available. inorder, postorder, preorder. Otto is a pointer to */
/* a function that will be executed on each of the nodes in the tree */
/* data, can contain any data you need to perform certain actions on */
/* the tree nodes. Preferably gather it in a structure if you need a */
/* lot of data. The data-pointer will be supplied to the otto        */
/* function as well. 						     */
void t_inorder(tree *head,printfunc otto,void *data)
{
  if (NULL != head) {
    t_inorder(LEFT(head),otto,data);
    otto(head,data);
    t_inorder(RIGHT(head),otto,data);
  }
}

void t_postorder(tree *head,printfunc otto,void *data)
{
  if (NULL != head) {
    t_postorder(LEFT(head),otto,data);
    t_postorder(RIGHT(head),otto,data);
    otto(head,data);
  }
}

void t_preorder(tree *head,printfunc otto,void *data)
{
  if (NULL != head) {
    otto(head,data);
    t_preorder(LEFT(head),otto,data);
    t_preorder(RIGHT(head),otto,data);
  }
}
