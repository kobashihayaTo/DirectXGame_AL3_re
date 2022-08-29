#include "EnemyBullet.h"
#include "MyMath.h"


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	//引数で受け取った速度をメンバ変数に代入
	model_ = model;
	velocity_ = velocity;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("EnemyBullet.png");
	//引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	worldTransform_.Initialize();
}

void EnemyBullet::Update()
{
	//座標を移動させる
	worldTransform_.translation_-= velocity_;

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

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	//デスフラグの立った
	isDead_ = true;
}
Vector3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
float EnemyBullet::GetRadius() { return radius_; }