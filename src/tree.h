/******************************************************************************
 * $Id: tree.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: tree.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: tree.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __TREE_H__
#define __TREE_H__

#define GOING_LEFT   1
#define GOING_RIGHT -1
#define IS_EQUAL     0

struct tree
{
  struct tree *tr_Left;
  struct tree *tr_Right;
  struct tree *tr_Parent;
} typedef tree;

#define TREE_SIZE sizeof(tree)
#define PARENT(x) ((x)->tr_Parent)
#define LEFT(x)   ((x)->tr_Left)
#define RIGHT(x)  ((x)->tr_Right)

/* Below is definitions for certain functions to manipulate the trees    */
/* Most functions in this library, takes a functionpointer in most cases */
/* For instance, to insert, search, delete tree walks, one can (or rather*/
/* must supply a function to get the library functions supplied to work  */
/* properly.								 */
typedef int  (*insertfunc)(tree *curr,tree *elem);
typedef void (*deletefunc)(tree *elem);
typedef void (*copyfunc)(tree *dst,tree *src);
typedef int  (*searchfunc)(tree *curr,tree *elem);
typedef void (*printfunc)(tree *elem,void *data);

extern void  t_insert(tree **head,tree *elem,insertfunc otto);
extern void  t_delete(tree **head,tree *elem,deletefunc otto,copyfunc perry);
extern tree *t_succ(tree *elem);
extern tree *t_min(tree *elem);
extern tree *t_max(tree *elem);
extern tree *t_search(tree *head,tree *elem,searchfunc otto);
extern void  t_inorder(tree *head,printfunc otto,void *data);
extern void  t_postorder(tree *head,printfunc otto,void *data);
extern void  t_preorder(tree *head,printfunc otto,void *data);

#endif
