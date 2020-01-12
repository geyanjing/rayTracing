#include "ray.h"
#include <iostream>
#include <fstream>
using namespace std;


//先解释“光线撞上球”的情况：
//球上任一点的坐标（向量）为P，球心坐标（向量）为C，球的半径为R，则球的方程为
//| P - C | 2 = R2即(P - C)(P - C) = R2，注意这里的P, C都是向量，所以，
//方程等价于：dot((P - C), (P - C)) = R2
//光线的方程：R(t) = A + t * B （参考“问题十二”）
//“光线撞上球”意味着光线和球在“撞点”上相交，将光线方程代入球方程，得到：
//dot((A + t * B - C), (A + t * B - C)) = R2
//(B*t + (A - C))·(B*t + (A - C)) = R2
//(B·B)*t2 + 2 * (B·(A - C))*t + (A - C)·(A - C) - R2 = 0
//所以，
//a = (B·B) = dot(B, B)，
//b = 2 * (B·(A - C)) = 2 * dot(B, (A - C))，
//c = (A - C)·(A - C) - R2 = dot((A - C), (A - C)) - R2，

bool hit_sphere(const vec3& center, float radious, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radious * radious;
	float discriminant = b * b - 4 * a*c;
	return(discriminant > 0);
}

vec3 color(const ray& r) {
	if (hit_sphere(vec3(0,0,-1),0.5,r))
		return vec3(1, 0, 0);
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	// 标准化之后的y值在[-1, 1]中y + 1在[0, 2]中0.5*(y + 1)在[0, 1]
	return(1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	//t=0时，color=vec3(1,1,1)，乘以255后对应的RGB为（255，255，255）
		//t=1时，color=vec3(0.5,0.7,1)，乘以255后对应的RGB为（127.5，178.5，255）
		//如上两个颜色分别对应着“白色”和“浅蓝色”。
/*画面颜色=(1-t)*“白色”+ t*“浅蓝色”，即画面颜色为“白色”和“浅蓝色”（沿着Y方向）的线性插值的结果。如果要换成X或者Z方向，将上面的.y()改成.x()或者.z()即可。
若要换其他颜色，设置对应的RGB值即可。RGB值和颜色的对应可参考word中“字体颜色设置”“	其他颜色”“自定义”*/


}

int main() {
	int nx = 200;
	int ny = 100;



	ofstream outfile("sphere.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, u*horizontal + v * vertical + lower_left_corner);
			/*由画面中每个像素点在画面中的相对位置每个像素点对应的光线的方向向量从而确定画面的范围/大小。（完成第一步）*/
			vec3 col = color(r);
			//根据光线对每一个像素点上色。（完成第二步）
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";

		}
	}
}