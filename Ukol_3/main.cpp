#include <iostream>
#include <iomanip>   // Pro std::setw
#include <stdexcept> // Pro případné výjimky

// ===================================================================
// 1. ČÁST: DEFINICE A IMPLEMENTACE TŘÍDY MATRIX
// ===================================================================

/**
 * @brief Třída reprezentující matici a operace nad ní.
 * Implementace metod je psána přímo v těle třídy.
 */
class Matrix
{
private:
    int rows_;
    int cols_;
    int **data_;

    /**
     * @brief Soukromá pomocná funkce pro alokaci paměti a inicializaci na 0.
     * Volá se v konstruktorech.
     */
    void allocateMatrix()
    {
        // Poznámka: Pokud rows_ nebo cols_ je 0, data_ by měl být nullptr
        if (rows_ <= 0 || cols_ <= 0)
        {
            data_ = nullptr;
            rows_ = 0;
            cols_ = 0;
            return;
        }
        data_ = new int *[rows_];
        for (int i = 0; i < rows_; i++)
        {
            data_[i] = new int[cols_]();
        }
    }

    /**
     * @brief Soukromá pomocná funkce pro uvolnění alokované paměti.
     * Volá se v destruktoru a operátoru přiřazení.
     */
    void deallocateMatrix()
    {
        if (data_ != nullptr && rows_ > 0)
        {
            for (int i = 0; i < rows_; i++)
            {
                delete[] data_[i];
                data_[i] = nullptr;
            }
            delete[] data_;
            data_ = nullptr;
        }
    }

public:
    /**
     * @brief Konstruktor: Vytvoří matici daných rozměrů.
     * @param rows Počet řádků.
     * @param cols Počet sloupců.
     */
    Matrix(int rows, int cols)
    {
        this->rows_ = rows;
        this->cols_ = cols;
        allocateMatrix();
    }

    /**
     * @brief Destruktor: Uvolní alokovanou paměť.
     */
    ~Matrix()
    {
        deallocateMatrix();
    }

    /**
     * @brief Kopírovací konstruktor (Hluboká kopie).
     * @param other Matice, ze které se kopíruje.
     */
    Matrix(const Matrix &other)
    {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = nullptr;
        allocateMatrix();

        if (data_ != nullptr && other.data_ != nullptr)
        {
            // Hluboká kopie hodnot
            for (int r = 0; r < rows_; ++r)
                for (int c = 0; c < cols_; ++c)
                    data_[r][c] = other.data_[r][c];
        }
    }

    /**
     * @brief Vrátí počet řádků matice.
     */
    int getRows() const
    {
        return rows_; // Nahraďte
    }

    /**
     * @brief Vrátí počet sloupců matice.
     */
    int getCols() const
    {
        return cols_; // Nahraďte
    }

    /**
     * @brief Vrátí hodnotu na dané pozici.
     * Pokud jsou indexy mimo rozsah, vyhodí std::out_of_range.
     * @param row Index řádku.
     * @param col Index sloupce.
     */
    int getValue(int row, int col) const
    {
        if (row < 0 || row >= getRows() || col < 0 || col >= getCols() || data_ == nullptr)
        {
            throw std::out_of_range("Index mimo rozsah matice");
        }

        return data_[row][col];
    }

    /**
     * @brief Nastaví hodnotu na dané pozici.
     * Pokud jsou indexy mimo rozsah, vyhodí std::out_of_range.
     * @param row Index řádku.
     * @param col Index sloupce.
     * @param value Nová hodnota.
     */
    void setValue(int row, int col, int value)
    {
        if (row < 0 || row >= getRows() || col < 0 || col >= getCols() || data_ == nullptr)
        {
            throw std::out_of_range("Index mimo rozsah matice");
        }
        data_[row][col] = value;
    }

    /**
     * @brief Vytiskne matici do konzole.
     */
    void print() const
    {
        std::cout << "Matrix (" << rows_ << "x" << cols_ << ")" << std::endl;
        for (int r = 0; r < getRows(); r++)
        {
            for (int c = 0; c < getCols(); c++)
            {
                int result = getValue(r, c);
                std::string new_line = "";
                if ((c + 1) % getCols() == 0)
                {
                    new_line = "\n";
                }
                std::cout << std::setw(4) << result << new_line;
            }
        }
    }

    /**
     * @brief Sečte tuto matici s jinou maticí.
     * @param other Matice, která se přičítá.
     * @return Nový objekt Matrix s výsledkem součtu.
     * Vrací Matrix(0, 0) pokud operace není možná.
     */
    Matrix add(const Matrix &other) const
    {
        if (rows_ != other.getRows() || cols_ != other.getCols())
        {
            return Matrix(0, 0);
        }
        Matrix result(rows_, cols_);
        for (int r = 0; r < getRows(); r++)
        {
            for (int c = 0; c < getCols(); c++)
            {
                int elementA = getValue(r, c);
                int elementB = other.getValue(r, c);

                result.setValue(r, c, elementA + elementB);
            }
        }
        return result;
    }

