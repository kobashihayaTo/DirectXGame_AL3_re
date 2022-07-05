#pragma once
#include "WorldTransform.h"
#include "MyMath.h"
#include <math.h>

//単位行列
Matrix4 Identity() {
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
	return matIdentity;
}
//スケーリング行列を宣言
Matrix4 Scale(Vector3 scale_) {
	//スケーリングの設定
	Matrix4 matScale;
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}

Matrix4 RotZ(Vector3 rotation_) {
	//Z軸回転行列を宣言
	Matrix4 matRotZ;
	matRotZ = Identity();
	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	return matRotZ;
}
Matrix4 RotX(Vector3 rotation_) {
	//X軸回転行列を宣言
	Matrix4 matRotX;
	matRotX = Identity();
	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	return matRotX;
}
Matrix4 RotY(Vector3 rotation_) {
	//Y軸回転行列を宣言
	Matrix4 matRotY;
	matRotY = Identity();
	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	return matRotY;
}
//平行移動行列を宣言
Matrix4 Trans(Vector3 translation_) {
	Matrix4 matTrans;
	matTrans = Identity();
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	return matTrans;
}

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

Vector3 Vector3sub(Vector3& v1, Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

Vector3 normalize(Vector3& vec)
{
	float len = length(vec);

	if (len != 0) {
		return vec /= len;
	}

	return vec;
}

float length(Vector3& vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}