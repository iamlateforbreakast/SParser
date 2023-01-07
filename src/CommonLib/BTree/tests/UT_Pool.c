#include "Types.h"
#include "Pool.h"

int main()
{
   Pool * testPool = 0;

   testPool = Pool_newFromFile("test.pool", 10, 10);
   Pool_delete(testPool);
}
