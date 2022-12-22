
#include "vector.hpp"
#include "matrix.hpp"

#include <iostream>

int main()
{
	matrix<2> mat = matrix<2>::identity();
	vector<2> pos({2, 3});

	std::cout << mat << std::endl;
	std::cout << (mat * pos) << std::endl;

	pos.normalize();

	std::cout << pos << std::endl;
}

