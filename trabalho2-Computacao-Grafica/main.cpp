#include <iostream>

#include "rtweekend.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

using namespace std;


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.3, 0.3, 0.3));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    /*for (int a = -1; a < 1; a++) {
        for (int b = -1; b < 1; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            //PARTE DA ALTERAÇAO DOS MATERIAIS
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }*/

    auto material1 = make_shared<metalModified>(color(0.7, 0.6, 0.5), 0.3);
                                        //Z,Y,X
    world.add(make_shared<sphere>(point3(3, 1, -1), 0.3, material1));

    auto material2 = make_shared<dielectricModified>(1.5);
    world.add(make_shared<sphere>(point3(2, 1, -0.5), 0.5, material2));

    auto material3 = make_shared<lambertianModified>(color(0.8, 0.5, 0.3));
    world.add(make_shared<sphere>(point3(0.5, 1, 0), 0.7, material3));

    auto material4 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.3);
    world.add(make_shared<sphere>(point3(-1.5, 1, 1), 0.9, material4));

    auto material5 = make_shared<dielectric>(1.7);
    world.add(make_shared<sphere>(point3(-4, 1, 2), 1.1, material5));
    
    

    return world;
}

int main() {

    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World
    auto world = random_scene();
    
    // Camera 1
    /*point3 lookfrom(13,2,2);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;*/
    //20
    // Camera 2
    /*point3 lookfrom(3,2,6);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = 5.0;
    auto aperture = 0.1;*/
    //90
    // Camera 3
    /*point3 lookfrom(4,1,-11);
    point3 lookat(1,1,1);
    vec3 vup(0,1,0);
    auto dist_to_focus = 15.0;
    auto aperture = 0.1;*/
    //40
    // Camera 4
    /*point3 lookfrom(7,2,-2);
    point3 lookat(-25,0,6);
    vec3 vup(0,1,0);
    auto dist_to_focus = 15.0;
    auto aperture = 0.1;*/
    //55
    // Camera 5
    point3 lookfrom(15,14,6);
    point3 lookat(0,1,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    //20
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    
    // Render

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(cout, pixel_color, samples_per_pixel);
        }
    }

    cerr << "\nDone.\n";
}