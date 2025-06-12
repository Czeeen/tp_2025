#include "commands.hpp"
#include <map>
#include <algorithm>
#include <iostream>
#include <functional>
#include <numeric>
#include <iomanip>
#include <string>
#include <cmath>

void prokopenko::Area(const std::vector<Polygon>& polygons, std::ostream& out, std::istream& in)
{
  std::map<std::string, std::function<void(const std::vector<Polygon>& polygon, std::ostream& out)>> areas;
  areas["EVEN"] = AreaEven;
  areas["ODD"] = AreaOdd;
  areas["MEAN"] = AreaMean;
  auto outInvalid = std::bind(outMessage, std::placeholders::_1, "<INVALID COMMAND>\n");
  std::string parameter;
  in >> parameter;
  try
  {
    areas.at(parameter)(polygons, out);
  }
  catch (const std::out_of_range& ex)
  {
    if (std::isdigit(parameter[0]) && std::stoi(parameter) > 2)
    {
      AreaVersNum(std::stoi(parameter), polygons, out);
    }
    else
    {
      outInvalid(out);
    }
  }
}

void prokopenko::AreaEven(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<Polygon> EvenPolygons_vec;
  std::copy_if(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(EvenPolygons_vec),
    [](const Polygon& polygon)
    {
      return !(polygon.points.size() % 2);
    });
  double sum = sumAreas(EvenPolygons_vec);
  out << std::setprecision(1) << std::fixed << sum << '\n';
}

void prokopenko::AreaOdd(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<Polygon> OddPolygons_vec;
  std::copy_if(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(OddPolygons_vec),
    [](const Polygon& polygon)
    {
      return (polygon.points.size() % 2);
    });
  double sum = sumAreas(OddPolygons_vec);
  out << std::setprecision(1) << std::fixed << sum << '\n';
}

void prokopenko::AreaMean(const std::vector<Polygon>& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::out_of_range("");
  }
  double sum = sumAreas(polygons);
  out << std::setprecision(1) << std::fixed << sum / polygons.size() << '\n';
}

void prokopenko::AreaVersNum(std::size_t vertexes, const std::vector<Polygon>& polygons, std::ostream& out)
{
  auto NVertexes = std::bind(std::equal_to<size_t>{}, vertexes, std::placeholders::_1);
  std::vector<Polygon> polygonsWithNVertexes;
  std::copy_if(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(polygonsWithNVertexes),
    [NVertexes](const Polygon& polygon)
    {
      return NVertexes(polygon.points.size());
    });

  double sum = sumAreas(polygonsWithNVertexes);
  out << std::setprecision(1) << std::fixed << sum << '\n';
}

double prokopenko::sumAreas(const std::vector<Polygon>& polygons)
{
  std::vector<double> AreasOfPolygons_vec;
  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(AreasOfPolygons_vec),
    [](const Polygon& polygon)
    {
      return getArea(polygon);
    });

  double sumOfAreas = std::accumulate(
    AreasOfPolygons_vec.begin(),
    AreasOfPolygons_vec.end(),
    0,
    std::bind(std::plus<double>{}, std::placeholders::_1, std::placeholders::_2));
  return sumOfAreas;
}

double prokopenko::getArea(const Polygon& polygon)
{
  std::vector<double> sumOfCoords_vec;
  std::transform(
    polygon.points.begin(),
    std::prev(polygon.points.end()),
    std::next(polygon.points.begin()),
    std::back_inserter(sumOfCoords_vec),
    [](const Point& a, const Point& b)
    {
      return a.x * b.y;
    });

  double sum = std::accumulate(
    sumOfCoords_vec.begin(),
    sumOfCoords_vec.end(),
    0,
    std::bind(std::plus<double>{}, std::placeholders::_1, std::placeholders::_2));

  std::vector<double> diffOfCoords_vec;
  std::transform(
    polygon.points.begin(),
    std::prev(polygon.points.end()),
    std::next(polygon.points.begin()),
    std::back_inserter(diffOfCoords_vec),
    [](const Point& a, const Point& b)
    {
      return a.y * b.x;
    });

  double diff = std::accumulate(
    diffOfCoords_vec.begin(),
    diffOfCoords_vec.end(),
    0,
    std::bind(std::plus<double>{}, std::placeholders::_1, std::placeholders::_2));

  return 0.5 * std::abs(sum + polygon.points.back().x * polygon.points.front().y -
    diff - polygon.points.front().x * polygon.points.back().y);
}

