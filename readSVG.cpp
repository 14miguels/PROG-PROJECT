
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <iostream>
#include <vector>
#include "Point.cpp"
#include "Point.hpp"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    //function to get each point from the SVG file
    Point PointCreator(string &sequence)
    {
        Point ponto;
        std::istringstream iss(sequence);
        char virg = ',';
        iss >> ponto.x >> virg >> ponto.y;
        return ponto;
    }
    //function to get the point of origin in the SVG file
    Point parseOrigin(std::string &transformOrigin)
    {
        Point origin;
        std::istringstream iss(transformOrigin);
        size_t space = transformOrigin.find(" ");
        std::string n1 = transformOrigin.substr(0, space);
        std::string n2 = transformOrigin.substr(space + 1, transformOrigin.size());

        origin.x = std::stoi(n1);
        origin.y = std::stoi(n2);

        return origin;
    }
    //function to get value from the translation attribute in the SVG file
    Point parseTranslation(std::string &transformValue)
    {
        Point translation = {0, 0};

        size_t start = 10;

        size_t end = transformValue.find(")", start);

        std::string translationString = transformValue.substr(start, end - start);

        for (size_t i = 0; i < translationString.size(); ++i)
        {
            if (translationString[i] == ' ')
            {
                translationString[i] = ',';
            }
        }

        char virg = ',';

        std::istringstream iss(translationString);

        iss >> translation.x >> virg >> translation.y;

        return translation;
    }
    //function to get value from the rotation attribute in the SVG file
    int parseRotation(const string &transformstr)
    {
        int rotation;

        size_t start = 7;

        size_t end = transformstr.find(")", start);

        std::string rotationValue = transformstr.substr(start, end - start);

        std::stringstream iss(rotationValue);
        iss >> rotation;

        return rotation;
    }
    //function to get value from the scalling attribute in the SVG file
    int parseScaling(std::string transformstr)
    {
        int scaling;

        size_t start = 6;

        size_t end = transformstr.find(")", start);

        std::string scaleValue = transformstr.substr(start, end - start);

        std::stringstream iss(scaleValue);
        iss >> scaling;
        return scaling;
    }
    //parse the values for each object, transformation and origin
    void readSVG(const string &svg_file, Point &dimensions, vector<SVGElement *> &svg_elements)
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


        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            // parse the origin of the transformation 
            Point origin{0, 0};
            if (child->Attribute("transform-origin"))
            {
                std::string transformOrigin = child->Attribute("transform-origin");
                origin = parseOrigin(transformOrigin);
            }
            if (strcmp(child->Name(), "g") == 0)
            {
                child = child->FirstChildElement();
            }
            //check if it's an ellipse
            if (strcmp(child->Name(), "ellipse") == 0)
            {

                Point center, radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius.x = child->IntAttribute("rx");
                radius.y = child->IntAttribute("ry");
                const char *fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                // check if there is a transformation
                if (child->Attribute("transform"))
                {
                    std::string transformstr = child->Attribute("transform");
                    // check if transformation is translation, rotation, or scale and push correct format to ellipse
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        Point transPoint = parseTranslation(transformstr);
                        Ellipse *ellipse = new Ellipse(fill, center.translate(transPoint), radius);
                        svg_elements.push_back(ellipse);
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        int degrees = parseRotation(transformstr);
                        center = center.rotate(origin, degrees);
                        Ellipse *ellipse = new Ellipse(fill, center, radius);
                        svg_elements.push_back(ellipse);
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        int scaling = parseScaling(transformstr);
                        radius = radius.scale(Point({0, 0}), scaling);
                        if (center.x != origin.x || center.y != origin.y)
                            center = center.scale(origin, scaling);
                        Ellipse *ellipse = new Ellipse(fill, center, radius);
                        svg_elements.push_back(ellipse);
                    }
                }
                // case without transformation
                else
                {
                    Ellipse *ellipse = new Ellipse(fill, center, radius);
                    svg_elements.push_back(ellipse);
                }
            }
            // check if it's a circle
            if (strcmp(child->Name(), "circle") == 0)
            {
                Point center;
                Point radius;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                radius.x = child->IntAttribute("r");
                radius.y = child->IntAttribute("r");
                const char *fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                // check if there is a transformation
                if (child->Attribute("transform"))
                {
                    // check if transformation is translation, rotation, or scale and push correct values to circle
                    std::string transformstr = child->Attribute("transform");
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        Point transPoint = parseTranslation(transformstr);
                        Circle *circle = new Circle(fill, center.translate(transPoint), radius);
                        svg_elements.push_back(circle);
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        int degrees = parseRotation(transformstr);
                        center = center.rotate(origin, degrees);
                        Circle *circle = new Circle(fill, center, radius);
                        svg_elements.push_back(circle);
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        int scaling = parseScaling(transformstr);
                        radius = radius.scale(Point({0, 0}), scaling);
                        center = center.scale(origin, scaling);
                        std::cout << center.x << " " << center.y << " " << radius.x << " " << radius.y << endl;
                        Circle *circle = new Circle(fill, center, radius);
                        svg_elements.push_back(circle);
                    }
                }
                // case without transformation
                else
                {

                    Circle *circle = new Circle(fill, center, radius);
                    svg_elements.push_back(circle);
                }
            }
            // check if it's a line
            if (strcmp(child->Name(), "line") == 0)
            {
                Point p1, p2;
                p1.x = child->IntAttribute("x1");
                p1.y = child->IntAttribute("y1");
                p2.x = child->IntAttribute("x2");
                p2.y = child->IntAttribute("y2");
                const char *strokeChar = child->Attribute("stroke");
                Color stroke = parse_color(strokeChar);
                //check if there is a transformation
                if (child->Attribute("transform"))
                {
                    // check if transformation is translation, rotation, or scale and push correct values to line
                    std::string transformstr = child->Attribute("transform");
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        Point transPoint = parseTranslation(transformstr);
                        Line *circle = new Line(p1.translate(transPoint), p2.translate(transPoint), stroke);
                        svg_elements.push_back(circle);
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        int degrees = parseRotation(transformstr);
                        p1 = p1.rotate(origin, degrees);
                        p2 = p2.rotate(origin, degrees);
                        Line *line = new Line(p1, p2, stroke);
                        svg_elements.push_back(line);
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        int scaling = parseScaling(transformstr);
                        p1 = p1.scale(origin, scaling);
                        p2 = p2.scale(origin, scaling);
                        Line *line = new Line(p1, p2, stroke);
                        svg_elements.push_back(line);
                    }
                }
                // case without transformation
                else
                {
                    Line *line = new Line(p1, p2, stroke);
                    svg_elements.push_back(line);
                }
            }
            // check if it's a polyline
            if (strcmp(child->Name(), "polyline") == 0)
            {
                vector<Point> vector;
                const string sequence = child->Attribute("points");
                istringstream iss(sequence);
                string token;
                while (std::getline(iss, token, ' '))
                {
                    vector.push_back(PointCreator(token));
                }

                const char *strokeChar = child->Attribute("stroke");
                Color stroke = parse_color(strokeChar);
                //check if there is a transformation
                if (child->Attribute("transform"))
                {
                    std::string transformstr = child->Attribute("transform");
                    // check if transformation is translation, rotation, or scale and push correct values to polyline
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            Point transPoint = parseTranslation(transformstr);
                            newVector.push_back(vector[i].translate(transPoint));
                            Polyline *polyline = new Polyline(newVector, stroke);
                            svg_elements.push_back(polyline);
                        }
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {

                            int degrees = parseRotation(transformstr);
                            newVector.push_back(vector[i].rotate(origin, degrees));
                            Polyline *polyline = new Polyline(newVector, stroke);
                            svg_elements.push_back(polyline);
                        }
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            int scaling = parseScaling(transformstr);
                            newVector.push_back(vector[i].scale(origin, scaling));
                            Polyline *polyline = new Polyline(newVector, stroke);
                            svg_elements.push_back(polyline);
                        }
                    }
                }
                // case without transformation
                else
                {
                    Polyline *polyline = new Polyline(vector, stroke);
                    svg_elements.push_back(polyline);
                }
            }
            //check if it's a polygon
            if (strcmp(child->Name(), "polygon") == 0)
            {
                vector<Point> vector;
                const string sequence = child->Attribute("points");
                istringstream iss(sequence);
                string token;
                while (std::getline(iss, token, ' '))
                {
                    vector.push_back(PointCreator(token));
                }

                const char *fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                // check if there is a transformation
                if (child->Attribute("transform"))
                {
                    std::string transformstr = child->Attribute("transform");
                    // check if transformation is translation, rotation, or scale and push correct values to polyhon
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            Point transPoint = parseTranslation(transformstr);
                            newVector.push_back(vector[i].translate(transPoint));
                            Polygon *polygon = new Polygon(newVector, fill);
                            svg_elements.push_back(polygon);
                        }
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {

                            int degrees = parseRotation(transformstr);
                            std::cout<<degrees<<endl;
                            newVector.push_back(vector[i].rotate(origin, degrees));
                            Polygon *polygon = new Polygon(newVector, fill);
                            svg_elements.push_back(polygon);
                        }
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            int scaling = parseScaling(transformstr);
                            newVector.push_back(vector[i].scale(origin, scaling));
                            Polygon *polygon = new Polygon(newVector, fill);
                            svg_elements.push_back(polygon);
                        }
                    }
                }
                // case without transformation
                else
                {

                    Polygon *polygon = new Polygon(vector, fill);
                    svg_elements.push_back(polygon);
                }
            }
            //check if it's a rectangle
            if (strcmp(child->Name(), "rect") == 0)
            {
                vector<Point> vector;
                int x, y, width, height;
                x = child->IntAttribute("x");
                y = child->IntAttribute("y");
                width = child->IntAttribute("width");
                height = child->IntAttribute("height");

                vector.push_back(Point({x, y}));
                vector.push_back(Point({x + width - 1, y}));
                vector.push_back(Point({x + width - 1, y + height - 1}));
                vector.push_back(Point({x, y + height - 1}));

                const char *fillChar = child->Attribute("fill");
                Color fill = parse_color(fillChar);
                //check if there is a transformation
                if (child->Attribute("transform"))
                {
                    std::string transformstr = child->Attribute("transform");
                    // check if transformation is translation, rotation, or scale and push correct values to rectangle
                    if (transformstr.find("translate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            Point transPoint = parseTranslation(transformstr);
                            newVector.push_back(vector[i].translate(transPoint));
                            Rectangle *rect = new Rectangle(newVector, fill);
                            svg_elements.push_back(rect);
                        }
                    }
                    else if (transformstr.find("rotate") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {

                            int degrees = parseRotation(transformstr);
                            newVector.push_back(vector[i].rotate(origin, degrees));
                            Rectangle *rect = new Rectangle(newVector, fill);
                            svg_elements.push_back(rect);
                        }
                    }
                    else if (transformstr.find("scale") != std::string::npos)
                    {
                        std::vector<Point> newVector;
                        for (size_t i = 0; i < vector.size(); i++)
                        {
                            int scaling = parseScaling(transformstr);
                            newVector.push_back(vector[i].scale(origin, scaling));
                            Rectangle *rect = new Rectangle(newVector, fill);
                            svg_elements.push_back(rect);
                        }
                    }
                }
                //case without transformation
                else
                {
                    Rectangle *rect = new Rectangle(vector, fill);
                    svg_elements.push_back(rect);
                }
            }
        }
    }
}