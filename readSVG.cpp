
// #include <iostream>
// #include "SVGElements.hpp"
// #include "external/tinyxml2/tinyxml2.h"

// using namespace std;
// using namespace tinyxml2;

// namespace svg
// {
//     void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
//     {
//         XMLDocument doc;
//         XMLError r = doc.LoadFile(svg_file.c_str());
//         if (r != XML_SUCCESS)
//         {
//             throw runtime_error("Unable to load " + svg_file);
//         }
//         XMLElement *xml_elem = doc.RootElement();

//         dimensions.x = xml_elem->IntAttribute("width");
//         dimensions.y = xml_elem->IntAttribute("height");

//         for (const XMLAttribute *attr = xml_elem->FirstAttribute(); attr != nullptr; attr = attr->Next())
//         // tamos a iterar sobre os nodes e quando percorrer todos vai ser null ptr e para
//             if(attr->Name() == )
//             svg_elements.push_back();
//         // TODO complete code -->
        
//     }
// }

#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
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
            if (strcmp(child->Name() , "ellipse")==0)
            {
                Point center, radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius.x = child->IntAttribute("rx");
                radius.y = child->IntAttribute("ry");
                const char* fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                Ellipse* ellipse = new Ellipse(fill, center, radius);
                svg_elements.push_back(ellipse);
            }
            if (strcmp(child->Name(),"circle")==0)
            {
                Point center;
                int radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius = child->IntAttribute("r");
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
          
        }
    }
}