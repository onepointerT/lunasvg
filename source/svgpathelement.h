#ifndef LUNASVG_SVGPAINTELEMENT_H
#define LUNASVG_SVGPAINTELEMENT_H

#include "svgelement.h"


namespace lunasvg {

namespace paths {
#include "nanosvg.h"

typedef struct NSVGimage nanosvg_path_image_t;
typedef struct NSVGshape nanosvg_shape_t;
} // namespace paths


class Canvas;

class SVGPathElement : public SVGElement {
protected:
    paths::nanosvg_path_image_t& mPaths;
    std::shared_ptr<Canvas> mCanvas;

public:
    SVGPathElement(Document* document, ElementID id);

    virtual bool isPathElement() const final { return true; }

    virtual bool applyPaint(SVGRenderState& state, float opacity) const = 0;
};


} // namespace lunasvg

#endif // LUNASVG_SVGPAINTELEMENT_H
