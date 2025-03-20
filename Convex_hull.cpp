#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

vector<Point> inputPoints() {
    vector<Point> points;
    int x, y;
    while (cin >> x >> y) {
        points.push_back({x, y});
    }
    return points;
}

Point findLeftBottomPoint(vector<Point>& points) {
    Point firstPoint = points[0];

    for (const Point& p : points) {
        if (p.x < firstPoint.x || (p.x == firstPoint.x && p.y < firstPoint.y)) {
            firstPoint = p;
        }
    }
    return firstPoint;
}

int crossProduct(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool isValidEdge(Point p, Point q, vector<Point>& points) {
    for (const Point& r : points) {
        if (crossProduct(p, q, r) > 0) {
            return false;
        }
    }
    return true;
}

vector<Point> constructConvexHull(vector<Point>& points) {
    vector<Point> hull;
    Point start = findLeftBottomPoint(points);
    hull.push_back(start);

    while (true) {
        Point nextPoint;
        bool found = false;

        for (const Point& candidate : points) {
            if (candidate.x == hull.back().x && candidate.y == hull.back().y) continue;

            if (!found || isValidEdge(hull.back(), candidate, points)) {
                nextPoint = candidate;
                found = true;
            }
        }

        if (nextPoint.x == start.x && nextPoint.y == start.y) break;

        hull.push_back(nextPoint);
    }
    return hull;
}

int main() {
    vector<Point> points = inputPoints();

    cout << "All points: ";
    for (const auto& p : points) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;
    
    Point firstPoint = findLeftBottomPoint(points);
    cout << "Left-bottom-most point: (" << firstPoint.x << ", " << firstPoint.y << ")" << endl;

    vector<Point> chain = constructConvexHull(points);
    cout << "Convex Hull: ";
    for (const auto& p : chain) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }

    return 0;
}