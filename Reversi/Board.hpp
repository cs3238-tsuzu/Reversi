#pragma once
#include<Siv3D.hpp>

enum class Piece{
	nown,
	black,
	white
};

enum class Difficulty {
	VeryEasy,
	Easy,
	Normal,
	Hard
};

class Board{
private:
	std::vector<std::vector<Piece>> data;
	Vec2 pos;
public:
	Board() = default;

	Board(Vec2 pos){
		Init(pos);
	}

	std::vector<std::vector<Piece>> GetData(){
		return data;
	}

	void Init(Vec2 pos){
		this->pos = pos;
		data = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8));
		for (int i = 0; i < data.size(); i++){
			for (int j = 0; j < data[i].size(); j++){
				data[i][j] = Piece::nown;
			}
		}
	}

	void SetPiece(Vec2 pos,Piece set){
		data[pos.x][pos.y] = set;
	}

	void Update(){

	}

	std::vector<Vec2> WhereSetAble(Piece set){
		std::vector<Vec2> ret;
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (IsSetAble({ i, j }, set, false)){
					ret.emplace_back(Vec2( i, j));
				}
			}
		}
		return ret;
	}


	bool IsSetAble(Vec2 pos, Piece set, bool canset,bool first = true, Vec2 dif = {0,0},bool second=false){
		if (pos.x < 0 || 7 < pos.x || pos.y < 0 || 7 < pos.y){
			return false;
		}
		if (first){
			if (data[pos.x][pos.y] != Piece::nown){
				return false;
			}
			int retcount = 0;
			retcount += IsSetAble({ pos.x - 1, pos.y }, set, canset,false, { -1, 0 },true);
			retcount += IsSetAble({ pos.x + 1, pos.y }, set, canset, false, { 1, 0 }, true);
			retcount += IsSetAble({ pos.x, pos.y - 1 }, set, canset, false, { 0, -1 }, true);
			retcount += IsSetAble({ pos.x, pos.y + 1 }, set, canset, false, { 0, 1 }, true);
			retcount += IsSetAble({ pos.x - 1, pos.y - 1 }, set, canset, false, { -1, -1 }, true);
			retcount += IsSetAble({ pos.x - 1, pos.y + 1 }, set, canset, false, { -1, 1 }, true);
			retcount += IsSetAble({ pos.x + 1, pos.y - 1 }, set, canset, false, { 1, -1 }, true);
			retcount += IsSetAble({ pos.x + 1, pos.y + 1 }, set, canset, false, { 1, 1 }, true);
			if (retcount > 0)
				return true;
			else
				return false;
		}
		else{
			if (data[pos.x][pos.y] == Piece::nown){
				return false;
			}
			if (second){
				if (data[pos.x][pos.y] == set)return false;
				else{
					if (IsSetAble({ pos.x + dif.x, pos.y + dif.y }, set, canset, false, { dif.x, dif.y })){
						if (canset)
						data[pos.x][pos.y] = set;
						return true;
					}
					else return false;
				}
			}
			if (data[pos.x][pos.y] == set) return true;
			else{
				if (IsSetAble({ pos.x + dif.x, pos.y + dif.y }, set, canset, false, { dif.x, dif.y })){
					if (canset)
					data[pos.x][pos.y] = set;
				return true;
			}
			else return false;
		}
		}
	}

	int PieceCount(Piece piece){
		int ret=0;
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (data[i][j] == piece)
					ret++;
			}
		}
		return ret;
	}

	bool IsAllSetAble(Piece piece){

		for (int i = 0; i < 8; i++){

			for (int j = 0; j < 8; j++){

				if(IsSetAble({ i, j }, piece, false))

				return true;
			}
		}
		return false;
	}

	void Render(){
		Rect(pos.x,pos.y, Size(480,480)).draw(Palette::Darkgreen);
		Rect(pos.x, pos.y, Size(480, 480)).drawFrame(0, 3, Palette::Black);
		for (int i = 0; i < data.size(); i++){
			if (i!=0)
			Line(pos.x + i * 60, pos.y, pos.x + i * 60, pos.y + 480).draw(3, Palette::Black);
			for (int j = 0; j < data[i].size(); j++){
				if (j!=0)
				Line(pos.x,pos.y + j * 60, pos.x+480,pos.y + j * 60).draw(3,Palette::Black);
				if (data[i][j] == Piece::black){
					Circle(pos.x + i*(60) + 32, pos.y + j*(60) + 32, 25).draw(Palette::White);
					Circle(pos.x + i*(60) + 30, pos.y + j*(60) + 30, 25).draw(Palette::Black);
				}
				if (data[i][j] == Piece::white){
					Circle(pos.x + i*(60) + 33, pos.y + j*(60) + 33, 25).draw(Palette::Black);
					Circle(pos.x + i*(60) + 30, pos.y + j*(60) + 30, 25).draw(Palette::White);
				}
			}
		}



	}

};