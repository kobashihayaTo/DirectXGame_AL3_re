#include "Player.h"
#include "MyMath.h"


//ベクトルと行列の掛け算
Vector3 Math_(Vector3& vec_, Matrix4& mat_)
{
	Vector3 matMath_;
	matMath_.x = vec_.x * mat_.m[0][0];
	matMath_.x += vec_.y * mat_.m[1][0];
	matMath_.x += vec_.z * mat_.m[2][0];

	matMath_.y = vec_.x * mat_.m[0][1];
	matMath_.y += vec_.y * mat_.m[1][1];
	matMath_.y += vec_.z * mat_.m[2][1];

	matMath_.z = vec_.x * mat_.m[0][2];
	matMath_.z += vec_.y * mat_.m[1][2];
	matMath_.z += vec_.z * mat_.m[2][2];

	return matMath_;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kCharacterSpeed = 0.2f;


	if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed,0,0 };
	}
	if (input_->PushKey(DIK_UP)) {
		move = { 0,kCharacterSpeed,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-kCharacterSpeed,0 };
	}

	worldTransform_.translation_ += move;



	//移動限界座標
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);



	//行列の再計算
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Player;(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("Player;(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);

	Rotation();
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Rotation()
{
	const float kCharacRotSpeed = 0.1f;

	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kCharacRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kCharacRotSpeed;
	}
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Trans(worldTransform_.translation_);
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Math_(velocity, worldTransform_.matWorld_);
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection_) {

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}