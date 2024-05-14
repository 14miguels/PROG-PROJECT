#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {

    }
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
    Circle::Circle(const Color &fill, const Point &center, const int radius)
    {
        this->center = center;
        this->fill = fill;
        this->radius = radius;
    }
    void Circle::draw(PNGImage &img) const{
        img.draw_ellipse(center,{radius,radius},fill);
    }
    Polyline::Polyline(std::string sequence, const Color &fill)
    {
        this->fill = fill;
        size_t start = 0;

        while (start < sequence.size())
        {
          size_t comma_pos = sequence.find(',',start);
          size_t space_pos = sequence.find(',',start);

          int first = std::stoi(sequence.substr(start,comma_pos));
          int second = std::stoi(sequence.substr(comma_pos+1,space_pos - comma_pos - 1));

          vector.push_back({first,second});          

        }
    }
    void Polyline::draw(PNGImage &img) const{
        for(auto i = 0; i < vector.size() - 1;i++){
            img.draw_line(vector[i],vector[i+1],fill);
        }

    }
    Line::Line(Point start, Point end, const Color &fill){
        this->start = start;
        this->end = end;
        this->fill = fill;
    }
    void Line::draw(PNGImage &img) const{
        img.draw_line(start,end,fill);
    }
    Polygon::Polygon(std::string sequence, const Color &fill){
        this->fill = fill;

        size_t start = 0;

        while (start < sequence.size())
        {
          size_t comma_pos = sequence.find(',',start);
          size_t space_pos = sequence.find(',',start);

          int first = std::stoi(sequence.substr(start,comma_pos));
          int second = std::stoi(sequence.substr(comma_pos+1,space_pos - comma_pos - 1));

          vector.push_back({first,second});          

        }        
    }
    void Polygon::draw(PNGImage &img) const{
        img.draw_polygon(vector,fill);
    }

    Rectangle::Rectangle(Point topLeft,const Color &fill,int width,int height){
        this->fill = fill;
        this->topLeft = topLeft;
        this->width = width;
        this->height = height;    
    }
    void Rectangle::draw(PNGImage &img) const{
        for(int i = 0; i < height; i++){
            Point p = {topLeft.x,topLeft.y+i};
            Point t = {topLeft.x + width,topLeft.y +i};
            img.draw_line(p,t,fill);
        }
    }

}
