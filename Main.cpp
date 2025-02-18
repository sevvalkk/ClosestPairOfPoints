#include <SDL.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept> 
#include <cctype> 
#include <iostream>
#define SIZE 500
using namespace std;


class Point
{
    double x;
    double y;
public:
    Point(double x, double y) : x(x), y(y) {}
    Point() : x(0.0), y(0.0) {}
    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double newx) { x = newx; }
    void setY(double newy) { y = newy; }
    double calculateDistance(const Point& p1, const Point& p2)
    {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
    ~Point() {}
};



struct Result
{
    Point p1;
    Point p2;
    double distance;

};


class Closest
{
private:
    vector<Point> Points;
public:



    Point parsLine(string line)
    {
        size_t pos = line.find(' ');
        if (pos == string::npos)
        {
            throw invalid_argument("Invalid line format: " + line);
        }
        double x = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        double y = stod(line);

        return Point(x, y);
    }

    void fileLoadingPoint(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error occurred while opening the file!" << endl;
            return;
        }

        string line;

        while (getline(file, line))
        {
            try
            {
                Point point = parsLine(line);
                Points.push_back(point);
            }
            catch (const exception& e)
            {
                cerr << "Error parsing line: " << e.what() << endl;
            }
        }

    }
    void displayPoints()
    {
        for (const Point& p : Points)
        {
            cout << '(' << p.getX() << ", " << p.getY() << ')' << endl;
        }
        cout << endl;
    }

    //Naive
    Result ClosestPairOfPointsNaive(int n)
    {
        Result R;
        if (n < 2)
        {
            cout << "There should be at least 2 points!" << endl;
            R.distance = DBL_MAX;
            R.p1.setX(0);
            R.p1.setY(0);
            R.p2.setX(0);
            R.p2.setY(0);
            return R;
        }

        Point p1, p2;
        double min = Points[0].calculateDistance(Points[0], Points[1]);

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                double dist = Points[i].calculateDistance(Points[i], Points[j]);
                if (dist < min)
                {
                    min = dist;
                    p1 = Points[i];
                    p2 = Points[j];
                }
            }
        }
        
        R.p1.setX(p1.getX());
        R.p1.setY(p1.getY());
        R.p2.setX(p2.getX());
        R.p2.setY(p2.getY());
        R.distance = min;
        return R;
    }

    /////////

    //Divide and Conquer

    void sort(char xy)
    {
        int size = Points.size();
        vector<Point> temp(size);
        MergeSort(Points, 0, size - 1, temp, xy);

    }

    void MergeSort(vector<Point>& point, int low, int high, vector<Point>& temp, char xy)
    {
        if (low >= high)
            return;
        int mid = (high + low) / 2;


        if (xy == 'x')
        {
            MergeSort(point, low, mid, temp, xy);
            MergeSort(point, mid + 1, high, temp, xy);
            MergeX(point, low, mid, mid + 1, high, temp);
        }
        else
        {
            MergeSort(point, low, mid, temp, xy);
            MergeSort(point, mid + 1, high, temp, xy);
            MergeY(point, low, mid, mid + 1, high, temp);
        }
    }

    void MergeX(vector<Point>& point, int alo, int ahi, int blo, int bhi, vector<Point>& temp)
    {
        int i = alo, j = blo, k = alo;

        while (i <= ahi && j <= bhi)
        {
            if (point[i].getX() < point[j].getX())
            {
                temp[k] = point[i];
                i++;
            }
            else if (point[i].getX() == point[j].getX())
            {
                if (point[i].getY() < point[j].getY())
                {
                    temp[k] = point[i];
                    i++;
                }
                else
                {
                    temp[k] = point[j];
                    j++;
                }
            }
            else
            {
                temp[k] = point[j];
                j++;
            }
            k++;
        }

        while (i <= ahi)
        {
            temp[k] = point[i];
            i++;
            k++;
        }

        while (j <= bhi)
        {
            temp[k] = point[j];
            j++;
            k++;
        }

        i = alo;

        while (i <= bhi)
        {
            point[i] = temp[i];
            i++;
        }

    }

    void MergeY(vector<Point>& point, int alo, int ahi, int blo, int bhi, vector<Point>& temp)
    {
        int i = alo, j = blo, k = alo;

        while (i <= ahi && j <= bhi)
        {
            if (point[i].getY() < point[j].getY())
            {
                temp[k] = point[i];
                i++;
            }
            else if (point[i].getY() == point[j].getY())
            {
                if (point[i].getX() < point[j].getX())
                {
                    temp[k] = point[i];
                    i++;
                }
                else
                {
                    temp[k] = point[j];
                    j++;
                }
            }
            else
            {
                temp[k] = point[j];
                j++;
            }
            k++;
        }

        while (i <= ahi)
        {
            temp[k] = point[i];
            i++;
            k++;
        }

        while (j <= bhi)
        {
            temp[k] = point[j];
            j++;
            k++;
        }

        i = alo;

        while (i <= bhi)
        {
            point[i] = temp[i];
            i++;
        }

    }


    Result NaiveforRecursive(vector<Point>& subset, int n, SDL_Renderer* rend)
    {
        Result R;
        if (n < 2)
        {
            cout << "There should be at least 2 points!" << endl;
            R.distance = DBL_MAX; 
			R.p1.setX(0);
			R.p1.setY(0);
			R.p2.setX(0);
			R.p2.setY(0);
            return R;
        }

        Point minP1, minP2;
        double minDist = DBL_MAX;

        // Render all points
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        for (const auto& point : subset)
        {
            int x = static_cast<int>(point.getX() * 9) + 500;
            int y = (1000 - static_cast<int>(point.getY() * 9)) - 500;

            SDL_Rect rect = { x - 1, y - 1, 2, 2 };
            SDL_RenderFillRect(rend, &rect);
        }

        SDL_RenderPresent(rend);

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                // Calculate distance
                double dist = subset[i].calculateDistance(subset[j], subset[i]);

                if (dist < minDist)
                {
                    minDist = dist;
                    minP1 = subset[i];
                    minP2 = subset[j];

                    SDL_SetRenderDrawColor(rend, 255, 255, 0, 255); // Green line for distance
                    int x1 = static_cast<int>(minP1.getX() * 9) + 500;
                    int y1 = (1000 - static_cast<int>(minP1.getY() * 9)) - 500;
                    int x2 = static_cast<int>(minP2.getX() * 9) + 500;
                    int y2 = (1000 - static_cast<int>(minP2.getY() * 9)) - 500;

                    SDL_RenderDrawLine(rend, x1, y1, x2, y2);
                    SDL_RenderPresent(rend);
                    SDL_Delay(150); // Delay for visualization
                }
            }
        }

        R.p1 = minP1;
        R.p2 = minP2;
        R.distance = minDist;
        return R;
    }


    Result ClosestPairOfPointsRec(vector<Point>& P, int n, int low, int high, SDL_Window* win, SDL_Renderer* rend)
    {
        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

        int mid = low + (high - low) / 2;
        Point midP = P[mid];

        if (n <= 3)
            return NaiveforRecursive(P, n, rend);

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  // Set background to black
        SDL_RenderClear(rend);  // Clear the screen
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);  // white for grid
        SDL_RenderDrawLine(rend, 0, 500, 1000, 500);  // horizontal axis
        SDL_RenderDrawLine(rend, 500, 0, 500, 1000);  // vertical axis

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // White for points
        for (const auto& point : Points)
        {
            int X = static_cast<int>(point.getX() * 9) + 500;
            int Y = (1000 - static_cast<int>(point.getY() * 9)) - 500;

            SDL_Rect rect;
            rect.x = X - 1;
            rect.y = Y - 1;
            rect.w = 2;
            rect.h = 2;
            SDL_RenderFillRect(rend, &rect);
        }

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // Red for division line
        int midX = static_cast<int>(midP.getX() * 9) + 500;
        SDL_RenderDrawLine(rend, midX, 0, midX, 1000);

        SDL_RenderPresent(rend); // Update rendering
        //SDL_Delay(150); // Delay for visualization

        vector<Point> leftSubset(P.begin() + low, P.begin() + mid);
        vector<Point> rightSubset(P.begin() + mid, P.begin() + high);

        Result left = ClosestPairOfPointsRec(leftSubset, leftSubset.size(), 0, leftSubset.size(), win, rend);
        Result right = ClosestPairOfPointsRec(rightSubset, rightSubset.size(), 0, rightSubset.size(), win, rend);

        double dist = min(left.distance, right.distance);

        int stripLeft = static_cast<int>((midP.getX() - dist) * 9) + 500;
        int stripRight = static_cast<int>((midP.getX() + dist) * 9) + 500;


        vector<Point> strip(n);
        int i = 0; // Num of points in the strip

        for (int j = low; j < high; j++)
        {
            if (abs(P[j].getX() - midP.getX()) < dist)
            {
                strip[i].setY(P[j].getY());
                strip[i].setX(P[j].getX());
                i++;
            }
        }

        int leftX = midX - dist;
        int rightX = midX + dist;
        int rectWidth = rightX - leftX;

        SDL_Rect rect = { leftX, 0, rectWidth, 1000 }; //strip area
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 200); // Semi-transparent white
        SDL_RenderFillRect(rend, &rect);
        SDL_RenderPresent(rend);

        // Draw the strip points
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // Red for strip points
        for (const auto& stripPoint : strip)
        {
            int stripX = static_cast<int>(stripPoint.getX() * 9) + 500;
            int stripY = (1000 - static_cast<int>(stripPoint.getY() * 9)) - 500;

            SDL_Rect stripRect = { stripX - 1, stripY - 1, 2, 2 };
            SDL_RenderFillRect(rend, &stripRect);
        }


        SDL_RenderPresent(rend); // Update rendering
        //SDL_Delay(150); // Delay for visualization

        Result t = stripPoints(strip, i, dist, win, rend);
        dist = min(dist, t.distance);
        if (dist == left.distance)
            return left;
        if (dist == right.distance)
            return right;

        Result R;
        R.distance = dist;
        R.p1.setX(t.p1.getX());
        R.p1.setY(t.p1.getY());
        R.p2.setX(t.p2.getX());
        R.p2.setY(t.p2.getY());

        return R;

    }




    Result stripPoints(vector<Point> strip, int size, double distance, SDL_Window* win, SDL_Renderer* rend)
    {

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);  // white for grid
        SDL_RenderDrawLine(rend, 0, 500, 1000, 500);  // horizontal axis
        SDL_RenderDrawLine(rend, 500, 0, 500, 1000);  // vertical axis
        Point point1, point2;
        double dist = distance;
        vector<Point> temp(size);
        MergeSort(strip, 0, size - 1, temp, 'y');
        for (int i = 0; i < size; i++)
        {
            for (int j = i + 1; j < size && (strip[j].getY() - strip[i].getY()) < dist; j++)
            {
                dist = strip[i].calculateDistance(strip[i], strip[j]);
                if (dist < distance)
                {
                    distance = dist;
                    point1.setX(strip[i].getX());
                    point1.setY(strip[i].getY());
                    point2.setX(strip[j].getX());
                    point2.setY(strip[j].getY());

                }

                SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // White for points
                for (const auto& point : Points)
                {
                    int X = static_cast<int>(point.getX() * 9) + 500;
                    int Y = (1000 - static_cast<int>(point.getY() * 9)) - 500;

                    SDL_Rect rect;
                    rect.x = X - 1;
                    rect.y = Y - 1;
                    rect.w = 2;
                    rect.h = 2;
                    SDL_RenderFillRect(rend, &rect);
                }
                SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); // Green for closest pair
                int X1 = static_cast<int>(point1.getX() * 9) + 500;
                int Y1 = (1000 - static_cast<int>(point1.getY() * 9)) - 500;
                int X2 = static_cast<int>(point2.getX() * 9) + 500;
                int Y2 = (1000 - static_cast<int>(point2.getY() * 9)) - 500;

                SDL_RenderDrawLine(rend, X1, Y1, X2, Y2);
                SDL_RenderPresent(rend);
            }
        }
        Result R;
        R.distance = distance;
        R.p1.setX(point1.getX());
        R.p1.setY(point1.getY());
        R.p2.setX(point2.getX());
        R.p2.setY(point2.getY());

        return R;
    }




    void functionRecursive(int size)
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            cout << "Error initializing SDL: " << SDL_GetError() << endl;
            return;
        }

        // Create a window
        SDL_Window* win = SDL_CreateWindow("Closest Pair Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

        if (!win)
        {
            cout << "Error creating window: " << SDL_GetError() << endl;
            SDL_Quit();
            return;
        }

        // Create a renderer
        SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (!rend)
        {
            cout << "Error creating renderer: " << SDL_GetError() << endl;
            SDL_DestroyWindow(win);
            SDL_Quit();
            return;
        }

        // Load points from file

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  // Set background to black
        SDL_RenderClear(rend);  // Clear the screen
        Recursive(Points.size(), *win, *rend);

        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }



    Result functionNaive(int size)
    {
        Result temp;
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            cout << "Error initializing SDL: " << SDL_GetError() << endl;
            return temp;
        }

        // Create a window
        SDL_Window* win = SDL_CreateWindow("Closest Pair Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

        if (!win)
        {
            cout << "Error creating window: " << SDL_GetError() << endl;
            SDL_Quit();
            return temp;
        }

        // Create a renderer
        SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (!rend)
        {
            cout << "Error creating renderer: " << SDL_GetError() << endl;
            SDL_DestroyWindow(win);
            SDL_Quit();
            return temp;
        }

        // Load points from file
        bool close = false;
        Result R;
        R = Naive(size);
        cout << "Closest pair of points: (" << R.p1.getX() << ", " << R.p1.getY() << ") and (" << R.p2.getX() << ", " << R.p2.getY() << ")" << endl;
        cout << "Minimum distance: " << R.distance << endl;
        while (!close)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    close = true;
                }
            }

            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  // Set background to black
            SDL_RenderClear(rend);  // Clear the screen

            Point p1, p2;
            Point minp1, minp2;
            double min = Points[0].calculateDistance(Points[0], Points[1]);

            for (int i = 0; i < size; i++)
            {
                for (int j = i + 1; j < size; j++)
                {
                    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  // set background to black
                    SDL_RenderClear(rend);

                    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);  // white for grid
                    SDL_RenderDrawLine(rend, 0, 500, 1000, 500);  // horizontal axis
                    SDL_RenderDrawLine(rend, 500, 0, 500, 1000);  // vertical axis

                    SDL_SetRenderDrawColor(rend, 225, 225, 225, 255); // points

                    for (int i = 0; i < Points.size(); i++)
                    {
                        int X = static_cast<int>(Points[i].getX() * 9);
                        int Y = static_cast<int>(Points[i].getY() * 9);
                        X = X + 500;
                        Y = (1000 - Y) - 500;
                        SDL_Rect rect;
                        rect.x = X - 1;
                        rect.y = Y - 1;
                        rect.w = 2;
                        rect.h = 2;
                        SDL_RenderFillRect(rend, &rect);

                    }

                    p1 = Points[i];
                    p2 = Points[j];
                    double dist = sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
                    if (dist < min)
                    {
                        min = dist;
                        minp1 = Points[i];
                        minp2 = Points[j];
                    }


                    int X1 = static_cast<int>(p1.getX() * 9) + 500;
                    int Y1 = (1000 - static_cast<int>(p1.getY() * 9)) - 500;
                    int X2 = static_cast<int>(p2.getX() * 9) + 500;
                    int Y2 = (1000 - static_cast<int>(p2.getY() * 9)) - 500;

                    int mX1 = static_cast<int>(minp1.getX() * 9) + 500;
                    int mY1 = (1000 - static_cast<int>(minp1.getY() * 9)) - 500;
                    int mX2 = static_cast<int>(minp2.getX() * 9) + 500;
                    int mY2 = (1000 - static_cast<int>(minp2.getY() * 9)) - 500;

                    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // distance
                    SDL_RenderDrawLine(rend, X1, Y1, X2, Y2);
                    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
                    SDL_RenderDrawLine(rend, mX1, mY1, mX2, mY2);
                    SDL_RenderPresent(rend);


                }
            }
            close = true;
            SDL_RenderPresent(rend);
            SDL_Delay(1000 / 60);
        }

        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return R;
    }

    Result Naive(int size)
    {
        //sort('x');
        return ClosestPairOfPointsNaive(size);
    }

    void Recursive(const int size, SDL_Window& win, SDL_Renderer& rend)
    {
        sort('x');

        Result R = ClosestPairOfPointsRec(Points, size, 0, size - 1, &win, &rend);
        cout << "Closest pair of points: (" << R.p1.getX() << ", " << R.p1.getY() << ") and (" << R.p2.getX() << ", " << R.p2.getY() << ")" << endl;
        cout << "Minimum distance: " << R.distance << endl;
    }

};




int main(int argc, char* argv[]) {
    Closest closest;
    closest.fileLoadingPoint("points.txt");

    cout << endl << "Naive: " << endl;
    Result R2 = closest.functionNaive(SIZE);
    cout << "Closest pair of points: (" << R2.p1.getX() << ", " << R2.p1.getY() << ") and (" << R2.p2.getX() << ", " << R2.p2.getY() << ")" << endl;
    cout << "Minimum distance: " << R2.distance << endl;

    cout << "Divide and Conquer: " << endl;
    closest.functionRecursive(SIZE);

    return 0;
}