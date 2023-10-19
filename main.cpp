#include <Novice.h>
#include <math.h>

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
		32.0f,
		180.0f
	};
	Enemy enemy = {
		{1000.0f, 270.0f, 64.0f, 64.0f},
		32.0f,
		3.0f
	};

	//カウントキャラクター
	float pos_x = 1200.0f, pos_y = 580.0f;

	//長押しカウント
	int count = 0;

	//曲の時間
	int time_count = 0;
	int time_AnimCount = 0;

	//背景画像
	int bg1X = 0, bg2X = 1280;
	int bg3X = 0, bg4X = 1280;
	int bg_speed = 4;

	//アニメーションカウント
	int frameCount = 0;
	int AnimCount = 0;

	//フラグ管理
	bool Gameflag = true;  //ゲームスタート用フラグ
	bool Stage_Easyflag = false;  //難易度EASYフラグ
	bool Stage_Easy_Clearflag = false;  //難易度EASYクリアフラグ
	bool Stage_Normalflag = false;  //難易度NORMALフラグ
	bool Stage_Normal_Clearflag = false;  //難易度Normalクリアフラグ
	bool Stage_Hardflag = false;  //難易度HARDフラグ
	bool Stage_Hard_Clearflag = false;  //難易度Hardクリアフラグ
	bool Switchingflag = false;  //画面切り替えフラグ
	bool Pushflag = false;  //押した判定用
	bool onceflag = false;  //押されたか用フラグ
	bool L_push = false;  //長押しフラグ
	bool S_push = false;  //短押しフラグ
	bool isEnemyAlive = true;  //敵生存フラグ
	bool Clearflag = false;  //クリアフラグ

	//画像切り替え
	enum Scene {
		eScene_START,
		eScene_SELECT,
		eScene_STAGE_EASY,
		eScene_STAGE_NORMAL,
		eScene_STAGE_HARD,
		eScene_CLEAR,
	};
	Scene scene_no = eScene_START;

	//BGM・SE
	int GameStart = Novice::LoadAudio("./Resources/BGM/GameStart.mp3");
	int BGM1 = Novice::LoadAudio("./Resources/BGM/BGM1.mp3");  
	int BGM2 = Novice::LoadAudio("./Resources/BGM/BGM2.mp3");
	int BGM3 = Novice::LoadAudio("./Resources/BGM/BGM3.mp3");
	int Result = Novice::LoadAudio("./Resources/BGM/Result.mp3");
	int Kill = Novice::LoadAudio("./Resources/SE/Kill.mp3");
	int voiceHandle1 = -1;
	int voiceHandle2 = -1;
	int voiceHandle3 = -1;
	int voiceHandle4 = -1;
	int voiceHandle5 = -1;
	int voiceHandle6 = -1;

	//画像読み込み
	int Ebi = Novice::LoadTexture("./Resources/ebi.png");
	int Ika = Novice::LoadTexture("./Resources/ika.png");
	int Maguro = Novice::LoadTexture("./Resources/maguro.png");
	int Start = Novice::LoadTexture("./Resources/Start.png");
	int Haikei[2] = {
		Novice::LoadTexture("./Resources/Haikei1.png"),
		Novice::LoadTexture("./Resources/Haikei2.png"),
	};
	int Clear = Novice::LoadTexture("./Resources/Clear.png");
	int ReadyUI = Novice::LoadTexture("./Resources/UI/Ready.png");
	int StartUI = Novice::LoadTexture("./Resources/UI/Start.png");
	int CountUI = Novice::LoadTexture("./Resources/UI/Count.png");
	int ClearUI = Novice::LoadTexture("./Resources/UI/Clear.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// ↓更新処理ここから

		switch (scene_no) {
		case eScene_START:

			if (!Novice::IsPlayingAudio(voiceHandle1) || voiceHandle1 == -1) {
				voiceHandle1 = Novice::PlayAudio(GameStart, true, 0.5f);
			}

			if (keys[DIK_RETURN] && !preKeys[DIK_RETURN]) {  //ENTER押したとき
				Novice::StopAudio(voiceHandle1);
				scene_no = eScene_STAGE_EASY;
			}

			break;
		case eScene_SELECT:
			break;
		case eScene_STAGE_EASY:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				frameCount++;
				time_count = 0;
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
				
				//時間制限

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle2) || voiceHandle2 == -1) {
					voiceHandle2 = Novice::PlayAudio(BGM1, false, 0.5f);
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

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else{
					S_push = false;
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

				if (S_push) {
					if (isEnemyAlive) {
						if ((ball.radius / 2 + enemy.radius / 2 ) * (ball.radius / 2 + enemy.radius / 2) > ((ball.position.x - 18) - (enemy.position.x + 18)) * ((ball.position.x - 18) - (enemy.position.x + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18))) {
							if (isEnemyAlive == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								isEnemyAlive = false;
							}
						}
					}
				}

				//カウントダウン
				if (time_count > 0) {
					pos_x -= 0.18f;
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
				
				if (time_AnimCount > 113) {
					Novice::StopAudio(voiceHandle2);
					Clearflag = true;
				}

				if (time_AnimCount == 116) {
					time_count = 0;
					Stage_Easyflag = false;
					Stage_Easy_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}
			break;
		case eScene_STAGE_NORMAL:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				frameCount++;
				time_count = 0;
				if (frameCount == 60) {
					AnimCount += 1;
				}
				if (frameCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Normalflag = true;
				}
			}

			//難易度普通

			if (Stage_Normalflag) {

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle3) || voiceHandle3 == -1) {
					voiceHandle3 = Novice::PlayAudio(BGM2, false, 0.5f);
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

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else {
					S_push = false;
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

				if (S_push) {
					if (isEnemyAlive) {
						if ((ball.radius / 2 + enemy.radius / 2) * (ball.radius / 2 + enemy.radius / 2) > ((ball.position.x - 18) - (enemy.position.x + 18)) * ((ball.position.x - 18) - (enemy.position.x + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18))) {
							if (isEnemyAlive == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								isEnemyAlive = false;
							}
						}
					}
				}

				//曲が変わったら切り替え

				//1回目の切り替え

				if (time_count < 6000) { 
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE] && !keys[DIK_SPACE]) {
							Pushflag = true;
							Switchingflag = false;
						}
					}
				}

				//2回目の切り替え

				if (time_count < 3000) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE] && !keys[DIK_SPACE]) {
							Pushflag = false;
							Switchingflag = false;
						}
					}
				}

				//カウントダウン

				if (time_count > 0) {
					pos_x -= 0.18f;
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				bg3X = bg3X - bg_speed;
				bg4X = bg4X - bg_speed;

				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}
				if (bg3X < -1270 && bg4X < 0) {
					bg3X = 1270;
				}
				if (bg4X < -1270 && bg3X < 0) {
					bg4X = 1270;
				}

				//曲が終わったら終了させる

				if (time_AnimCount > 113) {
					Novice::StopAudio(voiceHandle3);
					Clearflag = true;
				}

				if (time_AnimCount == 116) {
					time_count = 0;
					Stage_Normalflag = false;
					Stage_Normal_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}

			break;
		case eScene_STAGE_HARD:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				frameCount++;
				time_count = 0;
				if (frameCount == 60) {
					AnimCount += 1;
				}
				if (frameCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Hardflag = true;
				}
			}

			//難易度難しい

			if (Stage_Hardflag) {

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle4) || voiceHandle4 == -1) {
					voiceHandle4 = Novice::PlayAudio(BGM3, false, 0.5f);
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

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else {
					S_push = false;
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

				if (S_push) {
					if (isEnemyAlive) {
						if ((ball.radius / 2 + enemy.radius / 2) * (ball.radius / 2 + enemy.radius / 2) > ((ball.position.x - 18) - (enemy.position.x + 18)) * ((ball.position.x - 18) - (enemy.position.x + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18)) + ((ball.position.y - 18) - (enemy.position.y + 18))) {
							if (isEnemyAlive == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								isEnemyAlive = false;
							}
						}
					}
				}

				//カウントダウン

				if (time_count > 0) {
					pos_x -= 0.18f;
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

				if (time_count < 100) {
					Novice::StopAudio(voiceHandle4);
					Clearflag = true;
				}

				if (time_count < 0) {
					time_count = 0;
					Stage_Normalflag = false;
					Stage_Hard_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}

			break;
		case eScene_CLEAR:
			if (Clearflag) {
				if (!Novice::IsPlayingAudio(voiceHandle5) || voiceHandle5 == -1) {
					voiceHandle5 = Novice::PlayAudio(Result, true, 0.5f);
				}
				if (Stage_Easy_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					enemy.position.x = 1000.0f;
					enemy.position.y = 270.0f;
					pos_x = 1200.0f;
					frameCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						Clearflag = false;
						isEnemyAlive = true;
						Stage_Easy_Clearflag = false;
						scene_no = eScene_STAGE_NORMAL;
					}
				}
				if (Stage_Normal_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					enemy.position.x = 1000.0f; 
					enemy.position.y = 270.0f;
					pos_x = 1200.0f;
					frameCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						isEnemyAlive = true;
						Clearflag = false;
						Stage_Normal_Clearflag = false;
						scene_no = eScene_STAGE_HARD;
					}
				}
				if (Stage_Hard_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					enemy.position.x = 1000.0f;
					enemy.position.y = 270.0f;
					pos_x = 1200.0f;
					frameCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						isEnemyAlive = true;
						Clearflag = false;
						Stage_Hard_Clearflag = false;
						scene_no = eScene_STAGE_HARD;
					}
				}
			}
			if (!Clearflag) {
				Novice::StopAudio(voiceHandle5);
			}
			break;
		}
		/// ↑更新処理ここまで

		/// ↓描画処理ここから
		switch (scene_no) {
		case eScene_START:
			Novice::DrawSprite(0, 0, Start, 1, 1, 0.0f, WHITE);
			break;
		case eScene_SELECT:
			break;
		case eScene_STAGE_EASY:
			Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::ScreenPrintf(0, 0, "TimeCount = %d", time_AnimCount);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			if (isEnemyAlive) {
				Novice::DrawSprite(int(enemy.position.x), int(enemy.position.y), Ika, 1, 1, 0.0f, WHITE);
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if(Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_NORMAL:
			if (!Pushflag) {
				Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			}
			if (Pushflag) {
				Novice::DrawSprite(bg3X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg4X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
			}
			Novice::ScreenPrintf(0, 0, "TimeCount = %d", time_count);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			if (isEnemyAlive) {
				Novice::DrawSprite(int(enemy.position.x), int(enemy.position.y), Maguro, 1, 1, 0.0f, WHITE);
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if (Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_HARD:
			Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::ScreenPrintf(0, 0, "TimeCount = %d", time_count);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			if (isEnemyAlive) {
				Novice::DrawSprite(int(enemy.position.x), int(enemy.position.y), Ika, 1, 1, 0.0f, WHITE);
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if (Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_CLEAR:
			Novice::DrawSprite(0, 0, Clear, 1, 1, 0.0f, WHITE);
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
