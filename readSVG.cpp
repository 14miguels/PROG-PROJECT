
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <vector>

using namespace std;
using namespace tinyxml2;

namespace svg
{

//     Point parseTranslation(std::string& transformValue) {
//         Point translation;
//         std::stringstream ss(transformValue);
//         size_t start = transformValue.find("translate(");
//         size_t end = transformValue.find(")");
//         transformValue = transformValue.substr(start + 10, end);
//         size_t space = transformValue.find(" ");
//         std::string n1=transformValue.substr(0, space);
//         std::string n2=transformValue.substr(space+1, transformValue.size());
//         // char space = ' ';
//         // iss >> translation.x >> space >> translation.y;
//         return translation;
// }
    Point parseTranslation(std::string& transformValue) {
        Point translation;
        size_t start = transformValue.find("translate(");
        if (start == std::string::npos) {
            return translation;
        }
        
        start += 10;
        
        size_t end = transformValue.find(")", start);
        if (end == std::string::npos) {
            return translation;
        }

        std::string translationString = transformValue.substr(start, end - start);

        for (size_t i = 0; i < translationString.size(); ++i) {
            if (translationString[i] == ' ') {
                translationString[i] = ',';
            }
        }

        char virg = ',';

        std::istringstream iss(translationString);

        iss >> translation.x >> virg >> translation.y;

        return translation;
    }


    void translate(Point point, Point translate){
        point.x+=translate.x-1;
        point.y+=translate.y-1;
    }

    Point PointCreator(string& sequence){
        Point ponto;
        std::istringstream iss(sequence);
        char virg = ',';
        iss>>ponto.x >> virg >> ponto.y;
        return ponto;
    }

    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // TODO complete code -->
        
        for (XMLElement* child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            if (strcmp(child->Name() , "g")==0){
                child=child->FirstChildElement();
            }
            if (strcmp(child->Name() , "ellipse")==0)
            {
                Point center, radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius.x = child->IntAttribute("rx");
                radius.y = child->IntAttribute("ry");
                const char* fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                if (child->Attribute("transform"))
                {
                    std::string transformstr = child->Attribute("transform");
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        Point transPoint = parseTranslation(transformstr);
                        translate(center,transPoint);
                    }
                }
                
                Ellipse* ellipse = new Ellipse(fill, center, radius);
                svg_elements.push_back(ellipse);
            }
            if (strcmp(child->Name(),"circle")==0)
            {
                Point center;
                Point radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius.x = child->IntAttribute("r");
                radius.y = child->IntAttribute("r");
                const char* fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                Circle* circle = new Circle(fill, center, radius);
                svg_elements.push_back(circle);
            }
            if (strcmp(child->Name(),"line")==0)
            {
                Point p1, p2;
                p1.x=child->IntAttribute("x1");
                p1.y=child->IntAttribute("y1");
                p2.x=child->IntAttribute("x2");
                p2.y=child->IntAttribute("y2");
                const char* strokeChar = child->Attribute("stroke");
                Color stroke = parse_color(strokeChar);
                Line* line = new Line(p1, p2, stroke);
                svg_elements.push_back(line);
            }
            if (strcmp(child->Name(),"polyline")==0)
            {
                vector<Point> vector;
                const string sequence = child->Attribute("points");
                istringstream iss(sequence);
                string token;
                while (std::getline(iss, token, ' '))
                {
                    vector.push_back(PointCreator(token));
                }
                
                const char* strokeChar = child->Attribute("stroke");
                Color stroke = parse_color(strokeChar);
                Polyline* polyline = new Polyline(vector, stroke);
                svg_elements.push_back(polyline);
            }
            if (strcmp(child->Name(),"polygon")==0)
            {
                vector<Point> vector;
                const string sequence = child->Attribute("points");
                istringstream iss(sequence);
                string token;
                while (std::getline(iss, token, ' '))
                {
                    vector.push_back(PointCreator(token));
                }
                
                const char* fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                Polygon* polyline = new Polygon(vector, fill);
                svg_elements.push_back(polyline);
            }
            if (strcmp(child->Name(),"rect")==0)
            {
                vector<Point> vector;
                int x, y, width, height;
                x=child->IntAttribute("x");
                y=child->IntAttribute("y");
                width=child->IntAttribute("width");
                height=child->IntAttribute("height");

                vector.push_back(Point({x, y}));
                vector.push_back(Point({x + width - 1, y}));
                vector.push_back(Point({x + width - 1, y + height - 1}));
                vector.push_back(Point({x, y + height - 1}));
                
                const char* fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                Rectangle* rectangle = new Rectangle(vector, fill);
                svg_elements.push_back(rectangle);
            }
        }
    }
}