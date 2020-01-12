#include "ray.h"
#include <iostream>
#include "hitable.h"
#include "hitable_list.h"
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectic.h"
#include <fstream>
#include "random"
using namespace std;





vec3 color(const ray& r, hittable *world,int depth) {
	hit_record rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {

			/*获取反射光线向量scattered和反射衰减向量attenuation*/

			return attenuation * color(scattered, world, depth + 1);

			/*撞击一次，乘以系数0.5。然后以反射光线（以P为起点，PM为方向向量）去撞击球，直到没有撞击到任何球，（下方else语句中）最后带着系数乘以背景颜色值作为球体该像素点的颜色*/
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}


}

int main() {
	int nx = 200;
	int ny = 100;
	int ns = 100;//采样



	ofstream outfile("defocusBlur.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	
	hittable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hittable *world = new hitable_list(list, 5);

	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 2.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float random = rand() % (100) / (float)(100);
				float u = float(i + random) / float(nx);
				float v = float(j + random) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world,0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); //伽马矫正
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";

		}
	}
}