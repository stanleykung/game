#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define w 400
#define ball_radius w/32
using namespace cv;

class Ball{
protected:
    Point center;
    Scalar color;
public:
    void create_ball(Mat img,Point start_point, Scalar c){
        center = start_point;
        color = c;
        circle(img,center,ball_radius,color,FILLED,LINE_8 );
    }
    Point get_center(){ return center;}
};

class MovingBall: public Ball{
    Point vec;
public:
    int hit(){ return ( this->get_center().x <= 0 + ball_radius || 
                        this->get_center().y <= 0 + ball_radius ||
                        this->get_center().x >= w - ball_radius ||
                        this->get_center().y >= w - ball_radius);}
    void set_vec(Point v){ vec = v; }
    void bound(){
        if(this->get_center().x <= 0 + ball_radius || this->get_center().x >= w - ball_radius)
            this->set_vec(Point(-vec.x,vec.y));
        if(this->get_center().y <= 0 + ball_radius || this->get_center().y >= w - ball_radius)
            this->set_vec(Point(vec.x,-vec.y));
    }
    void move_ball(Mat img){
        //remove the original ball
        circle(img,this->get_center(),ball_radius,Scalar( 0, 0, 0 ),FILLED,LINE_8 );
        //draw the new moved ball
        center+=vec;
        circle(img,center,ball_radius,color,FILLED,LINE_8 );
    }
};

int main(){
    const char * window = "bounding ball";
    /*int flag = 0;
    while(1){
    if(flag==0){
        for(int i = 0 + w/32; i <= w-(w/32);i++){
            Mat image = Mat::zeros( w, w, CV_8UC3 );
            //circle(image,Point(w/2,w/2),w/32,Scalar( 0, 0, 255 ),FILLED,LINE_8 );
        
            Ball ball;
            ball.create_ball(image,Point(i,w/2));
            imshow( window, image );
            waitKey( 10 );
            if(i == w-(w/32)) flag=1;
        } 
    }
    else{
        for(int i = w-(w/32); i >= 0 + w/32;i--){
            Mat image = Mat::zeros( w, w, CV_8UC3 );
            Ball ball;
            ball.create_ball(image,Point(i,w/2));
            imshow( window, image );
            waitKey( 10 );
            if(i == 0 + w/32) flag=0;   
        }
    }
    }*/
    Mat image = Mat::zeros( w, w, CV_8UC3 );
    MovingBall mb,mb2;
    mb.create_ball(image,Point(w/2,w/2),Scalar( 255, 0, 255 ));
    mb2.create_ball(image,Point(w/2,w/2),Scalar( 255, 0, 0 ));
    imshow( window, image );
    waitKey( 20 );

    mb.set_vec(Point(3,4));
    mb2.set_vec(Point(5,12));
    for(int i = 0; i<1000; i++){
        if(mb.hit()==1)
            mb.bound();
        mb.move_ball(image);
        if(mb2.hit()==1)
            mb2.bound();
        mb2.move_ball(image);
        imshow( window, image );
        waitKey( 20 );
    }

    return(0);
}