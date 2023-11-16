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

	//Êëàññ ìàòðèöû
	template <typename T>
	class matrix
	{

	private:
		std::vector<std::vector<T>> M; // Ìàòðèöà ýëåìåíòîâ òèïà Ò

		//unsigned int row_count = 0; // Êîëè÷åñòâî ñòðîê
		//unsigned int col_count = 0; // Êîëè÷åñòâî ñòîëáöîâ

	public:

		//ïîëó÷èòü êîëè÷åñòâî ñòðîê
		unsigned int get_row_count() const { if (M.empty()) return 0; else return M.size(); }
		//ïîëó÷èòü êîëè÷åñòâî ñòîëáöîâ
		unsigned int get_col_count() const 
		{
			if (M.empty()) 
				return 0; // Ìàòðèöà ïóñòà, ïîýòîìó ñòîëáöîâ íåò
			return M[0].size(); // Âîçâðàùàåì êîëè÷åñòâî ýëåìåíòîâ â ïåðâîé ñòðîêå
		}
	
		//ïîëó÷èòü ìàòðèöó
		
		//çàäàòü êîëè÷åñòâî ñòðîê
		void set_row_count(unsigned const& row)
		{
			if (row > 0) M.resize(row, std::vector<T>(get_col_count())); else throw "row < 0";
		}
		//çàäàòü êîëè÷åñòâî ñòîëáöîâ
		void set_col_count(unsigned const& col)
		{
			if (col > 0) M.resize(get_row_count(), std::vector<T>(col)); else throw "col < 0";
		}

		//êîíñòðóêòîð ìàòðèöû
		matrix(unsigned int rows, unsigned int cols)
		{
			M.resize(rows, std::vector<T>(cols));
		}

		matrix(unsigned int size)
		{
			M.resize(size, std::vector<T>(1));
		}
		//ïóñòîé êîíñòðóêòîð
		matrix() {}
		// Êîíñòðóêòîð èç äâóìåðíîãî ñïèñêà
		matrix(const std::vector<std::vector<T>>& values) : M(values) {}
		//îïåðàòîð ïîëó÷åíèÿ çíà÷åíèÿ ïî èíäåêñó
		T& operator () (int row, int col = -1)
		{
			//todo: ïðîâåðêà íà ïðàâèëüíûé col,row > 0
			if (col > -1) return this->M[row][col];
			else return this->M[row][0];
		}
		//äîïîëíåíèå ïîä îïåðàòîð ñêîáîê
		const T& operator () (int row, int col = -1) const
		{
			if (col > -1) return this->M[row][col];
			else return this->M[row][0];
		}
		//îïåðàòîð ïðèñâàèâàíèÿ
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
		//îïåðàòîð óìíîæåíèÿ íà ñêàëÿð
		matrix& operator* (const T scalar)
		{
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					M[i][j] = scalar * M[i][j];
			return *this;
		}
		//îïåðàòîð óìíîæåíèÿ íà ìàòðèöó
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
		//îïåðàòîð äåëåíèÿ íà ñêàëÿð
		matrix& operator/ (const T scalar)
		{
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					M[i][j] = M[i][j] / scalar;
			return *this;
		}
		//îïåðàòîð äåëåíèÿ íà ìàòðèöó
		matrix& operator/ (matrix& I)
		{
			return *this;
		}
		//îïåðàòîð âû÷èòàíèÿ îäíîé ìàòðèöû èç äðóãîé
		matrix& operator- (matrix& I)
		{
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					M[i][j] = M[i][j] - I(i, j);
			return *this;
		}
		//îïåðàòîð çàïîëíåíèÿ âñåé ìàòðèöû îäíèì ýëåìåíòîì
		void operator << (T inp)
		{
			for (int i = 0; i < get_row_count(); i++)
				for (int j = 0; j < get_col_count(); j++)
					this->M[i][j] = inp;
			return;
		}

		//Âîçâðàùàåò ñòîëáåö èç ìàòðèöû //ïðîâåðêà
		matrix& col_copy(unsigned col)
		{
			matrix<T> result(get_row_count(), 1); // Ñîçäàåì íîâûé îáúåêò matrix
			for (int i = 0; i < get_row_count(); i++) {
				result(i, 0) = M[i][col];
			}
			return result;
		}
		//Âîçâðàùàåò ñòðîêó èç ìàòðèöû //ïðîâåðêà
		matrix& row_copy(unsigned row)
		{
			matrix<T> result(1, get_col_count()); // Ñîçäàåì íîâûé îáúåêò matrix
			for (int j = 0; j < get_col_count(); j++) {
				result(0, j) = M[row][j];
			}
			return result;
		}

		//ðàíäîìàéçåð ìàòðèöû
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
		//áîëåå ãèêèé ðàíäîìàéçåð ìàòðèöû
		void randomize_advanced(T min, T max) 
		{ for (int i = 0; i < get_row_count(); i++) 
			for (int j = 0; j < get_col_count(); j++) 
				M[i][j] = GetRandomNumber(min, max); 
		}
		//âûâîäèò â ñòðîêó ìàññèâ/ìàòðèöó
		std::string to_string()
		{
			std::string data = "";
			{
				for (int i = 0; i < get_row_count(); i++)
				{
					for (int j = 0; j < get_col_count(); j++)
						data = data + " " + std::to_string(M[i][j]);
					data = data + "\n"; //ïåðåíîñ ñòðîêè äëÿ ñîçäàíèÿ íîââîé ñòðîêè ìàòðèöû
				}
			}
			return data; //åñëè íàì íàäî êóäà-òî âûâåñòè ñòðîêó
		}
		//àíòèíîëü íà äèàãîíàëè (ìåíÿåò ñòðîêè ìåñòàìè äëÿ äàëüíåéøåé ðàáîòû)
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

		//çàìåíà ýëåìåíòîâ äèàãîíàëè, ñ íà÷àëîì â [col row] íà scalar
		void diag_replace(unsigned int row, unsigned int col, double scalar = 0)
		{
			for (int i = 0; i < get_row_count() - row && i < get_col_count() - col; i++)
				M[i + col][i + row] = scalar;
		}
		//äåëàåò îáðàòíóþ ìàòðèöó
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
		//òðàíñïîíèðîâàíèå òåêóùåé ìàòðèöû
		void transpond()
		{
			matrix<T> TEMP(get_col_count(), get_row_count()); // Ñîçäàåì âðåìåííóþ ìàòðèöó

			for (int i = 0; i < get_row_count(); i++) {
				for (int j = 0; j < get_col_count(); j++) {
					TEMP(j, i) = M[i][j]; // Çàïîëíÿåì âðåìåííóþ ìàòðèöó òðàíñïîíèðîâàííûìè çíà÷åíèÿìè
				}
			}

			*this = TEMP; // Êîïèðóåì çíà÷åíèÿ èç âðåìåííîé ìàòðèöû â òåêóùóþ ìàòðèöó
		}
		//ðåñàéç
		void resize(unsigned new_row, unsigned new_col)
		{
			if (new_col > 0 && new_row > 0)
				M.resize(new_row);
			for (int i = 0; i < new_row; i++)
				M[i].resize(new_col);
		}

		//todo: îïðåäåëèòåëè, ñîáñòâåííûå çíà÷åíèÿ, []
	void determinant()
	{
		T a = 0;
		std::string tp = typeid(T).name();
		if (tp == "int" || tp == "float" || tp == "double" || tp == "long" || tp == "short")
		{
		}
		else throw 1;
	}
	//todo: îïðåäåëèòåëè, ñîáñòâåííûå çíà÷åíèÿ
	//imp: â äðóãîì ôàéëå íàïèñàíî ïðî ïðîâåðêó íåâûðîæäåííîñòè

		void is_singular() //èñõîäÿ èç îïðåäåëèòåëÿ
		{}

		~matrix() {}
	};

	//îïåðàòîð âûâîäà ìàòðèöû
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
	//	std::vector<T> A; // Ìàññèâ ýëåìåíòîâ òèïà Ò
	//public:
	//	//êîíñòðóêòîð ìàññèâà (ìàòðèöà - ñòîëáåö)
	//	array(unsigned int size)
	//	{
	//		row_count = size;
	//		col_count = 1;
	//		M.resize(row_count, std::vector<T>(col_count));
	//		M[0] = A;
	//	}
	//};
}