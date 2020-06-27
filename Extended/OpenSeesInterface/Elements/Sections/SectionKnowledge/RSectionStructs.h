#ifndef RSECTIONSTRUCTS_H
#define RSECTIONSTRUCTS_H

#include <string>

/*
 * A =	Cross-sectional area of member (in.^2)
 * d =	Depth of member, parallel to Y-axis (in.)
 * h =	Depth of member, parallel to Y-axis (in.)
 * tw =	Thickness of web of member (in.)
 * bf =	Width of flange of member, parallel to X-axis (in.)
 * b =	Width of member, parallel to X-axis (in.)
 * tf =	Thickness of flange of member (in.)
 * k =	Distance from outer face of flange to web toe of fillet (in.)
 * k1 =	Distance from web centerline to flange toe of fillet (in.)
 * T =	Distance between fillets for wide-flange or channel shape = d(nom)-2*k(det) (in.)
 * gage =	Standard gage (bolt spacing) for member (in.)  (Note: gages for angles are available by viewing comment box at cell K18.)
 * Ix =	Moment of inertia of member taken about X-axis (in.^4)
 * Sx =	Elastic section modulus of member taken about X-axis (in.^3)
 * rx =	Radius of gyration of member taken about X-axis (in.) = SQRT(Ix/A)
 * Iy =	Moment of inertia of member taken about Y-axis (in.^4)
 * Sy =	Elastic section modulus of member taken about Y-axis (in.^3)
 * ry =	Radius of gyration of member taken about Y-axis (in.) = SQRT(Iy/A)
 * Zx =	Plastic section modulus of member taken about X-axis (in.^3)
 * Zy =	Plastic section modulus of member taken about Y-axis (in.^3)
 * rts =	SQRT(SQRT(Iy*Cw)/Sx) (in.)
 * xp =	horizontal distance from designated member edge to plastic neutral axis (in.)
 * yp =	vertical distance from designated member edge to plastic neutral axis (in.)
 * ho =	Distance between centroid of flanges, d-tf (in.)
 * J =	Torsional moment of inertia of member (in.^4)
 * Cw =	Warping constant (in.^6)
 * C =	Torsional constant for HSS shapes (in.^3)
 * a =	Torsional property, a = SQRT(E*Cw/G*J) (in.)
 * E =	Modulus of elasticity of steel = 29,000 ksi
 * G =	Shear modulus of elasticity of steel = 11,200 ksi
 * Wno =	Normalized warping function at a point at the flange edge (in.^2)
 * Sw =	Warping statical moment at a point on the cross section (in.^4)
 * Qf =	Statical moment for a point in the flange directly above the vertical edge of the web (in.^3)
 * Qw =	Statical moment at the mid-depth of the section (in.^3)
 * x(bar) =	Distance from outside face of web of channel shape or outside face of angle leg to Y-axis (in.)
 * y(bar) =	Distance from outside face of outside face of flange of WT or angle leg to Y-axis (in.)
 * eo =	Horizontal distance from the outer edge of a channel web to its shear center (in.) = (approx.) tf*(d-tf)^2*(bf-tw/2)^2/(4*Ix)-tw/2
 * xo =	x-coordinate of shear center with respect to the centroid of the section (in.)
 * yo =	y-coordinate of shear center with respect to the centroid of the section (in.)
 * ro(bar) =	Polar radius of gyration about the shear center = SQRT(xo^2+yo^2+(Ix+Iy)/A) (in.)
 * H =	Flexural constant, H = 1-(xo^2+yo^2)/ro(bar)^2)
 * LLBB =	Long legs back-to-back for double angles
 * SLBB =	Short legs back-to-back for double angles
 * h(flat) =	The workable flat (straight) dimension along the height, h (in.)
 * b(flat) =	The workable flat (straight) dimension along the width, b (in.)
 * A(surf) =	The total surface area of a rectangular or square HSS section (ft.^2/ft.)
 * STD =	Standard weight (Schedule 40) pipe section
 * XS =	Extra strong (Schedule 80) pipe section
 * XXS =	Double-extra strong pipe section
 */
