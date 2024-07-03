#pragma once
/* -*-Mode: C++;-*- */
//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
//#include "copyright.h"


//////////////////////////////////////////////////////////////////////
//  
//  list.h
//
//  This file implements a list template.
//  Adapted from similar templates written by Kathy Yelick and 
//  Paul Hilfinger.
//
//////////////////////////////////////////////////////////////////////


//#include "cool-io.h"  //includes iostream
#include <stdlib.h>

template <class T>
class List {
private:
  T *head;
  List<T>* tail;
public:
  List(T *h,List<T>* t = NULL): head(h), tail(t) { }

  T *head() const       { return head; }  
  List<T>* taill() const { return tail; }
  bool empty() const { return head == nullptr; }
  
  T* add(T* elt) {

  }
};

/////////////////////////////////////////////////////////////////////////
// 
// list function templates
//
// To avoid potential problems with mutliple definitions of 
// the List<> class members, the list functions are not members of the
// list class.
//
/////////////////////////////////////////////////////////////////////////

//
// Map a function for its side effect over a list.
//
template <class T>
void list_map(void f(T*), List<T> *l)
{
  for (l; l != NULL; l = l->tail())
    f(l->head());
}

//
// Print the given list on the standard output.
// Requires that "<<" be defined for the element type.
//
template <class S, class T>
void list_print(S &str, List<T> *l)
{
   str << "[\n";
   for(; l != NULL; l = l->tail())
	str << *(l->head()) << " ";
   str << "]\n";
}

//
// Compute the length of a list.
//
template <class T>
int list_length(List<T> *l)
{
  int i = 0;
  for (; l != NULL; l = l->tail())
    i++;
  return i;
}
