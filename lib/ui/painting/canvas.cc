// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/canvas.h"
#include "flutter/lib/ui/painting/image_filter.h"

#include <cmath>

#include "flutter/flow/layers/physical_shape_layer.h"
#include "flutter/lib/ui/painting/image.h"
#include "flutter/lib/ui/painting/matrix.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/platform_configuration.h"
#include "flutter/lib/ui/window/window.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkRSXform.h"
#include "third_party/tonic/converter/dart_converter.h"
#include "third_party/tonic/dart_args.h"
#include "third_party/tonic/dart_binding_macros.h"
#include "third_party/tonic/dart_library_natives.h"

using tonic::ToDart;

namespace flutter {

static void Canvas_constructor(Dart_NativeArguments args) {
  UIDartState::ThrowIfUIOperationsProhibited();
  DartCallConstructor(&Canvas::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, Canvas);

#define FOR_EACH_BINDING(V)         \
  V(Canvas, save)                   \
  V(Canvas, saveLayerWithoutBounds) \
  V(Canvas, saveLayer)              \
  V(Canvas, restore)                \
  V(Canvas, getSaveCount)           \
  V(Canvas, translate)              \
  V(Canvas, scale)                  \
  V(Canvas, rotate)                 \
  V(Canvas, skew)                   \
  V(Canvas, transform)              \
  V(Canvas, clipRect)               \
  V(Canvas, clipRRect)              \
  V(Canvas, clipPath)               \
  V(Canvas, drawColor)              \
  V(Canvas, drawLine)               \
  V(Canvas, drawPaint)              \
  V(Canvas, drawRect)               \
  V(Canvas, drawRRect)              \
  V(Canvas, drawDRRect)             \
  V(Canvas, drawOval)               \
  V(Canvas, drawCircle)             \
  V(Canvas, drawArc)                \
  V(Canvas, drawPath)               \
  V(Canvas, drawImage)              \
  V(Canvas, drawImageRect)          \
  V(Canvas, drawImageNine)          \
  V(Canvas, drawPicture)            \
  V(Canvas, drawPoints)             \
  V(Canvas, drawVertices)           \
  V(Canvas, drawAtlas)              \
  V(Canvas, drawShadow)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void Canvas::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"Canvas_constructor", Canvas_constructor, 6, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}

fml::RefPtr<Canvas> Canvas::Create(PictureRecorder* recorder,
                                   double left,
                                   double top,
                                   double right,
                                   double bottom) {
  if (!recorder) {
    Dart_ThrowException(
        ToDart("Canvas constructor called with non-genuine PictureRecorder."));
    return nullptr;
  }
  fml::RefPtr<Canvas> canvas = fml::MakeRefCounted<Canvas>(
      recorder->BeginRecording(SkRect::MakeLTRB(left, top, right, bottom)));
  recorder->set_canvas(canvas);
  canvas->display_list_recorder_ = recorder->display_list_recorder();
  return canvas;
}

Canvas::Canvas(SkCanvas* canvas) : canvas_(canvas) {}

Canvas::~Canvas() {}

void Canvas::save() {
  if (!canvas_) {
    return;
  }
  canvas_->save();
}

void Canvas::saveLayerWithoutBounds(const Paint& paint,
                                    const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  TRACE_EVENT0("flutter", "ui.Canvas::saveLayer (Recorded)");
  canvas_->saveLayer(nullptr, paint.paint());
}

void Canvas::saveLayer(double left,
                       double top,
                       double right,
                       double bottom,
                       const Paint& paint,
                       const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  TRACE_EVENT0("flutter", "ui.Canvas::saveLayer (Recorded)");
  SkRect bounds = SkRect::MakeLTRB(left, top, right, bottom);
  canvas_->saveLayer(&bounds, paint.paint());
}

void Canvas::restore() {
  if (!canvas_) {
    return;
  }
  canvas_->restore();
}

int Canvas::getSaveCount() {
  if (!canvas_) {
    return 0;
  }
  return canvas_->getSaveCount();
}

void Canvas::translate(double dx, double dy) {
  if (!canvas_) {
    return;
  }
  canvas_->translate(dx, dy);
}

void Canvas::scale(double sx, double sy) {
  if (!canvas_) {
    return;
  }
  canvas_->scale(sx, sy);
}

void Canvas::rotate(double radians) {
  if (!canvas_) {
    return;
  }
  canvas_->rotate(radians * 180.0 / M_PI);
}

void Canvas::skew(double sx, double sy) {
  if (!canvas_) {
    return;
  }
  canvas_->skew(sx, sy);
}

void Canvas::transform(const tonic::Float64List& matrix4) {
  if (!canvas_) {
    return;
  }
  canvas_->concat(SkM44(matrix4[0], matrix4[4], matrix4[8], matrix4[12],
                        matrix4[1], matrix4[5], matrix4[9], matrix4[13],
                        matrix4[2], matrix4[6], matrix4[10], matrix4[14],
                        matrix4[3], matrix4[7], matrix4[11], matrix4[15]));
}

void Canvas::clipRect(double left,
                      double top,
                      double right,
                      double bottom,
                      SkClipOp clipOp,
                      bool doAntiAlias) {
  if (!canvas_) {
    return;
  }
  canvas_->clipRect(SkRect::MakeLTRB(left, top, right, bottom), clipOp,
                    doAntiAlias);
}

void Canvas::clipRRect(const RRect& rrect, bool doAntiAlias) {
  if (!canvas_) {
    return;
  }
  canvas_->clipRRect(rrect.sk_rrect, doAntiAlias);
}

void Canvas::clipPath(const CanvasPath* path, bool doAntiAlias) {
  if (!canvas_) {
    return;
  }
  if (!path) {
    Dart_ThrowException(
        ToDart("Canvas.clipPath called with non-genuine Path."));
    return;
  }
  canvas_->clipPath(path->path(), doAntiAlias);
}

void Canvas::drawColor(SkColor color, SkBlendMode blend_mode) {
  if (!canvas_) {
    return;
  }
  canvas_->drawColor(color, blend_mode);
}

void Canvas::drawLine(double x1,
                      double y1,
                      double x2,
                      double y2,
                      const Paint& paint,
                      const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawLine(x1, y1, x2, y2, *paint.paint());
}

void Canvas::drawPaint(const Paint& paint, const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  const SkPaint* sk_paint = paint.paint();
  SkImageFilter* filter = sk_paint->getImageFilter();
  if (filter && !filter->asColorFilter(nullptr)) {
    // drawPaint does an implicit saveLayer if an SkImageFilter is
    // present that cannot be replaced by an SkColorFilter.
    TRACE_EVENT0("flutter", "ui.Canvas::saveLayer (Recorded)");
  }
  canvas_->drawPaint(*paint.paint());
}

void Canvas::drawRect(double left,
                      double top,
                      double right,
                      double bottom,
                      const Paint& paint,
                      const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawRect(SkRect::MakeLTRB(left, top, right, bottom), *paint.paint());
}

void Canvas::drawRRect(const RRect& rrect,
                       const Paint& paint,
                       const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawRRect(rrect.sk_rrect, *paint.paint());
}

void Canvas::drawDRRect(const RRect& outer,
                        const RRect& inner,
                        const Paint& paint,
                        const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawDRRect(outer.sk_rrect, inner.sk_rrect, *paint.paint());
}

void Canvas::drawOval(double left,
                      double top,
                      double right,
                      double bottom,
                      const Paint& paint,
                      const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawOval(SkRect::MakeLTRB(left, top, right, bottom), *paint.paint());
}

void Canvas::drawCircle(double x,
                        double y,
                        double radius,
                        const Paint& paint,
                        const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawCircle(x, y, radius, *paint.paint());
}

void Canvas::drawArc(double left,
                     double top,
                     double right,
                     double bottom,
                     double startAngle,
                     double sweepAngle,
                     bool useCenter,
                     const Paint& paint,
                     const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  canvas_->drawArc(SkRect::MakeLTRB(left, top, right, bottom),
                   startAngle * 180.0 / M_PI, sweepAngle * 180.0 / M_PI,
                   useCenter, *paint.paint());
}

void Canvas::drawPath(const CanvasPath* path,
                      const Paint& paint,
                      const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  if (!path) {
    Dart_ThrowException(
        ToDart("Canvas.drawPath called with non-genuine Path."));
    return;
  }
  canvas_->drawPath(path->path(), *paint.paint());
}

void Canvas::drawImage(const CanvasImage* image,
                       double x,
                       double y,
                       const Paint& paint,
                       const PaintData& paint_data,
                       int filterQualityIndex) {
  if (!canvas_) {
    return;
  }
  if (!image) {
    Dart_ThrowException(
        ToDart("Canvas.drawImage called with non-genuine Image."));
    return;
  }
  auto sampling = ImageFilter::SamplingFromIndex(filterQualityIndex);
  canvas_->drawImage(image->image(), x, y, sampling, paint.paint());
}

void Canvas::drawImageRect(const CanvasImage* image,
                           double src_left,
                           double src_top,
                           double src_right,
                           double src_bottom,
                           double dst_left,
                           double dst_top,
                           double dst_right,
                           double dst_bottom,
                           const Paint& paint,
                           const PaintData& paint_data,
                           int filterQualityIndex) {
  if (!canvas_) {
    return;
  }
  if (!image) {
    Dart_ThrowException(
        ToDart("Canvas.drawImageRect called with non-genuine Image."));
    return;
  }
  SkRect src = SkRect::MakeLTRB(src_left, src_top, src_right, src_bottom);
  SkRect dst = SkRect::MakeLTRB(dst_left, dst_top, dst_right, dst_bottom);
  auto sampling = ImageFilter::SamplingFromIndex(filterQualityIndex);
  canvas_->drawImageRect(image->image(), src, dst, sampling, paint.paint(),
                         SkCanvas::kFast_SrcRectConstraint);
}

void Canvas::drawImageNine(const CanvasImage* image,
                           double center_left,
                           double center_top,
                           double center_right,
                           double center_bottom,
                           double dst_left,
                           double dst_top,
                           double dst_right,
                           double dst_bottom,
                           const Paint& paint,
                           const PaintData& paint_data,
                           int bitmapSamplingIndex) {
  if (!canvas_) {
    return;
  }
  if (!image) {
    Dart_ThrowException(
        ToDart("Canvas.drawImageNine called with non-genuine Image."));
    return;
  }
  SkRect center =
      SkRect::MakeLTRB(center_left, center_top, center_right, center_bottom);
  SkIRect icenter;
  center.round(&icenter);
  SkRect dst = SkRect::MakeLTRB(dst_left, dst_top, dst_right, dst_bottom);
  auto filter = ImageFilter::FilterModeFromIndex(bitmapSamplingIndex);
  if (display_list_recorder_) {
    // SkCanvas turns a simple 2-rect DrawImageNine operation into a
    // drawImageLattice operation which has arrays to allocate and
    // pass along. For simplicity, we will bypass the canvas and ask
    // the recorder to record our paint attributes and record a much
    // simpler DrawImageNineOp record directly.
    display_list_recorder_->RecordPaintAttributes(
        paint.paint(), DisplayListCanvasRecorder::DrawType::kImageOpType);
    builder()->drawImageNine(image->image(), icenter, dst, filter, true);
  } else {
    canvas_->drawImageNine(image->image().get(), icenter, dst, filter,
                           paint.paint());
  }
}

void Canvas::drawPicture(Picture* picture) {
  if (!canvas_) {
    return;
  }
  if (!picture) {
    Dart_ThrowException(
        ToDart("Canvas.drawPicture called with non-genuine Picture."));
    return;
  }
  if (picture->picture()) {
    canvas_->drawPicture(picture->picture().get());
  } else if (picture->display_list()) {
    if (display_list_recorder_) {
      builder()->drawDisplayList(picture->display_list());
    } else {
      picture->display_list()->RenderTo(canvas_);
    }
  } else {
    FML_DCHECK(false);
  }
}

void Canvas::drawPoints(const Paint& paint,
                        const PaintData& paint_data,
                        SkCanvas::PointMode point_mode,
                        const tonic::Float32List& points) {
  if (!canvas_) {
    return;
  }

  static_assert(sizeof(SkPoint) == sizeof(float) * 2,
                "SkPoint doesn't use floats.");

  canvas_->drawPoints(point_mode,
                      points.num_elements() / 2,  // SkPoints have two floats.
                      reinterpret_cast<const SkPoint*>(points.data()),
                      *paint.paint());
}

void Canvas::drawVertices(const Vertices* vertices,
                          SkBlendMode blend_mode,
                          const Paint& paint,
                          const PaintData& paint_data) {
  if (!canvas_) {
    return;
  }
  if (!vertices) {
    Dart_ThrowException(
        ToDart("Canvas.drawVertices called with non-genuine Vertices."));
    return;
  }
  canvas_->drawVertices(vertices->vertices(), blend_mode, *paint.paint());
}

void Canvas::drawAtlas(const Paint& paint,
                       const PaintData& paint_data,
                       int filterQualityIndex,
                       CanvasImage* atlas,
                       const tonic::Float32List& transforms,
                       const tonic::Float32List& rects,
                       const tonic::Int32List& colors,
                       SkBlendMode blend_mode,
                       const tonic::Float32List& cull_rect) {
  if (!canvas_) {
    return;
  }
  if (!atlas) {
    Dart_ThrowException(
        ToDart("Canvas.drawAtlas or Canvas.drawRawAtlas called with "
               "non-genuine Image."));
    return;
  }

  sk_sp<SkImage> skImage = atlas->image();

  static_assert(sizeof(SkRSXform) == sizeof(float) * 4,
                "SkRSXform doesn't use floats.");
  static_assert(sizeof(SkRect) == sizeof(float) * 4,
                "SkRect doesn't use floats.");

  auto sampling = ImageFilter::SamplingFromIndex(filterQualityIndex);

  canvas_->drawAtlas(
      skImage.get(), reinterpret_cast<const SkRSXform*>(transforms.data()),
      reinterpret_cast<const SkRect*>(rects.data()),
      reinterpret_cast<const SkColor*>(colors.data()),
      rects.num_elements() / 4,  // SkRect have four floats.
      blend_mode, sampling, reinterpret_cast<const SkRect*>(cull_rect.data()),
      paint.paint());
}

void Canvas::drawShadow(const CanvasPath* path,
                        SkColor color,
                        double elevation,
                        bool transparentOccluder) {
  if (!path) {
    Dart_ThrowException(
        ToDart("Canvas.drawShader called with non-genuine Path."));
    return;
  }
  SkScalar dpr = UIDartState::Current()
                     ->platform_configuration()
                     ->get_window(0)
                     ->viewport_metrics()
                     .device_pixel_ratio;
  if (display_list_recorder_) {
    // The DrawShadow mechanism results in non-public operations to be
    // performed on the canvas involving an SkDrawShadowRec. Since we
    // cannot include the header that defines that structure, we cannot
    // record an operation that it injects into an SkCanvas. To prevent
    // that situation we bypass the canvas interface and inject the
    // shadow parameters directly into the underlying DisplayList.
    // See: https://bugs.chromium.org/p/skia/issues/detail?id=12125
    builder()->drawShadow(path->path(), color, elevation, transparentOccluder,
                          dpr);
  } else {
    flutter::PhysicalShapeLayer::DrawShadow(
        canvas_, path->path(), color, elevation, transparentOccluder, dpr);
  }
}

void Canvas::Invalidate() {
  canvas_ = nullptr;
  if (dart_wrapper()) {
    ClearDartWrapper();
  }
}

}  // namespace flutter
