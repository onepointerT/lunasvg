#include "svgpathelement.h"

#include "graphics.h"

#include <set>

namespace lunasvg {

SVGPathElement::SVGPathElement(Document* document, ElementID id)
    : SVGElement(document, id)
    , mPaths( *new paths::nanosvg_path_image_t{document->width(), document->height(), new paths::nanosvg_shape_t{} } )
    , mCanvas( Canvas::create(document->renderToBitmap()) )
{}

} // namespace lunasvg
