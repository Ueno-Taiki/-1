#pragma once
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
);