void prokopenko::Max(const std::vector<Polygon>& polygons, std::ostream& out, std::istream& in)
{
  std::map<std::string, std::function<void(const std::vector<Polygon>& polygons, std::ostream& out)>> maxes;
  maxes["AREA"] = maxArea;
  maxes["VERTEXES"] = maxVertexes;
  auto outInvalid = std::bind(outMessage, std::placeholders::_1, "<INVALID COMMAND>\n");
  std::string parameter;
  in >> parameter;
  try
  {
    maxes.at(parameter)(polygons, out);
  }
  catch (const std::out_of_range& ex)
  {
    outInvalid(out);
  }
}

void prokopenko::maxArea(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<double> areasOfPolygons_vec;
  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(areasOfPolygons_vec),
    [](const Polygon& polygon)
    {
      return getArea(polygon);
    });
  if (areasOfPolygons_vec.empty())
  {
    throw std::out_of_range("");
  }
  std::sort(areasOfPolygons_vec.begin(), areasOfPolygons_vec.end());
  out << std::setprecision(1) << std::fixed << areasOfPolygons_vec.back() << '\n';
}

void prokopenko::maxVertexes(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<std::size_t> vertexesOfPolygons_vec;
  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(vertexesOfPolygons_vec),
    [](const Polygon& polygon)
    {
      return polygon.points.size();
    });
  if (vertexesOfPolygons_vec.empty())
  {
    throw std::out_of_range("");
  }
  std::sort(vertexesOfPolygons_vec.begin(), vertexesOfPolygons_vec.end());
  out << vertexesOfPolygons_vec.back() << '\n';
}

void prokopenko::Min(const std::vector<Polygon>& polygons, std::ostream& out, std::istream& in)
{
  std::map<std::string, std::function<void(const std::vector<Polygon>& polygons, std::ostream& out)>> mines;
  mines["AREA"] = minArea;
  mines["VERTEXES"] = minVertexes;
  auto outInvalid = std::bind(outMessage, std::placeholders::_1, "<INVALID COMMAND>\n");
  std::string parameter;
  in >> parameter;
  try
  {
    mines.at(parameter)(polygons, out);
  }
  catch (const std::out_of_range& ex)
  {
    outInvalid(out);
  }
}

void prokopenko::minArea(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<double> areasOfPolygons_vec;
  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(areasOfPolygons_vec),
    [](const Polygon& polygon)
    {
      return getArea(polygon);
    });
  if (areasOfPolygons_vec.empty())
  {
    throw std::out_of_range("");
  }
  std::sort(areasOfPolygons_vec.begin(), areasOfPolygons_vec.end());
  out << std::setprecision(1) << std::fixed << areasOfPolygons_vec.front() << '\n';
}

void prokopenko::minVertexes(const std::vector<Polygon>& polygons, std::ostream& out)
{
  std::vector<std::size_t> vertexesOfPolygons_vec;
  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(vertexesOfPolygons_vec),
    [](const Polygon& polygon)
    {
      return polygon.points.size();
    });
  if (vertexesOfPolygons_vec.empty())
  {
    throw std::out_of_range("");
  }
  std::sort(vertexesOfPolygons_vec.begin(), vertexesOfPolygons_vec.end());
  out << vertexesOfPolygons_vec.front() << '\n';
}
