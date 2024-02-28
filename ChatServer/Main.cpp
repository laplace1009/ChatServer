#include "pch.h"
#include <iostream>
#include <vector>
#include "StlAllocator.h"
#include "Types.h"
#include "Memory.h"

using namespace std;

int main(void)
{
	int32* num = xnew<int32>(10);
	cout << *num << endl;
	xdelete<int32>(num);

	return 0;
}