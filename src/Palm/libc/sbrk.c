#include <PalmOS.h>

void * sbrk(long size)  
{
  void * result;
  static void *last=0;

  if (size <= 0) {
    if (size == 0) {
      return last;
    }
    else
      return (void *)-1;
  }
  
  if ((result = MemPtrNew(size))==NULL)
    return (void *)-1;

  last = (void *) ((long)result+size);
  return result;
}
