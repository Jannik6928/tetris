
#pragma once

#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

using namespace std;

static int** rotate_array(int** arr, int x, int y) {
    int** rot_arr = new int* [y];
    for (int i = 0; i < y; i++) {
        rot_arr[i] = new int[x];
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            rot_arr[j][i] = arr[x - i - 1][j];
        }
    }

    return rot_arr;
}

static int** initialize_matrix(int x, int y) {
    int** arr = new int* [x];
    for (int i = 0; i < x; i++) {
        arr[i] = new int[y]();
    }
    return arr;
}

static void output_matrix(int** arr, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


static void delete_matrix(int** arr, int x) {
    for (int i = 0; i < x; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}
