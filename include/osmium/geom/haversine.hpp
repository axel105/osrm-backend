#ifndef OSMIUM_GEOM_HAVERSINE_HPP
#define OSMIUM_GEOM_HAVERSINE_HPP

/*

This file is part of Osmium (http://osmcode.org/libosmium).

Copyright 2013-2015 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <cmath>
#include <iterator>

#include <osmium/geom/coordinates.hpp>
#include <osmium/geom/util.hpp>
#include <osmium/osm/node_ref.hpp>
#include <osmium/osm/way.hpp>

namespace osmium {

    namespace geom {

        /**
         * @brief Functions to calculate arc distance on Earth using the haversine formula.
         *
         * See http://en.wikipedia.org/wiki/Haversine_formula
         *
         * Implementation derived from
         * http://blog.julien.cayzac.name/2008/10/arc-and-distance-between-two-points-on.html
         */
        namespace haversine {

            /// @brief Earth's quadratic mean radius for WGS84
            constexpr double EARTH_RADIUS_IN_METERS = 6372797.560856;

            /**
             * Calculate distance in meters between two sets of coordinates.
             */
            inline double distance(const osmium::geom::Coordinates& c1, const osmium::geom::Coordinates& c2) {
                double lonh = sin(deg_to_rad(c1.x - c2.x) * 0.5);
                lonh *= lonh;
                double lath = sin(deg_to_rad(c1.y - c2.y) * 0.5);
                lath *= lath;
                const double tmp = cos(deg_to_rad(c1.y)) * cos(deg_to_rad(c2.y));
                return 2.0 * EARTH_RADIUS_IN_METERS * asin(sqrt(lath + tmp*lonh));
            }

            /**
             * Calculate length of way.
             */
            inline double distance(const osmium::WayNodeList& wnl) {
                double sum_length=0;

                for (auto it = wnl.begin(); it != wnl.end(); ++it) {
                    if (std::next(it) != wnl.end()) {
                        sum_length += distance(it->location(), std::next(it)->location());
                    }
                }

                return sum_length;
            }

        } // namespace haversine

    } // namespace geom

} // namespace osmium

#endif // OSMIUM_GEOM_HAVERSINE_HPP
