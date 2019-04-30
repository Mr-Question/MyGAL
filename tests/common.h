/* MyGAL
 * Copyright (C) 2019 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <MyGAL/FortuneAlgorithm.h>

// Tests

template<typename T>
bool isBounded(mygal::Diagram<T>& diagram)
{
    for (auto i = std::size_t(0); i < diagram.getNbSites(); ++i)
    {
        auto face = diagram.getFace(i);
        auto start = face->outerComponent;
        auto halfEdge = start;
        do
        {
            halfEdge = halfEdge->next;
            if (halfEdge == nullptr)
            {
                std::cerr << "Face " << i << " is not bounded.\n";
                return false;
            }
        } while (halfEdge != start);
    }
    return true;
}

template<typename T>
bool isIntersectionValid(mygal::Diagram<T>& diagram, const mygal::Box<T>& box)
{
    for (const auto& vertex : diagram.getVertices())
    {
        if (!box.contains(vertex.point))
            return false;
    }
    return true;
}

// Diagram

template<typename T>
mygal::Diagram<T> generateDiagram(const std::vector<mygal::Vector2<T>>& points)
{
    // Construct diagram
    auto algorithm = mygal::FortuneAlgorithm<T>(points);
    algorithm.construct();

    // Bound the diagram
    algorithm.bound(mygal::Box<T>{-0.05, -0.05, 1.05, 1.05});
    auto diagram = algorithm.getDiagram();
    assert(isBounded(diagram));

    // Intersect the diagram with a box
    auto box = mygal::Box<T>{0.0, 0.0, 1.0, 1.0};
    bool success = diagram.intersect(box);
    assert(success);
    assert(isIntersectionValid(diagram, box));

    return diagram;
}