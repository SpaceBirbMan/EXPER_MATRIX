#include "matrx.h"
#include <iostream>

int main()
{
	matrix <long long> G(1,1);
	std::cout << G;
	try { G.determinant(); }
	catch (...) { std::cout << "wrong type. unable to calculate determinant"; }
}
