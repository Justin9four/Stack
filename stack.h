/***********************************************************************
* Header:
*    Stack
* Summary:
*    This class contains the notion of a stack: a container
*    that follows the Last In First Out flow and 
*    expands as more items are put inside.
*
*    This will contain the class definition of:
*        Stack         : A class that holds stuff
* Author
*    Nathan Bench, Jeremy Chandler, Justin Chandler
************************************************************************/

#ifndef STACK_H
#define STACK_H

#include <cassert>


/************************************************
 * STACK
 * A class that holds stuff
 ***********************************************/
template <class T>
class Stack
{
public:
   // default constructor : empty and kinda useless
   Stack() : m_top(-1), m_capacity(0), m_data(NULL) {}

   // copy constructor : copy it
   Stack(const Stack & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Stack(int capacity) throw (const char *);
   
   // destructor : free everything
   ~Stack()        { if (m_capacity) delete [] m_data; }
   
   // is the container currently empty
   bool empty() const  { return m_top == -1;         }

   // remove all the items from the container
   void clear()        { m_top = -1;                 }

   // how many items can the stack currently contain?
   int capacity() const { return m_capacity;             }
   
   // how many items are currently in the container?
   int size() const    { return m_top;              }

   // add an item to the container
   void push(const T & t) throw (const char *);

   // Removes an item from the end of the stack, and reduces size by one
   Stack<T> & pop() throw (const char *);

   // Returns the item currently at the end of the stack
   Stack<T> & top() throw (const char *);
   
   // look up an item using the array index operator '[]'
   T & operator [] (int index) throw (const char *);
   const T & operator [] (int index) const throw (const char *);
   
   // assignment operator '='
   Stack<T> & operator = (const Stack <T> & rhs);
   
private:
   T * m_data;            // dynamically allocated array of T
   int m_top;           // the top of the stack
   int m_capacity;      // how many items can I put on the Stack before full?
};


/*******************************************
 * STACK :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Stack <T> :: Stack(const Stack <T> & rhs) throw (const char *)
{
   assert(rhs.m_capacity >= 0);
      
   // do nothing if there is nothing to do
   if (rhs.m_capacity == 0)
   {
      m_capacity = m_top = 0;
      m_data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      m_data = new T[rhs.m_capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }
   
   // copy over the capacity and size
   assert(rhs.m_top >= 0 && rhs.m_top <= rhs.m_capacity);
   m_capacity = rhs.m_capacity;
   m_top = rhs.m_top;

   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < m_top; i++)
      m_data[i] = rhs.m_data[i];

   // the rest needs to be filled with the default value for T
   for (int i = m_top; i < m_capacity; i++)
      m_data[i] = T();
}

/**********************************************
 * STACK : NON-DEFAULT CONSTRUCTOR
 * Preallocate the container to "capacity"
 **********************************************/
template <class T>
Stack <T> :: Stack(int capacity) throw (const char *)
{
   assert(capacity >= 0);
   
   // do nothing if there is nothing to do
   if (m_capacity == 0)
   {
      this->m_capacity = this->m_top = 0;
      this->m_data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      m_data = new T[capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

      
   // copy over the stuff
   this->m_capacity = capacity;
   this->m_top = 0;

   // initialize the container by calling the default constructor
   for (int i = 0; i < m_capacity; i++)
      m_data[i] = T();
}

/***************************************************
 * STACK :: PUSH
 * Insert an item on the end of the container
 **************************************************/
template <class T>
void Stack <T> :: push(const T & t) throw (const char *)
{
   // IF capacity == 0
   if (m_capacity == 0)
   {
      m_capacity = 1;
      try
      {
         m_data = new T[m_capacity];
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: unable to allocate a new buffer for Stack";
      }
   }
   
   // IF max capacity AND m_top is not less than 0
   if (m_capacity == m_top && m_top > 0)
   {
      m_capacity *= 2;
      try
      {
         T* tempArray = new T[m_capacity];
         
         // copy
         for (int i = 0; i < m_top; i++)
         {
            tempArray[i] = m_data[i];
         }

         // free memory
         delete[] m_data;

         // point to tempArray
         m_data = tempArray;
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Stack";
      }
   }
   
   // add an item to the end
   m_data[m_top++] = t;
}

/***************************************************
* STACK :: POP
* Removes an item from the end of the stack, and reduces size by one
**************************************************/
template<class T>
inline Stack<T>& Stack<T>::pop() throw(const char *)
{
   if (empty())
      throw "ERROR: Unable to reference the element from an empty Stack";
	try
	{
		// Create a temporary array, but if it fails
		// we don't want any changes to m_capacity or m_top
		T* tempArray = new T[(m_capacity - 1)];

		// copy
		for (int i = 0; i < (m_top - 1); i++)
		{
			tempArray[i] = m_data[i];
		}

		// free memory
		delete[] m_data;

		// point to tempArray
		m_data = tempArray;
		// Success! Now we can lower m_top and m_capacity for good.
		m_top--;
		m_capacity--;
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a new buffer for Stack";
	}
}

/***************************************************
* STACK :: TOP
* Returns the item currently at the end of the stack
**************************************************/
template<class T>
inline Stack<T>& Stack<T>::top() throw(const char *)
{
	// if empty: throw Unable to reference the element from an empty Stack
	if (empty())
		throw "ERROR: Unable to reference the element from an empty Stack";
	else
		return m_data[m_top];
}

/***************************************************
 * STACK :: =
 * Overload assignment operator
 **************************************************/
 template <class T>
Stack<T> & Stack <T> :: operator = (const Stack <T> & rhs)
{
   // don't copy yourself
   if (this != &rhs)
   {
      // clean up m_data
      if (m_data)
         delete [] m_data;
      
      // assign each member variable to right-hand-side
      m_capacity = rhs.m_capacity;
      m_top = rhs.m_top;
      
      // allocate new array
      try
      {
         m_data = new T[m_capacity];
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Stack";
      }
      // copy over values from rhs
      for (int i = 0; i < rhs.m_top; i++)
      {
         m_data[i] = rhs.m_data[i];
      }
      
      return *this;
   }
}

#endif // STACK_H

