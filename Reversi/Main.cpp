# include <Siv3D.hpp>
#include"Board.hpp"
#include"AI.hpp"
#include"System.hpp"

class PauseScene :public Scene{
private:
	Font font;
	double val1 = 0.;
	int select = 0;
public:
	PauseScene(){
		font = Font(30, Typeface::Light);
	}

	virtual void Update(){

		if (Input::KeyUp.clicked&&select>0){

		}

		if (Input::KeyDown.clicked&&select<3){

		}

	}

	virtual void Render(){
		font(L"Pause").draw(300, 130);
		
	}

};

class Play :public Scene {
private:
	Difficulty dif;
	bool pass = false;
	AI ai,ai1;
	Vec2 selectPos;
	bool start = false;
	Font font;
	bool selectMode = true;
	Board board;
	double val1 = 200.,val2=900.,val3=900.;
	bool black = true;
	bool myturn;
	bool setAble = false;
	int wait = 10;
	int waycount = 0;
	int passCount = 0;
	int blackCount = 0;
	int whiteCount = 0;
	Texture Board, Table;
	std::vector<Vec2> SetAble;
public:
	Play() = default;
	Play(Difficulty dif){
		this->dif = dif;
		board.Init({ 160, 60 });
		font = Font(21, Typeface::Light);
		selectPos = { 0, 0 };
		board.SetPiece({ 3, 3 }, Piece::white);
		board.SetPiece({ 4, 4 }, Piece::white);
		board.SetPiece({ 3, 4 }, Piece::black);
		board.SetPiece({ 4, 3 }, Piece::black);
		//pass = !board.IsAllSetAble(!black ? Piece::white : Piece::black);

		Image Cache1(FilePath(L"PNGs\\Melamine-wood-001.png"));
		Image Cache2(FilePath(L"PNGs\\Melamine-wood-002.png"));
		Table = Texture(Cache1);
		Board = Texture(Cache2);
	}

	void Update()override{
		blackCount = board.PieceCount(Piece::black);
		whiteCount = board.PieceCount(Piece::white);
		if (selectMode){
			if (start){
				if (this->black){
					if (Input::KeyRight.clicked){
						this->black = false;
					}
				}
				else{
					if (Input::KeyLeft.clicked){
						this->black = true;
					}
				}
				if (Input::KeyZ.clicked){
					selectMode = false;
					myturn = black;
					ai.SetPiece(!black ? Piece::black : Piece::white);
				}
			}
			else{
				if (Input::KeyZ.released){
					start = true;
				}
			}
		}
		else{
			if (passCount > 1 || waycount > 59){
				val1 += (200 - val1) / 5;
				if (abs(200 - val1) < 10){
					val3 += (300 - val3) / 10;
				}
				if (Input::KeyZ.clicked){
					Parent->BackScene(false);
				}
			}
			else{
				if (myturn){
					if (pass){
						val1 += (200 - val1) / 5;
						if (abs(200 - val1) < 10){
							if (val2 < 302){
								val2 += (-300 - val2) / 10;
								if (val2 < -290){

									pass = !board.IsAllSetAble(black ? Piece::white : Piece::black);
									val2 = 900.;
									passCount++;
									myturn = false;
								}
							}
							else{
								val2 += (300 - val2) / 10;
							}
						}
					}
					else{
						passCount = 0;
						val1 += (0 - val1) / 10;
						if (Input::KeyUp.clicked){
							if (selectPos.y == 0)
								selectPos.y = 7;
							else
								selectPos.y--;
						}
						if (Input::KeyLeft.clicked){
							if (selectPos.x == 0)
								selectPos.x = 7;
							else
							selectPos.x--;
						}
						if (Input::KeyRight.clicked) {
							if (selectPos.x == 7)
								selectPos.x = 0;
							else
								selectPos.x++;
						}
						if (Input::KeyDown.clicked) {
							if (selectPos.y == 7)
								selectPos.y = 0;
							else
								selectPos.y++;
						}

						if (Input::KeyZ.clicked){

							if (board.IsSetAble(selectPos, black ? Piece::black : Piece::white, true)){
								board.SetPiece(selectPos, black ? Piece::black : Piece::white);
								
								pass = !board.IsAllSetAble(black ? Piece::white : Piece::black);
								waycount++;
								myturn = false;
							}
						}
					}
				}
				else{
					if (pass){
						val1 += (200 - val1) / 5;
						if (abs(200 - val1) < 10){
							if (val2 < 302){
								val2 += (-300 - val2) / 10;
								if (val2 < -290){

									pass = !board.IsAllSetAble(black ? Piece::black : Piece::white);
									val2 = 900.;
									passCount++;
									myturn = true;
								}
							}
							else{
								val2 += (300 - val2) / 10;
							}
						}
					}
					else{
						passCount = 0;
						val1 += (0 - val1) / 10;
						if (wait < 1){
							Vec2 setPos = ai.Run(board, dif);

							board.IsSetAble(setPos, black ? Piece::white : Piece::black, true);
							board.SetPiece(setPos, black ? Piece::white : Piece::black);

							pass = !board.IsAllSetAble(!black ? Piece::white : Piece::black);
							waycount++;
							wait = 10;
							myturn = true;
						}
						else{

							wait--;
						}
					}
				}
			}
		}
	}

