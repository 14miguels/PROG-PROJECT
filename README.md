
# Programming project

## Group elements

Identify all group elements (numbers and names).

- upXXXXX Name of element 1
- upXXXXX Name of element 2
- upXXXXX Name of element 3


## Accomplished tasks

We started by editing the SVGElements.hpp. In this file, we defined various classes and functions for the diferente types of SVG elements provided in the tests. The SVGElement class serves as a base class for all SVG elements wich are extended by subclasses to draw speciffic elements. We defined:

- The Ellipse class defines a constructor to initialize its fill color, its center, and its radius. The draw method of the Ellipse class takes a PNGImage object as na argument to draw the ellipse.

- The Circle class inherits from Ellipse. It defines a constructor that initializes the circle's fill color, its center and its radius, making also use of the Ellipse class.

- The Polyline class defines a constructor that initializes the Polyline's stroke color and its vector of points. Its draw method draws the polyline using a series of line segments.

- The Line class inherits from Polyline. It defines a constructor that initializes the line's endpoints and its stroke color, while using the constructor of the Polyline class.

- The Polygon class defines a constructor that initializes its vector of points and its fill color. Its draw method draws the polygon by connecting the vertices.

- Finally, the Rectangle class inherits from Polygon. It defines a constructor that initializes the rectangle's vector of points and its fill color, while using the Polygon class.

We followed this by creating the corresponding drawing functions on the SVGElements.cpp file

In this file, by using the namespace SVG, we started by using the draw function in the Ellipse class so that it takes the PNG Image as the argument and initializes the fill color, center and the radius of the ellipse. The circle was nearly the same as the ellipse as it´s only diference is that in the radius de x,y are the same. We repeted this method for the rest of the elements as for example the line is a particular case of the poliline and the rectangle is a particular case of the polygon.

We then set up the readSVG.cpp file who's purpose was to pass the correct variables to the drawing functions

In this file, we defined the functions and classes that read the SVG format and extracted the respective values. We created the functions: PointCreator, parseOrigin, parseTranslation, parseRotation, and parseScalingto create point objects and the functions: parse transform origin, translation, rotation, and scaling from SVG attributes, respectively.

The readSVG function was created to read an SVG file and create a vector of SVGElement pointers representing the SVG elements found in the file. It extracts relevant attributes such as dimensions, center, radius, points, etc., and creates the corresponding SVG element objects (Ellipse, Circle, Line, Polyline, Polygon, or Rectangle) based on the element type.

For each SVG element, if a transformation (translate, rotate, scale) occurs, the corresponding transformation operation is applied to the element before creating the SVG element object.

