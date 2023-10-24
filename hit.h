#pragma once
// 当たり判定用関数
int isHit(
	float playerX,      // 自機のX座標
	float playerY,      // 自機のY座標
	float playerWidth,  // 自機の横幅
	float playerHeight, // 自機の縦幅

	float enemyX,      // 敵のX座標
	float enemyY,      // 敵のY座標
	float enemyWidth,  // 敵の横幅
	float enemyHeight, // 敵の縦幅
	bool flag          // フラグ
) {

	if (enemyX + (-enemyWidth / 2.0f) < playerX + (playerWidth / 2.0f) &&
		playerX + (-playerWidth / 2.0f) < enemyX + (enemyWidth / 2.0f) &&
		enemyY + (-enemyHeight / 2.0f) < playerY + (enemyHeight / 2.0f) &&
		playerY + (-playerHeight / 2.0f) < enemyY + (enemyHeight / 2.0f)) {
		if (flag == false) {

			flag = true;
		}
	}
	return flag;
}

