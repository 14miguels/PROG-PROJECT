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
    class Circle : public SVGElement
    {
        public:
            Circle(const Color &fill, const Point &center, const int radius);
            void draw(PNGImage &img) const override;

        private:
            Color fill;
            Point center;
            int radius;
    };
    class Polyline : public SVGElement
    {
        public: 
            Polyline(std::string sequence, const Color &fill);
            void draw(PNGImage &img) const override;
        
        private:
            Color fill;
            std::vector<Point> vector; 
    };
    class Line : public SVGElement
    {
        public:
            Line(Point start, Point end, const Color &fill);
            void draw(PNGImage &img) const override;
        
        private:
            Color fill;
            Point start;
            Point end;

    };

    class Polygon : public SVGElement
    {
        public:
            Polygon(std::string sequence, const Color &fill);
            void draw(PNGImage &img) const override;
        private:
            Color fill;
            std::vector<Point> vector; 
    };
    class Rectangle : public SVGElement
    {
        public:
            Rectangle(Point topLeft,const Color &fill,int width,int height);
            void draw(PNGImage &img) const override;
        private:
            Point topLeft;
            Color fill;
            int width, height;
    };

}
#endif
