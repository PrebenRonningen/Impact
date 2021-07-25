#pragma once
#include <DirectXMath.h>
enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

struct Double3 {
	double x, y, z;
};

void twoaxisrot(double r11, double r12, double r21, double r31, double r32, Double3& res) {
	res.x = atan2(r11, r12);
	res.y = acos(r21);
	res.z = atan2(r31, r32);
}


void threeaxisrot(double r11, double r12, double r21, double r31, double r32, Double3& res) {
	res.y = atan2(r11, r12);
	res.x = asin(r21);
	res.z = atan2(r31, r32);
}
DirectX::XMFLOAT3 QuaternionToEuler(DirectX::XMFLOAT4 q, RotSeq rotSeq = RotSeq::yxz)
{
	Double3 res{};

	switch (rotSeq) {
	case zyx:
		threeaxisrot(2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			-2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case zyz:
		twoaxisrot(2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			res);
		break;

	case zxy:
		threeaxisrot(-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case zxz:
		twoaxisrot(2 * (q.x * q.z + q.w * q.y),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			res);
		break;

	case yxz:
		threeaxisrot(2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			-2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case yxy:
		twoaxisrot(2 * (q.x * q.y - q.w * q.z),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			res);
		break;

	case yzx:
		threeaxisrot(-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case yzy:
		twoaxisrot(2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			res);
		break;

	case xyz:
		threeaxisrot(-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case xyx:
		twoaxisrot(2 * (q.x * q.y + q.w * q.z),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			res);
		break;

	case xzy:
		threeaxisrot(2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			-2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case xzx:
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
