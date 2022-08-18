#include "quilt.h" 
#include <cstdlib> 

using namespace std;

int main()
{
Quilt t0, t1, t2, t3, t4;
t0 = Quilt(a0);
t1 = turn(t0);
t2 = Quilt(b0);
t3 = turn(t2);
t4 = sew(t1, t3);

t4.print_quilt();

return 0;
}
