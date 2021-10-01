#pragma once
#include <DirectXMath.h>
enum class RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

struct Double3 {
	double x, y, z;
};

static void twoaxisrot(double r11, double r12, double r21, double r31, double r32, Double3& res) {
	res.x = atan2(r11, r12);
	res.y = acos(r21);
	res.z = atan2(r31, r32);
}


static void threeaxisrot(double r11, double r12, double r21, double r31, double r32, Double3& res) {
	res.y = atan2(r11, r12);
	res.x = asin(r21);
	res.z = atan2(r31, r32);
}

static DirectX::XMFLOAT3 QuaternionToEulerRad(DirectX::XMFLOAT4 q, RotSeq rotSeq = RotSeq::yxz)
{
	Double3 res{};

	switch (rotSeq) {
	case RotSeq::zyx:
		threeaxisrot(2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			-2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case RotSeq::zyz:
		twoaxisrot(2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			res);
		break;

	case RotSeq::zxy:
		threeaxisrot(-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case RotSeq::zxz:
		twoaxisrot(2 * (q.x * q.z + q.w * q.y),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			res);
		break;

	case RotSeq::yxz:
		threeaxisrot(2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			-2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case RotSeq::yxy:
		twoaxisrot(2 * (q.x * q.y - q.w * q.z),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			res);
		break;

	case RotSeq::yzx:
		threeaxisrot(-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case RotSeq::yzy:
		twoaxisrot(2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			res);
		break;

	case RotSeq::xyz:
		threeaxisrot(-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case RotSeq::xyx:
		twoaxisrot(2 * (q.x * q.y + q.w * q.z),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			res);
		break;

	case RotSeq::xzy:
		threeaxisrot(2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			-2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case RotSeq::xzx:
		twoaxisrot(2 * (q.x * q.z - q.w * q.y),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			res);
		break;
	default:
		break;
	}

	return DirectX::XMFLOAT3{ float(res.x), float(res.y), float(res.z) };
}

static DirectX::XMFLOAT3 QuaternionToEulerDeg(DirectX::XMFLOAT4 q, RotSeq rotSeq = RotSeq::yxz)
{
	DirectX::XMFLOAT3 res = QuaternionToEulerRad(q, rotSeq);
	return DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees(float(res.x)), DirectX::XMConvertToDegrees(float(res.y)), DirectX::XMConvertToDegrees(float(res.z)) };
}
