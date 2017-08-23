#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>
#include <cmath>
#include <stack>

using namespace std;

double distance(vector<double> &a, vector<double> b);

double cross_product(vector<double> &p1, vector<double> &p2, vector<double> &p3);

bool comp1(const vector<double> &a, vector<double> &b);

bool comp2(const vector<double> &a, vector<double> &b);

void Graham_scan(vector<vector<double>> &points, vector<vector<double>> &convex, vector<vector<double>> &non_convex);

int main() {
//    int point_num;
//    cin>>point_num;
//    vector<vector<double>> coor(point_num, vector<double>(2));
//    for(int i=0; i<point_num; i++){
//        int x, y;
//        cin>>x>>y;
//        coor[i][0]=x;
//        coor[i][1]=y;
//    }
    vector<vector<double>> coor={{0,0},{1,-1},{2,-2},{2,0},{4,0},{1,1},{2,2},{3,0.5},{1,-0.5},{3,-1},{2.5,-1.5}};
    vector<vector<double>> convex;
    vector<vector<double>> non_convex;
    Graham_scan(coor,convex, non_convex);
    return 0;
}

bool comp1(const vector<double> &a, vector<double> &b){
    return a[0]<b[0]||(a[0]==b[0]&&a[1]<b[1]);
}

bool comp2(const vector<double> &a, vector<double> &b){
    return a[1]<b[1]||(a[1]==b[1]&&a[0]<b[0]);
}

double distance(vector<double> &a, vector<double> b){
    double tmp=pow(a[0]-b[0],2)+pow(a[1]-b[1],2);
    return sqrt(tmp);
}
double cross_product(vector<double> &p1, vector<double> &p2, vector<double> &p3){
    double x1=p2[0]-p1[0], x2=p3[0]-p1[0], y1=p2[1]-p1[1], y2=p3[1]-p1[1];
    return x1*y2-x2*y1;

}
void Graham_scan(vector<vector<double>> &points, vector<vector<double>> &convex, vector<vector<double>> &non_convex) {
    int point_num = points.size();
    sort(points.begin(), points.end(), comp2);
    vector<double> init=points[0];
    vector<vector<double>> attr(point_num-1, vector<double>(3,0));
    for(int i=1; i<points.size(); i++){
        double degree = atan((points[i][1]-init[1])/(points[i][0]-init[0]));
        double pi = 4*atan(1);
        degree = degree>0? degree:degree+pi;
        double dis;
        dis = distance(points[i], init);
        attr[i-1][0]=degree/pi*180;
        attr[i-1][1]=dis;
        attr[i-1][2]=i;
    }
    sort(attr.begin(),attr.end(), comp1);
    //vector<vector<double>> non_convex;
    vector<vector<double>> convex_tmp;

    for(int i=1; i<attr.size(); i++){
        if(attr[i][0]==attr[i-1][0]){
            non_convex.push_back(points[attr[i-1][2]]);
        }
    }

    convex_tmp.push_back(points[attr[attr.size()-1][2]]);

    for(int i=attr.size()-1; i>=1; i--){
        if(attr[i][0]!=attr[i-1][0])
            convex_tmp.push_back(points[attr[i-1][2]]);
    }
    convex_tmp.push_back(init);
    reverse(convex_tmp.begin(),convex_tmp.end());

    /*
    //不把一条射线上距离近的点删去，可以求得正确的凸点集，但是复杂度会增加。
    convex_tmp.push_back(init);
    for(int i=0; i<attr.size(); i++){
        convex_tmp.push_back(points[attr[i][2]]);
    }
    */
    //vector<vector<double>> convex;
    convex.push_back(convex_tmp[0]);
    convex.push_back(convex_tmp[1]);
    convex.push_back(convex_tmp[2]);
    int top=2;
    for(int i = 3; i<convex_tmp.size(); i++){
        while(cross_product(convex[top-1],convex[top],convex_tmp[i])<0){
            non_convex.push_back(*(convex.begin()+top));
            convex.erase(convex.begin()+top);
            top--;
        }
        convex.push_back(convex_tmp[i]);
        top++;
    }
    // 把最小和最大角所在射线上的所有点都归为凸集点
    double min_angle=attr[0][0], max_angle = attr[attr.size()-1][0];
    double min_angle_max_distance = distance(convex[0],convex[1]);
    int count =0;
    for(int i=attr.size()-1; i>=0; i--){
        if(attr[i][0]==max_angle){
            if(count==0)
                count=1;
            else
                convex.push_back(points[attr[i][2]]);
        }
    }
    for(int i=0; i<attr.size(); i++){
        if(attr[i][0]==min_angle&&attr[i][1]!=min_angle_max_distance){
            convex.insert(convex.begin()+count,points[attr[i][2]]);
            count++;
        }
    }
    for(int i=0; i<non_convex.size(); i++){
        double pi = 4*atan(1);
        double degree = atan((non_convex[i][1]-init[1])/(non_convex[i][0]-init[0]));
        degree=degree>0? degree/pi*180:(degree+pi)/pi*180;
        if(degree==max_angle||degree==min_angle){
            non_convex.erase(non_convex.begin()+i);
            i--;
        }
    }
}
}