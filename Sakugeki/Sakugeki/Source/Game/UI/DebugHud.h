#pragma once
#include <string>
#include <vector>

// TODO(仮UI) 本物の UI ができたら差し替える
// 見た目はモック「桜蔓」の HUD に合わせている
//   左上：HPGauge と精霊の加護 / 右上：技リスト / 右下：蕾（技リストの達成度）

// 技リストの1項目
struct HudMove
{
	// 例：L"X+X+X長押し"。+ で区切り、X / Y / A / B / LT / RT はボタン表記で描く
	std::wstring label;
	bool isDone = false;
};

// HUD に表示する値
struct HudData
{
	// 加護ゲージが満タンのときのガイド
	enum class GUIDE
	{
		NONE,
		HEAL,		// 回復を勧める
		FINISHER,	// フィニッシュを勧める
	};

	int hp = 0;
	int maxHp = 0;
	float spirit = 0.0f;			// 精霊の加護
	float maxSpirit = 0.0f;
	GUIDE guide = GUIDE::NONE;
	std::vector<HudMove> moves;		// 技リスト
	bool isBloom = false;			// 技リストを全部達成した（必殺技が使える）
};

// 仮の HUD（デバッグ表示）
class DebugHud
{
public:

	DebugHud(void);
	~DebugHud(void);

	// フォントハンドルを持つのでコピー禁止
	DebugHud(const DebugHud&) = delete;
	DebugHud& operator=(const DebugHud&) = delete;

	// 初期化（フォントの作成）
	void Init(void);

	// 更新（遅れて減る HPGauge や点滅など、見た目だけの状態）
	void Update(const HudData& data, float deltaTime);

	// 描画
	void Draw(const HudData& data) const;

	// 解放
	void Release(void);

private:

	// 技ラベルを分解した部品
	enum class PIECE
	{
		TEXT,	// 普通の文字
		KEY,	// ボタン
		PLUS,	// 区切りの +
	};
	struct Piece
	{
		PIECE type;
		std::wstring text;
	};

	// ---- 色（モックの CSS 変数に対応） ----
	static constexpr unsigned int COL_LEAF = 0x8fd36a;
	static constexpr unsigned int COL_SAKURA = 0xf2a3c2;
	static constexpr unsigned int COL_PETAL = 0xffe4ee;
	static constexpr unsigned int COL_EMBER = 0xff7759;
	static constexpr unsigned int COL_INK = 0xeef4ef;
	static constexpr unsigned int COL_DIM = 0x93ab9e;
	static constexpr unsigned int COL_PANEL = 0x0a1410;
	static constexpr unsigned int COL_KEY_X = 0x4f8ff7;
	static constexpr unsigned int COL_KEY_Y = 0xf2c53d;
	static constexpr unsigned int COL_KEY_A = 0x5cc25a;
	static constexpr unsigned int COL_KEY_B = 0xe5534b;
	static constexpr unsigned int COL_KEY_TRIGGER = 0xc9d3cd;
	static constexpr unsigned int COL_KEY_DARK = 0x0d1712;

	// ---- 配置 ----
	static constexpr int MARGIN_X = 22;			// 左右の余白
	static constexpr int MARGIN_TOP = 14;		// 上の余白
	static constexpr int MARGIN_BOTTOM = 18;	// 下の余白
	static constexpr int HPGauge_ICON_R = 19;		// ハートのアイコンの半径
	static constexpr int HPGauge_BAR_W = 260;
	static constexpr int HPGauge_BAR_H = 14;
	static constexpr int HPGauge_BAR_TOP = 40;		// HPGauge ゲージの上にある吹き出しの領域
	static constexpr int SP_ICON_R = 14;		// 精霊のアイコンの半径
	static constexpr int SP_BAR_W = 65;			// HPGauge ゲージの 1/4
	static constexpr int SP_BAR_H = 7;
	static constexpr int MOVE_ROW_H = 28;		// 技リストの1行の高さ
	static constexpr int MOVE_ROW_GAP = 6;
	static constexpr int KEY_H = 20;			// ボタン表記の高さ
	static constexpr int GUIDE_KEY_H = 18;
	static constexpr int BUD_SIZE = 150;		// 蕾の直径

	// ---- 演出 ----
	static constexpr float LOW_HPGauge_RATE = 0.3f;	// これ未満で HPGauge を赤くする
	static constexpr float LAG_DELAY = 0.15f;	// 遅れて減る HPGauge が減り始めるまで（秒）
	static constexpr float LAG_SPEED = 4.0f;	// 遅れて減る HPGauge の追従の速さ
	static constexpr float FLASH_TIME = 0.6f;	// 技を達成したときの点灯時間（秒）

	// フォント
	int fontSmall_;		// ラベル
	int fontGuide_;		// ゲージのガイド、蕾のラベル
	int fontMove_;		// 技リスト
	int fontNum_;		// HPGauge の数値

	// 経過時間（点滅用）
	float time_;

	// 遅れて減る HPGauge
	float lagHp_;
	float lagWait_;
	int lastHp_;

	// 技を達成したときの点灯
	std::vector<bool> prevDone_;
	std::vector<float> flash_;

	// 各部分の描画
	void DrawHp(const HudData& data) const;
	void DrawSpirit(const HudData& data) const;
	void DrawMoves(const HudData& data, int screenW) const;
	void DrawBud(const HudData& data, int screenW, int screenH) const;

	// 部品の描画
	void DrawGauge(int x, int y, int w, int h, float rate,
		unsigned int colLeft, unsigned int colRight) const;
	void DrawPetal(float x, float y, float angle, float length, float width, unsigned int color) const;

	// ボタン表記を描画して幅を返す（isDraw が false なら幅を返すだけ）
	int DrawKey(int x, int centerY, const std::wstring& key, int height, int font, bool isDraw) const;

	// 部品を並べて描画して幅を返す（isDraw が false なら幅を返すだけ）
	int DrawPieces(int x, int centerY, const std::vector<Piece>& pieces,
		int keyH, int font, unsigned int textColor, bool isDraw) const;

	// 技ラベルを部品に分解
	static std::vector<Piece> ParseLabel(const std::wstring& label);

	// 色の補間
	static unsigned int LerpColor(unsigned int a, unsigned int b, float t);

	// 不透明度の設定（255 で通常の描画に戻す）
	static void SetAlpha(int alpha);
};