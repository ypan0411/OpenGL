#include <fstream>
#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

//#define DEBUG

int PIXEL_NUM= 500;
int SPHERE_NUM = 3;
//Set viewpoint
Point3f VIEW_POINT = {0,0,-2};
//set frame
Point3f R_UPPER = {1,1,0};
Point3f R_LOWER = {1,-1,0};
Point3f L_UPPER = {-1,1,0};
Point3f L_LOWER = {-1,-1,0};

//set view frustum
float F_FAR = 12.0;
float F_NEAR = 2.0;
//Arcsin(Sqrt(1/3)) = 35.3
float FOVY = 70.6;

struct Sphere {
    Point3f center;
    float radius;
};

//each ray represented as O + tD
struct Ray{
    Point3f origin;
    Point3f ray_direction;
};

//Normalize a  vector
Point3f normalize(const Point3f &p){
    float square = p.x*p.x + p.y*p.y + p.z*p.z;
    float length = sqrt(square);
    Point3f v(p.x/length, p.y/length,p.z/length);
    return v;
}

//for given screen coordinate, compute the ray
Ray compute_ray(const Point &p){
    Ray r;
    r.origin = VIEW_POINT;
    Point3f width = R_UPPER - L_UPPER;
    Point3f height = R_UPPER - R_LOWER;
#ifdef DEBUG2
    cout << "width:" << width << " height:" << height << endl;
#endif
    float p_width = width.x / PIXEL_NUM;
    float p_height = height.y / PIXEL_NUM;
#ifdef DEBUG2
    cout << "p_width:" << p_width << " p_height:" << p_height << endl;
#endif

    //compute the center coordinate of the pixel
    float center_x = L_UPPER.x + (p.x + 0.5f) *p_width;
    float center_y = L_UPPER.y - (p.y + 0.5f) *p_height;
#ifdef DEBUG2
    cout << "center_x" << center_x << " center_y:" << center_y << endl;
#endif

    //get the normalized ray_direction
    Point3f pixel_center(center_x, center_y, 0);
    r.ray_direction  = normalize(pixel_center - VIEW_POINT);

    return r;
}


float solveQuadratic(const float &a, const float &b, const float &c)
{
    float x = 0;
    float discr = b * b - 4 * a * c;

    if (discr > 0){
#ifdef DEBUG2
        cout << "delta > 0" << endl;
#endif
        float x0 = (-b + sqrt(discr))/2*a;
        float x1 = (-b - sqrt(discr))/2*a;
#ifdef DEBUG2
        cout << "solutions" << x0 << " " << x1 << endl;
#endif
        x0 > x1? x = x1 : x = x0;
    }
    return x;
}

//check if a ray intersect with a cycle, return the nearest t
float intersection(const Ray &r, const Sphere &s){
    float t = 0;
    float a = 1;
    Point3f OC = VIEW_POINT - s.center;
    float oc_square = OC.x*OC.x + OC.y*OC.y + OC.z*OC.z;
    float c = oc_square - s.radius*s.radius;
    float b = 2*(r.ray_direction.x*OC.x + r.ray_direction.y*OC.y + r.ray_direction.z*OC.z);
#ifdef DEBUG2
    cout << a  << " " << b << " " << c << endl;
#endif

    t = solveQuadratic(a, b, c);
#ifdef DEBUG2
    cout << "t in intersect function:" << t << endl;
#endif
//    if (t && t < origin_t){
//        p = r.origin + t*r.ray_direction;
//    }
    return t;
}

int main(int argc, char** argv){
    Mat image(PIXEL_NUM,PIXEL_NUM, CV_8UC3, Scalar(0,0,0));

    //Implicitly define three spheres
    //Three spheres are completely inside the view frustum
    //The rendering image would show that
    Sphere s[SPHERE_NUM];

    s[0].center = {0,0,2};
    s[0].radius = 1;

    s[1].center = {0.5, 0.5, 3};
    s[1].radius = 1.2;

    s[2].center = {-1, 1, 5};
    s[2].radius = 2;

#ifdef DEBUG2
    Ray r;
    r.origin = VIEW_POINT;
    r.ray_direction = {0,0,1};
    Point3f p;

    float t = intersection(r, s[0], p);
    cout << t << p << endl;

    Point p_test(2,2);
    Ray r_test;
    r_test = compute_ray(p_test);
    cout << r_test.origin << r_test.ray_direction << endl;
    t = intersection(r_test, s[0], p);
    cout << t << p << endl;
#endif

    for(int i = 0; i < PIXEL_NUM; i++){
        for(int j =0; j < PIXEL_NUM; j++){
            Point p(i,j);
            Ray r = compute_ray(p);
#ifdef DEBUG2
            cout << "ray:" << r.ray_direction << endl;
#endif
            float t = F_FAR;
            int nearest = -1;
            Point3f inter_p;
            for(int k = 0; k < SPHERE_NUM; k++){
                float inter = intersection(r, s[k]);
                if(t > inter && inter > F_NEAR){
#ifdef DEBUG2
                    cout << "t:" << t << " intersect:" << inter << endl;
#endif
                    inter_p = r.origin + inter*r.ray_direction;
                    t = inter;
                    nearest = k;
#ifdef DEBUG2
                    cout <<p << " t:" << t << endl;
#endif
                }
            }
#ifdef DEBUG2
            cout << p << nearest << endl;
#endif
            if(nearest != -1){
                int s = (int)((10 - inter_p.z)*25.5);
                unsigned char shader = (unsigned char) s;
                Vec3b color = image.at<Vec3b>(p);
                color[2 - nearest] = shader;
                image.at<Vec3b>(p) = color;
            }

        }
    }

#ifdef DEBUG
    cout << image << endl;
#endif

    imwrite("raster.png",image);
    namedWindow("raster", WINDOW_AUTOSIZE);
    imshow("raster", image);
    waitKey(0);
    return 0;

};