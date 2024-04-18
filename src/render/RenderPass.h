#ifndef SRC_RENDER_RENDER_H_
#define SRC_RENDER_RENDER_H_

class RenderPass {
public:
  void render();
protected:
  virtual void preDraw();
  virtual void draw();
};


#endif // !SRC_RENDER_RENDER_H_

