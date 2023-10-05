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
		{100.0f, 330.0f, 64.0f, 64.0f},
		64.0f,
		180.0f
	};
	Enemy enemy{
		{1000.0f, 330.0f, 64.0f, 64.0f},
		64.0f,
		3.0f
	};

	//長押しカウント
	int count = 0;

	//背景画像
	int bg1X = 0, bg2X = 1280;
	int bg_speed = 4;

	//フラグ管理
	bool onceflag = false;
	bool L_push = false;
	bool S_push = false;
	bool isEnemyAlive = true;

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

	//画像読み込み
	int Haikei = Novice::LoadTexture("./Resources/Haikei.png");

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
			break;
		case eScene_SELECT:
			break;
		case eScene_STAGE_EASY:

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
			}

			if (L_push == true) {  //長押し判定
				ball.position.y -= ball.speed;
				if (ball.position.y == 150) {  //上に移動した時
					ball.speed -= ball.speed + 180;
				}
				else if (ball.position.y == 510) {  //下に移動した時
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



			//画像スクロール

			bg1X = bg1X - bg_speed;
			bg2X = bg2X - bg_speed;
			if (bg1X < -1270 && bg2X < 0) {
				bg1X = 1270;
			}
			if (bg2X < -1270 && bg1X < 0) {
				bg2X = 1270;
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
			Novice::ScreenPrintf(0, 0, "Count = %d", count);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawBox(int(ball.position.x), int(ball.position.y), int(ball.position.w), int(ball.position.h), 0.0f, WHITE, kFillModeSolid);
			Novice::DrawBox(int(enemy.position.x), int(enemy.position.y), int(enemy.position.w), int(enemy.position.h), 0.0f, RED, kFillModeSolid);
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
