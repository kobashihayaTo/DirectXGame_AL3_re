#pragma once
#include "WorldTransform.h"

//�P�ʍs��
Matrix4 Identity();
//�X�P�[�����O�s���錾
Matrix4 Scale(Vector3 scale_);

Matrix4 RotZ(Vector3 rotation_);
Matrix4 RotX(Vector3 rotation_);
Matrix4 RotY(Vector3 rotation_);
//���s�ړ��s���錾
Matrix4 Trans(Vector3 translation_);
