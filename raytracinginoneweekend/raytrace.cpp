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

			/*��ȡ�����������scattered�ͷ���˥������attenuation*/

			return attenuation * color(scattered, world, depth + 1);

			/*ײ��һ�Σ�����ϵ��0.5��Ȼ���Է�����ߣ���PΪ��㣬PMΪ����������ȥײ����ֱ��û��ײ�����κ��򣬣��·�else����У�������ϵ�����Ա�����ɫֵ��Ϊ��������ص����ɫ*/
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
	/*����һ������n+1��Ԫ�ص����飬�����ÿ��Ԫ����ָ��hitable�����ָ�롣Ȼ�������ָ�븳ֵ��list�����ԣ�list��ָ���ָ�롣*/

	texture *checker = new checker_texture(
		new constant_texture(vec3(0.2, 0.3, 0.1)),
		new constant_texture(vec3(0.9, 0.9, 0.9))
	);
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	/*�ȴ���һ�������ڣ�0��-1000��0���뾶Ϊ1000�ĳ��������򣬽���ָ�뱣����list�ĵ�һ��Ԫ���С�*/
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			/*����forѭ���л������11+11��*(11+11)=484�����С��*/
			float choose_mat = (rand() % (100) / (float)(100));
			/*����һ����0��1�������������Ϊ����С����ϵķ�ֵ*/
			vec3 center(a + 0.9*(rand() % (100) / (float)(100)), 0.2,
				b + 0.9*(rand() % (100) / (float)(100)));
			/*�� a+0.9*(rand()%(100)/(float)(100))�����[-11,11]������-11��11��֮����������������[-11,11)֮���22��������ʹ�����ĵ�x,z�����ǣ�-11��11��֮��������*/
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				/*����С���λ�ú���ǰ��Ĵ����λ��̫����*/
				if (choose_mat < 0.8) {     //diffuse
/*���Ϸ�ֵС��0.8��������Ϊ�����������������˥��ϵ��x,y,z���ǣ�0��1��֮����������ƽ��*/
					list[i++] = new  moving_sphere(center, center + vec3(0, 0.5*(rand() % (100) / (float)(100)), 0),0.0,1.0, 0.2,
						new lambertian(new constant_texture(vec3(
						(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100))))));
				}
				else if (choose_mat < 0.95) {
					/*���Ϸ�ֵ���ڵ���0.8С��0.95��������Ϊ���淴���򣬾��淴�����˥��ϵ��x,y,z��ģ��ϵ�����ǣ�0��1��֮����������һ�ٳ���2*/
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100)))),
							0.5*(1 + (rand() % (100) / (float)(100)))));
				}
				else {
					/*���Ϸ�ֵ���ڵ���0.95��������Ϊ������*/
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	/*������������*/
	//return new hitable_list(list, i);
	return new bvh_node(list, i,0.0,1.0);

	
}

int main() {
	int nx = 200;
	int ny = 100;
	int ns = 100;//��



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
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); //٤�����
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";

		}
	}
}