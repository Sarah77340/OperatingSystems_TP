#include <iostream>
#include <chrono>
#include "matrix.h"

int main() {
    // Init
    const int rows = 2;
    const int cols = 2;

    std::cout << "Contenu de la matrice 'matrix A':" << std::endl;
    /*double matdataA[rows][cols] = { {7,8},{3,5} };*/
    Matrix<double, rows, cols> matA(2);
    matA.print("matA");

    std::cout << "Contenu de la matrice 'matrix B':" << std::endl;
    /*double matdataB[rows][cols] = { {1,2},{3,4} };*/
    Matrix<double, rows, cols> matB(2);
    matB.print("matB");


    // Measure performance of addition
    auto startAddition = std::chrono::high_resolution_clock::now();
    // Addition des deux matrices
    Matrix<double, rows, cols> resultAdd = matA.additionThr(matB);
    auto endAddition = std::chrono::high_resolution_clock::now();

    std::cout << "Resultat de la matrice 'matrix A + matrix B':" << std::endl;
    resultAdd.print("resultAdd");
    std::chrono::duration<double> durationAddition = endAddition - startAddition;
    std::cout << "Performance of matrix addition: " << durationAddition.count() << " seconds\n";


    // Measure performance of multiplication
    auto startMultiplication = std::chrono::high_resolution_clock::now();
    // Multiplication des deux matrices
    Matrix<double, rows, cols> resultMulti = matA.multiplicationThr(matB);
    auto endMultiplication = std::chrono::high_resolution_clock::now();

    std::cout << "Resultat de la matrice 'matrix A x matrix B':" << std::endl;
    resultMulti.print("resultMulti");
    std::chrono::duration<double> durationMultiplication = endMultiplication - startMultiplication;
    std::cout << "Performance of matrix multiplication: " << durationMultiplication.count() << " seconds\n";


    // Measure performance of transposition
    auto startTransposition = std::chrono::high_resolution_clock::now();
    // Transposition de la matrice A*B
    Matrix<double, rows, cols> resultTranspo = resultMulti.transpositionThr();
    auto endTransposition = std::chrono::high_resolution_clock::now();

    std::cout << "Resultat de la transposition matrice 'matrix A x matrix B':" << std::endl;
    resultTranspo.print("resultTranspo");
    std::chrono::duration<double> durationTransposition = endTransposition - startTransposition;
    std::cout << "Performance of matrix transposition: " << durationTransposition.count() << " seconds\n";

    return 0;
}
