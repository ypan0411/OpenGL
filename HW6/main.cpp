#include <fstream>
#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

#define DEBUG

int PIXEL_NUM= 50;
bool anti_aliasing = true;
int anti_num = 2;
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

//each ray represented as O + tD
struct Ray{
    Point3f origin;
    Point3f ray_direction;
};

//use reference to save space
struct Mesh{
    Point3f a;
    Point3f b;
    Point3f c;
};

//Normalize a  vector
Point3f normalize(const Point3f &p){
    float square = p.x*p.x + p.y*p.y + p.z*p.z;
    float length = sqrt(square);
    Point3f v(p.x/length, p.y/length,p.z/length);
    return v;
}

//for given screen coordinate, compute the ray
Ray compute_ray(const Point2f &p, float step){
    Ray r;
    r.origin = VIEW_POINT;
    Point3f width = R_UPPER - L_UPPER;
    Point3f height = R_UPPER - R_LOWER;

    float p_width = width.x / PIXEL_NUM;
    float p_height = height.y / PIXEL_NUM;

    //compute the center coordinate of the pixel
    float center_x = L_UPPER.x + (p.x + 0.5f * step) *p_width;
    float center_y = L_UPPER.y - (p.y + 0.5f * step) *p_height;

    //get the normalized ray_direction
    Point3f pixel_center(center_x, center_y, 0);
    r.ray_direction  = normalize(pixel_center - VIEW_POINT);

    return r;
}

bool loadOBJ(const char * path, vector<Mesh>& out_mesh){
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        cout << "Impossible to open the file !" << endl;
        return false;
    }
    vector<Point3f> vertices;
    while(1) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0) {
            Point3f vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertex.z += 4;
            vertex.y -= 1;
            vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "f") == 0) {
            unsigned int index1, index2, index3, n1, n2, n3;
            fscanf(file, "%d//%d %d//%d %d//%d\n", &index1, &n1, &index2, &n2, &index3, &n3);
            Mesh mesh;
            mesh.a = vertices[index1-1];
            mesh.b = vertices[index2-1];
            mesh.c = vertices[index3-1];
            out_mesh.push_back(mesh);
        }
    }
    return true;
}

float intersection( const Ray& R, const Mesh& M ) {
    Point3f v0 = M.b - M.a;
    Point3f v1 = M.c - M.a;

    //compute normal
    Point3f n = v1.cross(v0);

    Point3f u = R.origin - M.a;
    float t = -n.dot(u) / n.dot(R.ray_direction);
    
    //Intersection point
    Point3f I = R.origin + t * R.ray_direction;

    // is I inside the mesh?
    float D = v1.dot(v0) * v1.dot(v0) - v0.dot(v0) * v1.dot(v1);
    Point3f w = I - M.a;
    float s, r;
    s = (v1.dot(v0) * w.dot(v0) - v0.dot(v0) * w.dot(v1)) / D;
    if(s < 0.0 || s > 1.0){
        return 0;
    }
    r = (v1.dot(v0) * w.dot(v1) - v1.dot(v1) * w.dot(v0)) / D;
    if(r <  0.0 || (s+r) > 1.0){
        return 0;
    }
    return t;
}

int main(int argc, char** argv){
    Mat image(PIXEL_NUM,PIXEL_NUM, CV_32FC3, Scalar(0,0,0));

    float step = 1;
    vector<Mesh> out_mesh;
    if(!loadOBJ("bunny.obj", out_mesh)){
        cout << "Error loading object." << endl;
    }

    for(int i = 0; i < PIXEL_NUM; i++){
        for(int j =0; j < PIXEL_NUM; j++){
            float t = F_FAR;
            int nearest = -1;
            Point3f inter_p;
            if(anti_aliasing){
                step = (float) 1 / anti_num;
                Point2f p(i,j);
                int count = 0;
                for(int m = 0; m < anti_num; m++){
                    for(int n = 0; n < anti_num; n++){
                        Point2f sub_p(i + step*m, j + step*n);
                        Ray r = compute_ray(sub_p, step);
                        for(int k = 0; k < out_mesh.size(); k++){
                            float inter = intersection(r, out_mesh[k]);
                            if(t > inter && inter > F_NEAR){
                                inter_p = r.origin + inter*r.ray_direction;
                                t = inter;
                                nearest = k;
                            }
                        }
                        if(nearest != -1) count++;
                    }
                }
                float ratio = float(count) / (anti_num*anti_num);
                if(ratio > 0){
//                    cout << ratio << endl;
                    float B = (10 - inter_p.z) * 25.5f * ratio;
                    float G = (6 + inter_p.y) * 250 / 12 * ratio;
                    float R = (6 + inter_p.x) * 250 / 12 * ratio;
                    Vec3f color = image.at<Vec3f>(p);
                    color[0] = B;
                    color[1] = G;
                    color[2] = R;
                    image.at<Vec3f>(p) = color;
                }
            }
            else{
                Point2f p(i,j);
                Ray r = compute_ray(p, step);
                for(int k = 0; k < out_mesh.size(); k++){
                    float inter = intersection(r, out_mesh[k]);
                    if(t > inter && inter > F_NEAR){
                        inter_p = r.origin + inter*r.ray_direction;
                        t = inter;
                        nearest = k;
                    }
                }
                if(nearest != -1){
                    float B = (10 - inter_p.z) * 25.5f;
                    float G = (6 + inter_p.y) * 250 / 12;
                    float R = (6 + inter_p.x) * 250 / 12;
                    Vec3f color = image.at<Vec3f>(p);
                    color[0] = B;
                    color[1] = G;
                    color[2] = R;
                    image.at<Vec3f>(p) = color;
                }
            }
        }
    }

    cout << "Done" << endl;

    imwrite("raster.png",image);
    namedWindow("raster", WINDOW_AUTOSIZE);
//    imshow("raster", image);
    waitKey(0);
    return 0;

};
