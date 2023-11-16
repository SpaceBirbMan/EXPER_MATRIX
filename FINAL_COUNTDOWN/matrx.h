#pragma once
//#define MTRX_DEBUG
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <random>
#include <iomanip>
#include <time.h>
#include <typeinfo>

namespace nonstd
{
	template <typename T>
	T GetRandomNumber(T min, T max) {
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(rd());  // to seed mersenne twister.

		if constexpr (std::is_integral<T>::value) {
			std::uniform_int_distribution<T> dist(min, max);
			return dist(gen);
		}
		else {
			std::uniform_real_distribution<T> dist(min, max);
			return dist(gen);
		}
	}

	//����� �������
	template <typename T>
	class matrix
	{

	private:
		std::vector<std::vector<T>> M; // ������� ��������� ���� �

		//unsigned int row_count = 0; // ���������� �����
		//unsigned int col_count = 0; // ���������� ��������

	public:

		//�������� ���������� �����
		unsigned int get_row_count() const { if (M.empty()) return 0; else return M.size(); }
		//�������� ���������� ��������
		unsigned int get_col_count() const 
		{
			if (M.empty()) 
				return 0; // ������� �����, ������� �������� ���
			return M[0].size(); // ���������� ���������� ��������� � ������ ������
		}
	
		//�������� �������
		
		//������ ���������� �����
		void set_row_count(unsigned const& row)
		{
			if (row > 0) M.resize(row, std::vector<T>(get_col_count())); else throw "row < 0";
		}
		//������ ���������� ��������
		void set_col_count(unsigned const& col)
		{
			if (col > 0) M.resize(get_row_count(), std::vector<T>(col)); else throw "col < 0";
		}

		//����������� �������
		matrix(unsigned int rows, unsigned int cols)
		{
			M.resize(rows, std::vector<T>(cols));
		}

		matrix(unsigned int size)
		{
			M.resize(size, std::vector<T>(1));
		}
		//������ �����������
		matrix() {}
		// ����������� �� ���������� ������
		matrix(const std::vector<std::vector<T>>& values) : M(values) {}
		//�������� ��������� �������� �� �������
		T& operator () (int row, int col = -1)
		{
			//todo: �������� �� ���������� col,row > 0
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
			if (&R != this && this->get_row_count() >= R.get_row_count() && this->get_col_count() >= R.get_col_count())
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
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
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

			for (int i = 0; i < this->get_col_count(); ++i) {
				for (int j = 0; j < I.get_row_count(); ++j) {
					double sum = 0.0;
					for (int k = 0; k < this->get_row_count(); ++k) {
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
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
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
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					M[i][j] = M[i][j] - I(i, j);
			return *this;
		}
		//�������� ���������� ���� ������� ����� ���������
		void operator << (T inp)
		{
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					this->M[i][j] = inp;
			return;
		}

		//���������� ������� �� ������� //��������
		matrix& col_copy(unsigned col)
		{
			matrix<T> result(get_row_count(), 1); // ������� ����� ������ matrix
			for (int i = 0; i < get_row_count(); i++) {
				result(i, 0) = M[i][col];
			}
			return result;
		}
		//���������� ������ �� ������� //��������
		matrix& row_copy(unsigned row)
		{
			matrix<T> result(1, get_col_count()); // ������� ����� ������ matrix
			for (int j = 0; j < get_col_count(); j++) {
				result(0, j) = M[row][j];
			}
			return result;
		}

		//����������� �������
		void randomize(T min, T max)
		{
			for (int i = 0; i < get_row_count(); i++)
			{
				for (int j = 0; j < get_col_count(); j++)
				{
					M[i][j] = min + static_cast<T>(rand()) / RAND_MAX * (max - min);
				}
			}
		}
		//����� ����� ����������� �������
		void randomize_advanced(T min, T max) 
		{ for (int i = 0; i < get_row_count(); i++) 
			for (int j = 0; j < get_col_count(); j++) 
				M[i][j] = GetRandomNumber(min, max); 
		}
		//������� � ������ ������/�������
		std::string to_string()
		{
			std::string data = "";
			{
				for (int i = 0; i < get_row_count(); i++)
				{
					for (int j = 0; j < get_col_count(); j++)
						data = data + " " + std::to_string(M[i][j]);
					data = data + "\n"; //������� ������ ��� �������� ������ ������ �������
				}
			}
			return data; //���� ��� ���� ����-�� ������� ������
		}
		//�������� �� ��������� (������ ������ ������� ��� ���������� ������)
		void diag_antizero(unsigned int row, unsigned int col)
		{
			unsigned n = get_row_count(), m = get_col_count();
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
			for (int i = 0; i < get_row_count() - row && i < get_col_count() - col; i++)
				M[i + col][i + row] = scalar;
		}
		//������ �������� �������
		void inverse()
		{ 
			matrix <T> INV(get_row_count(), get_col_count() * 2);
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
				{
					INV(i, j) = M[i][j];
					INV(i,i+get_row_count()) = 1;
				}

			for (int k = 0; k < get_row_count(); k++)
			{
				for (int i = 0; i < get_row_count(); i++)
					for (int j = k + 1; j < 2 * get_row_count(); j++)
						if (i != k)
							INV(i, j) = (INV(i, j) * INV(k, k) - INV(k, j) * INV(i, k)) / INV(k, k);

				for (int i = 0; i < get_row_count(); i++)
					if (i != k)
						INV(i, k) = 0;

				for (int j = 2 * get_row_count() - 1; j >= k; j--)
					INV(k, j) = INV(k, j) / INV(k, k);
#ifdef MTRX_DEBUG
				std::cout << INV << std::endl;
#endif // MTRX_DEBUG
				
			}

			
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
				{
					M[i][j] = INV(i,j+get_row_count());
				}
		}
		//���������������� ������� �������
		void transpond()
		{
			matrix<T> TEMP(get_col_count(), get_row_count()); // ������� ��������� �������

			for (int i = 0; i < get_row_count(); i++) {
				for (int j = 0; j < get_col_count(); j++) {
					TEMP(j, i) = M[i][j]; // ��������� ��������� ������� ������������������ ����������
				}
			}

			*this = TEMP; // �������� �������� �� ��������� ������� � ������� �������
		}
		//������
		void resize(unsigned new_row, unsigned new_col)
		{
			if (new_col > 0 && new_row > 0)
				M.resize(new_row);
			for (int i = 0; i < new_row; i++)
				M[i].resize(new_col);
		}

		void determinant()
		{
		}

		//todo: ������������, ����������� ��������, []

		void is_singular() //������ �� ������������
		{}

		~matrix() {}
	};

	//�������� ������ �������
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const matrix<T>& mat)
	{
		unsigned setw = 5;
		if (typeid(T).name() == "int") setw = 3; else setw = 10;
		std::fixed;
		for (int i = 0; i < mat.get_row_count(); i++)
		{
			for (int j = 0; j < mat.get_col_count(); j++)
			{
				os << std::setw(setw) << std::setprecision(6) << mat(i, j) << std::setfill(' ');
			}
			os << std::endl;
		}
		return os;
	}

	//template <typename T>
	//class array: public matrix 
	//{
	//private:
	//	std::vector<T> A; // ������ ��������� ���� �
	//public:
	//	//����������� ������� (������� - �������)
	//	array(unsigned int size)
	//	{
	//		row_count = size;
	//		col_count = 1;
	//		M.resize(row_count, std::vector<T>(col_count));
	//		M[0] = A;
	//	}
	//};
}