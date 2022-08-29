#include "PlayerBullet.h"
#include "MyMath.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("PlayerBullet.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}
void PlayerBullet::Update()
{
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_ = MyMath::Identity();
	worldTransform_.matWorld_ *= MyMath::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MyMath::RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::Trans(worldTransform_.translation_);
	//行列の再計算
	worldTransform_.TransferMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
void PlayerBullet::OnCollision()
{
	//デスフラグの立った
	isDead_ = true;
}
Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
float PlayerBullet::GetRadius() { return radius_; }

