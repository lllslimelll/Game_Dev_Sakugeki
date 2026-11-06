#pragma once

class Health;

// プレイヤーの HPGauge ゲージ
class HPGauge
{
public:

	// health : 表示する体力（HPGauge より長く生きていること）
	explicit HPGauge(const Health& health);

	// デストラクタ
	~HPGauge(void);

	// フォントハンドルを持つのでコピー禁止
	HPGauge(const HPGauge&) = delete;
	HPGauge& operator=(const HPGauge&) = delete;

	// 初期化（フォントの作成。Health の初期化より後に呼ぶ）
	void Init(void);

	// 更新（遅れて減る部分の動き）
	void Update(float deltaTime);

	// 描画
	void Draw(void) const;

	// 解放
	void Release(void);

private:

	// ---- 色 ----
	static constexpr unsigned int COL_LEAF = 0x8fd36a;			// HPGauge（明）
	static constexpr unsigned int COL_LEAF_DARK = 0x4f9a4a;		// HPGauge（暗）
	static constexpr unsigned int COL_EMBER = 0xff7759;			// 残りわずか（明）
	static constexpr unsigned int COL_EMBER_DARK = 0xa3362a;	// 残りわずか（暗）
	static constexpr unsigned int COL_PETAL = 0xffe4ee;			// 遅れて減る部分
	static constexpr unsigned int COL_INK = 0xeef4ef;			// 枠、吹き出し
	static constexpr unsigned int COL_PANEL = 0x0a1410;			// アイコンの背景
	static constexpr unsigned int COL_HEART = 0xff6f91;			// ハート
	static constexpr unsigned int COL_NUM = 0x13201a;			// 数値
	static constexpr unsigned int COL_NUM_LOW = 0x2a0d07;		// 数値（残りわずか）

	// ---- 配置 ----
	static constexpr int POS_X = 22;			// 左端
	static constexpr int POS_Y = 14;			// 上端
	static constexpr int ICON_R = 19;			// ハートのアイコンの半径
	static constexpr int ICON_GAP = 10;			// アイコンとゲージの間
	static constexpr int BAR_W = 260;
	static constexpr int BAR_H = 14;
	static constexpr int BAR_TOP = 40;			// ゲージの上にある吹き出しの領域
	static constexpr int NUM_FONT_SIZE = 19;
	static constexpr int BUBBLE_PAD_X = 9;
	static constexpr int BUBBLE_R = 9;			// 吹き出しの角の丸み
	static constexpr int BUBBLE_TIP = 6;		// 吹き出しの先端の大きさ

	// ---- 演出 ----
	static constexpr float LOW_RATE = 0.3f;		// これ未満で赤くする
	static constexpr float LAG_DELAY = 0.15f;	// 遅れて減る部分が動き出すまで（秒）
	static constexpr float LAG_SPEED = 4.0f;	// 遅れて減る部分が追いかける速さ

	// 表示する体力（所有しない）
	const Health& health_;

	// 数値のフォント
	int font_;

	// 遅れて減る部分
	float lagHp_;
	float lagWait_;
	int lastHp_;

	// ハートのアイコン
	void DrawIcon(int barY) const;

	// ゲージ本体
	void DrawBar(int barX, int barY, float rate, float lagRate, bool isLow) const;

	// 数値の吹き出し（先端がゲージの末端を指す）
	void DrawBubble(int tipX, int hp, bool isLow) const;
};