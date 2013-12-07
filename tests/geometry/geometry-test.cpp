
#include <iostream>
#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/aabb.hpp"
#include "geometry/circle.hpp"
#include "geometry/polygon.hpp"

enum {POINT = 1, LINE = 2, AABB = 3, CIRCLE = 4, POLYGON = 5};

int main()
{
    int shapeID;

    std::cout << "1 : point" << std::endl;
    std::cout << "2 : line" << std::endl;
    std::cout << "3 : aabb" << std::endl;
    std::cout << "4 : circle" << std::endl;
    std::cout << "5 : polygon" << std::endl;
    std::cout << "Enter the ID corresponding to the shape you want to create :" << std::endl;

shapeIDInput:
    std::cin >> shapeID;

    switch(shapeID)
    {
        case POINT:
            {
                float x, y;
                std::cout << "Enter X coord : ";
                std::cin >> x;  
                std::cout << "Enter Y coord : ";
                std::cin >> y;

                geometry::Point point(x, y);
                std::cout << "Your point coords are (" << point.x << ";" << point.y << ")." << std::endl;
            }
            break;
        case LINE:
            {
                float x1, y1, x2, y2, xhp, yhp;
                std::cout << "Enter your first point X coord : ";
                std::cin >> x1;  
                std::cout << "Enter your first point Y coord : ";
                std::cin >> y1;
                std::cout << "Enter your second point X coord : ";
                std::cin >> x2;  
                std::cout << "Enter your second point Y coord : ";
                std::cin >> y2;
                std::cout << "These coords will be relative to a hotpoint." << std::endl;
                std::cout << "Enter the hotpoint X coord : ";
                std::cin >> xhp;
                std::cout << "Enter the hotpoint y coord : ";
                std::cin >> yhp;

                geometry::Line line(geometry::Point(x1, y1), geometry::Point(x2, y2), geometry::Point(xhp, yhp));
                std::cout << "Relatively to the hotpoint (" << xhp << ";" << yhp << "), your line starts at (" << line.p1.x << ";" << line.p1.y << "), ends at (" << line.p2.x << ";" << line.p2.y << ") and is " << line.length() << " long." << std::endl;
            }
            break;
        case AABB:
            break;
        case CIRCLE:
            break;
        case POLYGON:
            {
                geometry::Polygon poly;
                poly.points.resize(5);
                poly.points[0] = geometry::Point(0,1);
                poly.points[1] = geometry::Point(-1,2);
                poly.points[2] = geometry::Point(-2,-1);
                poly.points[3] = geometry::Point(1,-2);
                poly.points[4] = geometry::Point(3,2);
                std::vector<geometry::Polygon> cvx = poly.convexify();
                for(geometry::Polygon p : cvx) {
                    std::cout << "Sub polygon ";
                    for(geometry::Point pt : p.points) {
                        std::cout << "(" << pt.x << ";" << pt.y << ") ";
                    }
                    std::cout << std::endl;
                }
            }
            break;
        default:
            std::cout << "Incorrect value ; please try again :" << std::endl;
            goto shapeIDInput;
            break;
    }

    return EXIT_SUCCESS;
}
