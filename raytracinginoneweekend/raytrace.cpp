#include "ray.h"
#include <iostream>
#include "hitable.h"
#include "hitable_list.h"
#include "float.h"
#include "sphere.h"
#include "movesphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectic.h"
#include "bvh.h"
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

hittable *random_scene() {
	int n = 500;
	hittable **list = new hittable *[n + 1];
	/*定义一个包含n+1个元素的数组，数组的每个元素是指向hitable对象的指针。然后将数组的指针赋值给list。所以，list是指针的指针。*/

	texture *checker = new checker_texture(
		new constant_texture(vec3(0.2, 0.3, 0.1)),
		new constant_texture(vec3(0.9, 0.9, 0.9))
	);
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	/*先创建一个中心在（0，-1000，0）半径为1000的超大漫射球，将其指针保存在list的第一个元素中。*/
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			/*两个for循环中会产生（11+11）*(11+11)=484个随机小球*/
			float choose_mat = (rand() % (100) / (float)(100));
			/*产生一个（0，1）的随机数，作为设置小球材料的阀值*/
			vec3 center(a + 0.9*(rand() % (100) / (float)(100)), 0.2,
				b + 0.9*(rand() % (100) / (float)(100)));
			/*” a+0.9*(rand()%(100)/(float)(100))”配合[-11,11]产生（-11，11）之间的随机数，而不是[-11,11)之间的22个整数。使得球心的x,z坐标是（-11，11）之间的随机数*/
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				/*避免小球的位置和最前面的大球的位置太靠近*/
				if (choose_mat < 0.8) {     //diffuse
/*材料阀值小于0.8，则设置为漫反射球，漫反射球的衰减系数x,y,z都是（0，1）之间的随机数的平方*/
					list[i++] = new  moving_sphere(center, center + vec3(0, 0.5*(rand() % (100) / (float)(100)), 0),0.0,1.0, 0.2,
						new lambertian(new constant_texture(vec3(
						(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100))))));
				}
				else if (choose_mat < 0.95) {
					/*材料阀值大于等于0.8小于0.95，则设置为镜面反射球，镜面反射球的衰减系数x,y,z及模糊系数都是（0，1）之间的随机数加一再除以2*/
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100)))),
							0.5*(1 + (rand() % (100) / (float)(100)))));
				}
				else {
					/*材料阀值大于等于0.95，则设置为介质球*/
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	/*定义三个大球*/
	//return new hitable_list(list, i);
	return new bvh_node(list, i,0.0,1.0);

	
}

int main() {
	int nx = 200;
	int ny = 100;
	int ns = 100;//采



	ofstream outfile("texture.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	
	/*hittable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hittable *world = new hitable_list(list, 5);*/

	hittable *world = random_scene();

	vec3 lookfrom(13, 2, 3);

	vec3 lookat(0, 0, 0);

	float dist_to_focus = (lookfrom - lookat).length();

	float aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, 0.7*dist_to_focus,0.0,1.0);
	

	

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