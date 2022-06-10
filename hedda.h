#pragma once
#include "WorldTransform.h"

//�P�ʍs��
Matrix4 Identity() {
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
	return matIdentity;
}
//�X�P�[�����O�s���錾
Matrix4 Scale(Vector3 scale_) {
	//�X�P�[�����O�̐ݒ�
	Matrix4 matScale;
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}
Matrix4 RotZ(Vector3 rotation_) {
	//Z����]�s���錾
	Matrix4 matRotZ;
	matRotZ = Identity();
	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	return matRotZ;
}
Matrix4 RotX(Vector3 rotation_) {
	//X����]�s���錾
	Matrix4 matRotX;
	matRotX = Identity();
	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	return matRotX;
}
Matrix4 RotY(Vector3 rotation_) {
	//Y����]�s���錾
	Matrix4 matRotY;
	matRotY = Identity();
	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	return matRotY;
}
//���s�ړ��s���錾
Matrix4 Trans(Vector3 translation_) {
	Matrix4 matTrans;
	matTrans = Identity();
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	return matTrans;
}
