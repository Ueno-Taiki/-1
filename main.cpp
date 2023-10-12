#include <Novice.h>

const char kWindowTitle[] = "ゲームタイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//Novice::SetWindowMode(kFullscreen);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	//キャラクター
	struct Vector2 {
		float x;
		float y;
		float w;
		float h;
	};
	//Ball
	struct Ball {
		Vector2 position;
		float radius;
		float speed;
	};
	//敵
	struct Enemy {
		Vector2 position;
		float radius;
		float speed;
	};
	//初期値設定
	Ball ball = {
		{100.0f, 250.0f, 64.0f, 64.0f},
		64.0f,
		180.0f
	};
	Enemy enemy{
		{1000.0f, 270.0f, 64.0f, 64.0f},
		32.0f,
		3.0f
	};

	//長押しカウント
	int count = 0;

	//曲の時間
	int time_count = 6700;

	//背景画像
	int bg1X = 0, bg2X = 1280;
	int bg_speed = 4;

	//アニメーションカウント
	int frameCount = 0;
	int AnimCount = 0;

	//フラグ管理
	bool Gameflag = true;
	bool Stage_Easyflag = false;
	bool onceflag = false;
	bool L_push = false;
	bool S_push = false;
	bool isEnemyAlive = true;
	bool attackflag = false;
	bool Clearflag = false;

	//画像切り替え
	enum Scene {
		eScene_START,
		eScene_SELECT,
		eScene_STAGE_EASY,
		eScene_STAGE_NORMAL,
		eScene_STAGE_HARD,
		eScene_CLEAR,
	};
	Scene scene_no = eScene_STAGE_EASY;

	//BGM・SE
	int soundHandle1 = Novice::LoadAudio("./Resources/SE/BGM1.mp3");  //BGM1
	int voiceHandle1 = -1;

	//画像読み込み
	int Ebi = Novice::LoadTexture("./Resources/ebi.png");
	int Ika = Novice::LoadTexture("./Resources/ika.png");
	int Haikei = Novice::LoadTexture("./Resources/Haikei.png");
	int Ready = Novice::LoadTexture("./Resources/UI/Ready.png");
	int Start = Novice::LoadTexture("./Resources/UI/Start.png");
	int Clear = Novice::LoadTexture("./Resources/UI/Clear.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// ↓更新処理ここから

		//マウスカーソル削除
		Novice::SetMouseCursorVisibility(0);

		switch (scene_no) {
		case eScene_START:
			break;
		case eScene_SELECT:
			break;
		case eScene_STAGE_EASY:
			
			//ゲームスタートアニメーション

			if (Gameflag) {
				frameCount++;
				if (frameCount == 60) {
					AnimCount += 1;
				}
				if (frameCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Easyflag = true;
				}
			}

			//難易度簡単

			if (Stage_Easyflag) {
				time_count--;

				if (!Novice::IsPlayingAudio(voiceHandle1) || voiceHandle1 == -1) {
					voiceHandle1 = Novice::PlayAudio(soundHandle1, false, 0.5f);
				}

				//移動

				if (keys[DIK_SPACE]) {  //スペースを押されたら時間を計測
					count++;
					onceflag = true;
					L_push = false;
					S_push = false;
				}

				if (keys[DIK_SPACE] && onceflag == true && count >= 20) {  //スペースを長押しした時に移動
					onceflag = false;
					count = 0;
					L_push = true;
				}

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE]) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
					attackflag = true;
				}
				else{
					attackflag = false;
				}

				if (L_push == true) {  //長押し判定
					ball.position.y -= ball.speed;
					if (ball.position.y == 70) {  //上に移動した時
						ball.speed -= ball.speed + 180;
					}
					else if (ball.position.y == 430) {  //下に移動した時
						ball.speed -= ball.speed - 180;
					}
				}

				if (count == 0) {  //バグ対策
					L_push = false;
				}

				// 敵の移動

				if (isEnemyAlive) {
					enemy.position.x -= enemy.speed;
				}

				//敵の当たり判定

				if (attackflag) {
					if (isEnemyAlive) {
						if ((ball.radius + enemy.radius) * (ball.radius + enemy.radius) > ((ball.position.x - 18) - (enemy.position.x + 18)) * ((ball.position.x - 18) - (enemy.position.x + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18))) {
							if (isEnemyAlive == true) {
								isEnemyAlive = false;
							}
						}
					}
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}

				//曲が終わったら終了させる

				if (time_count < 0) {
					Novice::StopAudio(voiceHandle1);  
					time_count = 0;
					Stage_Easyflag = false;
					Clearflag = true;
				}
			}
			break;
		case eScene_STAGE_NORMAL:
			break;
		case eScene_STAGE_HARD:
			break;
		case eScene_CLEAR:
			break;
		}
		/// ↑更新処理ここまで

		/// ↓描画処理ここから
		switch (scene_no) {
		case eScene_START:
			break;
		case eScene_SELECT:
			break;
		case eScene_STAGE_EASY:
			Novice::DrawSprite(bg1X, 0, Haikei, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(bg2X, 0, Haikei, 1, 1, 0.0f, WHITE);
			Novice::ScreenPrintf(0, 0, "TimeCount = %d", time_count);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(enemy.position.x), int(enemy.position.y), Ika, 1, 1, 0.0f, WHITE);
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, Ready, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, Start, 1, 1, 0.0f, WHITE);
				}
			}
			if(Clearflag) {
				Novice::DrawSprite(260, 180, Clear, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_NORMAL:
			break;
		case eScene_STAGE_HARD:
			break;
		case eScene_CLEAR:
			break;
		}
		/// ↑描画処理ここまで

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
