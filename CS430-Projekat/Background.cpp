#include "Background.h"
#include "SOIL2.h"

Background::Background() : BaseGameEntity(0, Vector2D(0, 0), 0) {

    textures = new GLuint[1];
    current_frame = 0;
    number_of_frames = 0;
    transparent = false;
    visible = true;
}

void Background::render() {

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_DEPTH_TEST);

    if (visible) {
        if (transparent) {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glBindTexture(GL_TEXTURE_2D, textures[current_frame]);

        glBegin(GL_QUADS);


        glTexCoord2f(0, 0);  glVertex2f(position.x, position.y);
        glTexCoord2f(1, 0);  glVertex2f(position.x + size.width, position.y);
        glTexCoord2f(1, 1);  glVertex2f(position.x + size.width, position.y + size.height);
        glTexCoord2f(0, 1);  glVertex2f(position.x, position.y + size.height);

        glEnd();

        if (transparent) { glDisable(GL_BLEND); }
    }
    glDisable(GL_TEXTURE_2D);
}

bool Background::add_texture(const char* file_name, const bool _transparent) {

	GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (texture == 0) {
		return false;
	}

	textures[texture_index] = texture;
	texture_index++;

	transparent = _transparent;

	return true;
}