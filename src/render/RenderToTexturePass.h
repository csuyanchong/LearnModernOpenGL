#ifndef SRC_RENDER_RENDERTOTEXTUREPASS_H_
#define SRC_RENDER_RENDERTOTEXTUREPASS_H_

#include "RenderPass.h"
class RenderToTexturePass : public RenderPass {
public:
  void preDraw();
  void draw();
};


#endif // !SRC_RENDER_RENDERTOTEXTUREPASS_H_

