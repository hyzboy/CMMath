#include<numbers>
#include<cmath>

namespace hgl::math
{
    /**
        * fast atan2
        * from: Jim Shima
        * date: 1999/04/23
        */
    double Latan2(double y, double x)
    {
        double abs_y = fabs(y) + 1e-10;      // kludge to prevent 0/0 condition
        double angle;
        double r;

        if (x >= 0)
        {
            r = (x - abs_y) / (x + abs_y);
            angle = std::numbers::pi_v<double> / 4.0 - std::numbers::pi_v<double> / 4.0 * r;
        }
        else
        {
            r = (x + abs_y) / (abs_y - x);
            angle = 3.0 * std::numbers::pi_v<double> / 4.0 - std::numbers::pi_v<double> / 4.0 * r;
        }

        if (y < 0)
            return (-angle);     // negate if in quad III or IV
        else
            return (angle);
    }
}//namespace hgl::math
