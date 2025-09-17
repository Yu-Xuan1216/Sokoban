# Sokoban 推箱子遊戲

## 遊戲介紹
這是一個使用 Visual Studio MFC 開發的推箱子遊戲，共有151關，玩家需要在地圖中操控角色，可以在沒有阻礙物的情況下，向 **上、下、左、右** 移動，並將所有箱子推到指定位置，即可過關。

## 功能
- 關卡設計: 支援多張地圖，挑戰不同難度
- 鍵盤操作: 使用方向鍵操縱角色，可以在沒有阻礙物的情況下，按下鍵盤的上下左右鍵，即可推動箱子
- 箱子計數 (Arrival): 抵達的箱子計數
- 玩家移動步數 (Steps): 玩家移動的步數
- 重新開始: 按下R建，重置此關卡，回到初始位置 
- 過關判斷: 檢查是否所有的箱子都到達指定位置

## 專案結構
- Sokoban.sln
  - Visual Studio 解決方案，打開後可編譯整個專案
- source/
  - 存放關卡地圖(.txt)、遊戲圖示(.bmp)

## 執行方式
1. 確認已安裝 Visual Studio 並勾選 MFC 元件
2. 使用 Visual Studio 開啟 Sokoban.sln
3. 建置方案 (Ctrl+Shift+B)
4. 按下 F5 即可執行遊戲

## 畫面呈現
![level1_gif](https://github.com/user-attachments/assets/00fd552f-cc9e-4401-96ed-a9a90cf86c7a)
