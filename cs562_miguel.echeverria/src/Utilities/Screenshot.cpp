#include "pch.h"
#include <GL/glew.h>
#include <stb/stb_image_write.h>


// Saves the front framebuffer to an image with the specified filename
// Call it after swapping buffers to make sure you save the last frame rendered
void SaveScreenShot(const char* filename)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    std::vector<unsigned char> imageData(width * height * 4, 0);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename, width, height, 4, imageData.data(), 0);
}