	 void Render()override{
		Table.map(Window::Width(),Window::Height()).draw();
		Board.map(520, 520).draw(140,40);
		board.Render();

		if (!(passCount > 1 || waycount > 59)&&!selectMode) {
			SetAble = board.WhereSetAble(black ? Piece::black : Piece::white);
			for (auto set : SetAble) {
				Rect(160 + set.x * 60, 60 + 60 * set.y, 60, 60).draw(Color(255, 20, 20, 100));
			}
		}

		if (this->black){
			font(Widen("Black\n(You)\nScore:" + std::to_string(blackCount) + "")).draw({10, 100},Palette::Black);
			font(Widen("White\n(COM)\nScore:" + std::to_string(whiteCount) + "")).draw(670, 100);
		}
		else{
			font(Widen("Black\n(COM)\nScore:" + std::to_string(blackCount) + "")).draw({10, 100},Palette::Black);
			font(Widen("White\n(You)\nScore:" + std::to_string(whiteCount) + "")).draw(670, 100);
		}
		Rect(0, 0, Window::Width(), Window::Height()).draw(Color(0, 0, 0, val1));
		if (selectMode){
			if (this->black){
				font(L"Black\n  ↑").draw({ 200, 270 }, Palette::Red);
				font(L"White").draw({ 500, 270 }, Palette::White);
			}
			else{
				font(L"Black").draw({ 200, 270 }, Palette::White);
				font(L"White\n  ↑").draw({ 500, 270 }, Palette::Red);
			}
			
			font(L"Select Either").draw({ 160, 100 }, Palette::White);
		}
		else{
			if(!(passCount > 1 || waycount > 59))
			Rect(160 + selectPos.x * 60, 60 + selectPos.y * 60, 60, 60).drawFrame(0, 3, Palette::Red);
		}
#ifdef _DEBUG
		font(Profiler::FPS(), L"FPS").draw();
#endif
		font(L"Pass").draw(val2, 270);
		
		if (passCount > 1 || waycount > 59){
			font(L"Result Announcement").draw(val3-160, 130);
			if (black){
				font(L"Black(You):" + Widen(std::to_string(black ? blackCount : whiteCount))).draw(val3 - 100, 270);
				font(L"White(Computer):" + Widen(std::to_string(black ? whiteCount : blackCount))).draw(val3 - 100, 330);
				font(L"Press ZKey to Back to Title").draw(val3 - 100, 420);
				if (blackCount > whiteCount){
					font(L"You are a ").draw({ val3 - 100, 200 }, Palette::White);
					font(L"Winner!!").draw({ val3 + 50, 200 }, Palette::Red);
				}
				else if (blackCount == whiteCount){
					font(L"You draw with Computer").draw({ val3 - 100, 200 }, Palette::White);
				}
				else{
					font(L"You are a ").draw({ val3 - 100, 200 }, Palette::White);
					font(L"Loser...").draw({ val3 + 50, 200 }, Palette::Blue);
				}
			}
			else{
				font(L"black(Computer):" + Widen(std::to_string(!black ? blackCount : whiteCount))).draw(val3 - 100, 270);
				font(L"white(You):" + Widen(std::to_string(!black ? whiteCount : blackCount))).draw(val3 - 100, 330);
				font(L"Press ZKey to Back to Title").draw(val3 - 100, 420);
				if (blackCount < whiteCount){
					font(L"You are a ").draw({ val3 - 100, 200 }, Palette::White);
					font(L"Winner!!").draw({ val3 + 50, 200 }, Palette::Red);
				}
				else if (blackCount == whiteCount){
					font(L"You draw with Computer").draw({ val3 - 200, 200 }, Palette::White);
				}
				else{
					font(L"You are a ").draw({ val3 - 100, 200 }, Palette::White);
					font(L"Loser...").draw({ val3 + 50, 200 }, Palette::Blue);
				}
			}
		}
		else{
			if (wait > 0 && !selectMode&&!myturn && (passCount < 2 || waycount < 59)){
				font(L"Computer Thinking...").draw(400, 0);
			}
			else if (wait > 0 && !selectMode&&myturn && (passCount < 2 || waycount < 59)){
				font(L"Your Turn").draw(400, 0);
			}
		}

		

	}

};

