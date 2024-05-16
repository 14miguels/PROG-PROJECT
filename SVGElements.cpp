#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    Circle::Circle(const Color &fill, const Point &center, const Point radius) 
    : Ellipse(fill, center,  radius){}
    
    Polyline::Polyline(const std::vector<Point> &vector, const Color &stroke) : stroke(stroke), vector(vector) {}
    void Polyline::draw(PNGImage &img) const{
        for(size_t i = 0; i < vector.size() - 1;i++){
            img.draw_line(vector[i],vector[i+1],stroke);
        }

    }
    Line::Line(const Point &p1,const Point &p2, const Color &stroke): 
    Polyline(std::vector<Point>({p1,p2}), stroke){};
    
    Polygon::Polygon(std::vector<Point> &vector, const Color &fill) : vector(vector), fill(fill) {}
    void Polygon::draw(PNGImage &img) const{
        img.draw_polygon(vector,fill);
    }

    Rectangle::Rectangle(std::vector<Point> &vector, const Color &fill) 
    : Polygon(vector, fill) {}

}
