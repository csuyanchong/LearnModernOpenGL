#ifndef SRC_RENDER_NORMALRENDERPASS_H_
#define SRC_RENDER_NORMALRENDERPASS_H_

#include "RenderPass.h"

class NormalRenderPass : public RenderPass {
public:
  void preDraw();
  void draw();
};

#endif // !SRC_RENDER_NORMALRENDERPASS_H_




