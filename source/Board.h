#pragma once

#include <iostream>
#include <map>
#include <limits>
#include <vector>
#include <algorithm>

#include "Piece.h"

#include <SFML/Graphics.hpp>

using namespace std;

class Board : public sf::Drawable {

private:
	int** _board = nullptr;
	int _x_len = 0;
	int _y_len = 0;

	int _margin = 0;

	double _x_pos = 0;
	double _y_pos = 0;

	map<int, sf::Color> _colors;
	sf::Color _back_color;
	sf::Color _default_color;

	Piece* _piece;
	int _piece_x = 0;
	int _piece_y = 0;

	double _edge_len = 0;

	vector<Piece*> _next_pieces;
	vector<Piece*> _pieces;

	int _score = 0;

public:
	Board(int x, int y) {
		_x_len = x;
		_y_len = y;

		initialize_board();
	}

	void set_edge_length(double len) {
		_edge_len = len;
	}
	void set_default_color(sf::Color color) {
		_default_color = color;
	}
	void set_colors(map<int, sf::Color> colors) {
		_colors = colors;
	}
	void set_position(double x, double y) {
		_x_pos = x;
		_y_pos = y;
	}
	void set_margin(int m) {
		_margin = m;
	}
	void set_piece(Piece* piece) {
		_piece = piece;
	}
	void set_pieces(vector<Piece*> pieces) {
		_pieces = pieces;
	}

	Piece* piece() {
		return _piece;
	}
	int score() {
		return _score;
	}

	void move_x(int delta) {
		vector<pair<int, int>> bp = _piece->items();

		int x_max = numeric_limits<int>::min();
		int x_min = numeric_limits<int>::max();

		for (auto& val : bp) {
			x_max = max(val.first, x_max);
			x_min = min(val.first, x_min);
		}

		for (auto& val : bp) {
			if (val.first + delta + _piece_x > -1 && val.first + delta + _piece_x < _x_len) {
				if (_board[val.first + _piece_x + delta][val.second + _piece_y]) return;
			}
		}

		_piece_x = max(0 - x_min, _piece_x + delta);
		_piece_x = min(_x_len - 1 - x_max, _piece_x);

	}
	void rotate_piece(int delta) {
		_piece->rotate((delta + 4) % 4);
		for (auto& val : _piece->items()) {
			if (val.first + _piece_x < 0) {
				vector<pair<int, int>> offsetp = _piece->items();
				for (auto& val : offsetp) {
					val.first += 1;
				}
				if (!collides(offsetp)) _piece_x++;
				else _piece->rotate((-delta + 4) % 4);
				break;
			}
			if (val.first + _piece_x > _x_len - 1) {
				vector<pair<int, int>> offsetp = _piece->items();
				for (auto& val : offsetp) {
					val.first -= 1;
				}
				if (!collides(offsetp)) _piece_x--;
				else _piece->rotate((-delta + 4) % 4);
				break;
			}
		}
		if (collides(_piece->items())) {
			int offset = 0;
			for (int i = -1; i <= 1; i++) {
				if (i != 0) {
					vector<pair<int, int>> offsetp = _piece->items();
					for (auto& val : offsetp) {
						val.first += i;
					}
					if (!collides(offsetp)) {
						offset = i;
						_piece_x += i;
						break;
					}
				}
			}
			if (offset == 0) _piece->rotate((-delta + 4) % 4);
		}
	}
	void clear() {
		for (int i = 0; i < _x_len; i++) {
			for (int j = 0; j < _y_len; j++) {
				_board[i][j] = 0;
			}
		}
	}
	void clear_lines() {
		int cl = 0;
		for (int i = 0; i < _y_len; i++) {
			bool f = 1;
			for (int j = 0; j < _x_len; j++) {
				if (!_board[j][i]) {
					f = 0;
					break;
				}
			}
			if (f) {
				cl++;
				for (int k = i; k >= 1; k--) {
					for (int l = 0; l < _x_len; l++) {
						_board[l][k] = _board[l][k - 1];
					}
				}

			}

		}
		_score += cl * cl * 100;
	}
	bool collides(vector<pair<int, int>> locs) {
		for (auto& val : locs) {

			if (clamp(val.first + _piece_x, 0, _x_len - 1) == val.first + _piece_x) {
				if (clamp(val.second + _piece_y, 0, _y_len - 1) == val.second + _piece_y) {
					if (_board[val.first + _piece_x][val.second + _piece_y]) return 1;
				}
				else return 1;
			}
			else return 1;

		}
		return 0;
	}

	void initialize_board() {
		_board = new int* [_x_len];
		for (int i = 0; i < _x_len; i++) {
			_board[i] = new int[_y_len]();
		}
	}
	void randomize_piece() {
		_piece = _pieces[rand() % _pieces.size()];
	}

	bool lost() {
		for (int i = 0; i < _x_len; i++) {
			if (_board[i][_margin]) return 1;
		}
		return 0;
	}
	void restart_if_lost() {
		if (lost()) {
			clear();
			_score = 0;
		}
	}


	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		
		for (int i = 0; i < _x_len; i++) {
			for (int j = _margin; j < _y_len; j++) {
				sf::RectangleShape rect(sf::Vector2f(_edge_len, _edge_len));
				rect.setPosition(_x_pos + _edge_len * i, _y_pos + _edge_len * j);

				int key = _board[i][j];
				if (_colors.count(key) != 0) {
					rect.setFillColor(_colors.at(key));
				}
				else {
					rect.setFillColor(_default_color);
				}

				rect.setOutlineThickness(1.0f);
				rect.setOutlineColor(sf::Color(128, 128, 128));

				target.draw(rect);
			}
		}

		vector<pair<int, int>> movable = _piece->items();
		for (auto& val : movable) {

			if (val.second + _piece_y >= _margin && val.second + _piece_y < _y_len) {
				sf::RectangleShape rect(sf::Vector2f(_edge_len, _edge_len));
				rect.setPosition(_x_pos + _edge_len * (val.first + _piece_x), _y_pos + _edge_len * (val.second + _piece_y));

				int key = _piece->color_key();
				if (_colors.count(key) != 0) {
					rect.setFillColor(_colors.at(key));
				}
				else {
					rect.setFillColor(_default_color);
				}

				rect.setOutlineThickness(1.0f);
				rect.setOutlineColor(sf::Color(128, 128, 128));

				target.draw(rect);
			}

		}


	}

	void update() {

		clear_lines();

		vector<pair<int, int>> movable = _piece->items();

		for (auto& val : movable) {
			//cout << val.first + _piece_x << " " << val.second + _piece_y + 1 << endl;
			//cout << _piece_x << " " << _piece_y << endl;
			if (val.second + 1 + _piece_y == _y_len || _board[val.first + _piece_x][val.second + _piece_y + 1]) {
				for (auto& block : movable) {
					_board[block.first + _piece_x][block.second + _piece_y] = _piece->color_key();
				}
				_piece_y = 0;
				_piece_x = 4;
				_piece = _pieces[rand() % _pieces.size()];
				break;
			}
		}
		
		_piece_y++;

	}

};