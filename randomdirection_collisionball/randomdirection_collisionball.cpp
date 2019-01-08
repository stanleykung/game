#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define w 400
#define ball_radius w/20
using namespace cv;

class Ball{
protected:
    Point center;
    Scalar color;
    float radius;
    float mass;
public:
    void create_ball(Mat img,Point start_point, float r, Scalar c){
        center = start_point;
        color = c;
        radius = r;
        circle(img,center,radius,color,FILLED,LINE_8 );
    }
    Point get_center(){ return center;}
    float get_radius(){ return radius;}
    float cal_mas(){ return mass = M_PI * 4 * pow(radius,3) / 3; }
};

class MovingBall: public Ball{
    Point vec;
public:
    int hit(){ return ( this->get_center().x <= 0 + this->get_radius() || 
                        this->get_center().y <= 0 + this->get_radius() ||
                        this->get_center().x >= w - this->get_radius() ||
                        this->get_center().y >= w - this->get_radius());}
    int hit2(){return ( this->get_center().x <  0 + this->get_radius() || 
                        this->get_center().y <  0 + this->get_radius() ||
                        this->get_center().x >  w - this->get_radius() ||
                        this->get_center().y >  w - this->get_radius());}
    void set_vec(Point v){ vec = v; }
    Point get_vec(){ return vec; }
    void bound(){
        if(this->get_center().x <= 0 + this->get_radius() || this->get_center().x >= w - this->get_radius())
            this->set_vec(Point(-vec.x,vec.y));
        if(this->get_center().y <= 0 + this->get_radius() || this->get_center().y >= w - this->get_radius())
            this->set_vec(Point(vec.x,-vec.y));
    }
    void move_ball(Mat img){
        //remove the original ball
        circle(img,this->get_center(),this->get_radius(),Scalar( 0, 0, 0 ),FILLED,LINE_8 );
        //draw the new moved ball
        center+=vec;
        if(this->hit2()){// test this move (update of position) will hit the boundary or not
            if(this->get_center().x <= 0 + this->get_radius())
                center.x = -center.x;
            if(this->get_center().x >= w - this->get_radius())
                center.x = w - (center.x-w);
            if(this->get_center().y <= 0 + this->get_radius())
                center.y = -center.y;
            if(this->get_center().x >= w - this->get_radius())
                center.y = w - (center.y-w);
        }
        circle(img,center,this->get_radius(),color,FILLED,LINE_8 );
    }
};

class collision_detection: public Ball{
    float distant;
public:
    float cal_distant(Ball b1, Ball b2){
        return distant = sqrt(  (b1.get_center().x-b2.get_center().x)*(b1.get_center().x-b2.get_center().x)+
                                (b1.get_center().y-b2.get_center().y)*(b1.get_center().y-b2.get_center().y));
    }
    float cal_distant(MovingBall b1, MovingBall b2){
        return distant = sqrt(  (b1.get_center().x-b2.get_center().x)*(b1.get_center().x-b2.get_center().x)+
                                (b1.get_center().y-b2.get_center().y)*(b1.get_center().y-b2.get_center().y));
    }
    int collision(MovingBall b1, MovingBall b2){
        return (b1.get_radius()+b2.get_radius() >= cal_distant(b1,b2));
    }
};

class elastic_collision: public collision_detection{
    Point new_vec1;
    Point new_vec2;
public:
    Point get_new_vec1(){ return new_vec1; }
    Point get_new_vec2(){ return new_vec2; }
    void cal_new_vec_aftercollision(MovingBall b1, MovingBall b2){
        //new_vec1
        float temp = ( b2.cal_mas() * 2 / (b1.cal_mas()+b2.cal_mas()) ) * ( (b1.get_vec()-b2.get_vec()).dot(b1.get_center()-b2.get_center())  / pow(cal_distant(b1,b2),2) );
        Point p = temp * (b1.get_center() - b2.get_center());
        new_vec1 = b1.get_vec() - p;
        //new_vec2
        temp = ( b1.cal_mas() * 2 / (b1.cal_mas()+b2.cal_mas()) ) * ( (b2.get_vec()-b1.get_vec()).dot(b2.get_center()-b1.get_center())  / pow(cal_distant(b1,b2),2) );
        p = temp * (b2.get_center() - b1.get_center());
        new_vec2 = b2.get_vec() - p;
    }
};

int main(){
    const char * window = "bounding ball";
    Mat image = Mat::zeros( w, w, CV_8UC3 );
    MovingBall mb,mb2;
    mb.create_ball(image,Point(w/2,w/2),ball_radius,Scalar( 255, 0, 255 ));
    mb2.create_ball(image,Point(w/2+40,w/2+40),ball_radius,Scalar( 255, 0, 0 ));
    imshow( window, image );
    waitKey( 20 );

    mb.set_vec(Point(3,4));
    mb2.set_vec(Point(5,12));
    int count = 0;
    for(int i = 0; i<1000; i++){
        if(mb.hit()==1)
            mb.bound();
        if(mb2.hit()==1)
            mb2.bound();
        collision_detection detecter;
        if(detecter.collision(mb,mb2)==1){
            count++;
            std::cout << count << " collision!\n";
            elastic_collision ec;
            ec.cal_new_vec_aftercollision(mb,mb2);
            mb.set_vec(ec.get_new_vec1());
            mb2.set_vec(ec.get_new_vec2());
        }
        mb.move_ball(image);
        mb2.move_ball(image);
        imshow( window, image );
        waitKey( 200 );
    }

    return(0);
}