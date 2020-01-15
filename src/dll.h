/******************************************************************************
 * $Id: dll.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: dll.h
 *
 * Created: Mon Feb 21 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A linked list lib. :)
 * 
 *   $Log: dll.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.1  2000/02/21 14:47:58  mud
 *   First commit
 *
 *
 *****************************************************************************/

#ifndef __DLL_H__
#define __DLL_H__

struct dll;
typedef struct dll *DLL;
typedef void *ELEMENT;


/*
 * Create an empty linked list
 */
void *createDll();

/*
 * Use the element after current position as
 * current position.
 */
int next(DLL);

/*
 * Use the element before current position as
 * current position 
 */
int prev(DLL);

/*
 * Use the first element as current position 
 */
int first(DLL);

/*
 * Use the last element as current position
 */ 
int last(DLL);

/*
 * Peek if there are an element after current position
 * Zero is returned if there are none, nonezero otherwise 
 */
int peekDll(DLL);

/*
 * Return the element at a certain position
 * NULL is returned if no such position
 */
ELEMENT elementAt(DLL,int);

/*
 * Return the current element or NULL if noone
 */
ELEMENT this(DLL);

/*
 * Inserts the element LAST in the list and set current element
 * to the new element inserted
 */
void append(DLL,ELEMENT);


/*
 * Remove an entire list, you'll need to supply a
 * function that removes the individual elements
 */
void deleteAll(DLL,void (*f)(ELEMENT));

/*
 * Delete the current element. New current element
 * is the lement after or none if no such element
 */
void deleteCurrentElement(DLL,void (*f)(ELEMENT));

/*
 * Returns number of element in list.
 */
int getNumber(DLL list);

/*
 * Apply a function on every element in the list
 */
int map(DLL list, int (*f)(ELEMENT));

/* Searches the list and compares all elements with the equal function until the equal
 * function returns a nonzero value. The current element is then set to that element.
 * Return value is zero if an alement was found, otherwise nonzero
 */
int search(DLL, ELEMENT, int (*equal)(ELEMENT, ELEMENT));

#endif
