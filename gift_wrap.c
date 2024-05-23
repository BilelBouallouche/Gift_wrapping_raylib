#include <vector>
#include <iostream>
#include "raylib.h"

/*
 * Gives the index of the lowest point in the canvas
 * /!\ the screen is oriented upside down in raylib
 */
int getMostDownPoint(std::vector<Vector2> points)
{
    int j = 0;
    for(int i = 1; i < points.size(); i++)
    {
        if(points[i].y > points[j].y or points[i].y == points[j].y and points[i].x < points[j].x)
        {
            j = i;
        }
    }
    return j;
}

/*
 *  Let's consider 3 points p_i, p_j and p_k among all the others points
 *  We can get the orientation of the point by calculating d = det(p_ip_j, p_ip_k)
 *  /!\ the screen is oriented upside down in raylib, negating the value of d
 *  If d > 0 => we say that the points are oriented clockwise => we return -1
 *  If d < 0 => we say that the points are oriented counter-clockwise => we return 1
 *  If d = 0 => the points are aligned => we return 0
 */
int getOrientation(std::vector<Vector2> points, int i, int j, int k)
{
    float a = points[j].x - points[i].x;
    float b = points[k].x - points[i].x;
    float c = points[j].y - points[i].y;
    float d = points[k].y - points[i].y;

    float det = a*d-b*c;
    if(det < 0)
    {
        return 1;
    }
    if(det > 0)
    {
        return -1;
    }
    return 0;
}

/*
 * Gives the index of the next point to be added to the list of points
 * making up the wrap using the index of the last point added.
 * The new point must be the most outwardly oriented of what will be the inside
 * of the convex polygon to be formed
 */
int nextPointToAddToWrap(std::vector<Vector2> points, int lastPointAddedToWrap)
{
    //we want to make sure that the indices are differents
    int j = 1;
    if(lastPointAddedToWrap == 1)
    {
        j = 0;
    }

    for(int i=0; i < points.size(); i++)
    {
        if(getOrientation(points, lastPointAddedToWrap, j, i) > 0)
        {
            j = i;
        }
    }
    return j;
}

/*
 * Construct the wrap the the Jarvis algorithm
 */
std::vector<Vector2> getConvexJarvis(std::vector<Vector2> points)
{
    std::vector<Vector2> wrap;
    int i = getMostDownPoint(points);
    int j = nextPointToAddToWrap(points, i);
    wrap.push_back(points[i]);
    while(j != i)
    {
        wrap.push_back(points[j]);
        j = nextPointToAddToWrap(points, j);
    }

    return wrap;
}

int main()
{
    InitWindow(850, 450, "Gift wrapping algorithm");
    SetTargetFPS(60);

    Vector2 mousePosition;
    std::vector<Vector2> points;

    bool algo = false;
    std::vector<Vector2> pointsToWrap;
    while(!WindowShouldClose())
    {
        mousePosition = GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !algo)
        {
            points.push_back(mousePosition);
        }
        if(IsKeyDown(KEY_SPACE))
        {
            pointsToWrap.clear();
            points.clear();
            algo = false;
        }
        if(IsKeyDown(KEY_ENTER) && !points.empty())
        {
            algo = true;
            pointsToWrap = getConvexJarvis(points);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for(auto it=points.begin(); it < points.end(); it++)
            {
                DrawCircleV(*it, 5, RED);
            }

            if(algo)
            {
                int i;
                for(i = 0; i < pointsToWrap.size()-1; i++)
                {
                    DrawLineV(pointsToWrap[i], pointsToWrap[i+1], BLUE);
                }
                DrawLineV(pointsToWrap[i], pointsToWrap[0], BLUE);
            }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
