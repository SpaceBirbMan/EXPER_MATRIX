#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <random>
#include <iomanip>
#include <time.h>

template <typename T>
T GetRandomNumber(T min, T max)
{
	std::random_device rd;   // non-deterministic generator
	std::mt19937 gen(rd());  // to seed mersenne twister.
	std::uniform_int_distribution<> dist(min, max); // distribute results between min and max inclusive.
	return dist(gen) + dist(gen); // pass the generator to the distribution.
}

//����� �������
template <typename T>
class matrix
{

private:
	std::vector<std::vector<T>> M; // ������� ��������� ���� �

	unsigned int row_count = 0; // ���������� �����
	unsigned int col_count = 0; // ���������� ��������

public:

	//�������� ���������� �����
	unsigned int get_row_count() const { return row_count; }
	//�������� ���������� ��������
	unsigned int get_col_count() const { return col_count; }
	//������ ���������� �����
	void set_row_count(unsigned const& row) 
	{
		row_count = row;
		M.resize(row_count, std::vector<T>(col_count));
	}
	//������ ���������� ��������
	void set_col_count(unsigned const& col) 
	{ 
		col_count = col; 
		M.resize(row_count, std::vector<T>(col_count));
	}

	//����������� �������
	matrix(unsigned int rows, unsigned int cols)
	{
		row_count = rows;
		col_count = cols;
		M.resize(row_count, std::vector<T>(col_count));
	}

	//����������� ������� (������� - �������)
	matrix(unsigned int size)
	{
		row_count = size;
		col_count = 1;
		M.resize(row_count, std::vector<T>(col_count));
	}

	//������ �����������
	matrix();

	//�������� ��������� �������� �� �������
	T& operator () (int row, int col = -1)
	{
		//�������� �� ���������� col,row > 0
		if (col > -1) return this->M[row][col];
		else return this->M[row][0];
	}
	//���������� ��� �������� ������
	const T& operator () (int row, int col = -1) const
	{
		if (col > -1) return this->M[row][col];
		else return this->M[row][0];
	}
	//�������� ������������
	matrix& operator = (const matrix& R)
	{
		if (&R != this && this->row_count >= R.get_row_count() && this->col_count >= R.get_col_count())
		{
			for (int i = 0; i < R.get_row_count(); i++)
				for (int j = 0; j < R.get_col_count(); j++)
				{
					this->M[i][j] = R.M[i][j];
				}
		}
		else { throw "INCOMPATBLE_MATRICES"; abort; }
		return *this;
	}
	//�������� ��������� �� ������
	matrix& operator* (const T scalar)
	{
		for (int i = 0; i < row_count; i++)
			for (int j = 0; j < col_count; j++)
				M[i][j] = scalar * M[i][j];
		return *this;
	}
	//�������� ��������� �� �������
	matrix& operator* (matrix& I)
	{
		if (this->col_count != I.get_row_count()) {
			throw "unable_to_mult";
			exit;
		}

		matrix result(this->col_count, I.get_row_count());

		for (int i = 0; i < this->col_count; ++i) {
			for (int j = 0; j < I.row_count; ++j) {
				double sum = 0.0;
				for (int k = 0; k < this->row_count; ++k) {
					sum += this->M[i][k] * I(k, j);
				}
				result(i, j) = sum;
			}
		}
		return result;
	}
	//�������� ������� �� ������
	matrix& operator/ (const T scalar)
	{
		for (int i = 0; i < row_count; i++)
			for (int j = 0; j < col_count; j++)
				M[i][j] = M[i][j] / scalar;
		return *this;
	}
	//�������� ������� �� �������
	matrix& operator/ (matrix& I)
	{
		return *this;
	}
	//�������� ��������� ����� ������� �� ������
	matrix& operator- (matrix& I)
	{
		for (int i = 0; i < row_count; i++)
			for (int j = 0; j < col_count; j++)
				M[i][j] = M[i][j] - I(i, j);
		return *this;
	}
	//�������� ���������� ���� ������� ����� ���������
	void operator << (T inp)
	{
		for (int i = 0; i < row_count; i++)
			for (int j = 0; j < col_count; j++)
				this->M[i][j] = inp;
		return;
	}

	//���������� ������� �� ������� //��������
	matrix& col_copy(unsigned col)
	{
		matrix<T> result(row_count, 1); // ������� ����� ������ matrix
		for (int i = 0; i < row_count; i++) {
			result(i, 0) = M[i][col];
		}
		return result;
	}
	//���������� ������ �� ������� //��������
	matrix& row_copy(unsigned row)
	{
		matrix<T> result(1, col_count); // ������� ����� ������ matrix
		for (int j = 0; j < col_count; j++) {
			result(0, j) = M[row][j];
		}
		return result;
	}

	//����������� �������
	void randomize(T min, T max)
	{
		for (int i = 0; i < row_count; i++)
		{
			for (int j = 0; j < col_count; j++)
			{
				M[i][j] = min + rand() % (max - min + 1) + static_cast<T>(rand()) / RAND_MAX;
			}
		}
	}
	//����� ����� ����������� �������
	void randomize_advanced(T min, T max) { for_each(M.begin(), M.end(), GetRandomNumber(min, max)); }
	//������� � ������ ������/�������
	std::string to_string()
	{
		std::string data = "";
		{
			for (int i = 0; i < row_count; i++)
			{
				for (int j = 0; j < col_count; j++)
					data = data + " " + std::to_string(M[i][j]);
				data = data + "\n"; //������� ������ ��� �������� ������ ������ �������
			}
		}
		return data; //���� ��� ���� ����-�� ������� ������
	}
	//�������� �� ��������� (������ ������ ������� ��� ���������� ������)
	void diag_antizero(unsigned int row, unsigned int col)
	{
		unsigned n = row_count, m = col_count;
		matrix <T> Mx(n, m);
		for (int i = row; i < n; i++)
		{
			if (abs(this->M[i][i]) < 0.000001)
			{
				for (int ii = 0; ii < col - 1; ii++)
					if (this->M[i][i] != 0)
					{
						Mx = this->M[ii][i];
						this->M[ii][i] = this->M[i][i];
					}
			}
		}
	}
	//������ ��������� ���������, � ������� � [col row] �� scalar
	void diag_replace(unsigned int row, unsigned int col, double scalar = 0)
	{
		for (int i = 0; i < row_count - row && i < col_count - col; i++)
			M[i + col][i + row] = scalar;
	}
	//������ �������� �������
	void inverse()
	{}
	//����������������
	void transpond()
	{
		matrix<T> TEMP(col_count, row_count); // ������� ��������� �������

		for (int i = 0; i < row_count; i++) {
			for (int j = 0; j < col_count; j++) {
				TEMP(j, i) = M[i][j]; // ��������� ��������� ������� ������������������ ����������
			}
		}

		*this = TEMP; // �������� �������� �� ��������� ������� � ������� �������
	}
	//������
	void resize(unsigned new_row, unsigned new_col = 0)
	{
		set_row_count(new_row);
		if (new_col > 0)
			set_col_count(new_col);
		M.resize(new_row, std::vector<T>(new_col));
	}

	//todo: ������������, ����������� ��������

	~matrix() {}
};

//�������� ������ �������
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
