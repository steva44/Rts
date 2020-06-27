#ifndef RTOPOPOINT_H
#define RTOPOPOINT_H

#include "gp_Pnt.hxx"
#include "osg/Vec3d"

class RNode;

class RTopoVertex
{
public:
    RTopoVertex();
    RTopoVertex(RNode* node, const double dx, const double dy, const double dz);
    RTopoVertex(const RTopoVertex& other)     // copy-constructor
    {
        this->theNode = other.theNode;
        this->OCCPoint = other.OCCPoint;
        this->OSGPoint = other.OSGPoint;
        this->deltax   = other.deltax;
        this->deltay   = other.deltay;
        this->deltaz   = other.deltaz;
    }
    RTopoVertex& operator=(RTopoVertex other) //Assignment operator
    {
        this->swap(*this, other);
        return *this;
    }

    void swap(RTopoVertex& first, RTopoVertex& second)
    {
        // by swapping the members of two objects, the two objects are effectively swapped
        std::swap(first.theNode, second.theNode);
        std::swap(first.OCCPoint, second.OCCPoint);
        std::swap(first.OSGPoint, second.OSGPoint);
        std::swap(first.deltax, second.deltax);
        std::swap(first.deltay, second.deltay);
        std::swap(first.deltaz, second.deltaz);
    }
    ~RTopoVertex();

    //For now use the OCC point check with the tolerance
    inline bool operator==(const RTopoVertex& vertex)
     {
         if(vertex.OCCPoint.IsEqual(OCCPoint,1.e-7))
             return true;
         else
             return false;
     }

    inline double operator[](unsigned i) {
      return this->OSGPoint[i];
    }

    inline const osg::Vec3d operator - (const RTopoVertex& rhs) const {
        return osg::Vec3d(OSGPoint.x()-rhs.x(), OSGPoint.y()-rhs.y(), OSGPoint.z()-rhs.z());
    }

    inline double x(void) const {
      return this->OSGPoint.x();
    }

    inline double y(void) const {
      return this->OSGPoint.y();
    }

    inline double z(void) const {
      return this->OSGPoint.z();
    }

    inline gp_Pnt getOCCPoint(){return OCCPoint;}

    inline osg::Vec3d getOSGPoint(){return OSGPoint;}

    void commitState(void);

private:
    RNode* theNode = nullptr;
    gp_Pnt OCCPoint;
    osg::Vec3d OSGPoint;
    double deltax = 0.0;
    double deltay = 0.0;
    double deltaz = 0.0;
};

#endif // RTOPOPOINT_H