class TitleScene :public Scene {
private:

	int select = 0;
	Font font, font1,font2;
	Texture Back;
	bool start = false;
	double val1 = 0.,val2=0.,val3=500.,val4=0., val5= 500., val6 = 0.,val7=300,val8=-60;
public:
	TitleScene() {
		font = Font(60, Typeface::Light);
		font1 = Font(30, Typeface::Light);
		font2 = Font(25, Typeface::Light);
		Image Cache = Image(FilePath(L"Title.png"));

		Back = Texture(Cache);
	}

	virtual void Update() override{

		if (Input::KeyZ.clicked) {
			start = true;
		}

		if (start) {
			val1 += (-400 - val1) / 20;
			val2 += (-800 - val2) / 20;
			
			if (val1 < -395) {
				if (select == 0)
					val3 += (50 - val3) / 20;
				else
					val3 += (80 - val3) / 20;
				val4 += (255 - val4) / 20;
			}
			if (val4 > 200) {
				if (select == 0)
					val5 += (80 - val5) / 20;
				else
					val5 += (50 - val5) / 20;
				val8 += (10 - val8) / 10;
				val6 += (255 - val6) / 20;
			}
			if (val6 > 250) {
				if (Input::KeyUp.clicked) {
					select--;
					if (select < 0)
						select = 1;
				}
				if (Input::KeyDown.clicked) {
					select++;
					if (select > 1)
						select = 0;
				}
				if (Input::KeyZ.clicked) {
					Parent->NextAddScene(std::make_unique<Scene>(Play(select == 0 ? Difficulty::VeryEasy : Difficulty::Easy)));
				}
				if (!select){
					val7 += (300 - val7) / 10;
				}
				if (select == 1){
					val7 += (400 - val7) / 10;
				}
			}
		}
	}

	virtual void Render()override {
		Back.draw(val1,0);
		font(L"Reversi").draw(50, 50);
		font1(L"Press ZKey!").draw(val2+400, 400);
		font2(L"Easy").draw(val3, 300,Color(255,255,255,val4));
		font2(L"Normal").draw(val5, 400, Color(255, 255, 255, val6));
		font2(L"→").draw(val8, val7, Palette::Red);
	}

};

void Main()
{
	SceneManager manager(std::make_unique<Scene>(TitleScene()));
	Window::SetTitle(L"Reversi");
	Window::Resize(800, 600);
	Graphics::SetBackground(Palette::Darkgray);
	while (System::Update())
	{
		manager.Update();
		manager.Render();
	}
}
