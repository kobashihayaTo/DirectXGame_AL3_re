#pragma once
#include "WorldTransform.h"

//単位行列
Matrix4 Identity();
//スケーリング行列を宣言
Matrix4 Scale(Vector3 scale_);

Matrix4 RotZ(Vector3 rotation_);
Matrix4 RotX(Vector3 rotation_);
Matrix4 RotY(Vector3 rotation_);
//平行移動行列を宣言
Matrix4 Trans(Vector3 translation_);

Vector3 Math_(Vector3& vec_, Matrix4& mat_);
