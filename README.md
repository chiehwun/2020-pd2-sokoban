# 2020-pd2-sokoban
## Video link
#### [PD2 SOKOBAN Project Demo](https://www.youtube.com/watch?v=rVglG0at9Io&feature=youtu.be)
## How to play
#### 主選單界面操作
![Imgur](https://i.imgur.com/7bMrlIP.png)
1. 輸入玩家名稱或雙擊歷史清單內的名稱快速填入
2. 點 **Start Game** 重新從上次玩的關卡開始
3. 點選 **Select Level** 進入 **選擇關卡界面**
4. 點選 **Quit** 跳出確認視窗確認是否要離開遊戲
![Imgur](https://i.imgur.com/DXbCdmd.png)

#### 選擇關卡界面操作
1. 遊戲分為兩種模式
   - Normal (普通關卡)
   - Special (特殊關卡)
2. 顯示該玩家破關進度
![Imgur](https://i.imgur.com/Y9Gat4x.png)
3. 選擇關卡後直接開始遊戲
4. 點選 **Back to Menu** 返回 **主選單界面**

#### 遊戲畫面
![Imgur](https://i.imgur.com/wkWllGl.png)
1. 按 **上下左右鍵** 移動玩家
2. 按 **Space鍵** 重新開始
3. 破關後，按 **Enter鍵** 直接進入下一關
4. 按 **M鍵** 返回 **主選單界面**
5. 左側欄位顯示目前步數與關卡排名
6. 右上欄位顯示 **遊戲進行時間** 與 **玩家名稱**
7. **將所有棕色及紅色箱子推到相對應的顏色定點即可過關**
![Imgur](https://i.imgur.com/jz66FGw.gif)

## Features
#### 1. Main menu: 主選單畫面
   - Start Game
   - Select Level
   - Quit
#### 2. Game items (共3種game items)
##### 在特殊關卡中新增以下物件
   1. 坑洞
      - 分為深坑與淺坑
      - ![Imgur](https://i.imgur.com/V2gSjYp.png)
      - 玩家若跌入坑洞，遊戲結束
      - ![Imgur](https://i.imgur.com/85jRVtS.gif)
      - 將箱子(棕 or 紅)推入坑洞，遊戲結束 (因為箱子數 < 終點數)
      - ![Imgur](https://i.imgur.com/Py0NWJy.gif)
   2. 石頭(灰)
      - 移動行為與箱子相同
      - 可以將石頭推入坑洞，填平後，玩家便可跨越
      - ![Imgur](https://i.imgur.com/4MbrNRR.gif)
   3. 紅色箱子
      - 移動行為與箱子相同
      - 當紅色箱子推到終點後，便不可再推動
      - ![Imgur](https://i.imgur.com/jz66FGw.gif)
#### 3. The number of steps
#### 4. Saved Game (詳見Bonus)

## Bonus
1. 紀錄玩家名稱(主選單界面有做欄位不為空的防呆)
2. 顯示玩家破關時間
3. 遊戲重啟後自動填入最後玩遊戲的玩家名稱
4. 自動紀錄玩家破關紀錄
4. 排名榜顯示在遊戲畫面左側，並且在遊戲結束後自動新增到排行榜內(順序先依據**步數**再依據**時間**)
5. 使用**SQLite**資料庫，遊戲重啟後，所有玩家名稱以及紀錄仍存在

## Notes
#### 此遊戲使用Qt開發，其版本資訊如下：
![Imgur](https://i.imgur.com/4877JmO.png)
#### 若遇到無法開啟問題，請聯絡以下Email:
#### e14066282@gs.ncku.edu.tw
