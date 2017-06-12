#ifndef PriorityQueue_hpp
#define PriorityQueue_hpp

// include Arduino basic header.
#include <Arduino.h>
#include "../CustomTypes/Checked.hpp"

// the definition of the queue class.
template<typename T>
class PriorityQueue
{
public:
  // init the queue (constructor).
  PriorityQueue();

  // clear the queue (destructor).
  ~PriorityQueue();

  // push an item to the queue.
  bool push (const T i, float priority);

  // Pega e remove o item de maior prioridade da lista
  Checked<T> pop ();

  // check if the queue is empty.
  bool isEmpty () const;

  // get the number of items in the queue.
  int count () const;

  // set the printer of the queue.
  void setPrinter (Print & p);

private:
  // the structure of each node in the list.
  typedef struct node
  {
    T item;      // the item in the node.
    float priority;
    node * next; // the next node in the list.
  } node;

  typedef node * link; // synonym for pointer to a node.

  Print * m_Printer; // the printer of the queue.
  unsigned int m_Size;        // the size of the queue.
  link m_Head;       // the head of the list.
  link m_Tail;       // the tail of the list.
};

// init the queue (constructor).
template<typename T>
PriorityQueue<T>::PriorityQueue () {
  m_Size = 0;       // set the size of queue to zero.
  m_Head = nullptr;    // set the head of the list to point nowhere.
  m_Tail = nullptr;    // set the tail of the list to point nowhere.
  m_Printer = nullptr; // set the printer of queue to point nowhere.
}

// clear the queue (destructor).
template<typename T>
PriorityQueue<T>::~PriorityQueue () {
  // deallocate memory space of each node in the list.
  for (link t = m_Head; t != nullptr; m_Head = t)
  {
    t = m_Head->next; delete m_Head;
  }

  m_Size = 0;       // set the size of queue to zero.
  m_Tail = nullptr;    // set the tail of the list to point nowhere.
  m_Printer = nullptr; // set the printer of queue to point nowhere.
}

// push an item to the queue.
template<typename T>
bool PriorityQueue<T>::push (const T i, float priority) {
  // create a temporary pointer to tail.
  link t = m_Tail;

  // create a new node for the tail.
  m_Tail = (link) new node;

  // if there is a memory allocation error.
  if (m_Tail == nullptr)
    return false;

  // set the next of the new node.
  m_Tail->next = nullptr;

  // store the item to the new node.
  m_Tail->item = i;
  m_Tail->priority = priority;

  // check if the queue is empty.
  if (isEmpty ())
    // make the new node the head of the list.
    m_Head = m_Tail;
  else
    // make the new node the tail of the list.
    t->next = m_Tail;

  // increase the items.
  m_Size++;
  return true;
}

// pop an item from the queue.
template<typename T>
Checked<T> PriorityQueue<T>::pop ()
{
  // check if the queue is empty.
  if (isEmpty ())
    return Checked<T>(false);

  link previous = m_Head;
  link current = previous;
  link trace = nullptr;

  link priorityItem = nullptr;
  float bestPriority = 9999999;

  while(current->next != nullptr)
  {
    if(current->priority < bestPriority)
    {
      priorityItem = current;
      bestPriority = priorityItem->priority;
      trace = previous;
    }
    previous = current;
    current = current->next;
  }

  // remove only the head node.
  link t = current->next;
  delete current;
  current = t;

  // decrease the items.
  m_Size--;

  // return the item.
  return Checked<T>(true, priorityItem->item);
}

// check if the queue is empty.
template<typename T>
bool PriorityQueue<T>::isEmpty () const {
  return m_Head == nullptr;
}

// get the number of items in the queue.
template<typename T>
int PriorityQueue<T>::count () const {
  return m_Size;
}

#endif // PriorityQueue_hpp
