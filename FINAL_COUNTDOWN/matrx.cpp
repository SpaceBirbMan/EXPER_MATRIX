#include <iostream>
#include <ostream>

template <typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& mat)
{
	for (int i = 0; i < mat.get_row_count(); i++)
	{
		for (int j = 0; j < mat.get_col_count(); j++)
		{
			os << std::setw(5) << mat(i, j) << std::setfill(' ');
		}
		os << std::endl;
	}
	return os;
}