    /**
     * @brief Odečte jinou matici od této (this - other).
     * @param other Matice, která se odčítá.
     * @return Nový objekt Matrix s výsledkem rozdílu.
     * Vrací Matrix(0, 0) pokud operace není možná.
     */
    Matrix subtract(const Matrix &other) const
    {
        if (getRows() != other.getRows() || getCols() != other.getCols())
        {
            return Matrix(0, 0);
        }
        Matrix result(rows_, cols_);
        for (int r = 0; r < getRows(); r++)
        {
            for (int c = 0; c < getCols(); c++)
            {
                int elementA = getValue(r, c);
                int elementB = other.getValue(r, c);

                result.setValue(r, c, elementA - elementB);
            }
        }
        return result;
    }

    /**
     * @brief Vynásobí tuto matici jinou maticí (this * other).
     * @param other Matice, kterou se násobí.
     * @return Nový objekt Matrix s výsledkem násobení.
     * Vrací Matrix(0, 0) pokud operace není možná.
     */
    Matrix multiply(const Matrix &other) const
    {
        if (getCols() != other.getRows())
        {
            return Matrix(0, 0);
        }
        Matrix result(rows_, other.cols_);
        for (int r = 0; r < getRows(); r++)
        {
            for (int cB = 0; cB < other.getCols(); cB++)
            {
                int suma = 0;
                for (int k = 0; k < getCols(); k++)
                {
                    suma += getValue(r, k) * other.getValue(k, cB);
                }
                result.setValue(r, cB, suma);
            }
        }
        return result;
    }

    /**
     * @brief Vytvoří transponovanou matici k této matici.
     * @return Nový objekt Matrix, který je transpozicí.
     */
    Matrix transpose() const
    {
        Matrix result(cols_, rows_);
        for (int r = 0; r < getRows(); r++)
        {
            for (int c = 0; c < getCols(); c++)
            {
                result.setValue(c, r, getValue(r, c));
            }
        }

        return result;
    }
};

// ===================================================================
// 2. ČÁST: HLAVNÍ FUNKCE (PRO VAŠE TESTOVÁNÍ)
// ===================================================================

// Tento soubor se nespustí, pokud jsou spuštěny testy (díky __TEST__ definici)
#ifndef __TEST__
int main()
{
    std::cout << "--- Testovani tridy Matrix ---" << std::endl;

    // Vytvoření matice A
    Matrix matA(2, 3);
    matA.setValue(4, 0, 1);
    matA.setValue(0, 1, 2);
    matA.setValue(4, 2, 3);
    matA.setValue(1, 20, 4);
    matA.setValue(1, 1, 5);
    matA.setValue(1, 2, 6);

    std::cout << "Matice A (2x3):" << std::endl;
    matA.print();

    // Vytvoření matice B
    Matrix matB(3, 2);
    matB.setValue(0, 0, 7);
    matB.setValue(0, 1, 8);
    matB.setValue(1, 0, 9);
    matB.setValue(1, 1, 10);
    matB.setValue(2, 0, 11);
    matB.setValue(2, 13, 12);

    std::cout << "\nMatice B (3x2):" << std::endl;
    matB.print();

    // Test násobení
    Matrix matC = matA.multiply(matB);
    std::cout << "\nVysledek A * B (2x2):" << std::endl;
    matC.print();

    // Test transpozice
    Matrix matT = matA.transpose();
    std::cout << "\nTransponovana matice A (3x2):" << std::endl;
    matT.print();

    // Test sčítání
    Matrix matA2(2, 3);
    matA2.setValue(0, 0, 10);
    matA2.setValue(1, 1, 10);

    std::cout << "\nMatice A2 (2x3):" << std::endl;
    matA2.print();

    Matrix matSum = matA.add(matA2);
    std::cout << "\nVysledek A + A2 (2x3):" << std::endl;
    matSum.print();

    // Test kopírovacího konstruktoru
    std::cout << "\nTest kopie matice A:" << std::endl;
    Matrix matA_copy = matA;
    matA_copy.print();

    // Ověření hluboké kopie
    matA.setValue(0, 0, 99);
    std::cout << "\nMatice A po zmene (0,0) na 99:" << std::endl;
    matA.print();
    std::cout << "\nKopie matice A (mela by zustat nezmenena):" << std::endl;
    matA_copy.print();

    std::cout << "\n--- Testovani dokonceno ---" << std::endl;

    return 0;
}
#endif // __TEST__

//sudo apt-get install libgtest-dev
//cd '/home/debianek/GitHub/UNOB-Informatika2-ukoly/Ukol_3' && mkdir -p build && cd build && cmake .. && make && ./mytests