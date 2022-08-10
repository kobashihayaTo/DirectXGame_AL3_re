#include "Enemy.h"
#include <cassert>
#include "Player.h"
#include "MyMath.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Red.png");

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = { 5,0,40 };

	worldTransform_.Initialize();
	//接近フェーズ初期化
	ApproachPhaseInt();

	Hp = 20;
}

void Enemy::Update() {
	Translation();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	switch (phase_)
	{
	case Phase::Approach:
		ApproachVelocity();
		break;
	case Phase::Leave:
		LeaveVelocity();
		break;
	}
	worldTransform_.matWorld_ = MyMath::Identity();
	worldTransform_.matWorld_ *= MyMath::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MyMath::RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::Trans(worldTransform_.translation_);
	//行列の再計算
	worldTransform_.TransferMatrix();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

}

void Enemy::Translation()
{
	Vector3 move = { 0,0,0 };
	const float speed = 0.0003f;
	move.z -= speed;

	worldTransform_.translation_ += move;
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
	//----HPの表示----
	if (Hp== 1)
	{

	}
}

void Enemy::Fire() {


	//敵の座標コピー
	Vector3 position = worldTransform_.translation_;
	//弾の速度
	const float kBulletSpeed = 1.0f;

	//プレイヤーのワールド座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	Vector3 enemyPos = GetWorldPosition();
	//敵キャラ->自キャラの差分ベクトルを求める
	Vector3 velocity = MyMath::Vector3sub(playerPos, enemyPos);
	//ベクトルの正規化
	velocity = MyMath::normalize(velocity);
	//ベクトルの長さを、早さに合わせる
	velocity *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Math_(velocity, worldTransform_.matWorld_);
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, position, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachPhaseInt()
{
	//発射タイマーを初期化
	fileTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の並行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
//接近フェーズ
void Enemy::ApproachVelocity() {
	//発射タイマーカウントダウン
	fileTimer--;
	//指定時間に達した
	if (fileTimer <= 0)
	{
		//弾の発射
		Fire();
		//発射タイマーを初期化
		fileTimer = kFireInterval;
	}
	//移動(ベクトルを加算)
	worldTransform_.translation_ += ApproachVelocity_;
	//特定の位置に達したら離脱
	if (worldTransform_.translation_.z < -1.0f) {
		phase_ = Phase::Leave;
	}
}
//離脱フェーズ
void Enemy::LeaveVelocity() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += LeaveVelocity_;
}

 void Enemy::OnCollision() {
	
 	Hp--;

	if (Hp <= 0)
	{
		//デスフラグの立った
		isDead_ = true;
	}

}

float Enemy::GetRadius() { return radius_; }