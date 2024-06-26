//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);
    //initiate class for Ellipse
    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };
    // initiate subclass for Circle from Ellipse
    class Circle : public Ellipse
    {
        public:
            Circle(const Color &fill, const Point &center, const Point radius);
    };
    //initiate class for Polyline
    class Polyline : public SVGElement
    {
        public: 
            Polyline(const std::vector<Point> &vector, const Color &stroke);
            void draw(PNGImage &img) const override;
        
        private:
            Color stroke;
            std::vector<Point> vector; 
    };
    // initiate subclass for Line from Polyline
    class Line : public Polyline
    {
        public:
            Line(const Point &p1, const Point &p2, const Color &stroke);
    };
    //initiate class for Polygon
    class Polygon : public SVGElement
    {
        public:
            Polygon(std::vector<Point> &vector, const Color &fill);
            void draw(PNGImage &img) const override;
        private:
            std::vector<Point> vector; 
            Color fill;
    };
    // initiate subclass for Rectangle from Polygon
    class Rectangle : public Polygon
    {
        public:
            Rectangle(std::vector<Point> &vector, const Color &fill);
        // private:
        //     Color fill;
        //     std::vector<Point> vector;
    };

}
#endif
