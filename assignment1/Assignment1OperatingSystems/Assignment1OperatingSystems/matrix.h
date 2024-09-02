#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <functional>

template <typename T, int row_len, int col_len>
class Matrix {
private:
    T** matrix;
    int prow_len;
    int pcol_len;
    int thr_count;
    std::vector<std::thread> thrPool;

    bool checkAddCompat(Matrix a, Matrix b) {  //Check if addition can be performed
        return (a.prow_len == b.prow_len) && (a.pcol_len == b.pcol_len);
    }

    bool checkMulCompat(Matrix a, Matrix b) {  //Check if multiplication can be performed
        return (a.pcol_len == b.prow_len) && (a.prow_len == b.pcol_len);
    }

    bool checkTranspose(Matrix a) { //Check if transposition can be performed
        return a.prow_len == a.pcol_len;
    }

    int findMaxElementWidth() {
        int maxWidth = 0;
        for (int i = 0; i < prow_len; i++) {
            for (int j = 0; j < pcol_len; j++) {
                int width = std::to_string(this->matrix[i][j]).length();
                if (width > maxWidth) {
                    maxWidth = width;
                }
            }
        }
        return maxWidth;
    }

    void distributeWorkload(const std::function<void(int, int)>& func) {
        // Adjust the thread count if the number of rows is less than the thread count
        int actual_thr_count = std::min(thr_count, prow_len);

        // Calculate the number of rows each thread will process
        int rows_per_thread = prow_len / actual_thr_count;
        int remaining_rows = prow_len % actual_thr_count;

        // Resize thrPool if necessary
        thrPool.resize(actual_thr_count);

        // Create threads and assign workload
        int start_row = 0;
        for (int i = 0; i < actual_thr_count; ++i) {
            int end_row = start_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
            thrPool.emplace_back(func, start_row, end_row);
            start_row = end_row;
        }

        // Join threads
        for (auto& thread : thrPool) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

public:
    // Basic constructor
    Matrix(T data[row_len][col_len], int thr_count) { 
        this->matrix = new T * [row_len];
        for (int i = 0; i < row_len; ++i) {
            this->matrix[i] = new T[col_len];
            for (int j = 0; j < col_len; ++j) {
                this->matrix[i][j] = data[i][j];
            }
        }
        this->pcol_len = col_len;
        this->prow_len = row_len;
        this->thr_count = thr_count;
    }

    // Copy constructor
    Matrix(const Matrix& other) {
        this->prow_len = other.prow_len;
        this->pcol_len = other.pcol_len;
        this->thr_count = other.thr_count;

        // Allocation memory for new matrix
        this->matrix = new T * [prow_len];
        for (int i = 0; i < prow_len; ++i) {
            this->matrix[i] = new T[pcol_len];
            for (int j = 0; j < pcol_len; ++j) {
                this->matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    // Random matrix
    Matrix(int thr_count) {
        this->matrix = new T * [row_len];
        for (int i = 0; i < row_len; ++i) {
            this->matrix[i] = new T[col_len];
            for (int j = 0; j < col_len; ++j) {
                this->matrix[i][j] = static_cast<T>(rand() % 1000);
            }
        }
        this->pcol_len = col_len;
        this->prow_len = row_len;
        this->thr_count = thr_count;
    }

    ~Matrix() {
        for (int i = 0; i < row_len; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void print(std::string matrixName) {
        std::cout << "Here is " << matrixName << ":" << std::endl;

        for (int i = 0; i < prow_len; ++i) {
            for (int j = 0; j < pcol_len; ++j) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    Matrix addition(const Matrix& other) {
        if (!checkAddCompat(*this, other)) {
            throw std::invalid_argument("Matrixes are not compatible for addition");
        }
        else {
            Matrix result(*this);
            for (int i = 0; i < prow_len; ++i) {
                for (int j = 0; j < pcol_len; ++j) {
                    result.matrix[i][j] += other.matrix[i][j];
                }
            }
            return result;
        }
    }

    Matrix additionThr(const Matrix& other) {
        if (!checkAddCompat(*this, other)) {
            throw std::invalid_argument("Matrices are not compatible for addition");
        }
        else {
            Matrix result(*this);
            auto addition_func = [&](int start_row, int end_row) {
                for (int i = start_row; i < end_row; ++i) {
                    for (int j = 0; j < pcol_len; ++j) {
                        result.matrix[i][j] += other.matrix[i][j];
                    }
                }
                };
            distributeWorkload(addition_func);
            return result;
        }
    }

    Matrix multiplication(const Matrix& other) {
        if (!checkMulCompat(*this, other)) {
            throw std::invalid_argument("Matrixes are not compatible for multiplication");
        }
        else {
            Matrix result(*this);
            for (int i = 0; i < prow_len; ++i) {
                for (int j = 0; j < other.pcol_len; ++j) {
                    result.matrix[i][j] = 0;
                    for (int k = 0; k < pcol_len; ++k) {
                        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                    }
                }
            }
            return result;
        }
    }

    Matrix multiplicationThr(const Matrix& other) {
        if (!checkMulCompat(*this, other)) {
            throw std::invalid_argument("Matrices are not compatible for multiplication");
        }
        else {
            Matrix result(*this);
            auto multiplication_func = [&](int start_row, int end_row) {
                for (int i = start_row; i < end_row; ++i) {
                    for (int j = 0; j < other.pcol_len; ++j) {
                        result.matrix[i][j] = 0; // Initialize to zero
                        for (int k = 0; k < pcol_len; ++k) {
                            result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                        }
                    }
                }
                };
            distributeWorkload(multiplication_func);
            return result;
        }
    }

    Matrix transposition() {
        Matrix result(*this); 
        if (!checkTranspose(*this)) {
            throw std::invalid_argument("Matrix can't be transposable: not the same size");
        }
        else {
            for (int i = 0; i < pcol_len; ++i) {
                for (int j = 0; j < prow_len; ++j) {
                    result.matrix[i][j] = matrix[j][i];
                }
            }
            return result;
        } 
    }

    Matrix transpositionThr() {
        if (!checkTranspose(*this)) {
            throw std::invalid_argument("Matrix can't be transposable: not the same size");
        }
        else {
            Matrix transposed(*this);
            auto transpose_func = [&](int start_row, int end_row) {
                for (int i = start_row; i < end_row; ++i) {
                    for (int j = 0; j < pcol_len; ++j) {
                        transposed.matrix[j][i] = matrix[i][j];
                    }
                }
                };
            distributeWorkload(transpose_func);
            return transposed;
        }
    }
};