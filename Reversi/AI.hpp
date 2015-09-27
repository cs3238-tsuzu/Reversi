#pragma once
#include<Siv3D.hpp>
#include<algorithm>
#include"Board.hpp"

struct Reversi_Info{
	Vec2 setPos;
	Piece color;
	Reversi_Info(Vec2 setPos,Piece color){
		this->setPos = setPos;
		this->color = color;
	}
};

class TitacManager{
private:
	std::vector<Reversi_Info> Informations;
	Board board;
public:
	TitacManager(){
		board.SetPiece({ 3, 3 }, Piece::white);
		board.SetPiece({ 4, 4 }, Piece::white);
		board.SetPiece({ 3, 4 }, Piece::black);
		board.SetPiece({ 4, 3 }, Piece::black);
	}

	void SetData(Vec2 pos, Piece piece){
		Informations.emplace_back(Reversi_Info(pos, piece));
	}


};



class AI{
private:
	std::vector<std::vector<int>> EvaluationMap1;
	Piece mypiece;
	Font font;
public:

	AI(){
		font = Font(30, L"Yu Gothic");
		EvaluationMap1=std::vector<std::vector<int>>(8, std::vector<int>(8));
		CSVReader em1(L"EvaluationMap1.csv");
		if (!em1)
		{
			return;
		}
		int Rows = em1.rows;
		int Columns = em1.columns(0);
		for (int i = 0; i < em1.rows; i++){
			for (int j = 0; j < em1.columns(i);j++){
				EvaluationMap1[i][j] = em1.get<int>(i, j);
			}
		}

	}

	void SetPiece(Piece piece){
		this->mypiece = piece;
	}

	Vec2 BestN(Board& board, Piece piece , unsigned int n = 1){
		std::vector<Vec2> SetAble = board.WhereSetAble(piece);
		if (n > SetAble.size())
			return Vec2{ 0, 0 };

		auto& eMap = EvaluationMap1;

		std::sort(SetAble.begin(), SetAble.end(), [&eMap](const Vec2& p1, const Vec2& p2){
			return eMap[p1.x][p1.y] < eMap[p2.x][p2.y];
		});
		if (n < 0)
			return SetAble[0];
		else
			return SetAble[SetAble.size() - n];
	}

	Vec2 MiniMax(Board& board, unsigned int depth) {
		if (board.IsAllSetAble(mypiece)){
			return MiniMax(board, depth, true, false).first;
		}
		else{
			return MiniMax(board, depth, true, true).first;
		}
	}

	std::pair<Vec2,int> MiniMax(Board& board, unsigned int depth, bool myturn,bool pass) {
		Piece nowpiece = myturn ? (mypiece == Piece::black ? Piece::black : Piece::white) : (mypiece == Piece::black ? Piece::white : Piece::black);
		if (depth == 0)
			if (myturn)
				return std::make_pair(BestN(board, nowpiece), EvaluationMap1[BestN(board, nowpiece).x][BestN(board, nowpiece).y]);
			else
				return std::make_pair(BestN(board, nowpiece, -1), EvaluationMap1[BestN(board, nowpiece, -1).x][BestN(board, nowpiece, -1).y]);
		
		if (!board.IsAllSetAble(nowpiece)){
			if (pass){
				if (myturn)
					return std::make_pair(BestN(board, nowpiece), EvaluationMap1[BestN(board, nowpiece).x][BestN(board, nowpiece).y]);
				else
					return std::make_pair(BestN(board, nowpiece, -1), EvaluationMap1[BestN(board, nowpiece, -1).x][BestN(board, nowpiece, -1).y]);
			}
			else{
				return MiniMax(board, depth - 1, !myturn, true);
			}
		}
		auto where_=board.WhereSetAble(nowpiece);

		auto SetAble = std::vector<std::pair<Vec2,int>>(where_.size());

		for (int i = 0; i < where_.size();i++){
			SetAble[i].first = where_[i];
			auto board_ = board;
			board_.SetPiece(SetAble[i].first, nowpiece);
			SetAble[i].second=MiniMax(board_, depth - 1, !myturn,false).second;
		}

		std::sort(SetAble.begin(), SetAble.end(), [](std::pair<Vec2, int> p1, std::pair<Vec2, int> p2){
			return p1.second < p2.second;
		});

		if (myturn)
			return SetAble[SetAble.size() - 1];
		else
			return SetAble[0];

	}

	Vec2 VeryEasy(Board& board){
		return BestN(board,mypiece);
	}

	Vec2 Easy(Board& board){
		return MiniMax(board, 3);
	}

	Vec2 Run(Board& board, Difficulty dif) {
		if (dif == Difficulty::VeryEasy)
			return VeryEasy(board);
		else if (dif == Difficulty::Easy)
			return Easy(board);
		else
			return Vec2{ -1, -1 };
	}

};