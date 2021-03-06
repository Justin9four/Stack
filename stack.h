/***********************************************************************
* Header:
*    Stack
* Summary:
*    This class contains the notion of a stack: a container
*    that expands as more items are put inside.
*
*    This will contain the class definition of:
*        Stack         : A class that holds stuff
*        StackIterator : An interator through Stack
* Author
*    Nathan Bench
************************************************************************/

#ifndef STACK_H
#define STACK_H
#define NULL 0

#include <cassert>
#include <new>

// forward declaration for StackIterator
template <class T>
class StackIterator;

// forward declaration for StackConstIterator
template <class T>
class StackConstIterator;

/************************************************
 * STACK
 * A class that holds stuff
 ***********************************************/
template <class T>
class Stack
{
public:
   // default constructor : empty and kinda useless
   Stack() : numItems(0), vCapacity(0), data(NULL) {}

   // copy constructor : copy it
   Stack(const Stack & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Stack(int capacity) throw (const char *);
   
   // destructor : free everything
   ~Stack()        { if (vCapacity) delete [] data; }
   
   // is the container currently empty
   bool empty() const  { return numItems == 0;         }

   // remove all the items from the container
   void clear()        { numItems = 0;                 }

   // how many items can the stack currently contain?
   int capacity() const { return vCapacity;             }
   
   // how many items are currently in the container?
   int size() const    { return numItems;              }

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
   
   // return an iterator to the beginning of the stack
   StackIterator <T> begin() { return StackIterator<T>(data); }

   // return an iterator to the end of the stack
   StackIterator <T> end() { return StackIterator<T>(data + numItems);}
   
   // return a constant iterator to the beginning of the stack
   StackConstIterator <T> cbegin() const { return StackConstIterator<T>(data); }
   
   // return a constant iterator to the end of the stack
   StackConstIterator <T> cend() const   { return StackConstIterator<T>(data + numItems); }
   
private:
   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Stack?
   int vCapacity;      // how many items can I put on the Stack before full?
};


/*******************************************
 * STACK :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Stack <T> :: Stack(const Stack <T> & rhs) throw (const char *)
{
   assert(rhs.vCapacity >= 0);
      
   // do nothing if there is nothing to do
   if (rhs.vCapacity == 0)
   {
      vCapacity = numItems = 0;
      data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[rhs.vCapacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }
   
   // copy over the capacity and size
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.vCapacity);
   vCapacity = rhs.vCapacity;
   numItems = rhs.numItems;

   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < numItems; i++)
      data[i] = rhs.data[i];

   // the rest needs to be filled with the default value for T
   for (int i = numItems; i < vCapacity; i++)
      data[i] = T();
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
   if (vCapacity == 0)
   {
      this->vCapacity = this->numItems = 0;
      this->data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

      
   // copy over the stuff
   this->vCapacity = capacity;
   this->numItems = 0;

   // initialize the container by calling the default constructor
   for (int i = 0; i < vCapacity; i++)
      data[i] = T();
}

/***************************************************
 * STACK :: PUSH
 * Insert an item on the end of the container
 **************************************************/
template <class T>
void Stack <T> :: push(const T & t) throw (const char *)
{
   // IF capacity == 0
   if (vCapacity == 0)
   {
      vCapacity = 1;
      try
      {
         data = new T[vCapacity];
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: unable to allocate a new buffer for Stack";
      }
   }
   
   // IF max capacity AND numItems is not less than 0
   if (vCapacity == numItems && numItems > 0)
   {
      vCapacity *= 2;
      try
      {
         T* tempArray = new T[vCapacity];
         
         // copy
         for (int i = 0; i < numItems; i++)
         {
            tempArray[i] = data[i];
         }

         // free memory
         delete[] data;

         // point to tempArray
         data = tempArray;
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Stack";
      }
   }
   
   // add an item to the end
   data[numItems++] = t;
}

/***************************************************
* STACK :: POP
* Removes an item from the end of the stack, and reduces size by one
**************************************************/
template<class T>
inline Stack<T>& Stack<T>::pop() throw(const char *)
{
	try
	{
		// Create a temporary array, but if it fails
		// we don't want any changes to vCapacity or numItems
		T* tempArray = new T[(vCapacity - 1)];

		// copy
		for (int i = 0; i < (numItems - 1); i++)
		{
			tempArray[i] = data[i];
		}

		// free memory
		delete[] data;

		// point to tempArray
		data = tempArray;
		// Success! Now we can lower numItems and vCapacity for good.
		numItems--;
		vCapacity--;
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
		return data[numItems];
}

/***************************************************
 * STACK :: []
 * Overload array index operator
 **************************************************/
template <class T>
T & Stack<T> :: operator [] (int index) throw (const char *)
{
   // return if index valid
   if (index >= 0 && index < numItems)
      return data[index];
   // throw invalid index
   throw "ERROR: Invalid index";
}

/***************************************************
 * STACK :: [] const
 * Overload array index operator
 **************************************************/
template <class T>
const T & Stack <T> :: operator [] (int index) const throw (const char *)
{
   // return if index valid
   if (index >= 0 && index < numItems)
      return data[index];
   // throw invalid index
   throw "ERROR: Invalid index";
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
      // clean up data
      if (data)
         delete [] data;
      
      // assign each member variable to right-hand-side
      vCapacity = rhs.vCapacity;
      numItems = rhs.numItems;
      
      // allocate new array
      try
      {
         data = new T[vCapacity];
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Stack";
      }
      // copy over values from rhs
      for (int i = 0; i < rhs.numItems; i++)
      {
         data[i] = rhs.data[i];
      }
      
      return *this;
   }
}

#endif // STACK_H

