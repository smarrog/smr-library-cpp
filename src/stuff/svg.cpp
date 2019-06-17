#include "svg.h"

using namespace smr;

namespace smr {
    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << p.x << ',' << p.y;
    }

	std::ostream& operator<<(std::ostream& os, const Color& c) {
		if (c.isRgba()) {
			return os << "rgba("
			          << std::to_string(c.asRgba().red) << ","
			          << std::to_string(c.asRgba().green) << ","
			          << std::to_string(c.asRgba().blue) << ","
			          << c.asRgba().alpha
			          << ")";
		}
		if (c.isRgb()) {
			return os << "rgb("
			          << std::to_string(c.asRgb().red) << ","
			          << std::to_string(c.asRgb().green) << ","
			          << std::to_string(c.asRgb().blue)
			          << ")";
		}
		if (c.isString()) {
			return os << c.asString();
		}
		return os << "none";
	}
}

// CommonProperties

template<typename Owner>
Owner& CommonProperties<Owner>::SetFillColor(const Color& color) {
    _fillColor = color;
    return AsOwner();
}

template<typename Owner>
Owner& CommonProperties<Owner>::SetStrokeColor(const Color& color) {
    _strokeColor = color;
    return AsOwner();
}

template<typename Owner>
Owner& CommonProperties<Owner>::SetStrokeWidth(double value) {
    _strokeWidth = value;
    return AsOwner();
}

template<typename Owner>
Owner& CommonProperties<Owner>::SetStrokeLineCap(const std::string& value) {
    _strokeLineCap = value;
    return AsOwner();
}

template<typename Owner>
Owner& CommonProperties<Owner>::SetStrokeLineJoin(const std::string& value) {
    _strokeLineJoin = value;
    return AsOwner();
}

template<typename Owner>
void CommonProperties<Owner>::RenderCommon(std::ostream& os) const {
    RenderField(os, "fill", _fillColor);
    RenderField(os, "stroke", _strokeColor);
    RenderField(os, "stroke-width", _strokeWidth);
    if (!_strokeLineCap.empty()) {
        RenderField(os, "stroke-linecap", _strokeLineCap);
    }
    if (!_strokeLineJoin.empty()) {
        RenderField(os, "stroke-linejoin", _strokeLineJoin);
    }
}

template<typename Owner>
Owner& CommonProperties<Owner>::AsOwner() {
    return static_cast<Owner&>(*this);
}

// Circle

Circle& Circle::SetCenter(Point point) {
    _center = std::move(point);
    return *this;
}

Circle& Circle::SetRadius(double value) {
    _r = value;
    return *this;
}

void Circle::Render(std::ostream& os) const {
    os << "<circle";
    RenderCommon(os);
    RenderField(os, "cx", _center.x);
    RenderField(os, "cy", _center.y);
    RenderField(os, "r", _r);
    os << " />";
}

// Rectangle

Rectangle& Rectangle::SetPosition(Point point) {
    _position = point;
    return *this;
}

Rectangle& Rectangle::SetSize(Point point) {
    _size = point;
    return *this;
}

void Rectangle::Render(std::ostream& os) const {
    os << "<rect";
    RenderCommon(os);
    RenderField(os, "x", _position.x);
    RenderField(os, "y", _position.y);
    RenderField(os, "width", _size.x);
    RenderField(os, "height", _size.y);
    os << " />";
}

// Polyline

Polyline& Polyline::AddPoint(Point point) {
    _points.emplace_back(point.x, point.y);
    return *this;
}

void Polyline::Render(std::ostream& os) const {
    os << "<polyline";
    RenderCommon(os);
    os << " points=\"";
    auto isFirst = true;
    for (const auto& point : _points) {
        if (!isFirst) {
            os << ' ';
        }
        isFirst = false;
        os << point;
    }
    os << "\" />";
}

// Text

Text& Text::SetPoint(Point point) {
    _coords = point;
    return *this;
}

Text& Text::SetOffset(Point point) {
    _offset = point;
    return *this;
}

Text& Text::SetFontSize(uint32_t value) {
    _size = value;
    return *this;
}

Text& Text::SetFontFamily(const std::string& value) {
    _family = value;
    return *this;
}

Text& Text::SetFontWeight(const std::string& value) {
    _weight = value;
    return *this;
}

Text& Text::SetData(const std::string& value) {
    _data = value;
    return *this;
}

void Text::Render(std::ostream& os) const {
    os << "<text";
    RenderCommon(os);
    RenderField(os, "x", _coords.x);
    RenderField(os, "y", _coords.y);
    RenderField(os, "dx", _offset.x);
    RenderField(os, "dy", _offset.y);
    RenderField(os, "font-size", _size);
    if (!_family.empty()) {
        RenderField(os, "font-family", _family);
    }
    if (!_weight.empty()) {
        RenderField(os, "font-weight", _weight);
    }
    os << " >";
    os << _data;
    os << "</text>";
}

// Document

void Document::Render(std::ostream& os) {
    os << R"(<?xml version="1.0" encoding="UTF-8" ?><svg xmlns="http://www.w3.org/2000/svg" version="1.1">)";
    for (const auto& elem : _data) {
        elem->Render(os);
    }
    os << "</svg>";
}

