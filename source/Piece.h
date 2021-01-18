#pragma once

#include <vector>

#include "Utility.h"

using namespace std;

class Piece
{

private:
	int** _grid = nullptr;
	int _len = 0;

	int _color_key = 0;

public:
	Piece(int len, int** grid) {
		_len = len;

		_grid = initialize_matrix(len, len);
		for (int i = 0; i < _len; i++) {
			for (int j = 0; j < _len; j++) {
				_grid[i][j] = grid[i][j];
			}
		}

	}
	Piece() {}
	~Piece() {
		delete_matrix(_grid, _len);
	}

	void initialize_board() {
		_grid = new int* [_len];
		for (int i = 0; i < _len; i++) {
			_grid[i] = new int[_len];
		}
	}
	void rotate(int step) {
		for (int j = 0; j < step; j++) {
			int** temp = this->_grid;
			this->_grid = rotate_array(this->_grid, _len, _len);
			delete_matrix(temp, _len);
		}
	}

	void set_color_key(int key) {
		_color_key = key;
	}
	void set_grid(int** grid, int len) {
		delete_matrix(_grid, _len);

		_grid = grid;
		_len = len;
	}

	int** grid() {
		return _grid;
	}
	int color_key() const  {
		return _color_key;
	}

	vector<pair<int, int>> items() const  {
		vector<pair<int, int>> ite;
		for (int i = 0; i < _len; i++) {
			for (int j = 0; j < _len; j++) {
				if (_grid[i][j] != 0) {
					ite.push_back(pair<int, int>(i, j));
				}
			}
		}
		return ite;
	}


};

