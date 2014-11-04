//  Created by Alejandro Isaza on 2014-05-12.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "SpanDirectionGeometry.h"
#include "PartGeometry.h"

#include <memory>
#include <vector>

namespace mxml {

class SpanDirectionGeometryFactory {
public:
    typedef std::vector<std::unique_ptr<Geometry>> CollectionType;
    
public:
    explicit SpanDirectionGeometryFactory(const PartGeometry& partGeometry);
    
    CollectionType&& buildTieGeometries(const CollectionType& geometries);
    
private:
    void createGeometries(const CollectionType& geometries);
    void createGeometryFromDirection(const SpanDirectionGeometry& geometry);
    
private:
    const PartGeometry& _partGeometry;
    
    Point _origin;
    CollectionType _geometries;
    std::map<std::pair<int, std::type_info>, const SpanDirectionGeometry*> _startGeometries;
};

} // namespace mxml
