#include "ray.h"
#include <iostream>
#include "hitable.h"
#include "hitable_list.h"
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include <fstream>
#include "random"
using namespace std;




vec3 color(const ray& r,hittable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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



	ofstream outfile("antialiasing.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	camera cam;
	hittable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hittable *world = new hitable_list(list, 2);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float random = rand() % (100) / (float)(100);
				float u = float(i + random) / float(nx);
				float v = float(j + random) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";

		}
	}
}