struct WSection
{
   std::string Shape = "NULL";
   double Area_a = 0.0;
   double depth_d = 0.0;
   double webThickness_tw = 0.0;
   double flangeWidth_bf = 0.0;
   double flangeThickness_tf = 0.0;
   double kdes = 0.0;
   double kdet = 0.0;
   double k1 = 0.0;
   double webHeight_T = 0.0;
   double gage = 0.0;
   double wtperft = 0.0;
   double bfdiv2tf = 0.0;
   double hdivtw = 0.0;
   double Ix = 0.0;
   double Sx = 0.0;
   double rx = 0.0;
   double Zx = 0.0;
   double Iy = 0.0;
   double Sy = 0.0;
   double ry = 0.0;
   double Zy = 0.0;
   double rts = 0.0;
   double ho = 0.0;
   double J = 0.0;
   double Cw = 0.0;
   double Wno = 0.0;
   double Sw1 = 0.0;
   double Qf = 0.0;
   double Qw = 0.0;

};

struct HSSRoundSection
{
   std::string Shape = "NULL";
   double A         = 0.0;
   double outerDiameter_OD        = 0.0;
   double ID        = 0.0;
   double thicknessNominal_tnom      = 0.0;
   double thicknessDesign_tdes      = 0.0;
   double wtperft   = 0.0;
   double Ddivt     = 0.0;
   double I         = 0.0;
   double S         = 0.0;
   double r         = 0.0;
   double Z         = 0.0;
   double J         = 0.0;
   double C         = 0.0;

};

struct LSection
{
   std::string Shape = "NULL";
   double A         = 0.0;
   double d         = 0.0;
   double b         = 0.0;
   double t         = 0.0;
   double kdes      = 0.0;
   double kdet      = 0.0;
   double wtperft   = 0.0;
   double Ix        = 0.0;
   double Sx        = 0.0;
   double rx        = 0.0;
   double ybar      = 0.0;
   double Zx        = 0.0;
   double yp        = 0.0;
   double Iy        = 0.0;
   double Sy        = 0.0;
   double ry        = 0.0;
   double xbar      = 0.0;
   double Zy        = 0.0;
   double xp        = 0.0;
   double Iz        = 0.0;
   double Sz        = 0.0;
   double rz        = 0.0;
   double TANa      = 0.0;
   double Qs        = 0.0;
   double J         = 0.0;
   double Cw        = 0.0;
   double robar     = 0.0;
   double H         = 0.0;
};

struct WTSection
{
   std::string Shape = "NULL";
   double A         = 0.0;
   double d         = 0.0;
   double tw        = 0.0;
   double bf        = 0.0;
   double tf        = 0.0;
   double kdes      = 0.0;
   double kdet      = 0.0;
   double gage      = 0.0;
   double wtperft   = 0.0;
   double bfdiv2tf  = 0.0;
   double ddivtw    = 0.0;
   double Ix        = 0.0;
   double Sx        = 0.0;
   double rx        = 0.0;
   double ybar      = 0.0;
   double Zx        = 0.0;
   double yp        = 0.0;
   double Iy        = 0.0;
   double Sy        = 0.0;
   double ry        = 0.0;
   double Zy        = 0.0;
   double Qs        = 0.0;
   double J         = 0.0;
   double Cw        = 0.0;
   double robar     = 0.0;
   double H         = 0.0;
};

struct HSSRECTSection
{
   std::string Shape = "NULL";
   double A         = 0.0;
   double h         = 0.0;
   double b         = 0.0;
   double tnom      = 0.0;
   double tdes      = 0.0;
   double wtperft   = 0.0;
   double bdivt     = 0.0;
   double hdivt     = 0.0;
   double Ix        = 0.0;
   double Sx        = 0.0;
   double rx        = 0.0;
   double Zx        = 0.0;
   double Iy        = 0.0;
   double Sy        = 0.0;
   double ry        = 0.0;
   double Zy        = 0.0;
   double hflat     = 0.0;
   double bflat     = 0.0;
   double J         = 0.0;
   double C         = 0.0;
   double Asurf     = 0.0;
};

struct CSection
{
   std::string Shape = "NULL";
   double A         = 0.0;
   double d         = 0.0;
   double tw        = 0.0;
   double bf        = 0.0;
   double tf        = 0.0;
   double kdes      = 0.0;
   double kdet      = 0.0;
   double T         = 0.0;
   double gage      = 0.0;
   double rts       = 0.0;
   double ho        = 0.0;
   double wtperft   = 0.0;
   double eo        = 0.0;
   double Ix        = 0.0;
   double Sx        = 0.0;
   double rx        = 0.0;
   double Zx        = 0.0;
   double Iy        = 0.0;
   double Sy        = 0.0;
   double ry        = 0.0;
   double xbar      = 0.0;
   double Zy        = 0.0;
   double xp        = 0.0;
   double J         = 0.0;
   double Cw        = 0.0;
   double robar     = 0.0;
   double H         = 0.0;
};


#endif // RSECTIONSTRUCTS_H
