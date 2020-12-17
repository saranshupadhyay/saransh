 <imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <stack>

#define N 4
#define M 5

using namespace cv;
using namespace std;

int **mt = {0};

class node {
public:
    int x, y;
    int dir;

    node(int i, int j)
    {
        x = i;
        y = j;
        dir = 0;
    }
};


int n = N, m = M;

int fx, fy;
bool visited[N][M];

vector<Mat> imageToMatrix(Mat src){

    Mat bw;
    cvtColor(src, bw, cv::COLOR_BGR2GRAY);
    threshold(bw, bw, 10, 255, cv::THRESH_BINARY_INV);

    vector<vector<cv::Point> > contours;
    findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    if (contours.size() != 2)
    {
        cout << "MAZE NOT DETECTED....CHECK THE IMAGE!" << endl;
        getch();
        exit(-1);
    }

    Mat path = Mat::zeros(src.size(), CV_8UC1);
    drawContours(path, contours, 0, CV_RGB(255,255,255), cv::FILLED);

    Mat kernel = Mat::ones(21, 21, CV_8UC1);
    dilate(path, path, kernel);

    Mat path_erode;
    erode(path, path_erode, kernel);

    absdiff(path, path_erode, path);

    vector<Mat> channels;
    split(src, channels);
    channels[0] &= ~path;
    channels[1] &= ~path;
    channels[2] |= path;

    return channels;

}

bool solveMaze(vector<Mat> mt , int **maze){

    cout<<"\n(+) SOLVING\n";

    int i = 0, j = 0;

    stack<node> s;

    node temp(i, j);

    s.push(temp);

    while (!s.empty()) {

        temp = s.top();
        int d = temp.dir;
        i = temp.x, j = temp.y;

        temp.dir++;
        s.pop();
        s.push(temp);


        if (i == fx and j == fy) {
            return true;
        }


        if (d == 0) {
            if (i - 1 >= 0 and maze[i - 1][j] and
                                    visited[i - 1][j]) {
                node temp1(i - 1, j);
                visited[i - 1][j] = false;
                s.push(temp1);
            }
        }


        else if (d == 1) {
            if (j - 1 >= 0 and maze[i][j - 1] and
                                    visited[i][j - 1]) {
                node temp1(i, j - 1);
                visited[i][j - 1] = false;
                s.push(temp1);
            }
        }

        else if (d == 2) {
            if (i + 1 < n and maze[i + 1][j] and
                                    visited[i + 1][j]) {
                node temp1(i + 1, j);
                visited[i + 1][j] = false;
                s.push(temp1);
            }
        }

        else if (d == 3) {
            if (j + 1 < m and maze[i][j + 1] and
                                    visited[i][j + 1]) {
                node temp1(i, j + 1);
                visited[i][j + 1] = false;
                s.push(temp1);
            }
        }

        else {
            visited[temp.x][temp.y] = true;
            s.pop();
        }
    }

    return false;

}

void MazeSolver(string imagepath){

    Mat src = imread(imagepath);
    if( !src.data ) {
        cout<<"ERROR LOADING SOURCE....";
        getch();
        exit(-1);
    }

    cout<<"(+) SHOWING INPUT MAZE.>>>>>"<<endl;

    imshow("Input Maze" , src);
    //waitKey(0);
    vector<Mat> maze = imageToMatrix(src);

    solveMaze(maze , mt);

    Mat dst;
    merge(maze, dst);
    cout<<"(+) SHOWING MAZE SOLUTION>>>>>>"<<endl;
    imshow("solution", dst);
    waitKey(0);


}

int main()
{
    string imagerelpath;
    cout<<"+++++++++++++++++++++++MAZE SOVLER++++++++++++++++++++++\n";
    cout<<"To note:- The maze image must be inside the mazes folder\n";
    cout<<"ENTER MAZE IMAGE NAME :-    ";
    getline(cin , imagerelpath);
    string locpath = "C:\\Users\\ajax9\\OneDrive\\Desktop\\Maze Solver\\mazes\\";
    string imagepath = locpath + imagerelpath;
    MazeSolver(imagepath);
    return 0;
}
